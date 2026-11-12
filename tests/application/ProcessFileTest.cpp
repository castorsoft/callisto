#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <QFile>
#include <QTemporaryDir>

#include "application/use_cases/ProcessFile.h"
#include "core/GlobalState.h"
#include "../mocks/MockRepositories.h"

using ::testing::_;
using ::testing::DoAll;
using ::testing::Invoke;
using ::testing::NiceMock;
using ::testing::Return;

namespace {

QString buildSampleNFeXml(const QString &chaveNumerica44,
                          const QString &tpNF,
                          const QString &emitCnpj,
                          const QString &destCnpj,
                          const QString &cProd,
                          const QString &cEAN)
{
    return QString(R"(<?xml version="1.0" encoding="UTF-8"?>
<nfeProc versao="4.00" xmlns="http://www.portalfiscal.inf.br/nfe">
  <NFe>
    <infNFe Id="NFe%1" versao="4.00">
      <ide>
        <cUF>35</cUF>
        <natOp>VENDA DE MERCADORIA</natOp>
        <mod>55</mod>
        <serie>1</serie>
        <nNF>987</nNF>
        <dhEmi>2026-10-20T10:30:00-03:00</dhEmi>
        <tpNF>%2</tpNF>
      </ide>
      <emit>
        <CNPJ>%3</CNPJ>
        <xNome>FORNECEDOR EXEMPLO LTDA</xNome>
        <xFant>FORNECEDOR EXEMPLO</xFant>
        <IE>110042490114</IE>
        <enderEmit>
          <xLgr>RUA DAS INDUSTRIAS</xLgr>
          <nro>500</nro>
          <xCpl>GALPAO 2</xCpl>
          <xBairro>DISTRITO INDUSTRIAL</xBairro>
          <cMun>3550308</cMun>
          <xMun>SAO PAULO</xMun>
          <UF>SP</UF>
          <CEP>01001000</CEP>
        </enderEmit>
      </emit>
      <dest>
        <CNPJ>%4</CNPJ>
        <xNome>DESTINATARIO COMERCIO LTDA</xNome>
        <IE>220042490114</IE>
        <enderDest>
          <xLgr>AV CENTRAL</xLgr>
          <nro>100</nro>
          <xBairro>CENTRO</xBairro>
          <cMun>3509502</cMun>
          <xMun>CAMPINAS</xMun>
          <UF>SP</UF>
          <CEP>13010000</CEP>
        </enderDest>
      </dest>
      <det nItem="1">
        <prod>
          <cProd>%5</cProd>
          <cEAN>%6</cEAN>
          <xProd>PRODUTO TESTE NFE</xProd>
          <NCM>84713012</NCM>
          <CEST>2103100</CEST>
          <CFOP>5102</CFOP>
          <uCom>UN</uCom>
          <qCom>2.0000</qCom>
          <vUnCom>150.0000</vUnCom>
          <vProd>300.00</vProd>
          <vDesc>10.00</vDesc>
        </prod>
        <imposto>
          <ICMS>
            <ICMS00>
              <orig>0</orig>
              <CST>00</CST>
              <vBC>290.00</vBC>
              <pICMS>18.00</pICMS>
              <vICMS>52.20</vICMS>
            </ICMS00>
          </ICMS>
          <IPI>
            <IPITrib>
              <CST>50</CST>
              <vBC>290.00</vBC>
              <pIPI>5.00</pIPI>
              <vIPI>14.50</vIPI>
            </IPITrib>
          </IPI>
          <PIS>
            <PISAliq>
              <CST>01</CST>
              <vBC>290.00</vBC>
              <pPIS>1.65</pPIS>
              <vPIS>4.79</vPIS>
            </PISAliq>
          </PIS>
          <COFINS>
            <COFINSAliq>
              <CST>01</CST>
              <vBC>290.00</vBC>
              <pCOFINS>7.60</pCOFINS>
              <vCOFINS>22.04</vCOFINS>
            </COFINSAliq>
          </COFINS>
        </imposto>
      </det>
    </infNFe>
  </NFe>
</nfeProc>)")
        .arg(chaveNumerica44, tpNF, emitCnpj, destCnpj, cProd, cEAN);
}

} // namespace

class ProcessFileTest : public ::testing::Test {
protected:
    NiceMock<MockDocumentoFiscalRepository> docRepo;
    NiceMock<MockDocumentoFiscalArquivoRepository> arqRepo;
    NiceMock<MockPessoaRepository> pessoaRepo;
    NiceMock<MockProdutoRepository> prodRepo;
    NiceMock<MockProdutoFornecedorRepository> pfRepo;
    NiceMock<MockUnidadeMedidaRepository> unRepo;
    NiceMock<MockNCMRepository> ncmRepo;
    NiceMock<MockCidadeRepository> cidadeRepo;

