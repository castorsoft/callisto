#include "FormListagemBase.h"
#include "ui_FormListagemBase.h"
#include "providers/TableDataProviderAdapter.h"

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
    ui->tableView->horizontalHeader()->setStretchLastSection(false);
}

void FormListagemBase::setModel(QAbstractItemModel *model)
{
    m_model = model;
    ui->tableView->setModel(model);

    auto *header = ui->tableView->horizontalHeader();
    header->setStretchLastSection(false);

    if (auto adapter = qobject_cast<TableDataProviderAdapter*>(model)) {
        if (auto provider = adapter->provider()) {
            for (int col = 0; col < provider->columnCount(); ++col) {
                auto resizeMode = provider->columnResizeMode(col);
                if (resizeMode.has_value()) {
                    header->setSectionResizeMode(col, resizeMode.value());
                } else {
                    QString headerText = provider->headerData(col).toLower();
                    if (headerText.contains("descri") || headerText.contains("razão") || headerText.contains("razao")) {
                        header->setSectionResizeMode(col, QHeaderView::Stretch);
                    } else if (provider->columnWidth(col).has_value()) {
                        header->setSectionResizeMode(col, QHeaderView::Interactive);
                    } else {
                        header->setSectionResizeMode(col, QHeaderView::ResizeToContents);
                    }
                }

                auto width = provider->columnWidth(col);
                if (width.has_value()) {
                    ui->tableView->setColumnWidth(col, width.value());
                }
            }
        }
    }
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
