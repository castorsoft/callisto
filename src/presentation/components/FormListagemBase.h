#pragma once

#include <QWidget>
#include <QAbstractItemModel>
#include <QTimer>

namespace Ui {
class FormListagemBase;
}

class MultiColumnFilterProxyModel;

class FormListagemBase : public QWidget
{
    Q_OBJECT

public:
    explicit FormListagemBase(QWidget *parent = nullptr);
    ~FormListagemBase() override;

    virtual void carregarLista() = 0;
    virtual int getIdSelecionado(int rowIndex) = 0;

protected:
    void setModel(QAbstractItemModel *model);
    virtual void onPesquisar(const QString &termo);
    QString termoPesquisa() const;
    int sourceRowIndex(const QModelIndex &viewIndex) const;

    QAbstractItemModel *m_model = nullptr;
    MultiColumnFilterProxyModel *m_proxyModel = nullptr;

signals:
    void actionNewRequested();
    void actionEditRequested(int rowIndex);
    void actionDeleteRequested(int rowIndex);

private slots:
    void on_btnNew_clicked();
    void on_btnEdit_clicked();
    void on_btnDelete_clicked();
    void onSearchTextChanged(const QString &text);
    void onSearchTimerTimeout();

private:
    Ui::FormListagemBase *ui;
    QTimer *m_searchDebounceTimer = nullptr;
    void applyTableConfigurations();
};
