#include "FormSpedFiscal.h"
#include "../collector/SpedDataCollector.h"
#include "../builder/SpedBlockBuilder.h"
#include "../writer/SpedFileWriter.h"
#include "presentation/components/inputs/InputText.h"
#include "presentation/components/inputs/InputMasked.h"
#include "presentation/components/inputs/InputComboBox.h"
#include "presentation/components/inputs/InputCheckBox.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QGroupBox>
#include <QLabel>
#include <QDateEdit>
#include <QPushButton>
#include <QProgressBar>
#include <QTextEdit>
#include <QFileDialog>
#include <QMessageBox>
#include <QStandardPaths>
#include <QDir>
#include <QFile>
#include <QJsonDocument>
#include <QApplication>

FormSpedFiscal::FormSpedFiscal(std::shared_ptr<IPluginContext> context, QWidget *parent)
    : QWidget(parent)
    , m_context(context)
{
    setupUi();
    carregarConfiguracaoContador();
}

void FormSpedFiscal::setupUi()
{
    setWindowTitle("SPED Fiscal (EFD ICMS/IPI)");

    auto *mainLayout = m_context ? m_context->createVBoxLayout(this) : new QVBoxLayout(this);
    mainLayout->setContentsMargins(15, 15, 15, 15);
    mainLayout->setSpacing(12);

    // 1. Grupo Empresa
    auto *groupEmpresa = m_context ? m_context->createGroupBox("Empresa Ativa", this) : new QGroupBox("Empresa Ativa", this);
    auto *layoutEmpresa = m_context ? m_context->createHBoxLayout(groupEmpresa) : new QHBoxLayout(groupEmpresa);
    layoutEmpresa->setContentsMargins(10, 8, 10, 8);

    m_lblEmpresaInfo = new QLabel(groupEmpresa);
    m_lblEmpresaInfo->setStyleSheet("font-weight: bold; font-size: 13px; color: #1565C0;");

    if (m_context && m_context->empresaAtual().has_value()) {
        const auto &emp = m_context->empresaAtual().value();
        m_lblEmpresaInfo->setText(QString("%1 (CNPJ: %2)").arg(emp.razaoSocial(), emp.cnpjCpf()));
    } else {
        m_lblEmpresaInfo->setText("Nenhuma empresa ativa selecionada no sistema!");
    }
    layoutEmpresa->addWidget(m_lblEmpresaInfo);
    mainLayout->addWidget(groupEmpresa);

    // 2. Grupo Parâmetros de Geração
    auto *groupParametros = m_context ? m_context->createGroupBox("Parâmetros do Período e Layout", this) : new QGroupBox("Parâmetros do Período e Layout", this);
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
    auto *groupContador = m_context ? m_context->createGroupBox("Dados do Contabilista / Contador (Registro 0100)", this) : new QGroupBox("Dados do Contabilista / Contador (Registro 0100)", this);
    auto *gridContador = m_context ? m_context->createGridLayout(groupContador) : new QGridLayout(groupContador);
    gridContador->setContentsMargins(10, 10, 10, 10);
    gridContador->setSpacing(10);

    m_inputContadorNome = m_context ? m_context->createInputText("Nome do Contador", "Nome completo do contabilista", groupContador) : new InputText(groupContador);
    m_inputContadorCpf = m_context ? m_context->createInputMasked("CPF", "999.999.999-99;_", groupContador) : new InputMasked(groupContador);
    m_inputContadorCrc = m_context ? m_context->createInputText("CRC", "Ex: UF-999999", groupContador) : new InputText(groupContador);
    m_inputContadorCnpj = m_context ? m_context->createInputMasked("CNPJ do Escritório", "99.999.999/9999-99;_", groupContador) : new InputMasked(groupContador);
    m_inputContadorEndereco = m_context ? m_context->createInputText("Logradouro", "Rua / Av / Alameda", groupContador) : new InputText(groupContador);
    m_inputContadorNumero = m_context ? m_context->createInputText("Número", "Ex: 123", groupContador) : new InputText(groupContador);
    m_inputContadorBairro = m_context ? m_context->createInputText("Bairro", "Bairro", groupContador) : new InputText(groupContador);
    m_inputContadorCodMun = m_context ? m_context->createInputMasked("Cód. Município (IBGE)", "9999999;_", groupContador) : new InputMasked(groupContador);
    m_inputContadorCep = m_context ? m_context->createInputMasked("CEP", "99.999-999;_", groupContador) : new InputMasked(groupContador);
    m_inputContadorFone = m_context ? m_context->createInputMasked("Telefone", "(99) 99999-9999;_", groupContador) : new InputMasked(groupContador);
    m_inputContadorEmail = m_context ? m_context->createInputText("E-mail", "contador@exemplo.com.br", groupContador) : new InputText(groupContador);

    gridContador->addWidget(m_inputContadorNome, 0, 0, 1, 2);
    gridContador->addWidget(m_inputContadorCpf, 0, 2);

    gridContador->addWidget(m_inputContadorCrc, 1, 0);
    gridContador->addWidget(m_inputContadorCnpj, 1, 1, 1, 2);

    gridContador->addWidget(m_inputContadorEndereco, 2, 0, 1, 2);
    gridContador->addWidget(m_inputContadorNumero, 2, 2);

    gridContador->addWidget(m_inputContadorBairro, 3, 0);
    gridContador->addWidget(m_inputContadorCodMun, 3, 1);
    gridContador->addWidget(m_inputContadorCep, 3, 2);

    gridContador->addWidget(m_inputContadorFone, 4, 0);
    gridContador->addWidget(m_inputContadorEmail, 4, 1, 1, 2);

    mainLayout->addWidget(groupContador);

    // 4. Grupo Destino do Arquivo
    auto *groupDestino = m_context ? m_context->createGroupBox("Arquivo de Destino (.txt)", this) : new QGroupBox("Arquivo de Destino (.txt)", this);
    auto *layoutDestino = m_context ? m_context->createHBoxLayout(groupDestino) : new QHBoxLayout(groupDestino);
    layoutDestino->setContentsMargins(10, 10, 10, 10);
    layoutDestino->setSpacing(8);

    m_inputCaminhoArquivo = m_context ? m_context->createInputText("Caminho do Arquivo Gerado", "Caminho do arquivo .txt", groupDestino) : new InputText(groupDestino);
    QString docsDir = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation);
    m_inputCaminhoArquivo->setText(docsDir + "/SPED_FISCAL.txt");

    auto *btnProcurar = new QPushButton("Procurar...", groupDestino);
    btnProcurar->setFixedHeight(32);
    connect(btnProcurar, &QPushButton::clicked, this, &FormSpedFiscal::onSelecionarDestinoClicked);

    m_btnGerar = new QPushButton("Gerar SPED Fiscal", groupDestino);
    m_btnGerar->setFixedHeight(32);
    m_btnGerar->setStyleSheet("background-color: #2E7D32; color: white; font-weight: bold; padding: 6px 16px; font-size: 13px;");
    connect(m_btnGerar, &QPushButton::clicked, this, &FormSpedFiscal::onGerarSpedClicked);

    layoutDestino->addWidget(m_inputCaminhoArquivo, 1);
    layoutDestino->addWidget(btnProcurar, 0, Qt::AlignBottom);
    layoutDestino->addWidget(m_btnGerar, 0, Qt::AlignBottom);

    mainLayout->addWidget(groupDestino);

    // 5. Progresso e Log
    m_progressBar = new QProgressBar(this);
    m_progressBar->setRange(0, 100);
    m_progressBar->setValue(0);
    m_progressBar->setTextVisible(true);
    mainLayout->addWidget(m_progressBar);

    m_textLog = new QTextEdit(this);
    m_textLog->setReadOnly(true);
    m_textLog->setMaximumHeight(150);
    m_textLog->setPlaceholderText("Acompanhamento das etapas de geração...");
    mainLayout->addWidget(m_textLog);

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

    if (m_btnGerar) m_btnGerar->setEnabled(false);
    if (m_progressBar) m_progressBar->setValue(10);
    if (m_textLog) m_textLog->clear();

    // Salva configuração do contador para persistência
    salvarConfiguracaoContador();

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

    SpedContadorData contador;
    if (m_inputContadorNome) contador.nome = m_inputContadorNome->text().trimmed();
    if (m_inputContadorCpf) contador.cpf = m_inputContadorCpf->text().trimmed();
    if (m_inputContadorCrc) contador.crc = m_inputContadorCrc->text().trimmed();
    if (m_inputContadorCnpj) contador.cnpj = m_inputContadorCnpj->text().trimmed();
    if (m_inputContadorCep) contador.cep = m_inputContadorCep->text().trimmed();
    if (m_inputContadorEndereco) contador.logradouro = m_inputContadorEndereco->text().trimmed();
    if (m_inputContadorNumero) contador.numero = m_inputContadorNumero->text().trimmed();
    if (m_inputContadorBairro) contador.bairro = m_inputContadorBairro->text().trimmed();
    if (m_inputContadorFone) contador.telefone = m_inputContadorFone->text().trimmed();
    if (m_inputContadorEmail) contador.email = m_inputContadorEmail->text().trimmed();
    if (m_inputContadorCodMun) contador.codigoMunicipioIbge = m_inputContadorCodMun->text().trimmed();

    int finalidade = m_inputFinalidade->currentData().toInt();
    QString perfil = m_inputPerfil->currentData().toString();

    SpedCollectedData collected = SpedDataCollector::collect(
        *m_context,
        dtIni,
        dtFin,
        finalidade,
        perfil,
        contador
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

QString FormSpedFiscal::caminhoConfigContador() const
{
    QString appData = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    QDir dir(appData);
    if (!dir.exists()) dir.mkpath(".");
    return dir.absoluteFilePath("contador_config.json");
}

void FormSpedFiscal::carregarConfiguracaoContador()
{
    QFile file(caminhoConfigContador());
    if (!file.open(QIODevice::ReadOnly)) return;

    QByteArray data = file.readAll();
    file.close();

    QJsonDocument doc = QJsonDocument::fromJson(data);
    if (!doc.isObject()) return;

    SpedContadorData c = SpedContadorData::fromJson(doc.object());
    if (m_inputContadorNome) m_inputContadorNome->setText(c.nome);
    if (m_inputContadorCpf) m_inputContadorCpf->setText(c.cpf);
    if (m_inputContadorCrc) m_inputContadorCrc->setText(c.crc);
    if (m_inputContadorCnpj) m_inputContadorCnpj->setText(c.cnpj);
    if (m_inputContadorCep) m_inputContadorCep->setText(c.cep);
    if (m_inputContadorEndereco) m_inputContadorEndereco->setText(c.logradouro);
    if (m_inputContadorNumero) m_inputContadorNumero->setText(c.numero);
    if (m_inputContadorBairro) m_inputContadorBairro->setText(c.bairro);
    if (m_inputContadorFone) m_inputContadorFone->setText(c.telefone);
    if (m_inputContadorEmail) m_inputContadorEmail->setText(c.email);
    if (m_inputContadorCodMun) m_inputContadorCodMun->setText(c.codigoMunicipioIbge);
}

void FormSpedFiscal::salvarConfiguracaoContador()
{
    SpedContadorData c;
    if (m_inputContadorNome) c.nome = m_inputContadorNome->text().trimmed();
    if (m_inputContadorCpf) c.cpf = m_inputContadorCpf->text().trimmed();
    if (m_inputContadorCrc) c.crc = m_inputContadorCrc->text().trimmed();
    if (m_inputContadorCnpj) c.cnpj = m_inputContadorCnpj->text().trimmed();
    if (m_inputContadorCep) c.cep = m_inputContadorCep->text().trimmed();
    if (m_inputContadorEndereco) c.logradouro = m_inputContadorEndereco->text().trimmed();
    if (m_inputContadorNumero) c.numero = m_inputContadorNumero->text().trimmed();
    if (m_inputContadorBairro) c.bairro = m_inputContadorBairro->text().trimmed();
    if (m_inputContadorFone) c.telefone = m_inputContadorFone->text().trimmed();
    if (m_inputContadorEmail) c.email = m_inputContadorEmail->text().trimmed();
    if (m_inputContadorCodMun) c.codigoMunicipioIbge = m_inputContadorCodMun->text().trimmed();

    QJsonDocument doc(c.toJson());
    QFile file(caminhoConfigContador());
    if (file.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
        file.write(doc.toJson());
        file.close();
    }
}

