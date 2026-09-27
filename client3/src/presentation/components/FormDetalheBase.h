#pragma once

#include <QWidget>
#include <optional>

class FormDetalheBase : public QWidget {
    Q_OBJECT

public:
    explicit FormDetalheBase(QWidget *parent = nullptr) : QWidget(parent) {}
    virtual ~FormDetalheBase() = default;

    std::optional<int> idRegistro() const { return m_idRegistro; }

    // Set the ID for edit mode, and automatically load the data
    void setIdRegistro(int id);

signals:
    void fechar();

protected slots:
    virtual void onSalvar() = 0;

protected:
    std::optional<int> m_idRegistro;

    // Must be implemented to validate form fields
    virtual bool validar() = 0;

    // Can be overridden to fetch data from repository and fill the form
    virtual void carregarDados() {}
};
