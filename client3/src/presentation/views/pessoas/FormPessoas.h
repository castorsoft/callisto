#pragma once

#include "../../components/FormMultiViewBase.h"

class FormPessoas : public FormMultiViewBase {
    Q_OBJECT

public:
    explicit FormPessoas(QWidget *parent = nullptr);

protected:
    FormDetalheBase* criarFormDetalhe() override;

protected slots:
    void onEditarRegistro(int rowIndex) override;
};
