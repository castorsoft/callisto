#include "FormProcessarXml.h"
#include "ui_FormProcessarXml.h"

#include <QFileDialog>
#include <QDirIterator>
#include <QTableWidgetItem>
#include <QHeaderView>
#include <QPointer>
#include <QColor>
#include <QBrush>
#include <QFont>
#include <QThread>
#include <QDesktopServices>
#include <QUrl>

#include "../../components/ToastNotification.h"

namespace {

class ProcessFileTask : public QRunnable {
public:
    ProcessFileTask(int row, const QString &filePath, FormProcessarXml *form, std::shared_ptr<std::atomic<bool>> cancelFlag)
        : m_row(row), m_filePath(filePath), m_form(form), m_cancelFlag(cancelFlag) {
        setAutoDelete(true);
    }

    void run() override {
        if (!m_form || (m_cancelFlag && m_cancelFlag->load())) {
            return;
        }

        QMetaObject::invokeMethod(m_form, [form = m_form, row = m_row]() {
            if (form) form->onItemStarted(row);
        }, Qt::QueuedConnection);

        if (m_cancelFlag && m_cancelFlag->load()) {
            return;
        }

        ProcessFile processor;
        ResultadoProcessamento resultado = processor.processarArquivo(m_filePath);

        if (m_cancelFlag && m_cancelFlag->load()) {
            return;
        }

        QMetaObject::invokeMethod(m_form, [form = m_form, row = m_row, resultado]() {
            if (form) form->onItemFinished(row, resultado);
        }, Qt::QueuedConnection);
    }

private:
    int m_row;
    QString m_filePath;
    QPointer<FormProcessarXml> m_form;
    std::shared_ptr<std::atomic<bool>> m_cancelFlag;
};

} // namespace

FormProcessarXml::FormProcessarXml(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::FormProcessarXml)
    , m_cancelFlag(std::make_shared<std::atomic<bool>>(false))
{
    ui->setupUi(this);
    configurarTabela();

    connect(ui->btnSelecionarPasta, &InputButton::clicked, this, &FormProcessarXml::onSelecionarPastaClicked);
    connect(ui->btnIniciar, &InputButton::clicked, this, &FormProcessarXml::onIniciarClicked);
    connect(ui->btnCancelar, &InputButton::clicked, this, &FormProcessarXml::onCancelarClicked);
    connect(ui->chkSubpastas, &QCheckBox::toggled, this, &FormProcessarXml::onSubpastasToggled);
    connect(ui->cbFiltroStatus, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &FormProcessarXml::onFiltroStatusChanged);
    connect(ui->tableArquivos, &QTableWidget::cellDoubleClicked, this, &FormProcessarXml::onTabelaDoubleClicked);

    int maxWorkers = qBound(1, QThread::idealThreadCount() / 2, 4);
    m_threadPool.setMaxThreadCount(maxWorkers);
}

FormProcessarXml::~FormProcessarXml()
{
    if (m_cancelFlag) {
        m_cancelFlag->store(true);
    }
    m_threadPool.clear();
    m_threadPool.waitForDone(2000);
    delete ui;
}

void FormProcessarXml::configurarTabela()
{
    ui->tableArquivos->setColumnCount(5);
    QStringList headers = {"#", "Arquivo", "Chave de Acesso / Modelo", "Status", "Detalhes"};
    ui->tableArquivos->setHorizontalHeaderLabels(headers);

    auto *header = ui->tableArquivos->horizontalHeader();
    header->setStretchLastSection(false);
    header->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    header->setSectionResizeMode(1, QHeaderView::Interactive);
    header->setSectionResizeMode(2, QHeaderView::Interactive);
    header->setSectionResizeMode(3, QHeaderView::ResizeToContents);
    header->setSectionResizeMode(4, QHeaderView::Stretch);

    ui->tableArquivos->setColumnWidth(1, 240);
    ui->tableArquivos->setColumnWidth(2, 340);
}

