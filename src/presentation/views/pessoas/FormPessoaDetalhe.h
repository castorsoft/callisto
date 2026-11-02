#ifndef FormPessoaDetalheH
#define FormPessoaDetalheH

#include <QWidget>
#include "../../components/FormDetalheBase.h"
#include "../../../core/entities/Pessoa.h"
#include "../../../core/interfaces/ICnpjService.h"

namespace Ui {
class FormPessoaDetalhe;
}

class FormPessoaDetalhe : public FormDetalheBase
{
    Q_OBJECT

public:
    explicit FormPessoaDetalhe(QWidget *parent = nullptr, ICnpjService *cnpjService = nullptr);
    ~FormPessoaDetalhe();

private:
    Ui::FormPessoaDetalhe *ui;

protected slots:
    void onSalvar() override;

private slots:
    void onPessoaChange(int index);
    void atualizarCidades(int indexUF);
    void onCnpjEditingFinished();
    void onCnpjRetornado(const struct CnpjResult &resultado);
    void onCnpjErro(const QString &erro);
    void atualizarVisibilidadeAbas();

protected:
    bool validar() override;
    void carregarDados() override;

private:
    void preencherUF();
    std::optional<Pessoa> m_PessoaAtual;
    ICnpjService *m_cnpjService{nullptr};
};

#endif // FormPessoaDetalheH
