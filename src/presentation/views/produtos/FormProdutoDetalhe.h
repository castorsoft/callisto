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

    void preencherDadosIniciais(const QString &descricao,
                                const QString &codigoBarras = QString(),
                                const QString &siglaUnidade = QString(),
                                const QString &codigoNcm = QString());

    bool salvo() const { return m_salvo; }
    std::optional<Produto> produtoAtual() const { return m_produtoAtual; }

signals:
    void produtoSalvo(const Produto &produto);

private:
    Ui::FormProdutoDetalhe *ui;
    bool m_salvo{false};

protected slots:
    void onSalvar() override;

private slots:
    void buscarNCM(QString aTermo);
    void buscarUnMedida(QString aTermo);

protected:
    bool validar() override;
    void carregarDados() override;

private:
    std::optional<Produto> m_produtoAtual;
};

#endif // FormProdutoDetalheH
