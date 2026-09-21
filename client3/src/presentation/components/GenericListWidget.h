#pragma once

#include <QWidget>
#include <QAbstractItemModel>

namespace Ui {
class GenericListWidget;
}

class GenericListWidget : public QWidget
{
    Q_OBJECT

public:
    explicit GenericListWidget(QWidget *parent = nullptr);
    ~GenericListWidget() override;

    void setModel(QAbstractItemModel *model);

signals:
    void actionNewRequested();
    void actionEditRequested(int rowIndex);
    void actionDeleteRequested(int rowIndex);

private slots:
    void on_btnNew_clicked();
    void on_btnEdit_clicked();
    void on_btnDelete_clicked();

private:
    Ui::GenericListWidget *ui;
    void applyTableConfigurations();
};
