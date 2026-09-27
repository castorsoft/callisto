#pragma once

#include <QWidget>
#include <QAbstractItemModel>

namespace Ui {
class FormListagemBase;
}

class FormListagemBase : public QWidget
{
    Q_OBJECT

public:
    explicit FormListagemBase(QWidget *parent = nullptr);
    ~FormListagemBase() override;

    virtual void carregarLista() = 0;

protected:
    void setModel(QAbstractItemModel *model);
    QAbstractItemModel *m_model = nullptr;

signals:
    void actionNewRequested();
    void actionEditRequested(int rowIndex);
    void actionDeleteRequested(int rowIndex);

private slots:
    void on_btnNew_clicked();
    void on_btnEdit_clicked();
    void on_btnDelete_clicked();

private:
    Ui::FormListagemBase *ui;
    void applyTableConfigurations();
};
