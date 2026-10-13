#pragma once

#include <QDialog>
#include <QList>
#include "../../../core/entities/Produto.h"
#include "../../../core/interfaces/IDocumentoFiscalRepository.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class DialogSelecionarProduto;
}
QT_END_NAMESPACE

class DialogSelecionarProduto : public QDialog {
    Q_OBJECT

public:
    explicit DialogSelecionarProduto(int idFornecedor,
                                    const QString &nomeFornecedor,
                                    const ItemNotaTerceiroInfo &itemInfo,
                                    QWidget *parent = nullptr);
    ~DialogSelecionarProduto() override;

    int idProdutoSelecionado() const { return m_idProdutoSelecionado; }
    double multiplicador() const;
    int idUnidadeMedida() const;

private slots:
    void onBuscaChanged(const QString &texto);
    void onTabelaSelectionChanged();
    void onTabelaDoubleClicked(int row, int col);
    void onConfirmarClicked();

private:
    void configurarTabela();
    void carregarUnidades();
    void carregarProdutos(const QString &filtro = QString());

    Ui::DialogSelecionarProduto *ui;

    int m_idFornecedor{0};
    ItemNotaTerceiroInfo m_itemInfo;
    QList<Produto> m_produtosFiltrados;
    int m_idProdutoSelecionado{-1};
};

