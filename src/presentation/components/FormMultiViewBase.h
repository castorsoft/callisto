#pragma once

#include <QWidget>
#include <QStackedWidget>
#include <QVBoxLayout>
#include "FormDetalheBase.h"
#include "FormListagemBase.h"

class FormMultiViewBase : public QWidget {
    Q_OBJECT

public:
    explicit FormMultiViewBase(QWidget *parent = nullptr);
    virtual ~FormMultiViewBase() = default;

protected:
    // Deve retornar uma nova instância do formulário de detalhe
    virtual FormDetalheBase* criarFormDetalhe() = 0;

    // Define a listagem principal do gerenciamento
    void setListagem(FormListagemBase *formListagem);

    QStackedWidget *m_stackedWidget;
    FormListagemBase *m_formListagem;

public slots:
    virtual void onNovoRegistro();

protected slots:
    virtual void onEditarRegistro(int rowIndex);
    virtual void onDeletarRegistro(int rowIndex) {};
    virtual void voltarParaListagem();

    void abrirDetalhe(int idRegistro);
};
