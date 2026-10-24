#include "SpedBlockBuilder.h"

#include "../records/bloco0/Registro0000.h"
#include "../records/bloco0/Registro0100.h"
#include "../records/bloco0/Registro0200.h"
#include "../records/blocoC/RegistroC100.h"
#include "../records/blocoC/RegistroC170.h"
#include "../records/blocoVazio/RegistroBlocoVazio.h"
#include <QMap>

std::vector<std::shared_ptr<SpedRecordBase>> SpedBlockBuilder::build(const SpedCollectedData &data)
{
    std::vector<std::shared_ptr<SpedRecordBase>> records;

    // ==========================================
    // BLOCO 0: ABERTURA, IDENTIFICAÇÃO E CADASTROS
    // ==========================================
    int inicioBloco0 = records.size();

    // 0000: Abertura do Arquivo
    auto r0000 = std::make_shared<Registro0000>();
    r0000->codFin = data.finalidade;
    r0000->dtIni = data.dataInicio;
    r0000->dtFin = data.dataFim;
    r0000->nome = data.empresa.razaoSocial;
    r0000->cnpj = data.empresa.cnpjCpf;
    r0000->ie = data.empresa.inscricaoEstadual;
    r0000->codMun = data.empresa.codigoMunicipioIbge;
    r0000->indPerfil = data.perfil;
    r0000->indAtiv = data.empresa.indicadorAtividade;
    records.push_back(r0000);

    // 0001: Abertura do Bloco 0
    records.push_back(std::make_shared<Registro0001>(0));

    // 0005: Dados Complementares da Entidade
    if (!data.empresa.nomeFantasia.isEmpty() || !data.empresa.logradouro.isEmpty()) {
        auto r0005 = std::make_shared<Registro0005>();
        r0005->fantasia = data.empresa.nomeFantasia;
        r0005->cep = data.empresa.cep;
        r0005->endereco = data.empresa.logradouro;
        r0005->num = data.empresa.numero;
        r0005->complemento = data.empresa.complemento;
        r0005->bairro = data.empresa.bairro;
        r0005->fone = data.empresa.telefone;
        r0005->email = data.empresa.email;
        records.push_back(r0005);
    }

    // 0100: Dados do Contabilista
    if (!data.contador.nome.isEmpty() || !data.contador.cpf.isEmpty()) {
        auto r0100 = std::make_shared<Registro0100>();
        r0100->nome = data.contador.nome;
        r0100->cpf = data.contador.cpf;
        r0100->crc = data.contador.crc;
        r0100->cnpj = data.contador.cnpj;
        r0100->cep = data.contador.cep;
        r0100->endereco = data.contador.logradouro;
        r0100->num = data.contador.numero;
        r0100->complemento = data.contador.complemento;
        r0100->bairro = data.contador.bairro;
        r0100->fone = data.contador.telefone;
        r0100->email = data.contador.email;
        r0100->codMun = data.contador.codigoMunicipioIbge;
        records.push_back(r0100);
    }

    // 0150: Tabela de Cadastro do Participante
    for (const auto &p : data.participantes) {
        auto r0150 = std::make_shared<Registro0150>();
        r0150->codPart = p.codigoParticipante;
        r0150->nome = p.nome;
        if (p.cnpjCpf.length() > 11) {
            r0150->cnpj = p.cnpjCpf;
        } else {
            r0150->cpf = p.cnpjCpf;
        }
        r0150->ie = p.inscricaoEstadual;
        r0150->codMun = p.codigoMunicipioIbge;
        r0150->endereco = p.logradouro;
        r0150->num = p.numero;
        r0150->complemento = p.complemento;
        r0150->bairro = p.bairro;
        records.push_back(r0150);
    }

    // 0190: Tabela de Unidades de Medida
    for (const auto &u : data.unidades) {
        auto r0190 = std::make_shared<Registro0190>();
        r0190->unid = u.sigla;
        r0190->descr = u.descricao;
        records.push_back(r0190);
    }

    // 0200: Tabela de Identificação do Item (Produtos)
    for (const auto &it : data.itens) {
        auto r0200 = std::make_shared<Registro0200>();
        r0200->codItem = it.codigoItem;
        r0200->descrItem = it.descricao;
        r0200->codBarra = it.codigoBarra;
        r0200->unidInv = it.unidadeMedida;
        r0200->tipoItem = it.tipoItem;
        r0200->codNcm = it.ncm;
        r0200->aliqIcms = it.aliquotaIcms;
        records.push_back(r0200);
    }

    // 0990: Encerramento do Bloco 0 (quantidade incluindo o próprio 0990)
    int qtdLinhas0 = (records.size() - inicioBloco0) + 1;
    records.push_back(std::make_shared<Registro0990>(qtdLinhas0));

    // ==========================================
    // BLOCO C: DOCUMENTOS FISCAIS I - MERCADORIAS
    // ==========================================
    int inicioBlocoC = records.size();

    bool temDocumentos = !data.documentos.isEmpty();
    records.push_back(std::make_shared<RegistroC001>(temDocumentos ? 0 : 1));

    if (temDocumentos) {
        for (const auto &doc : data.documentos) {
            // C100: Nota Fiscal
            auto rC100 = std::make_shared<RegistroC100>();
            rC100->indOper = doc.indicadorOperacao;
            rC100->indEmit = doc.indicadorEmissao;
            rC100->codPart = doc.codigoParticipante;
            rC100->codMod = doc.codigoModelo;
            rC100->codSit = doc.codigoSituacao;
            rC100->ser = doc.serie;
            rC100->numDoc = doc.numero;
            rC100->chvNfe = doc.chaveAcesso;
            rC100->dtDoc = doc.dataEmissao;
            rC100->dtES = doc.dataEntradaSaida;
            rC100->vlDoc = doc.valorTotalDocumento;
            rC100->indPgto = doc.indicadorPagamento;
            rC100->vlDesc = doc.valorDesconto;
            rC100->vlAbatNt = doc.valorAbatimento;
            rC100->vlMerc = doc.valorMercadorias;
            rC100->indFrt = doc.indicadorFrete;
            rC100->vlFrt = doc.valorFrete;
            rC100->vlSeg = doc.valorSeguro;
            rC100->vlOutDa = doc.outrasDespesas;
            rC100->vlBcIcms = doc.baseCalculoIcms;
            rC100->vlIcms = doc.valorIcms;
            rC100->vlBcIcmsSt = doc.baseCalculoIcmsSt;
            rC100->vlIcmsSt = doc.valorIcmsSt;
            rC100->vlIpi = doc.valorIpi;
            rC100->vlPis = doc.valorPis;
            rC100->vlCofins = doc.valorCofins;
            records.push_back(rC100);

            // C170: Itens do Documento Fiscal e Acumulador para C190
            struct AnaliticoKey {
                QString cst;
                QString cfop;
                double aliq{0.0};

                bool operator<(const AnaliticoKey &other) const {
                    if (cst != other.cst) return cst < other.cst;
                    if (cfop != other.cfop) return cfop < other.cfop;
                    return aliq < other.aliq;
                }
            };

            struct AnaliticoValores {
                double vlOpr{0.0};
                double vlBcIcms{0.0};
                double vlIcms{0.0};
                double vlBcIcmsSt{0.0};
                double vlIcmsSt{0.0};
                double vlIpi{0.0};
            };

            QMap<AnaliticoKey, AnaliticoValores> analiticoMap;

            for (const auto &it : doc.itens) {
                auto rC170 = std::make_shared<RegistroC170>();
                rC170->numItem = it.numeroItem;
                rC170->codItem = it.codigoItem;
                rC170->descrCompl = it.descricaoComplementar;
                rC170->qtd = it.quantidade;
                rC170->unid = it.unidadeMedida;
                rC170->vlItem = it.valorTotal;
                rC170->vlDesc = it.valorDesconto;
                rC170->indMov = it.indicadorMovimentacaoFisica;
                rC170->cstIcms = it.cstIcms;
                rC170->cfop = it.cfop;
                rC170->vlBcIcms = it.baseCalculoIcms;
                rC170->aliqIcms = it.aliquotaIcms;
                rC170->vlIcms = it.valorIcms;
                rC170->vlBcIcmsSt = it.baseCalculoIcmsSt;
                rC170->aliqSt = it.aliquotaIcmsSt;
                rC170->vlIcmsSt = it.valorIcmsSt;
                rC170->cstIpi = it.cstIpi;
                rC170->vlBcIpi = it.baseCalculoIpi;
                rC170->aliqIpi = it.aliquotaIpi;
                rC170->vlIpi = it.valorIpi;
                rC170->cstPis = it.cstPis;
                rC170->vlBcPis = it.baseCalculoPis;
                rC170->aliqPis = it.aliquotaPis;
                rC170->vlPis = it.valorPis;
                rC170->cstCofins = it.cstCofins;
                rC170->vlBcCofins = it.baseCalculoCofins;
                rC170->aliqCofins = it.aliquotaCofins;
                rC170->vlCofins = it.valorCofins;
                records.push_back(rC170);

                // Acumula no Analítico C190
                AnaliticoKey key{it.cstIcms, it.cfop, it.aliquotaIcms};
                auto &val = analiticoMap[key];
                val.vlOpr += it.valorTotal;
                val.vlBcIcms += it.baseCalculoIcms;
                val.vlIcms += it.valorIcms;
                val.vlBcIcmsSt += it.baseCalculoIcmsSt;
                val.vlIcmsSt += it.valorIcmsSt;
                val.vlIpi += it.valorIpi;
            }

            // C190: Registro Analítico da NF-e
            for (auto it = analiticoMap.constBegin(); it != analiticoMap.constEnd(); ++it) {
                auto rC190 = std::make_shared<RegistroC190>();
                rC190->cstIcms = it.key().cst;
                rC190->cfop = it.key().cfop;
                rC190->aliqIcms = it.key().aliq;
                rC190->vlOpr = it.value().vlOpr;
                rC190->vlBcIcms = it.value().vlBcIcms;
                rC190->vlIcms = it.value().vlIcms;
                rC190->vlBcIcmsSt = it.value().vlBcIcmsSt;
                rC190->vlIcmsSt = it.value().vlIcmsSt;
                rC190->vlIpi = it.value().vlIpi;
                records.push_back(rC190);
            }
        }
    }

    // C990: Encerramento do Bloco C
    int qtdLinhasC = (records.size() - inicioBlocoC) + 1;
    records.push_back(std::make_shared<RegistroC990>(qtdLinhasC));

    // ==========================================
    // BLOCOS SEM MOVIMENTO (IND_MOV = 1)
    // ==========================================
    // Bloco D
    records.push_back(std::make_shared<RegistroAberturaVazio>("D001"));
    records.push_back(std::make_shared<RegistroEncerramentoVazio>("D990", 2));

    // Bloco E
    records.push_back(std::make_shared<RegistroAberturaVazio>("E001"));
    records.push_back(std::make_shared<RegistroEncerramentoVazio>("E990", 2));

    // Bloco G
    records.push_back(std::make_shared<RegistroAberturaVazio>("G001"));
    records.push_back(std::make_shared<RegistroEncerramentoVazio>("G990", 2));

    // Bloco H
    records.push_back(std::make_shared<RegistroAberturaVazio>("H001"));
    records.push_back(std::make_shared<RegistroEncerramentoVazio>("H990", 2));

    // Bloco 1
    records.push_back(std::make_shared<RegistroAberturaVazio>("1001"));
    records.push_back(std::make_shared<RegistroEncerramentoVazio>("1990", 2));

    return records;
}
