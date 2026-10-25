#pragma once

#include <QWidget>
#include <QFileInfo>
#include <QList>
#include <QThreadPool>
#include <memory>
#include <atomic>

#include "../../../application/use_cases/ProcessFile.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class FormProcessarXml;
}
QT_END_NAMESPACE

class FormProcessarXml : public QWidget {
    Q_OBJECT

public:
    explicit FormProcessarXml(QWidget *parent = nullptr);
    ~FormProcessarXml() override;

public slots:
    void onItemStarted(int row);
    void onItemFinished(int row, const ResultadoProcessamento &resultado);

private slots:
    void onSelecionarPastaClicked();
    void onIniciarClicked();
    void onCancelarClicked();
    void onSubpastasToggled(bool checked);
    void onFiltroStatusChanged(int index);
    void onTabelaDoubleClicked(int row, int column);

private:
    void configurarTabela();
    void carregarArquivosDaPasta();
    void atualizarContadores();
    void aplicarFiltroTabela();
    void finalizarProcessamento(bool cancelado);

    Ui::FormProcessarXml *ui;

    QString m_pastaSelecionada;
    QList<QFileInfo> m_arquivos;

    QThreadPool m_threadPool;
    std::shared_ptr<std::atomic<bool>> m_cancelFlag;
    bool m_isProcessing{false};

    int m_total{0};
    int m_concluidos{0};
    int m_sucessos{0};
    int m_jaImportados{0};
    int m_erros{0};
};

