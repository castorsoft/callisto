#pragma once

#include <QDialog>
#include "../../../core/entities/Produto.h"
#include "../../../core/interfaces/IDocumentoFiscalRepository.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class DialogNovoProdutoRapido;
}
QT_END_NAMESPACE

class DialogNovoProdutoRapido : public QDialog {
    Q_OBJECT

public:
    explicit DialogNovoProdutoRapido(int idFornecedor,
                                     const QString &nomeFornecedor,
                                     const ItemNotaTerceiroInfo &itemInfo,
                                     const QString &ncmSugerido = QString(),
                                     QWidget *parent = nullptr);
    ~DialogNovoProdutoRapido() override;

    int idProdutoCriado() const { return m_idProdutoCriado; }
    double multiplicador() const;
    int idUnidadeMedida() const;

private slots:
    void onSalvarClicked();

private:
    void carregarCombos();
    int obterOuCriarUnidadeMedida(const QString &sigla);
    int obterOuCriarNCM(const QString &codigo);

    Ui::DialogNovoProdutoRapido *ui;

    int m_idFornecedor{0};
    ItemNotaTerceiroInfo m_itemInfo;
    int m_idProdutoCriado{-1};
};

