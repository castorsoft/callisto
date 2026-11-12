#include <gtest/gtest.h>
#include <QSqlField>
#include <QSqlRecord>
#include <QJsonObject>
#include <QUuid>
#include <QDate>
#include <QDateTime>

#include "core/entities/Cidade.h"
#include "core/entities/DocumentoFiscal.h"
#include "core/entities/DocumentoFiscalArquivo.h"
#include "core/entities/DocumentoFiscalItem.h"
#include "core/entities/DocumentoFiscalItemTributo.h"
#include "core/entities/Empresa.h"
#include "core/entities/Endereco.h"
#include "core/entities/Estado.h"
#include "core/entities/NCM.h"
#include "core/entities/Pessoa.h"
#include "core/entities/PessoaContabilista.h"
#include "core/entities/Produto.h"
#include "core/entities/ProdutoFornecedor.h"
#include "core/entities/UnidadeMedida.h"
#include "core/entities/virtual/NotaTerceiroItem.h"
#include "core/entities/virtual/NotaTerceiroResumo.h"

namespace {

void addField(QSqlRecord &rec, const QString &name, const QVariant &val) {
    QSqlField field(name, val.metaType());
    if (val.isNull()) {
        field.clear();
    } else {
        field.setValue(val);
    }
    rec.append(field);
}

void addNullField(QSqlRecord &rec, const QString &name, QMetaType::Type type = QMetaType::QString) {
    QSqlField field(name, QMetaType(type));
    field.clear();
    rec.append(field);
}

} // namespace

TEST(EntitiesTest, EstadoAndCidadeConstruction) {
    Estado estado(26, "São Paulo", "SP", 35);
    EXPECT_EQ(estado.id(), 26);
    EXPECT_EQ(estado.nome(), "São Paulo");
    EXPECT_EQ(estado.sigla(), "SP");
    EXPECT_EQ(estado.idIbge(), 35);

    Cidade cidade(100, "Campinas", 3509502, 26);
    EXPECT_EQ(cidade.id(), 100);
    EXPECT_EQ(cidade.nome(), "Campinas");
    EXPECT_EQ(cidade.idIbge(), 3509502);
    EXPECT_EQ(cidade.idEstado(), 26);
    EXPECT_EQ(cidade.idUF(), 26);
}

TEST(EntitiesTest, EnderecoFromRecordWithAndWithoutPrefix) {
    QUuid uuid = QUuid::createUuid();
    QDateTime now = QDateTime::currentDateTime();

    QSqlRecord rec;
    addField(rec, "end_id", 42);
    addField(rec, "end_uuid", uuid);
    addField(rec, "end_logradouro", QString("Av. Paulista"));
    addField(rec, "end_numero", QString("1000"));
    addNullField(rec, "end_complemento");
    addField(rec, "end_bairro", QString("Bela Vista"));
    addField(rec, "end_cep", QString("01310100"));
    addField(rec, "end_id_cidade", 3550308);
    addField(rec, "end_criado_em", now);
    addField(rec, "end_atualizado_em", now);

    Endereco end = Endereco::fromRecord(rec, "end_");
    ASSERT_TRUE(end.id().has_value());
    EXPECT_EQ(end.id().value(), 42);
    ASSERT_TRUE(end.uuid().has_value());
    EXPECT_EQ(end.uuid().value(), uuid);
    EXPECT_EQ(end.logradouro(), "Av. Paulista");
    ASSERT_TRUE(end.numero().has_value());
    EXPECT_EQ(end.numero().value(), "1000");
    EXPECT_FALSE(end.complemento().has_value());
    EXPECT_EQ(end.bairro(), "Bela Vista");
    EXPECT_EQ(end.cep(), "01310100");
    EXPECT_EQ(end.idCidade(), 3550308);

    Cidade sp(500, "São Paulo", 3550308, 26);
    end.setCidade(sp);
    EXPECT_EQ(end.idCidade(), 500);
    EXPECT_EQ(end.cidade().nome(), "São Paulo");
}

