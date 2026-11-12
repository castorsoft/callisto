#pragma once

#include <QPushButton>
#include <QEvent>

class InputButton : public QPushButton {
    Q_OBJECT
    Q_PROPERTY(ColorScheme colorScheme READ colorScheme WRITE setColorScheme NOTIFY colorSchemeChanged)

public:
    enum ColorScheme {
        Default = 0,
        Primary,
        Secondary,
        Success,
        Warning,
        Danger
    };
    Q_ENUM(ColorScheme)

    explicit InputButton(QWidget *parent = nullptr);
    explicit InputButton(const QString &text, QWidget *parent = nullptr);
    explicit InputButton(const QString &text, ColorScheme scheme, QWidget *parent = nullptr);

    ColorScheme colorScheme() const { return m_colorScheme; }
    void setColorScheme(ColorScheme scheme);

signals:
    void colorSchemeChanged(ColorScheme scheme);

protected:
    void changeEvent(QEvent *event) override;

private:
    void applyThemeStyle();

    ColorScheme m_colorScheme = Default;
};

