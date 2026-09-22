#ifndef FORMEMPRESADETALHEH
#define FORMEMPRESADETALHEH

#include <QWidget>

namespace Ui {
class FormEmpresaDetalhe;
}

class FormEmpresaDetalhe : public QWidget
{
    Q_OBJECT

public:
    explicit FormEmpresaDetalhe(QWidget *parent = nullptr);
    ~FormEmpresaDetalhe();

private:
    Ui::FormEmpresaDetalhe *ui;

private slots:
    void onPessoaChange(int index);
};

#endif // FORMEMPRESADETALHEH