TEST(EntitiesTest, EmpresaFromRecordAndAccessors) {
    QUuid uuid = QUuid::createUuid();
    QDateTime now = QDateTime::currentDateTime();

    QSqlRecord rec;
    addField(rec, "id", 1);
    addField(rec, "uuid", uuid);
    addField(rec, "cnpj_cpf", QString("12345678000199"));
    addField(rec, "razao_social", QString("CastorSoft Tecnologia Ltda"));
    addField(rec, "nome_fantasia", QString("CastorSoft"));
    addField(rec, "regime_tributario", QString("Simples Nacional"));
    addField(rec, "inscricao_estadual", QString("110042490114"));
    addNullField(rec, "inscricao_municipal");
    addField(rec, "id_endereco", 10);
    addField(rec, "criado_em", now);
    addField(rec, "atualizado_em", now);

    Empresa emp = Empresa::fromRecord(rec);
    ASSERT_TRUE(emp.id().has_value());
    EXPECT_EQ(emp.id().value(), 1);
    EXPECT_EQ(emp.uuid().value(), uuid);
    EXPECT_EQ(emp.cnpjCpf(), "12345678000199");
    EXPECT_EQ(emp.razaoSocial(), "CastorSoft Tecnologia Ltda");
    ASSERT_TRUE(emp.nomeFantasia().has_value());
    EXPECT_EQ(emp.nomeFantasia().value(), "CastorSoft");
    EXPECT_EQ(emp.regimeTributario(), "Simples Nacional");
    ASSERT_TRUE(emp.inscricaoEstadual().has_value());
    EXPECT_EQ(emp.inscricaoEstadual().value(), "110042490114");
    EXPECT_FALSE(emp.inscricaoMunicipal().has_value());
    EXPECT_EQ(emp.idEndereco(), 10);

    Endereco end("Rua A", "10", std::nullopt, "Centro", "80000000", 1);
    end.setId(99);
    emp.setEndereco(end);
    EXPECT_EQ(emp.idEndereco(), 99);
    EXPECT_EQ(emp.endereco().logradouro(), "Rua A");
}

TEST(EntitiesTest, PessoaAndPessoaContabilistaFromRecord) {
    QSqlRecord rec;
    addField(rec, "id", 7);
    addField(rec, "id_empresa", 1);
    addField(rec, "cnpj_cpf", QString("98765432000188"));
    addField(rec, "razao_social", QString("Contabilidade Modelo S/S"));
    addNullField(rec, "nome_fantasia");
    addNullField(rec, "inscricao_estadual");
    addField(rec, "fornecedor", QString("N"));
    addField(rec, "contabilista", QString("S"));
    addField(rec, "id_endereco", 15);
    addField(rec, "cont_id", 3);
    addField(rec, "cont_id_pessoa", 7);
    addField(rec, "cont_crc", QString(" 1SP123456/O-0 "));
    addField(rec, "cont_cpf", QString(" 12345678901 "));
    addField(rec, "end_id", 15);
    addField(rec, "end_logradouro", QString("Rua dos Contadores"));
    addField(rec, "end_bairro", QString("Centro"));
    addField(rec, "end_cep", QString("01001000"));
    addField(rec, "end_id_cidade", 10);

    Pessoa p = Pessoa::fromRecord(rec);
    ASSERT_TRUE(p.id().has_value());
    EXPECT_EQ(p.id().value(), 7);
    EXPECT_EQ(p.idEmpresa(), 1);
    EXPECT_EQ(p.cnpjCpf(), "98765432000188");
    EXPECT_EQ(p.razaoSocial(), "Contabilidade Modelo S/S");
    EXPECT_EQ(p.fornecedor(), "N");
    EXPECT_EQ(p.contabilista(), "S");
    EXPECT_EQ(p.textoExibicao(), "98765432000188 - Contabilidade Modelo S/S");

    ASSERT_TRUE(p.pessoaContabilista().has_value());
    EXPECT_EQ(p.pessoaContabilista()->id().value(), 3);
    EXPECT_EQ(p.pessoaContabilista()->crc(), "1SP123456/O-0");
    EXPECT_EQ(p.pessoaContabilista()->cpf(), "12345678901");

    EXPECT_EQ(p.idEndereco(), 15);
    EXPECT_EQ(p.endereco().logradouro(), "Rua dos Contadores");
}

