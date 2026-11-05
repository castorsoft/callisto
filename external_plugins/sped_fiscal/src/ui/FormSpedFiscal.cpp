#include "FormSpedFiscal.h"
#include "../collector/SpedDataCollector.h"
#include "../builder/SpedBlockBuilder.h"
#include "../writer/SpedFileWriter.h"
#include "presentation/components/inputs/InputText.h"
#include "presentation/components/inputs/InputMasked.h"
#include "presentation/components/inputs/InputComboBox.h"
#include "presentation/components/inputs/InputCheckBox.h"
#include "presentation/components/inputs/InputLookup.h"
#include "presentation/components/inputs/InputButton.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QGroupBox>
#include <QScrollArea>
#include <QLabel>
#include <QDateEdit>
#include <QProgressBar>
#include <QTextEdit>
#include <QFileDialog>
#include <QMessageBox>
#include <QStandardPaths>
#include <QDir>
#include <QFile>
#include <QJsonDocument>
#include <QApplication>

FormSpedFiscal::FormSpedFiscal(std::shared_ptr<IUIPluginContext> context, QWidget *parent)
    : QWidget(parent)
    , m_context(context)
{
    setupUi();
}

void FormSpedFiscal::setupUi()
{
    setWindowTitle("SPED Fiscal (EFD ICMS/IPI)");

    auto *outerLayout = m_context ? m_context->createVBoxLayout(this) : new QVBoxLayout(this);
    outerLayout->setContentsMargins(0, 0, 0, 0);
    outerLayout->setSpacing(0);

    auto *scrollArea = m_context ? m_context->createScrollArea(this) : new QScrollArea(this);
    scrollArea->setWidgetResizable(true);
    scrollArea->setFrameShape(QFrame::NoFrame);

    auto *scrollContent = new QWidget(scrollArea);
    auto *mainLayout = m_context ? m_context->createVBoxLayout(scrollContent) : new QVBoxLayout(scrollContent);
    mainLayout->setContentsMargins(15, 15, 15, 15);
    mainLayout->setSpacing(12);

    // 1. Grupo Empresa
    auto *groupEmpresa = m_context ? m_context->createGroupBox("Empresa Ativa", scrollContent) : new QGroupBox("Empresa Ativa", scrollContent);
    auto *layoutEmpresa = m_context ? m_context->createHBoxLayout(groupEmpresa) : new QHBoxLayout(groupEmpresa);
    layoutEmpresa->setContentsMargins(10, 8, 10, 8);

    m_lblEmpresaInfo = new QLabel(groupEmpresa);
    m_lblEmpresaInfo->setStyleSheet("font-weight: bold; font-size: 13px;");

    if (m_context && m_context->empresaAtual().has_value()) {
        const auto &emp = m_context->empresaAtual().value();
        m_lblEmpresaInfo->setText(QString("%1 (CNPJ: %2)").arg(emp.razaoSocial(), emp.cnpjCpf()));
    } else {
        m_lblEmpresaInfo->setText("Nenhuma empresa ativa selecionada no sistema!");
    }
    layoutEmpresa->addWidget(m_lblEmpresaInfo);
    mainLayout->addWidget(groupEmpresa);

    // 2. Grupo Parâmetros de Geração
    auto *groupParametros = m_context ? m_context->createGroupBox("Parâmetros do Período e Layout", scrollContent) : new QGroupBox("Parâmetros do Período e Layout", scrollContent);
    auto *gridParam = m_context ? m_context->createGridLayout(groupParametros) : new QGridLayout(groupParametros);
    gridParam->setContentsMargins(10, 10, 10, 10);
    gridParam->setSpacing(10);

    m_comboMesAno = m_context ? m_context->createInputComboBox("Mês/Ano Referência", groupParametros) : new InputComboBox(groupParametros);
    QDate hoje = QDate::currentDate();
    for (int i = 0; i < 12; ++i) {
        QDate mesRef = hoje.addMonths(-i);
        QString textoMes = mesRef.toString("MM/yyyy");
        m_comboMesAno->addItem(textoMes, mesRef);
    }

    auto *widgetDataIni = new QWidget(groupParametros);
    auto *layoutDataIni = m_context ? m_context->createVBoxLayout(widgetDataIni) : new QVBoxLayout(widgetDataIni);
    layoutDataIni->setContentsMargins(0, 0, 0, 0);
    layoutDataIni->setSpacing(4);
    auto *lblDataIni = new QLabel("Data Inicial", widgetDataIni);
    lblDataIni->setStyleSheet("font-weight: bold;");
    m_dateInicio = new QDateEdit(widgetDataIni);
    m_dateInicio->setCalendarPopup(true);
    m_dateInicio->setDisplayFormat("dd/MM/yyyy");
    m_dateInicio->setFixedHeight(32);
    layoutDataIni->addWidget(lblDataIni);
    layoutDataIni->addWidget(m_dateInicio);

    auto *widgetDataFim = new QWidget(groupParametros);
    auto *layoutDataFim = m_context ? m_context->createVBoxLayout(widgetDataFim) : new QVBoxLayout(widgetDataFim);
    layoutDataFim->setContentsMargins(0, 0, 0, 0);
    layoutDataFim->setSpacing(4);
    auto *lblDataFim = new QLabel("Data Final", widgetDataFim);
    lblDataFim->setStyleSheet("font-weight: bold;");
    m_dateFim = new QDateEdit(widgetDataFim);
    m_dateFim->setCalendarPopup(true);
    m_dateFim->setDisplayFormat("dd/MM/yyyy");
    m_dateFim->setFixedHeight(32);
    layoutDataFim->addWidget(lblDataFim);
    layoutDataFim->addWidget(m_dateFim);

    m_inputFinalidade = m_context ? m_context->createInputComboBox("Finalidade do Arquivo", groupParametros) : new InputComboBox(groupParametros);
    m_inputFinalidade->addItem("0 - Remessa do arquivo original", 0);
    m_inputFinalidade->addItem("1 - Remessa do arquivo substituto", 1);

    m_inputPerfil = m_context ? m_context->createInputComboBox("Perfil de Apresentação", groupParametros) : new InputComboBox(groupParametros);
    m_inputPerfil->addItem("Perfil A (Mais detalhado)", "A");
    m_inputPerfil->addItem("Perfil B (Médio)", "B");

    gridParam->addWidget(m_comboMesAno, 0, 0);
    gridParam->addWidget(widgetDataIni, 0, 1);
    gridParam->addWidget(widgetDataFim, 0, 2);
    gridParam->addWidget(m_inputFinalidade, 1, 0, 1, 2);
    gridParam->addWidget(m_inputPerfil, 1, 2);

    mainLayout->addWidget(groupParametros);

    // 3. Grupo Dados do Contabilista (Registro 0100)
    auto *groupContador = m_context ? m_context->createGroupBox("Dados do Contabilista / Contador (Registro 0100)", scrollContent) : new QGroupBox("Dados do Contabilista / Contador (Registro 0100)", scrollContent);
    auto *layoutContador = m_context ? m_context->createVBoxLayout(groupContador) : new QVBoxLayout(groupContador);
    layoutContador->setContentsMargins(10, 10, 10, 10);
    layoutContador->setSpacing(10);

    m_lookupContador = m_context
        ? m_context->createInputLookup("Contabilista / Contador", "Digite o nome ou CPF/CNPJ do contador...", groupContador)
        : new InputLookup(groupContador);
    connect(m_lookupContador, &InputLookup::searchRequested, this, &FormSpedFiscal::onBuscarContador);

    layoutContador->addWidget(m_lookupContador);

    mainLayout->addWidget(groupContador);

    // 4. Grupo Destino do Arquivo
    auto *groupDestino = m_context ? m_context->createGroupBox("Arquivo de Destino (.txt)", scrollContent) : new QGroupBox("Arquivo de Destino (.txt)", scrollContent);
    auto *layoutDestino = m_context ? m_context->createHBoxLayout(groupDestino) : new QHBoxLayout(groupDestino);
    layoutDestino->setContentsMargins(10, 10, 10, 10);
    layoutDestino->setSpacing(8);

    m_inputCaminhoArquivo = m_context ? m_context->createInputText("Caminho do Arquivo Gerado", "Caminho do arquivo .txt", groupDestino) : new InputText(groupDestino);
    QString docsDir = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation);
    m_inputCaminhoArquivo->setText(docsDir + "/SPED_FISCAL.txt");

    auto *btnProcurar = m_context ? m_context->createInputButton("Procurar...", groupDestino) : new InputButton("Procurar...", groupDestino);
    btnProcurar->setFixedHeight(32);
    connect(btnProcurar, &InputButton::clicked, this, &FormSpedFiscal::onSelecionarDestinoClicked);

    m_btnGerar = m_context ? m_context->createInputButton("Gerar SPED Fiscal", groupDestino) : new InputButton("Gerar SPED Fiscal", groupDestino);
    m_btnGerar->setFixedHeight(32);
    m_btnGerar->setColorScheme(InputButton::Success);
    connect(m_btnGerar, &InputButton::clicked, this, &FormSpedFiscal::onGerarSpedClicked);

    layoutDestino->addWidget(m_inputCaminhoArquivo, 1);
    layoutDestino->addWidget(btnProcurar, 0, Qt::AlignBottom);
    layoutDestino->addWidget(m_btnGerar, 0, Qt::AlignBottom);

    mainLayout->addWidget(groupDestino);

    // 5. Progresso e Log
    m_progressBar = new QProgressBar(scrollContent);
    m_progressBar->setRange(0, 100);
    m_progressBar->setValue(0);
    m_progressBar->setTextVisible(true);
    mainLayout->addWidget(m_progressBar);

    m_textLog = new QTextEdit(scrollContent);
    m_textLog->setReadOnly(true);
    m_textLog->setMinimumHeight(100);
    m_textLog->setMaximumHeight(150);
    m_textLog->setPlaceholderText("Acompanhamento das etapas de geração...");
    mainLayout->addWidget(m_textLog);

    mainLayout->addStretch();

    scrollArea->setWidget(scrollContent);
    outerLayout->addWidget(scrollArea);

    // Conectar eventos após todos os componentes terem sido devidamente instanciados
    connect(m_comboMesAno, &InputComboBox::currentIndexChanged, this, &FormSpedFiscal::onMesAnoChanged);
    onMesAnoChanged(0);
}

