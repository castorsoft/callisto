#pragma once

#include "../../components/FormMultiViewBase.h"

class FormEmpresas : public FormMultiViewBase {
    Q_OBJECT

public:
    explicit FormEmpresas(QWidget *parent = nullptr);

protected:
    FormDetalheBase* criarFormDetalhe() override;

protected slots:
    void onEditarRegistro(int rowIndex) override;
};
