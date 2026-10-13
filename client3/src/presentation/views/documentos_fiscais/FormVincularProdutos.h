#pragma once

#include <QWidget>
#include <QList>
#include "../../../core/interfaces/IDocumentoFiscalRepository.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class FormVincularProdutos;
}
QT_END_NAMESPACE

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

    Ui::FormVincularProdutos *ui;

    QList<NotaTerceiroResumo> m_todasNotas;
    QList<NotaTerceiroResumo> m_notasFiltradas;
    QList<ItemNotaTerceiroInfo> m_itensNotaAtual;

    int m_idNotaSelecionada{-1};
    int m_idFornecedorNotaAtual{-1};
    QString m_nomeFornecedorNotaAtual;
};