void FormSpedFiscal::onMesAnoChanged(int /*index*/)
{
    if (!m_comboMesAno) return;

    QDate dataSel = m_comboMesAno->currentData().toDate();
    if (!dataSel.isValid()) return;

    QDate primeiroDia(dataSel.year(), dataSel.month(), 1);
    QDate ultimoDia(dataSel.year(), dataSel.month(), dataSel.daysInMonth());

    if (m_dateInicio) {
        m_dateInicio->setDate(primeiroDia);
    }
    if (m_dateFim) {
        m_dateFim->setDate(ultimoDia);
    }

    if (m_inputCaminhoArquivo) {
        QString nomeArq = QString("SPED_FISCAL_%1_%2.txt")
            .arg(QString::number(dataSel.year()))
            .arg(QString::number(dataSel.month()), 2, '0');

        QString textoAtual = m_inputCaminhoArquivo->text().trimmed();
        if (textoAtual.isEmpty()) {
            QString docsDir = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation);
            m_inputCaminhoArquivo->setText(docsDir + "/" + nomeArq);
        } else {
            QFileInfo fi(textoAtual);
            m_inputCaminhoArquivo->setText(fi.dir().absoluteFilePath(nomeArq));
        }
    }
}

void FormSpedFiscal::onSelecionarDestinoClicked()
{
    if (!m_inputCaminhoArquivo) return;

    QString caminho = QFileDialog::getSaveFileName(
        this,
        "Salvar Arquivo SPED Fiscal",
        m_inputCaminhoArquivo->text(),
        "Arquivos de Texto (*.txt);;Todos os Arquivos (*.*)"
    );

    if (!caminho.isEmpty()) {
        m_inputCaminhoArquivo->setText(caminho);
    }
}

