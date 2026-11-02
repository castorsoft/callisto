#include <gtest/gtest.h>
#include <QColor>
#include <QBrush>
#include <QFont>

#include "core/entities/virtual/NotaTerceiroResumo.h"
#include "core/entities/virtual/NotaTerceiroItem.h"
#include "presentation/views/providers/NotaTerceiroListProvider.h"
#include "presentation/views/providers/NotaTerceiroItemListProvider.h"
#include "presentation/components/providers/TableDataProviderAdapter.h"

TEST(ProvidersTest, NotaTerceiroListProviderReturnsCorrectDataAndRoles) {
    NotaTerceiroResumo n1;
    n1.setIdDocumentoFiscal(10);
    n1.setNumero(1234);
    n1.setSerie("1");
    n1.setDataEmissao(QDateTime::fromString("2026-10-20T14:30:00", Qt::ISODate));
    n1.setFornecedorNome("Fornecedor Alpha");
    n1.setFornecedorCnpj("12345678000199");
    n1.setChaveAcesso("35261012345678000199550010000012341000001234");
    n1.setItensPendentes(2);
    n1.setTotalItens(5);

    NotaTerceiroResumo n2;
    n2.setIdDocumentoFiscal(20);
    n2.setNumero(5678);
    n2.setSerie("2");
    n2.setDataEmissao(QDateTime::fromString("2026-10-21T09:15:00", Qt::ISODate));
    n2.setFornecedorNome("Fornecedor Beta");
    n2.setFornecedorCnpj("98765432000188");
    n2.setChaveAcesso("35261098765432000188550020000056781000005678");
    n2.setItensPendentes(0);
    n2.setTotalItens(3);

    QList<NotaTerceiroResumo> notas{n1, n2};
    NotaTerceiroListProvider provider(notas);

    EXPECT_EQ(provider.rowCount(), 2);
    EXPECT_EQ(provider.columnCount(), 8);
    EXPECT_EQ(provider.headerData(1), "NF-e / Série");
    EXPECT_EQ(provider.headerData(7), "Status");

    // Test row 0 (Pendente)
    EXPECT_EQ(provider.data(0, 0).toString(), "1");
    EXPECT_EQ(provider.data(0, 1).toString(), "1234 / 1");
    EXPECT_EQ(provider.data(0, 2).toString(), "20/10/2026 14:30");
    EXPECT_EQ(provider.data(0, 3).toString(), "Fornecedor Alpha");
    EXPECT_EQ(provider.data(0, 4).toString(), "12345678000199");
    EXPECT_EQ(provider.data(0, 5).toString(), "35261012345678000199550010000012341000001234");
    EXPECT_EQ(provider.data(0, 6).toString(), "2 / 5");
    EXPECT_EQ(provider.data(0, 7).toString(), "Pendente");

    // Tooltip on chave
    EXPECT_EQ(provider.data(0, 5, Qt::ToolTipRole).toString(), "35261012345678000199550010000012341000001234");

    // Colors on status: Pendente = orange (#E65100), Concluído = green (#2E7D32)
    QBrush brushPendente = provider.data(0, 7, Qt::ForegroundRole).value<QBrush>();
    EXPECT_EQ(brushPendente.color().name().toUpper(), "#E65100");

    QBrush brushConcluido = provider.data(1, 7, Qt::ForegroundRole).value<QBrush>();
    EXPECT_EQ(brushConcluido.color().name().toUpper(), "#2E7D32");

    // Alignment role
    EXPECT_EQ(provider.data(0, 1, Qt::TextAlignmentRole).toInt(), static_cast<int>(Qt::AlignCenter));
    EXPECT_EQ(provider.data(0, 3, Qt::TextAlignmentRole).toInt(), static_cast<int>(Qt::AlignLeft | Qt::AlignVCenter));

    // Find row by id
    EXPECT_EQ(provider.findRowById(20), 1);
    EXPECT_EQ(provider.findRowById(999), -1);

    // Get entity
    auto optNota = provider.getNotaAt(0);
    ASSERT_TRUE(optNota.has_value());
    EXPECT_EQ(optNota->idDocumentoFiscal(), 10);
    EXPECT_FALSE(provider.getNotaAt(99).has_value());

    // Widths and resize modes
    EXPECT_TRUE(provider.columnWidth(1).has_value());
    EXPECT_EQ(provider.columnResizeMode(3).value(), QHeaderView::Stretch);
}

