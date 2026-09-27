#include "FormMultiViewBase.h"

FormMultiViewBase::FormMultiViewBase(QWidget *parent) : QWidget(parent) {
    auto *layout = new QVBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);

    m_stackedWidget = new QStackedWidget(this);
    layout->addWidget(m_stackedWidget);

    m_formListagem = nullptr;
}

void FormMultiViewBase::setListagem(FormListagemBase *formListagem) {
    if (m_formListagem) {
        m_stackedWidget->removeWidget(m_formListagem);
    }

    m_formListagem = formListagem;
    m_stackedWidget->addWidget(m_formListagem);
    m_stackedWidget->setCurrentWidget(m_formListagem);

    connect(m_formListagem, &FormListagemBase::actionNewRequested, this, &FormMultiViewBase::onNovoRegistro);
    connect(m_formListagem, &FormListagemBase::actionEditRequested, this, &FormMultiViewBase::onEditarRegistro);
}

void FormMultiViewBase::onNovoRegistro() {
    auto *detalhe = criarFormDetalhe();

    connect(detalhe, &FormDetalheBase::fechar, this, &FormMultiViewBase::voltarParaListagem);

    m_stackedWidget->addWidget(detalhe);
    m_stackedWidget->setCurrentWidget(detalhe);
}

void FormMultiViewBase::onEditarRegistro(int rowIndex) {
    // A classe filha deve implementar essa lógica (sobrecarregando este método)
    // ou o FormMultiViewBase deve obter o ID.
    // O ideal é que a classe filha (ex: FormEmpresas) sobrecarregue
    // onEditarRegistro(int rowIndex), extraia o ID do model, e chame abrirDetalhe(id).
}

void FormMultiViewBase::abrirDetalhe(int idRegistro) {
    auto *detalhe = criarFormDetalhe();
    detalhe->setIdRegistro(idRegistro);

    connect(detalhe, &FormDetalheBase::fechar, this, &FormMultiViewBase::voltarParaListagem);

    m_stackedWidget->addWidget(detalhe);
    m_stackedWidget->setCurrentWidget(detalhe);
}

void FormMultiViewBase::voltarParaListagem() {
    QWidget *widgetAtual = m_stackedWidget->currentWidget();
    m_stackedWidget->setCurrentWidget(m_formListagem);

    if (widgetAtual != m_formListagem) {
        m_stackedWidget->removeWidget(widgetAtual);
        widgetAtual->deleteLater();
    }

    if (m_formListagem) {
        m_formListagem->carregarLista();
    }
}