TEST(EntitiesTest, UnidadeMedidaAndNCMFromRecord) {
    QSqlRecord unRec;
    addField(unRec, "id", 5);
    addField(unRec, "sigla", QString("CX"));
    addField(unRec, "descricao", QString("Caixa com 12 unidades"));

    UnidadeMedida um = UnidadeMedida::fromRecord(unRec);
    ASSERT_TRUE(um.id().has_value());
    EXPECT_EQ(um.id().value(), 5);
    EXPECT_EQ(um.sigla(), "CX");
    EXPECT_EQ(um.descricao(), "Caixa com 12 unidades");
    EXPECT_EQ(um.textoExibicao(), "CX - Caixa com 12 unidades");

    QSqlRecord ncmRec;
    addField(ncmRec, "id", 12);
    addField(ncmRec, "codigo", QString("84713012"));
    addField(ncmRec, "descricao", QString("Computadores portáteis"));
    addField(ncmRec, "aliquota_municipal", 0.0);
    addField(ncmRec, "aliquota_estadual", 18.0);
    addField(ncmRec, "aliquota_federal", 13.45);
    addField(ncmRec, "data_vigencia_inicio", QDate(2024, 1, 1));
    addField(ncmRec, "data_vigencia_fim", QDate(2026, 12, 31));

    NCM ncm = NCM::fromRecord(ncmRec);
    ASSERT_TRUE(ncm.id().has_value());
    EXPECT_EQ(ncm.id().value(), 12);
    EXPECT_EQ(ncm.codigo(), "84713012");
    EXPECT_EQ(ncm.descricao(), "Computadores portáteis");
    EXPECT_DOUBLE_EQ(ncm.aliquotaEstadual(), 18.0);
    EXPECT_DOUBLE_EQ(ncm.aliquotaFederal(), 13.45);
    EXPECT_EQ(ncm.textoExibicao(), "84713012 - Computadores portáteis");
}

TEST(EntitiesTest, ProdutoAndProdutoFornecedorFromRecord) {
    Produto prodPadrao("Teclado Mecânico", 1, 10);
    EXPECT_EQ(prodPadrao.tipoAtividade(), TipoProdutoAtividade::MercadoriaRevenda);
    EXPECT_FALSE(prodPadrao.codigoBarra().has_value());

    QSqlRecord prodRec;
    addField(prodRec, "id", 25);
    addField(prodRec, "descricao", QString("Monitor 27 Polegadas"));
    addField(prodRec, "codigo_barra", QString("7891234567890"));
    addField(prodRec, "id_unidade_medida", 1);
    addField(prodRec, "tipo_atividade", TipoProdutoAtividade::MercadoriaRevenda);
    addField(prodRec, "id_ncm", 10);
    addField(prodRec, "codigo_interno", QString("MON-27"));
    addNullField(prodRec, "id_anp", QMetaType::Int);

    Produto prod = Produto::fromRecord(prodRec);
    ASSERT_TRUE(prod.id().has_value());
    EXPECT_EQ(prod.id().value(), 25);
    EXPECT_EQ(prod.descricao(), "Monitor 27 Polegadas");
    ASSERT_TRUE(prod.codigoBarra().has_value());
    EXPECT_EQ(prod.codigoBarra().value(), "7891234567890");
    ASSERT_TRUE(prod.codigoInterno().has_value());
    EXPECT_EQ(prod.codigoInterno().value(), "MON-27");
    EXPECT_FALSE(prod.idAnp().has_value());

    QSqlRecord pfRec;
    addField(pfRec, "id", 8);
    addField(pfRec, "id_produto", 25);
    addField(pfRec, "id_fornecedor", 3);
    addField(pfRec, "codigo_fornecedor", QString("FORN-MON-01"));
    addField(pfRec, "id_unidade_medida", 2);
    addField(pfRec, "multiplicador", 12.0);
    addField(pfRec, "codigo_barra_caixa", QString("17891234567897"));

    ProdutoFornecedor pf = ProdutoFornecedor::fromRecord(pfRec);
    ASSERT_TRUE(pf.id().has_value());
    EXPECT_EQ(pf.id().value(), 8);
    EXPECT_EQ(pf.idProduto(), 25);
    EXPECT_EQ(pf.idFornecedor(), 3);
    EXPECT_EQ(pf.codigoFornecedor(), "FORN-MON-01");
    EXPECT_EQ(pf.idUnidadeMedida(), 2);
    EXPECT_DOUBLE_EQ(pf.multiplicador(), 12.0);
    ASSERT_TRUE(pf.codigoBarraCaixa().has_value());
    EXPECT_EQ(pf.codigoBarraCaixa().value(), "17891234567897");
}