void FormProcessarXml::onSelecionarPastaClicked()
{
    QString dir = QFileDialog::getExistingDirectory(
        this,
        "Selecionar Pasta com XMLs ou JSONs",
        m_pastaSelecionada.isEmpty() ? QDir::homePath() : m_pastaSelecionada,
        QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks
    );

    if (dir.isEmpty()) {
        return;
    }

    m_pastaSelecionada = dir;
    ui->editCaminho->setText(m_pastaSelecionada);

    carregarArquivosDaPasta();
}

void FormProcessarXml::onSubpastasToggled(bool /*checked*/)
{
    if (!m_pastaSelecionada.isEmpty()) {
        carregarArquivosDaPasta();
    }
}

void FormProcessarXml::carregarArquivosDaPasta()
{
    if (m_isProcessing) {
        return;
    }

    m_arquivos.clear();
    ui->tableArquivos->setRowCount(0);
    m_concluidos = 0;
    m_sucessos = 0;
    m_jaImportados = 0;
    m_erros = 0;
    ui->progressBar->setValue(0);

    if (m_pastaSelecionada.isEmpty()) {
        m_total = 0;
        atualizarContadores();
        ui->btnIniciar->setEnabled(false);
        return;
    }

    QStringList nameFilters = {"*.xml", "*.XML", "*.json", "*.JSON"};
    QDirIterator::IteratorFlags flags = QDirIterator::NoIteratorFlags;
    if (ui->chkSubpastas->isChecked()) {
        flags = QDirIterator::Subdirectories;
    }

    QDirIterator it(m_pastaSelecionada, nameFilters, QDir::Files, flags);
    while (it.hasNext()) {
        it.next();
        m_arquivos.append(it.fileInfo());
    }

    m_total = m_arquivos.size();
    ui->tableArquivos->setRowCount(m_total);
    ui->progressBar->setMaximum(m_total > 0 ? m_total : 1);

    for (int i = 0; i < m_total; ++i) {
        const QFileInfo &info = m_arquivos[i];

        auto *itemNum = new QTableWidgetItem(QString::number(i + 1));
        itemNum->setTextAlignment(Qt::AlignCenter);

        auto *itemNome = new QTableWidgetItem(info.fileName());
        itemNome->setToolTip(QString("%1\n(Duplo clique para abrir)").arg(info.absoluteFilePath()));
        itemNome->setData(Qt::UserRole, info.absoluteFilePath());

        auto *itemChave = new QTableWidgetItem("-");
        itemChave->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);

        auto *itemStatus = new QTableWidgetItem("Pendente");
        itemStatus->setTextAlignment(Qt::AlignCenter);
        itemStatus->setForeground(QBrush(QColor("#757575")));

        auto *itemDetalhes = new QTableWidgetItem("Aguardando processamento");
        itemDetalhes->setForeground(QBrush(QColor("#757575")));

        ui->tableArquivos->setItem(i, 0, itemNum);
        ui->tableArquivos->setItem(i, 1, itemNome);
        ui->tableArquivos->setItem(i, 2, itemChave);
        ui->tableArquivos->setItem(i, 3, itemStatus);
        ui->tableArquivos->setItem(i, 4, itemDetalhes);
    }

    atualizarContadores();
    aplicarFiltroTabela();
    ui->btnIniciar->setEnabled(m_total > 0);

    if (m_total == 0) {
        ToastNotification::show(this, "Aviso", "Nenhum arquivo XML ou JSON encontrado na pasta.", ToastNotification::Warning, 3000);
    }
}

void FormProcessarXml::atualizarContadores()
{
    ui->lblTotal->setText(QString("Total: %1").arg(m_total));
    ui->lblProcessados->setText(QString("Sucessos: %1").arg(m_sucessos));
    ui->lblJaImportados->setText(QString("Já Importados: %1").arg(m_jaImportados));
    ui->lblErros->setText(QString("Erros: %1").arg(m_erros));
}

void FormProcessarXml::onFiltroStatusChanged(int /*index*/)
{
    aplicarFiltroTabela();
}

