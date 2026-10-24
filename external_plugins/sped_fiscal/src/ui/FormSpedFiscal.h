#pragma once

#include <QWidget>
#include <memory>
#include "../../../src/plugins/api/IPluginContext.h"
#include "../models/SpedContadorData.h"

class InputText;
class InputMasked;
class InputComboBox;

class QDateEdit;
class QComboBox;
class QProgressBar;
class QTextEdit;
class QPushButton;
class QLabel;

class FormSpedFiscal : public QWidget {
    Q_OBJECT

public:
    explicit FormSpedFiscal(std::shared_ptr<IPluginContext> context, QWidget *parent = nullptr);
    ~FormSpedFiscal() override = default;

private slots:
    void onSelecionarDestinoClicked();
    void onGerarSpedClicked();
    void onMesAnoChanged(int index);

private:
    void setupUi();
    void carregarConfiguracaoContador();
    void salvarConfiguracaoContador();
    QString caminhoConfigContador() const;

    std::shared_ptr<IPluginContext> m_context;

    QLabel *m_lblEmpresaInfo{nullptr};

    InputComboBox *m_comboMesAno{nullptr};
    QDateEdit *m_dateInicio{nullptr};
    QDateEdit *m_dateFim{nullptr};
    InputComboBox *m_inputFinalidade{nullptr};
    InputComboBox *m_inputPerfil{nullptr};

    InputText *m_inputContadorNome{nullptr};
    InputMasked *m_inputContadorCpf{nullptr};
    InputText *m_inputContadorCrc{nullptr};
    InputMasked *m_inputContadorCnpj{nullptr};
    InputMasked *m_inputContadorCep{nullptr};
    InputText *m_inputContadorEndereco{nullptr};
    InputText *m_inputContadorNumero{nullptr};
    InputText *m_inputContadorBairro{nullptr};
    InputMasked *m_inputContadorFone{nullptr};
    InputText *m_inputContadorEmail{nullptr};
    InputMasked *m_inputContadorCodMun{nullptr};

    InputText *m_inputCaminhoArquivo{nullptr};
    QProgressBar *m_progressBar{nullptr};
    QTextEdit *m_textLog{nullptr};
    QPushButton *m_btnGerar{nullptr};
};