    ProcessFileRepositories repos;

    void SetUp() override {
        GlobalState::instance().clearEmpresaAtual();
        repos.documentoFiscalRepo = &docRepo;
        repos.documentoFiscalArquivoRepo = &arqRepo;
        repos.pessoaRepo = &pessoaRepo;
        repos.produtoRepo = &prodRepo;
        repos.produtoFornecedorRepo = &pfRepo;
        repos.unidadeMedidaRepo = &unRepo;
        repos.ncmRepo = &ncmRepo;
        repos.cidadeRepo = &cidadeRepo;
    }

    void TearDown() override {
        GlobalState::instance().clearEmpresaAtual();
    }
};

TEST_F(ProcessFileTest, RejectsMissingFileEmptyXmlAndUnknownStructure) {
    ProcessFile useCase(repos);

    auto resMissing = useCase.processarArquivo("/caminho/inexistente/nota.xml");
    EXPECT_EQ(resMissing.status, ResultadoProcessamento::Status::Erro);
    EXPECT_EQ(resMissing.mensagem, "Arquivo não encontrado.");

    auto resEmpty = useCase.processarXml("   \n\t  ");
    EXPECT_EQ(resEmpty.status, ResultadoProcessamento::Status::Erro);
    EXPECT_EQ(resEmpty.mensagem, "Conteúdo XML vazio.");

    auto resUnknown = useCase.processarXml("<?xml version=\"1.0\"?><OutroDocumento></OutroDocumento>");
    EXPECT_EQ(resUnknown.status, ResultadoProcessamento::Status::Erro);
    EXPECT_EQ(resUnknown.mensagem, "Estrutura XML não reconhecida.");
}

TEST_F(ProcessFileTest, DetectsAlreadyImportedDocumentByChaveAcesso) {
    const QString chave = "35261022222222000122550010000009871000009870";
    QString xml = buildSampleNFeXml(chave, "1", "22222222000122", "11111111000111", "FORN-01", "SEM GTIN");

    EXPECT_CALL(docRepo, existeDocumento(chave)).WillOnce(Return(true));
    EXPECT_CALL(docRepo, salvar(_)).Times(0);

    ProcessFile useCase(repos);
    auto res = useCase.processarXml(xml);

    EXPECT_EQ(res.status, ResultadoProcessamento::Status::JaImportado);
    EXPECT_EQ(res.chaveAcesso, chave);
    EXPECT_EQ(res.modelo, "55");
    EXPECT_EQ(res.mensagem, "Documento já importado anteriormente.");
}