void FormProcessarXml::aplicarFiltroTabela()
{
    QString filtro = ui->cbFiltroStatus->currentText().trimmed();
    int rowCount = ui->tableArquivos->rowCount();

    for (int r = 0; r < rowCount; ++r) {
        if (filtro == "Todos") {
            ui->tableArquivos->setRowHidden(r, false);
            continue;
        }

        auto *itemStatus = ui->tableArquivos->item(r, 3);
        if (!itemStatus) {
            ui->tableArquivos->setRowHidden(r, false);
            continue;
        }

        QString statusText = itemStatus->text().trimmed();
        bool match = false;

        if (filtro == "Pendentes" && (statusText == "Pendente" || statusText == "Processando...")) {
            match = true;
        } else if (filtro == "Sucessos" && statusText == "Sucesso") {
            match = true;
        } else if (filtro == "Já Importados" && statusText == "Já Importado") {
            match = true;
        } else if (filtro == "Erros" && (statusText == "Erro" || statusText == "Cancelado")) {
            match = true;
        }

        ui->tableArquivos->setRowHidden(r, !match);
    }
}

void FormProcessarXml::onTabelaDoubleClicked(int row, int /*column*/)
{
    if (row < 0 || row >= ui->tableArquivos->rowCount()) return;

    auto *itemNome = ui->tableArquivos->item(row, 1);
    if (!itemNome) return;

    QString filePath = itemNome->data(Qt::UserRole).toString();
    if (!filePath.isEmpty() && QFileInfo::exists(filePath)) {
        QDesktopServices::openUrl(QUrl::fromLocalFile(filePath));
    }
}

void FormProcessarXml::onIniciarClicked()
{
    if (m_isProcessing || m_arquivos.isEmpty()) {
        return;
    }

    m_isProcessing = true;
    m_cancelFlag = std::make_shared<std::atomic<bool>>(false);

    ui->btnIniciar->setEnabled(false);
    ui->btnSelecionarPasta->setEnabled(false);
    ui->chkSubpastas->setEnabled(false);
    ui->btnCancelar->setEnabled(true);
    ui->btnCancelar->setText("Cancelar");

    m_concluidos = 0;
    m_sucessos = 0;
    m_jaImportados = 0;
    m_erros = 0;
    atualizarContadores();
    ui->progressBar->setValue(0);
    ui->progressBar->setMaximum(m_total);

    // Reset status nas linhas da tabela
    for (int i = 0; i < m_total; ++i) {
        if (auto *itemStatus = ui->tableArquivos->item(i, 3)) {
            itemStatus->setText("Pendente");
            itemStatus->setForeground(QBrush(QColor("#757575")));
        }
        if (auto *itemDetalhes = ui->tableArquivos->item(i, 4)) {
            itemDetalhes->setText("Na fila...");
            itemDetalhes->setForeground(QBrush(QColor("#757575")));
        }
    }

    aplicarFiltroTabela();

    ToastNotification::show(this, "Iniciando", QString("Processando lote de %1 arquivo(s)...").arg(m_total), ToastNotification::Info, 2500);

    for (int i = 0; i < m_arquivos.size(); ++i) {
        QString filePath = m_arquivos[i].absoluteFilePath();
        auto *task = new ProcessFileTask(i, filePath, this, m_cancelFlag);
        m_threadPool.start(task);
    }
}

void FormProcessarXml::onCancelarClicked()
{
    if (!m_isProcessing) {
        return;
    }

    if (m_cancelFlag) {
        m_cancelFlag->store(true);
    }
    m_threadPool.clear();

    ui->btnCancelar->setEnabled(false);
    ui->btnCancelar->setText("Cancelando...");

    // Marca pendentes como Cancelado
    for (int i = 0; i < m_total; ++i) {
        auto *itemStatus = ui->tableArquivos->item(i, 3);
        if (itemStatus && (itemStatus->text() == "Pendente" || itemStatus->text() == "Processando...")) {
            itemStatus->setText("Cancelado");
            itemStatus->setForeground(QBrush(QColor("#9E9E9E")));
            if (auto *itemDetalhes = ui->tableArquivos->item(i, 4)) {
                itemDetalhes->setText("Operação cancelada pelo usuário.");
            }
        }
    }

    aplicarFiltroTabela();
    finalizarProcessamento(true);
}

