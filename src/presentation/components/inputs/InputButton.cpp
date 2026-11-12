#include "InputButton.h"
#include <QPalette>
#include <QColor>
#include <QCursor>

namespace {

QColor blendColors(const QColor &c1, const QColor &c2, double ratio)
{
    int r = qBound(0, static_cast<int>(c1.red() * (1.0 - ratio) + c2.red() * ratio), 255);
    int g = qBound(0, static_cast<int>(c1.green() * (1.0 - ratio) + c2.green() * ratio), 255);
    int b = qBound(0, static_cast<int>(c1.blue() * (1.0 - ratio) + c2.blue() * ratio), 255);
    return QColor(r, g, b);
}

} // namespace

InputButton::InputButton(QWidget *parent)
    : QPushButton(parent)
{
    // setCursor(Qt::PointingHandCursor);
    applyThemeStyle();
}

InputButton::InputButton(const QString &text, QWidget *parent)
    : QPushButton(text, parent)
{
    // setCursor(Qt::PointingHandCursor);
    applyThemeStyle();
}

InputButton::InputButton(const QString &text, ColorScheme scheme, QWidget *parent)
    : QPushButton(text, parent)
    , m_colorScheme(scheme)
{
    // setCursor(Qt::PointingHandCursor);
    applyThemeStyle();
}

void InputButton::setColorScheme(ColorScheme scheme)
{
    if (m_colorScheme == scheme) {
        return;
    }
    m_colorScheme = scheme;
    applyThemeStyle();
    emit colorSchemeChanged(m_colorScheme);
}

void InputButton::changeEvent(QEvent *event)
{
    QPushButton::changeEvent(event);
    if (event->type() == QEvent::PaletteChange ||
        event->type() == QEvent::ApplicationPaletteChange ||
        event->type() == QEvent::StyleChange) {
        applyThemeStyle();
    }
}

void InputButton::applyThemeStyle()
{
    if (m_colorScheme == Default) {
        setStyleSheet(QString());
        return;
    }

    const QPalette pal = palette();
    const QColor windowColor = pal.color(QPalette::Window);
    const QColor buttonColor = pal.color(QPalette::Button);
    const bool isDark = windowColor.lightness() < 128;

    QColor bg;
    QColor fg("#FFFFFF");

    switch (m_colorScheme) {
    case Primary: {
        QColor highlight = pal.color(QPalette::Highlight);
        if (highlight.isValid() && highlight.saturation() > 30) {
            bg = highlight;
            fg = pal.color(QPalette::HighlightedText);
        } else {
            bg = isDark ? QColor("#1976D2") : QColor("#1565C0");
        }
        break;
    }
    case Secondary: {
        bg = isDark ? buttonColor.lighter(125) : buttonColor.darker(112);
        fg = pal.color(QPalette::ButtonText);
        break;
    }
    case Success:
        bg = isDark ? QColor("#388E3C") : QColor("#2E7D32");
        break;
    case Warning:
        bg = isDark ? QColor("#F57C00") : QColor("#EF6C00");
        break;
    case Danger:
        bg = isDark ? QColor("#D32F2F") : QColor("#C62828");
        break;
    case Default:
        break;
    }

    // Harmoniza levemente a cor com a superfície do tema atual do Qt
    bg = blendColors(bg, buttonColor, 0.08);

    QColor hoverBg = isDark ? bg.lighter(115) : bg.darker(110);
    QColor pressedBg = isDark ? bg.darker(115) : bg.darker(125);
    QColor borderCol = isDark ? bg.lighter(125) : bg.darker(118);

    QColor disabledBg = blendColors(buttonColor, windowColor, 0.5);
    QColor disabledFg = pal.color(QPalette::Disabled, QPalette::ButtonText);
    QColor disabledBorder = pal.color(QPalette::Mid);

    // QString qss = QString(
    //     "InputButton {"
    //     "   background-color: %1;"
    //     "   color: %2;"
    //     "   border: 1px solid %3;"
    //     "   border-radius: 4px;"
    //     "   padding: 5px 14px;"
    //     "   font-weight: 600;"
    //     "}"
    //     "InputButton:hover {"
    //     "   background-color: %4;"
    //     "}"
    //     "InputButton:pressed {"
    //     "   background-color: %5;"
    //     "}"
    //     "InputButton:disabled {"
    //     "   background-color: %6;"
    //     "   color: %7;"
    //     "   border: 1px solid %8;"
    //     "}"
    // ).arg(
    //     bg.name(),
    //     fg.name(),
    //     borderCol.name(),
    //     hoverBg.name(),
    //     pressedBg.name(),
    //     disabledBg.name(),
    //     disabledFg.name(),
    //     disabledBorder.name()
    // );

    // setStyleSheet(qss);
}