TEST(EntitiesTest, DocumentoFiscalItemJsonTaxHelpers) {
    QJsonObject icmsJson{
        {"cst", "00"},
        {"vBC", 1000.0},
        {"pICMS", 18.0},
        {"vICMS", 180.0}
    };
    QJsonObject pisJson{{"cst", "01"}};
    QJsonObject cofinsJson{{"cst", "01"}};
    QJsonObject ipiJson{{"cst", "50"}};
    QJsonObject impostosJson{
        {"icms", icmsJson},
        {"pis", pisJson},
        {"cofins", cofinsJson},
        {"ipi", ipiJson}
    };
    QJsonObject dadosJson{{"impostos", impostosJson}};

    DocumentoFiscalItem item(10, 1, "P001", "Produto Teste", "UN", 2.0, 500.0, 1000.0, 0.0, dadosJson, "7890000000001");
    EXPECT_EQ(item.cstIcms(), "00");
    EXPECT_EQ(item.cstPis(), "01");
    EXPECT_EQ(item.cstCofins(), "01");
    EXPECT_EQ(item.cstIpi(), "50");
    EXPECT_DOUBLE_EQ(item.baseIcms(), 1000.0);
    EXPECT_DOUBLE_EQ(item.aliquotaIcms(), 18.0);
    EXPECT_DOUBLE_EQ(item.valorIcms(), 180.0);

    // Fallback CSOSN quando CST não está presente
    QJsonObject icmsSnJson{{"csosn", "102"}};
    QJsonObject dadosSnJson{{"impostos", QJsonObject{{"icms", icmsSnJson}}}};
    DocumentoFiscalItem itemSn(10, 2, "P002", "Produto Simples", "UN", 1.0, 100.0, 100.0, 0.0, dadosSnJson);
    EXPECT_EQ(itemSn.cstIcms(), "102");
    EXPECT_TRUE(itemSn.cstPis().isEmpty());
    EXPECT_DOUBLE_EQ(itemSn.baseIcms(), 0.0);
}

TEST(EntitiesTest, DocumentoFiscalArquivoAndTributoFromRecord) {
    QSqlRecord arqRec;
    addField(arqRec, "id", 101);
    addField(arqRec, "id_documento", 50);
    addField(arqRec, "conteudo_xml", QString("<NFe/>"));
    addField(arqRec, "tipo_arquivo", QString("DOCUMENTO_FISCAL"));

    DocumentoFiscalArquivo arq = DocumentoFiscalArquivo::fromRecord(arqRec);
    ASSERT_TRUE(arq.id().has_value());
    EXPECT_EQ(arq.id().value(), 101);
    EXPECT_EQ(arq.idDocumento(), 50);
    EXPECT_EQ(arq.conteudoXml(), "<NFe/>");
    EXPECT_EQ(arq.tipoArquivo(), "DOCUMENTO_FISCAL");

    QJsonObject tribDados{{"cst", "00"}, {"valor", "18.0000"}};
    QSqlRecord tribRec;
    addField(tribRec, "id", 300);
    addField(tribRec, "id_item", 90);
    addField(tribRec, "tipo_tributo", QString("icms"));
    addField(tribRec, "dados", QVariant::fromValue(tribDados));

    DocumentoFiscalItemTributo trib = DocumentoFiscalItemTributo::fromRecord(tribRec);
    ASSERT_TRUE(trib.id().has_value());
    EXPECT_EQ(trib.id().value(), 300);
    EXPECT_EQ(trib.idItem(), 90);
    EXPECT_EQ(trib.tipoTributo(), "icms");
}