TEST_F(ProcessFileTest, ProcessesThirdPartyInvoiceAutoAssociatesByBarcodeAndSavesXml) {
    // Empresa ativa tem CNPJ diferente do emitente -> emissão de terceiro
    Endereco endEmp("Rua Sede", "1", std::nullopt, "Centro", "01000000", 1);
    Empresa empAtiva("11111111000111", "Minha Empresa Ltda", std::nullopt, "Simples", endEmp, std::nullopt, std::nullopt);
    empAtiva.setId(1);
    GlobalState::instance().setEmpresaAtual(empAtiva);

    const QString chave = "35261022222222000122550010000009881000009880";
    QString xml = buildSampleNFeXml(chave, "0", "22222222000122", "11111111000111", "COD-FORN-99", "7891000100103");

    EXPECT_CALL(docRepo, existeDocumento(chave)).WillOnce(Return(false));

    // Fornecedor ainda não existe: deve buscar cidade IBGE 3550308 e criar Pessoa com fornecedor = "S"
    EXPECT_CALL(pessoaRepo, buscarPorCnpjCpf(QString("22222222000122"))).WillOnce(Return(std::nullopt));
    EXPECT_CALL(cidadeRepo, buscarPorIbge(3550308))
        .WillOnce(Return(Cidade(50, "São Paulo", 3550308, 26)));

    Pessoa pessoaSalva;
    EXPECT_CALL(pessoaRepo, salvar(_))
        .WillOnce(Invoke([&](Pessoa &p) {
            p.setId(44);
            pessoaSalva = p;
            return true;
        }));

    // Auto-associação por código de barras (RN19)
    EXPECT_CALL(pfRepo, buscarPorFornecedorECodigo(44, QString("COD-FORN-99")))
        .WillOnce(Return(std::nullopt));

    Produto prodInterno("Notebook Corporativo", 3, 12, TipoProdutoAtividade::MercadoriaRevenda, "7891000100103", "NB-01");
    prodInterno.setId(70);
    EXPECT_CALL(prodRepo, buscarPorCodigoBarra(QString("7891000100103")))
        .WillOnce(Return(prodInterno));

    ProdutoFornecedor pfSalvo(0, 0, "", 0, 0.0, std::nullopt);
    EXPECT_CALL(pfRepo, salvar(_))
        .WillOnce(Invoke([&](ProdutoFornecedor &pf) {
            pf.setId(500);
            pfSalvo = pf;
            return true;
        }));

    DocumentoFiscal docSalvo;
    EXPECT_CALL(docRepo, salvar(_))
        .WillOnce(Invoke([&](DocumentoFiscal &df) {
            df.setId(123);
            docSalvo = df;
            return true;
        }));

    DocumentoFiscalArquivo arqSalvo;
    EXPECT_CALL(arqRepo, salvar(_))
        .WillOnce(Invoke([&](DocumentoFiscalArquivo &dfa) {
            dfa.setId(900);
            arqSalvo = dfa;
            return true;
        }));

    ProcessFile useCase(repos);
    auto res = useCase.processarXml(xml);

    EXPECT_EQ(res.status, ResultadoProcessamento::Status::Sucesso);
    EXPECT_EQ(res.idDocumentoFiscal, 123);
    EXPECT_EQ(res.chaveAcesso, chave);

    // Verifica que o emitente foi salvo como fornecedor ("S") e vinculado à cidade 50
    EXPECT_EQ(pessoaSalva.cnpjCpf(), "22222222000122");
    EXPECT_EQ(pessoaSalva.fornecedor(), "S");
    EXPECT_EQ(pessoaSalva.endereco().idCidade(), 50);

    // Verifica o vínculo automático ProdutoFornecedor (RN19)
    EXPECT_EQ(pfSalvo.idProduto(), 70);
    EXPECT_EQ(pfSalvo.idFornecedor(), 44);
    EXPECT_EQ(pfSalvo.codigoFornecedor(), "COD-FORN-99");
    EXPECT_EQ(pfSalvo.idUnidadeMedida(), 3);
    EXPECT_DOUBLE_EQ(pfSalvo.multiplicador(), 1.0);

    // Verifica cabeçalho, item e tributos do DocumentoFiscal
    EXPECT_EQ(docSalvo.tipoEmissao(), TipoEmissao::Terceiro);
    EXPECT_EQ(docSalvo.tipoOperacao(), TipoOperacao::Entrada);
    EXPECT_EQ(docSalvo.idPessoa().value(), 44);
    EXPECT_EQ(docSalvo.numero(), 987);
    ASSERT_EQ(docSalvo.itens().size(), 1u);

    const auto &item = docSalvo.itens().front();
    EXPECT_EQ(item.codigoProduto(), "COD-FORN-99");
    EXPECT_EQ(item.codigoBarras(), "7891000100103");
    EXPECT_DOUBLE_EQ(item.quantidade(), 2.0);
    EXPECT_DOUBLE_EQ(item.valorUnitario(), 150.0);
    EXPECT_DOUBLE_EQ(item.valorTotal(), 300.0);
    EXPECT_DOUBLE_EQ(item.valorDesconto(), 10.0);
    EXPECT_EQ(item.dados()["ncm"].toString(), "84713012");
    EXPECT_EQ(item.dados()["cfop"].toString(), "5102");
    EXPECT_EQ(item.dados()["cest"].toString(), "2103100");
    EXPECT_EQ(item.tributos().size(), 4u);

    // Verifica persistência do XML original (RF16)
    EXPECT_EQ(arqSalvo.idDocumento(), 123);
    EXPECT_EQ(arqSalvo.tipoArquivo(), "DOCUMENTO_FISCAL");
    EXPECT_TRUE(arqSalvo.conteudoXml().contains(chave));
}