void FormSpedFiscal::onGerarSpedClicked()
{
    if (!m_context) {
        QMessageBox::critical(this, "Erro", "Contexto de plugins não inicializado.");
        return;
    }

    if (!m_context->empresaAtual().has_value()) {
        QMessageBox::critical(this, "Aviso", "Nenhuma empresa ativa está selecionada.");
        return;
    }

    if (!m_dateInicio || !m_dateFim || !m_inputCaminhoArquivo || !m_inputFinalidade || !m_inputPerfil) {
        return;
    }

    QDate dtIni = m_dateInicio->date();
    QDate dtFin = m_dateFim->date();

    if (dtIni > dtFin) {
        QMessageBox::warning(this, "Aviso", "A data inicial não pode ser superior à data final.");
        return;
    }

    QString destino = m_inputCaminhoArquivo->text().trimmed();
    if (destino.isEmpty()) {
        QMessageBox::warning(this, "Aviso", "Informe o caminho do arquivo de destino.");
        return;
    }

    if (!m_lookupContador || !m_lookupContador->hasSelection() || m_lookupContador->currentData().toInt() <= 0) {
        QMessageBox::warning(this, "Aviso", "Selecione o Contabilista / Contador antes de gerar o SPED Fiscal.");
        return;
    }

    int idContador = m_lookupContador->currentData().toInt();

    if (m_btnGerar) m_btnGerar->setEnabled(false);
    if (m_progressBar) m_progressBar->setValue(10);
    if (m_textLog) m_textLog->clear();

    if (m_textLog) {
        m_textLog->append("=== Iniciando Geração do SPED Fiscal ===");
        m_textLog->append(QString("Período: %1 até %2").arg(dtIni.toString("dd/MM/yyyy"), dtFin.toString("dd/MM/yyyy")));
    }
    qApp->processEvents();

    // ----------------------------------------------------
    // ETAPA 1: Coleta dos Dados dos Repositórios (Em memória)
    // ----------------------------------------------------
    if (m_textLog) m_textLog->append("\n[Etapa 1/3] Coletando documentos e cadastros dos repositórios...");
    if (m_progressBar) m_progressBar->setValue(30);
    qApp->processEvents();

    int finalidade = m_inputFinalidade->currentData().toInt();
    QString perfil = m_inputPerfil->currentData().toString();

    SpedCollectedData collected = SpedDataCollector::collect(
        *m_context,
        dtIni,
        dtFin,
        finalidade,
        perfil,
        idContador
    );

    if (m_textLog) {
        m_textLog->append(QString("• Notas Fiscais no período: %1").arg(collected.documentos.size()));
        m_textLog->append(QString("• Participantes identificados: %1").arg(collected.participantes.size()));
        m_textLog->append(QString("• Produtos movimentados: %1").arg(collected.itens.size()));
        m_textLog->append(QString("• Unidades de medida: %1").arg(collected.unidades.size()));
    }
    qApp->processEvents();

    // ----------------------------------------------------
    // ETAPA 2: Mapeamento de Blocos e Registros SPED
    // ----------------------------------------------------
    if (m_textLog) m_textLog->append("\n[Etapa 2/3] Mapeando blocos e registros do SPED Fiscal...");
    if (m_progressBar) m_progressBar->setValue(60);
    qApp->processEvents();

    auto records = SpedBlockBuilder::build(collected);
    if (m_textLog) {
        m_textLog->append(QString("• Registros estruturados nos Blocos 0, C e vazios: %1").arg(records.size()));
    }
    qApp->processEvents();

    // ----------------------------------------------------
    // ETAPA 3: Serialização e Geração do Arquivo .txt
    // ----------------------------------------------------
    if (m_textLog) m_textLog->append("\n[Etapa 3/3] Calculando Bloco 9 e gravando arquivo em disco...");
    if (m_progressBar) m_progressBar->setValue(85);
    qApp->processEvents();

    SpedWriteResult result = SpedFileWriter::writeToFile(records, destino);

    if (m_progressBar) m_progressBar->setValue(100);
    if (m_btnGerar) m_btnGerar->setEnabled(true);

    if (result.sucesso) {
        if (m_textLog) {
            m_textLog->append(QString("\n✓ Concluído! %1 linhas gravadas com sucesso.").arg(result.totalLinhas));
            m_textLog->append("Arquivo salvo em: " + result.caminhoArquivo);
        }

        QMessageBox::information(
            this,
            "Sucesso",
            QString("Arquivo SPED Fiscal gerado com sucesso!\n\nLinhas: %1\nDestino: %2")
                .arg(QString::number(result.totalLinhas), result.caminhoArquivo)
        );
    } else {
        if (m_textLog) m_textLog->append("\n✗ Erro: " + result.mensagem);
        QMessageBox::critical(this, "Erro", result.mensagem);
    }
}

void FormSpedFiscal::onBuscarContador(const QString &termo)
{
    if (!m_context || !m_lookupContador) return;

    auto lista = m_context->pessoaRepository().buscarContadorPorTermo(termo);

    QList<QPair<QString, QVariant>> resultados;
    for (const auto &pessoa : lista) {
        if (pessoa.id().has_value()) {
            resultados.append({pessoa.textoExibicao(), pessoa.id().value()});
        }
    }
    m_lookupContador->setResults(resultados);
}
