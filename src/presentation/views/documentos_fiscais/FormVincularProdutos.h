#pragma once

#include <QWidget>
#include <QList>
#include <memory>
#include <optional>
#include <QItemSelection>
#include "../../../core/interfaces/IDocumentoFiscalRepository.h"
#include "../providers/NotaTerceiroListProvider.h"
#include "../providers/NotaTerceiroItemListProvider.h"
#include "../../components/providers/TableDataProviderAdapter.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class FormVincularProdutos;
}
QT_END_NAMESPACE

class QSortFilterProxyModel;

class FormVincularProdutos : public QWidget {
    Q_OBJECT

public:
    explicit FormVincularProdutos(QWidget *parent = nullptr);
    ~FormVincularProdutos() override;

private slots:
    void onFiltroChanged(int index);
    void onBuscaChanged(const QString &texto);
    void onRecarregarClicked();
    void onNotaSelectionChanged();
    void onItemSelectionChanged();
    void onVincularExistenteClicked();
    void onNovoProdutoClicked();
    void onDesvincularClicked();

private:
    void configurarTabelas();
    void carregarNotas();
    void carregarItensDaNotaSelecionada();
    void atualizarBotoesAcaoItem();

    void aplicarConfiguracoesTabelaNotas();
    void aplicarConfiguracoesTabelaItens();

    std::optional<NotaTerceiroResumo> notaSelecionada() const;
    std::optional<NotaTerceiroItem> itemSelecionado() const;

    Ui::FormVincularProdutos *ui;

    std::shared_ptr<NotaTerceiroListProvider> m_notasProvider;
    TableDataProviderAdapter *m_notasAdapter{nullptr};
    QSortFilterProxyModel *m_notasProxy{nullptr};

    std::shared_ptr<NotaTerceiroItemListProvider> m_itensProvider;
    TableDataProviderAdapter *m_itensAdapter{nullptr};

    int m_idNotaSelecionada{-1};
    int m_idFornecedorNotaAtual{-1};
    QString m_nomeFornecedorNotaAtual;
};