TEST(EntitiesTest, VirtualEntitiesNotaTerceiroResumoAndItemFromRecord) {
    QDateTime emissao = QDateTime::fromString("2026-10-15T14:30:00", Qt::ISODate);

    QSqlRecord resRec;
    addField(resRec, "id_documento_fiscal", 77);
    addField(resRec, "chave_acesso", QString("35261012345678000199550010000001231000001230"));
    addField(resRec, "modelo", QString("55"));
    addField(resRec, "serie", QString("1"));
    addField(resRec, "numero", 123);
    addField(resRec, "dh_emissao", emissao);
    addField(resRec, "id_pessoa", 4);
    addField(resRec, "razao_social", QString("Fornecedor Industrial Ltda"));
    addField(resRec, "cnpj_cpf", QString("11222333000144"));
    addField(resRec, "total_itens", 5);
    addField(resRec, "itens_pendentes", 2);

    NotaTerceiroResumo resumo = NotaTerceiroResumo::fromRecord(resRec);
    EXPECT_EQ(resumo.idDocumentoFiscal(), 77);
    EXPECT_EQ(resumo.numero(), 123);
    EXPECT_EQ(resumo.idFornecedor(), 4);
    EXPECT_EQ(resumo.fornecedorNome(), "Fornecedor Industrial Ltda");
    EXPECT_EQ(resumo.fornecedorCnpj(), "11222333000144");
    EXPECT_EQ(resumo.totalItens(), 5);
    EXPECT_EQ(resumo.itensPendentes(), 2);

    // Item vinculado
    QSqlRecord itemVincRec;
    addField(itemVincRec, "id_item", 10);
    addField(itemVincRec, "id_documento_fiscal", 77);
    addField(itemVincRec, "numero_item", 1);
    addField(itemVincRec, "codigo_produto", QString("CF-01"));
    addField(itemVincRec, "codigo_barras", QString("7891112223334"));
    addField(itemVincRec, "descricao", QString("Cabo HDMI 2m"));
    addField(itemVincRec, "unidade_medida", QString("CX"));
    addField(itemVincRec, "quantidade", 2.0);
    addField(itemVincRec, "valor_unitario", 100.0);
    addField(itemVincRec, "valor_total", 200.0);
    addField(itemVincRec, "ncm", QString("85444200"));
    addField(itemVincRec, "id_produto_fornecedor", 15);
    addField(itemVincRec, "id_produto", 50);
    addField(itemVincRec, "multiplicador", 10.0);
    addField(itemVincRec, "id_unidade_interna", 1);
    addField(itemVincRec, "produto_codigo_interno", QString("INT-HDMI"));
    addField(itemVincRec, "produto_descricao", QString("Cabo HDMI 2.0 Interno"));
    addField(itemVincRec, "unidade_sigla", QString("UN"));

    NotaTerceiroItem itemVinc = NotaTerceiroItem::fromRecord(itemVincRec);
    EXPECT_TRUE(itemVinc.vinculado());
    ASSERT_TRUE(itemVinc.idProdutoFornecedor().has_value());
    EXPECT_EQ(itemVinc.idProdutoFornecedor().value(), 15);
    ASSERT_TRUE(itemVinc.idProduto().has_value());
    EXPECT_EQ(itemVinc.idProduto().value(), 50);
    EXPECT_DOUBLE_EQ(itemVinc.multiplicador(), 10.0);
    EXPECT_EQ(itemVinc.produtoCodigoInterno(), "INT-HDMI");
    EXPECT_EQ(itemVinc.siglaUnidadeMedidaInterna(), "UN");

    // Item pendente (id_produto_fornecedor NULL)
    QSqlRecord itemPendRec;
    addField(itemPendRec, "id", 11);
    addField(itemPendRec, "id_documento_fiscal", 77);
    addField(itemPendRec, "numero_item", 2);
    addField(itemPendRec, "codigo_produto", QString("CF-02"));
    addNullField(itemPendRec, "id_produto_fornecedor", QMetaType::Int);

    NotaTerceiroItem itemPend = NotaTerceiroItem::fromRecord(itemPendRec);
    EXPECT_EQ(itemPend.idItem(), 11);
    EXPECT_FALSE(itemPend.vinculado());
    EXPECT_FALSE(itemPend.idProdutoFornecedor().has_value());
    EXPECT_DOUBLE_EQ(itemPend.multiplicador(), 1.0);
}

