#ifndef FormProdutoDetalheH
#define FormProdutoDetalheH

#include <QWidget>
#include "../../components/FormDetalheBase.h"
#include "../../../core/entities/Produto.h"

namespace Ui {
class FormProdutoDetalhe;
}

class FormProdutoDetalhe : public FormDetalheBase
{
    Q_OBJECT

public:
    explicit FormProdutoDetalhe(QWidget *parent = nullptr);
    ~FormProdutoDetalhe();

private:
    Ui::FormProdutoDetalhe *ui;

protected slots:
    void onSalvar() override;

private slots:
    void buscarNCM(QString aTermo);
    void buscarUnMedida(QString aTermo);
    // void atualizarVisibilidadeAbas();

protected:
    bool validar() override;
    void carregarDados() override;

private:
    std::optional<Produto> m_ProdutoAtual;
};

#endif // FormProdutoDetalheH