void FormProcessarXml::onItemStarted(int row)
{
    if (row < 0 || row >= ui->tableArquivos->rowCount()) return;

    if (auto *itemStatus = ui->tableArquivos->item(row, 3)) {
        itemStatus->setText("Processando...");
        QFont f = itemStatus->font();
        f.setBold(true);
        itemStatus->setFont(f);
        itemStatus->setForeground(QBrush(QColor("#1976D2")));
    }
    if (auto *itemDetalhes = ui->tableArquivos->item(row, 4)) {
        itemDetalhes->setText("Lendo arquivo...");
    }

    // Se estiver filtrando, pode precisar ajustar visibilidade se mudou de estado
    if (ui->cbFiltroStatus->currentIndex() != 0) {
        aplicarFiltroTabela();
    }
}

void FormProcessarXml::onItemFinished(int row, const ResultadoProcessamento &resultado)
{
    if (row < 0 || row >= ui->tableArquivos->rowCount()) return;

    if (!resultado.chaveAcesso.isEmpty()) {
        if (auto *itemChave = ui->tableArquivos->item(row, 2)) {
            QString descChave = resultado.chaveAcesso;
            if (!resultado.modelo.isEmpty()) {
                descChave += QString(" (Mod: %1)").arg(resultado.modelo);
            }
            itemChave->setText(descChave);
        }
    }

    auto *itemStatus = ui->tableArquivos->item(row, 3);
    auto *itemDetalhes = ui->tableArquivos->item(row, 4);

    QFont fontBold;
    fontBold.setBold(true);

    switch (resultado.status) {
    case ResultadoProcessamento::Status::Sucesso:
        m_sucessos++;
        if (itemStatus) {
            itemStatus->setText("Sucesso");
            itemStatus->setFont(fontBold);
            itemStatus->setForeground(QBrush(QColor("#2E7D32")));
        }
        if (itemDetalhes) {
            itemDetalhes->setText(resultado.mensagem.isEmpty() ? "Importado com sucesso" : resultado.mensagem);
            itemDetalhes->setForeground(QBrush(QColor("#2E7D32")));
        }
        break;

    case ResultadoProcessamento::Status::JaImportado:
        m_jaImportados++;
        if (itemStatus) {
            itemStatus->setText("Já Importado");
            itemStatus->setFont(fontBold);
            itemStatus->setForeground(QBrush(QColor("#E65100")));
        }
        if (itemDetalhes) {
            itemDetalhes->setText(resultado.mensagem.isEmpty() ? "Documento já cadastrado" : resultado.mensagem);
            itemDetalhes->setForeground(QBrush(QColor("#E65100")));
        }
        break;

    case ResultadoProcessamento::Status::Erro:
    default:
        m_erros++;
        if (itemStatus) {
            itemStatus->setText("Erro");
            itemStatus->setFont(fontBold);
            itemStatus->setForeground(QBrush(QColor("#C62828")));
        }
        if (itemDetalhes) {
            itemDetalhes->setText(resultado.mensagem.isEmpty() ? "Falha desconhecida" : resultado.mensagem);
            itemDetalhes->setForeground(QBrush(QColor("#C62828")));
        }
        break;
    }

    m_concluidos++;
    ui->progressBar->setValue(m_concluidos);
    atualizarContadores();

    if (ui->cbFiltroStatus->currentIndex() != 0) {
        aplicarFiltroTabela();
    }

    if (m_concluidos >= m_total) {
        finalizarProcessamento(false);
    }
}

void FormProcessarXml::finalizarProcessamento(bool cancelado)
{
    m_isProcessing = false;

    ui->btnIniciar->setEnabled(true);
    ui->btnSelecionarPasta->setEnabled(true);
    ui->chkSubpastas->setEnabled(true);
    ui->btnCancelar->setEnabled(false);
    ui->btnCancelar->setText("Cancelar");

    if (cancelado) {
        ToastNotification::show(
            this,
            "Cancelado",
            "Processamento interrompido pelo usuário.",
            ToastNotification::Warning,
            3000
        );
    } else {
        ToastNotification::show(
            this,
            "Processamento Concluído",
            QString("Total: %1 | Sucessos: %2 | Já Existentes: %3 | Falhas: %4")
                .arg(m_total).arg(m_sucessos).arg(m_jaImportados).arg(m_erros),
            m_erros == 0 ? ToastNotification::Success : ToastNotification::Warning,
            4000
        );
    }
}
