#include "GenericListWidget.h"
#include "ui_GenericListWidget.h"

GenericListWidget::GenericListWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::GenericListWidget)
{
    ui->setupUi(this);
    applyTableConfigurations();
}

GenericListWidget::~GenericListWidget()
{
    delete ui;
}

void GenericListWidget::applyTableConfigurations()
{
    ui->tableView->setAlternatingRowColors(true);
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableView->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableView->horizontalHeader()->setStretchLastSection(true);
}

void GenericListWidget::setModel(QAbstractItemModel *model)
{
    ui->tableView->setModel(model);
}

void GenericListWidget::on_btnNew_clicked()
{
    emit actionNewRequested();
}

void GenericListWidget::on_btnEdit_clicked()
{
    QModelIndexList selection = ui->tableView->selectionModel()->selectedRows();
    if (!selection.isEmpty()) {
        emit actionEditRequested(selection.first().row());
    }
}

void GenericListWidget::on_btnDelete_clicked()
{
    QModelIndexList selection = ui->tableView->selectionModel()->selectedRows();
    if (!selection.isEmpty()) {
        emit actionDeleteRequested(selection.first().row());
    }
}