TEST(ProvidersTest, NotaTerceiroItemListProviderReturnsCorrectDataAndRoles) {
    NotaTerceiroItem itemVinc;
    itemVinc.setIdItem(100);
    itemVinc.setNumeroItem(1);
    itemVinc.setCodigoProduto("COD-001");
    itemVinc.setDescricao("Fonte ATX 600W");
    itemVinc.setCodigoBarras("7891234567890");
    itemVinc.setUnidadeMedida("CX");
    itemVinc.setVinculado(true);
    itemVinc.setIdProduto(50);
    itemVinc.setProdutoCodigoInterno("FONTE-600");
    itemVinc.setProdutoDescricao("Fonte Real 600W");
    itemVinc.setMultiplicador(10.0);
    itemVinc.setSiglaUnidadeMedidaInterna("UN");

    NotaTerceiroItem itemPend;
    itemPend.setIdItem(101);
    itemPend.setNumeroItem(2);
    itemPend.setCodigoProduto("COD-002");
    itemPend.setDescricao("Cabo de Forca 1.5m");
    itemPend.setCodigoBarras("");
    itemPend.setUnidadeMedida("PC");
    itemPend.setVinculado(false);

    QList<NotaTerceiroItem> itens{itemVinc, itemPend};
    NotaTerceiroItemListProvider provider(itens);

    EXPECT_EQ(provider.rowCount(), 2);
    EXPECT_EQ(provider.columnCount(), 8);

    // Item vinculado
    EXPECT_EQ(provider.data(0, 0).toString(), "1");
    EXPECT_EQ(provider.data(0, 1).toString(), "COD-001");
    EXPECT_EQ(provider.data(0, 2).toString(), "Fonte ATX 600W");
    EXPECT_EQ(provider.data(0, 3).toString(), "7891234567890");
    EXPECT_EQ(provider.data(0, 4).toString(), "CX");
    EXPECT_EQ(provider.data(0, 5).toString(), "Vinculado");
    EXPECT_EQ(provider.data(0, 6).toString(), "[FONTE-600] Fonte Real 600W");
    EXPECT_EQ(provider.data(0, 7).toString(), "x10.00 UN");

    QBrush brushVinc = provider.data(0, 5, Qt::ForegroundRole).value<QBrush>();
    EXPECT_EQ(brushVinc.color().name().toUpper(), "#2E7D32");

    // Item pendente
    EXPECT_EQ(provider.data(1, 0).toString(), "2");
    EXPECT_EQ(provider.data(1, 3).toString(), "-");
    EXPECT_EQ(provider.data(1, 5).toString(), "Pendente");
    EXPECT_EQ(provider.data(1, 6).toString(), "Nenhum produto vinculado");
    EXPECT_EQ(provider.data(1, 7).toString(), "-");

    QBrush brushPend = provider.data(1, 5, Qt::ForegroundRole).value<QBrush>();
    EXPECT_EQ(brushPend.color().name().toUpper(), "#E65100");

    QBrush brushCinza = provider.data(1, 6, Qt::ForegroundRole).value<QBrush>();
    EXPECT_EQ(brushCinza.color().name().toUpper(), "#757575");

    auto optItem = provider.getItemAt(0);
    ASSERT_TRUE(optItem.has_value());
    EXPECT_EQ(optItem->idItem(), 100);
}

TEST(ProvidersTest, TableDataProviderAdapterPropagatesCustomRoles) {
    NotaTerceiroResumo nota;
    nota.setIdDocumentoFiscal(1);
    nota.setNumero(999);
    nota.setSerie("1");
    nota.setItensPendentes(1);
    nota.setTotalItens(1);

    auto provider = std::make_shared<NotaTerceiroListProvider>(QList<NotaTerceiroResumo>{nota});
    TableDataProviderAdapter adapter(provider);

    QModelIndex idxStatus = adapter.index(0, 7);
    EXPECT_EQ(adapter.data(idxStatus, Qt::DisplayRole).toString(), "Pendente");

    QBrush brush = adapter.data(idxStatus, Qt::ForegroundRole).value<QBrush>();
    EXPECT_EQ(brush.color().name().toUpper(), "#E65100");

    QFont font = adapter.data(idxStatus, Qt::FontRole).value<QFont>();
    EXPECT_TRUE(font.bold());
}

