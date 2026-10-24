#pragma once

#include "../../components/FormMultiViewBase.h"

class FormProdutos : public FormMultiViewBase {
    Q_OBJECT

public:
    explicit FormProdutos(QWidget *parent = nullptr);

protected:
    FormDetalheBase* criarFormDetalhe() override;

protected slots:
    void onEditarRegistro(int rowIndex) override;
};
