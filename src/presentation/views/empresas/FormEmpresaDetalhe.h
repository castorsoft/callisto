#ifndef FORMEMPRESADETALHEH
#define FORMEMPRESADETALHEH

#include <QWidget>
#include "../../components/FormDetalheBase.h"
#include "../../../core/entities/Empresa.h"
#include "../../../core/interfaces/ICnpjService.h"

namespace Ui {
class FormEmpresaDetalhe;
}

class FormEmpresaDetalhe : public FormDetalheBase
{
    Q_OBJECT

public:
    explicit FormEmpresaDetalhe(QWidget *parent = nullptr, ICnpjService *cnpjService = nullptr);
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
    ICnpjService *m_cnpjService{nullptr};
};

#endif // FORMEMPRESADETALHEH
