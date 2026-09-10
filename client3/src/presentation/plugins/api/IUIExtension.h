#pragma once

#include <QAction>

class IUIExtension {
public:
    virtual ~IUIExtension() = default;

    // Método para criar um componente clicável
    virtual QAction* getAction(QObject *parent = nullptr) = 0;

    virtual void initializeUI() const = 0;
};