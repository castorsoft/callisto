#pragma once

#include <QObject>
#include <optional>
#include "entities/Empresa.h"

class GlobalState : public QObject {
    Q_OBJECT
public:
    static GlobalState& instance() {
        static GlobalState _instance;
        return _instance;
    }

    std::optional<Empresa> empresaAtual() const { return m_empresaAtual; }
    void setEmpresaAtual(const Empresa &empresa) {
        m_empresaAtual = empresa;
        emit empresaAtualChanged();
    }

signals:
    void empresaAtualChanged();

private:
    GlobalState() = default;
    ~GlobalState() = default;

    std::optional<Empresa> m_empresaAtual;
};
