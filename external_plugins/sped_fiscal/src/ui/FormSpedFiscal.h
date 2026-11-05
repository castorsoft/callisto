#pragma once

#include <QWidget>
#include <memory>
#include "../../../src/presentation/plugins/api/IUIPluginContext.h"
#include "../models/SpedContadorData.h"

class InputText;
class InputMasked;
class InputComboBox;
class InputLookup;
class InputButton;

class QDateEdit;
class QComboBox;
class QProgressBar;
class QTextEdit;
class QLabel;

class FormSpedFiscal : public QWidget {
    Q_OBJECT

public:
    explicit FormSpedFiscal(std::shared_ptr<IUIPluginContext> context, QWidget *parent = nullptr);
    ~FormSpedFiscal() override = default;

private slots:
    void onSelecionarDestinoClicked();
    void onGerarSpedClicked();
    void onMesAnoChanged(int index);
    void onBuscarContador(const QString &termo);

private:
    void setupUi();

    std::shared_ptr<IUIPluginContext> m_context;

    QLabel *m_lblEmpresaInfo{nullptr};

    InputComboBox *m_comboMesAno{nullptr};
    QDateEdit *m_dateInicio{nullptr};
    QDateEdit *m_dateFim{nullptr};
    InputComboBox *m_inputFinalidade{nullptr};
    InputComboBox *m_inputPerfil{nullptr};

    InputLookup *m_lookupContador{nullptr};

    InputText *m_inputCaminhoArquivo{nullptr};
    QProgressBar *m_progressBar{nullptr};
    QTextEdit *m_textLog{nullptr};
    InputButton *m_btnGerar{nullptr};
};
