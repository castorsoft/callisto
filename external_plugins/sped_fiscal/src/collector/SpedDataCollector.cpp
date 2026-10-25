#include "SpedDataCollector.h"
#include <QSet>
#include <QJsonObject>
#include <QJsonValue>

SpedCollectedData SpedDataCollector::collect(
    IPluginContext &context,
    const QDate &dataInicio,
    const QDate &dataFim,
    int finalidade,
    const QString &perfil,
    const SpedContadorData &contador)
{
    SpedCollectedData result;
    result.dataInicio = dataInicio;
    result.dataFim = dataFim;
    result.finalidade = finalidade;
    result.perfil = perfil;
    result.contador = contador;

    // 1. Dados da Empresa
    auto optEmpresa = context.empresaAtual();
    if (optEmpresa.has_value()) {
        const auto &emp = optEmpresa.value();
        result.empresa.cnpjCpf = emp.cnpjCpf();
        result.empresa.razaoSocial = emp.razaoSocial();
        result.empresa.nomeFantasia = emp.nomeFantasia().value_or("");
        result.empresa.inscricaoEstadual = emp.inscricaoEstadual().value_or("");
        result.empresa.perfilSped = perfil;
    }

    // 2. Documentos Fiscais do Período
    QList<DocumentoFiscal> docs = context.documentoFiscalRepository().listarPorPeriodo(dataInicio, dataFim, true);

    QSet<int> pessoasIds;
    QSet<QString> unidadesSet;
    QMap<QString, SpedItemData> itensMap;

    for (const auto &doc : docs) {
        SpedDocumentoFiscalData docData;

        docData.indicadorOperacao = (doc.tipoOperacao() == "saida") ? 1 : 0;
        docData.indicadorEmissao = (doc.tipoEmissao() == "propria") ? 0 : 1;
        docData.codigoModelo = doc.modelo().isEmpty() ? "55" : doc.modelo();
        docData.serie = doc.serie();
        docData.numero = doc.numero();
        docData.chaveAcesso = doc.chaveAcesso();
        docData.dataEmissao = doc.dhEmissao().date();
        docData.dataEntradaSaida = doc.dhEmissao().date();

        if (doc.idPessoa().has_value() && doc.idPessoa().value() > 0) {
            int idPessoa = doc.idPessoa().value();
            pessoasIds.insert(idPessoa);
            docData.codigoParticipante = QString::number(idPessoa);
        }

        // Totais e detalhes do JSON da nota (se houver)
        QJsonObject dadosDoc = doc.dados();
        if (dadosDoc.contains("vNF")) docData.valorTotalDocumento = dadosDoc.value("vNF").toDouble();
        if (dadosDoc.contains("vProd")) docData.valorMercadorias = dadosDoc.value("vProd").toDouble();
        if (dadosDoc.contains("vDesc")) docData.valorDesconto = dadosDoc.value("vDesc").toDouble();
        if (dadosDoc.contains("vFrete")) docData.valorFrete = dadosDoc.value("vFrete").toDouble();
        if (dadosDoc.contains("vSeg")) docData.valorSeguro = dadosDoc.value("vSeg").toDouble();
        if (dadosDoc.contains("vOutro")) docData.outrasDespesas = dadosDoc.value("vOutro").toDouble();
        if (dadosDoc.contains("vBC")) docData.baseCalculoIcms = dadosDoc.value("vBC").toDouble();
        if (dadosDoc.contains("vICMS")) docData.valorIcms = dadosDoc.value("vICMS").toDouble();
        if (dadosDoc.contains("vBCST")) docData.baseCalculoIcmsSt = dadosDoc.value("vBCST").toDouble();
        if (dadosDoc.contains("vST")) docData.valorIcmsSt = dadosDoc.value("vST").toDouble();
        if (dadosDoc.contains("vIPI")) docData.valorIpi = dadosDoc.value("vIPI").toDouble();
        if (dadosDoc.contains("vPIS")) docData.valorPis = dadosDoc.value("vPIS").toDouble();
        if (dadosDoc.contains("vCOFINS")) docData.valorCofins = dadosDoc.value("vCOFINS").toDouble();

        // Processa Itens
        double somaItens = 0.0;
        int numItem = 1;
        for (const auto &item : doc.itens()) {
            SpedDocumentoItemData itemData;
            itemData.numeroItem = (item.numeroItem() > 0) ? item.numeroItem() : numItem;
            itemData.codigoItem = item.codigoProduto();
            itemData.quantidade = item.quantidade();
            itemData.unidadeMedida = item.unidadeMedida().isEmpty() ? "UN" : item.unidadeMedida().toUpper();
            itemData.valorTotal = item.valorTotal();
            itemData.valorDesconto = item.valorDesconto();

            somaItens += itemData.valorTotal;

            // Extrai tributos do item
            QJsonObject dadosItem = item.dados();
            if (dadosItem.contains("cfop")) itemData.cfop = dadosItem.value("cfop").toString();
            if (dadosItem.contains("cst")) itemData.cstIcms = dadosItem.value("cst").toString();
            if (dadosItem.contains("cst_icms")) itemData.cstIcms = dadosItem.value("cst_icms").toString();
            if (dadosItem.contains("vBC")) itemData.baseCalculoIcms = dadosItem.value("vBC").toDouble();
            if (dadosItem.contains("pICMS")) itemData.aliquotaIcms = dadosItem.value("pICMS").toDouble();
            if (dadosItem.contains("vICMS")) itemData.valorIcms = dadosItem.value("vICMS").toDouble();

            docData.itens.push_back(itemData);

            // Coleta para os blocos cadastrais
            unidadesSet.insert(itemData.unidadeMedida);

            if (!itensMap.contains(itemData.codigoItem)) {
                SpedItemData cadItem;
                cadItem.codigoItem = itemData.codigoItem;
                cadItem.descricao = item.descricao();
                cadItem.codigoBarra = item.codigoBarras();
                cadItem.unidadeMedida = itemData.unidadeMedida;
                if (dadosItem.contains("ncm")) cadItem.ncm = dadosItem.value("ncm").toString();
                cadItem.aliquotaIcms = itemData.aliquotaIcms;
                itensMap.insert(cadItem.codigoItem, cadItem);
            }

            numItem++;
        }

        if (docData.valorMercadorias <= 0.0) {
            docData.valorMercadorias = somaItens;
        }
        if (docData.valorTotalDocumento <= 0.0) {
            docData.valorTotalDocumento = somaItens - docData.valorDesconto + docData.valorFrete + docData.outrasDespesas;
        }

        result.documentos.append(docData);
    }

    // 3. Participantes (Bloco 0150)
    for (int idPessoa : pessoasIds) {
        auto optPessoa = context.pessoaRepository().buscarPorId(idPessoa);
        if (optPessoa.has_value()) {
            const auto &p = optPessoa.value();
            SpedParticipanteData part;
            part.codigoParticipante = QString::number(p.id().value_or(idPessoa));
            part.nome = p.razaoSocial();
            part.cnpjCpf = p.cnpjCpf();
            part.inscricaoEstadual = p.inscricaoEstadual().value_or("");
            result.participantes.append(part);
        }
    }

    // 4. Unidades de Medida (Bloco 0190)
    for (const QString &sigla : unidadesSet) {
        SpedUnidadeData un;
        un.sigla = sigla;
        auto optUn = context.unidadeMedidaRepository().buscarPorSigla(sigla);
        if (optUn.has_value()) {
            un.descricao = optUn.value().descricao();
        } else {
            un.descricao = sigla;
        }
        result.unidades.append(un);
    }

    // 5. Itens / Produtos (Bloco 0200)
    result.itens = itensMap.values();

    return result;
}

