#ifndef FORMEMPRESALISTAGEM_H
#define FORMEMPRESALISTAGEM_H

#include <QWidget>

namespace Ui {
class FormEmpresaListagem;
}

class FormEmpresaListagem : public QWidget
{
    Q_OBJECT

public:
    explicit FormEmpresaListagem(QWidget *parent = nullptr);
    ~FormEmpresaListagem();

private slots:
    void onNovoClicked();
    void onAlterarClicked(int rowIndex);
    void voltarParaListagem();
    void carregarLista();

private:
    Ui::FormEmpresaListagem *ui;
};

#endif // FORMEMPRESALISTAGEM_H