TEST_F(ProcessFileTest, ProcessesOwnIssuedInvoiceSanitizesSemGtinAndAutoCreatesCatalogEntities) {
    // Empresa ativa com máscara tem o mesmo CNPJ do emitente -> emissão própria (RN15)
    Endereco endEmp("Rua Sede", "1", std::nullopt, "Centro", "01000000", 1);
    Empresa empAtiva("22.222.222/0001-22", "Fornecedor Exemplo Ltda", std::nullopt, "Lucro Real", endEmp, std::nullopt, std::nullopt);
    empAtiva.setId(2);
    GlobalState::instance().setEmpresaAtual(empAtiva);

    const QString chave = "35261022222222000122550010000009891000009890";
    QString xml = buildSampleNFeXml(chave, "1", "22222222000122", "11111111000111", "SKU-PROPRIO-01", "sem gtin");

    EXPECT_CALL(docRepo, existeDocumento(chave)).WillOnce(Return(false));

    // Destinatário já existe no banco
    Pessoa destExistente("11111111000111", "Destinatario Comercio Ltda", std::nullopt, std::nullopt, "N", "N", endEmp, 2);
    destExistente.setId(88);
    EXPECT_CALL(pessoaRepo, buscarPorCnpjCpf(QString("11111111000111")))
        .WillOnce(Return(destExistente));

    // Emissão própria: auto-cadastra Produto, UnidadeMedida e NCM se não existirem (RN16)
    EXPECT_CALL(prodRepo, buscarPorCodigoInterno(QString("SKU-PROPRIO-01")))
        .WillOnce(Return(std::nullopt));
    EXPECT_CALL(unRepo, buscarPorSigla(QString("UN")))
        .WillOnce(Return(std::nullopt));
    EXPECT_CALL(unRepo, salvar(_))
        .WillOnce(Invoke([](UnidadeMedida &um) {
            um.setId(10);
            return true;
        }));
    EXPECT_CALL(ncmRepo, buscarPorCodigo(QString("84713012")))
        .WillOnce(Return(std::nullopt));
    EXPECT_CALL(ncmRepo, salvar(_))
        .WillOnce(Invoke([](NCM &ncm) {
            ncm.setId(20);
            return true;
        }));

    Produto prodAutoCriado;
    EXPECT_CALL(prodRepo, salvar(_))
        .WillOnce(Invoke([&](Produto &p) {
            p.setId(30);
            prodAutoCriado = p;
            return true;
        }));

    DocumentoFiscal docSalvo;
    EXPECT_CALL(docRepo, salvar(_))
        .WillOnce(Invoke([&](DocumentoFiscal &df) {
            df.setId(124);
            docSalvo = df;
            return true;
        }));
    EXPECT_CALL(arqRepo, salvar(_)).WillOnce(Return(true));

    ProcessFile useCase(repos);
    auto res = useCase.processarXml(xml);

    EXPECT_EQ(res.status, ResultadoProcessamento::Status::Sucesso);
    EXPECT_EQ(docSalvo.tipoEmissao(), TipoEmissao::Propria);
    EXPECT_EQ(docSalvo.tipoOperacao(), TipoOperacao::Saida);
    EXPECT_EQ(docSalvo.idPessoa().value(), 88);

    // Verifica que "sem gtin" foi higienizado (RN11 / RN17)
    ASSERT_EQ(docSalvo.itens().size(), 1u);
    EXPECT_TRUE(docSalvo.itens().front().codigoBarras().isEmpty());
    EXPECT_FALSE(docSalvo.itens().front().dados().contains("cEAN"));
    EXPECT_TRUE(prodAutoCriado.codigoBarra().value_or("").isEmpty());
    EXPECT_EQ(prodAutoCriado.idUnidadeMedida(), 10);
    EXPECT_EQ(prodAutoCriado.idNCM(), 20);
}

TEST_F(ProcessFileTest, PreservesOriginalFileOnDiskWhenProcessingByPath) {
    QTemporaryDir tempDir;
    ASSERT_TRUE(tempDir.isValid());

    const QString chave = "35261022222222000122550010000009901000009900";
    QString xmlContent = buildSampleNFeXml(chave, "1", "22222222000122", "11111111000111", "P-1", "SEM GTIN");
    QString filePath = tempDir.filePath("nota_teste.xml");
    {
        QFile f(filePath);
        ASSERT_TRUE(f.open(QIODevice::WriteOnly | QIODevice::Text));
        f.write(xmlContent.toUtf8());
    }

    EXPECT_CALL(docRepo, existeDocumento(chave)).WillOnce(Return(false));
    Pessoa emitente("22222222000122", "Emitente", std::nullopt, std::nullopt, "S", "N", Endereco(), 1);
    emitente.setId(5);
    EXPECT_CALL(pessoaRepo, buscarPorCnpjCpf(_)).WillOnce(Return(emitente));
    EXPECT_CALL(pfRepo, buscarPorFornecedorECodigo(_, _)).WillOnce(Return(std::nullopt));
    EXPECT_CALL(docRepo, salvar(_)).WillOnce(Invoke([](DocumentoFiscal &df) {
        df.setId(200);
        return true;
    }));
    // Simula falha ao gravar o XML em documentos_fiscais_arquivos para validar o aviso
    EXPECT_CALL(arqRepo, salvar(_)).WillOnce(Return(false));

    ProcessFile useCase(repos);
    auto res = useCase.processarArquivo(filePath);

    EXPECT_EQ(res.status, ResultadoProcessamento::Status::Sucesso);
    EXPECT_TRUE(res.mensagem.contains("(aviso: XML não foi armazenado)"));
    // RN13: O arquivo original jamais deve ser removido ou alterado
    EXPECT_TRUE(QFile::exists(filePath));
}

