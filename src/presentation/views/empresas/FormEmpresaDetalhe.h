#ifndef FORMEMPRESADETALHEH
#define FORMEMPRESADETALHEH

#include <QWidget>
#include "../../components/FormDetalheBase.h"
#include "../../../core/entities/Empresa.h"

namespace Ui {
class FormEmpresaDetalhe;
}

class FormEmpresaDetalhe : public FormDetalheBase
{
    Q_OBJECT

public:
    explicit FormEmpresaDetalhe(QWidget *parent = nullptr);
    ~FormEmpresaDetalhe();

private:
    Ui::FormEmpresaDetalhe *ui;

protected slots:
    void onSalvar() override;

private slots:
    void onPessoaChange(int index);
    void atualizarCidades(int indexUF);
    void onCnpjEditingFinished();
    void onCnpjRetornado(const struct CnpjResult &resultado);
    void onCnpjErro(const QString &erro);

protected:
    bool validar() override;
    void carregarDados() override;

private:
    void preencherUF();
    std::optional<Empresa> m_empresaAtual;
};

#endif // FORMEMPRESADETALHEH
