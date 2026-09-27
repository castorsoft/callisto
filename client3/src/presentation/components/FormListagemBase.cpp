#include "FormListagemBase.h"
#include "ui_FormListagemBase.h"

FormListagemBase::FormListagemBase(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FormListagemBase)
{
    ui->setupUi(this);
    applyTableConfigurations();
}

FormListagemBase::~FormListagemBase()
{
    delete ui;
}

void FormListagemBase::applyTableConfigurations()
{
    ui->tableView->setAlternatingRowColors(true);
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableView->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableView->horizontalHeader()->setStretchLastSection(true);
}

void FormListagemBase::setModel(QAbstractItemModel *model)
{
    m_model = model;
    ui->tableView->setModel(model);
}

void FormListagemBase::on_btnNew_clicked()
{
    emit actionNewRequested();
}

void FormListagemBase::on_btnEdit_clicked()
{
    QModelIndexList selection = ui->tableView->selectionModel()->selectedRows();
    if (!selection.isEmpty()) {
        emit actionEditRequested(selection.first().row());
    }
}

void FormListagemBase::on_btnDelete_clicked()
{
    QModelIndexList selection = ui->tableView->selectionModel()->selectedRows();
    if (!selection.isEmpty()) {
        emit actionDeleteRequested(selection.first().row());
    }
}
