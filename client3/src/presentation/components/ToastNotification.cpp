#include "ToastNotification.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPainter>
#include <QGraphicsOpacityEffect>
#include <QApplication>
#include <QScreen>

ToastNotification::ToastNotification(QWidget *parent, const QString &title, const QString &body, Type type, int durationMs)
    : QWidget(parent), m_duration(durationMs), m_elapsed(0)
{
    // Configure window to be a frameless tooltip on top
    setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
    setAttribute(Qt::WA_TranslucentBackground);
    setAttribute(Qt::WA_ShowWithoutActivating);
    setAttribute(Qt::WA_DeleteOnClose);

    // Setup UI
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);

    QWidget *container = new QWidget(this);
    container->setObjectName("toastContainer");
    QVBoxLayout *containerLayout = new QVBoxLayout(container);
    containerLayout->setContentsMargins(10, 10, 10, 5);

    QLabel *lblTitle = new QLabel(title, container);
    lblTitle->setStyleSheet("font-weight: bold; font-size: 14px;");

    QLabel *lblBody = new QLabel(body, container);
    lblBody->setWordWrap(true);
    lblBody->setStyleSheet("font-size: 12px;");

    containerLayout->addWidget(lblTitle);
    containerLayout->addWidget(lblBody);

    m_progressBar = new QProgressBar(this);
    m_progressBar->setFixedHeight(3);
    m_progressBar->setTextVisible(false);
    m_progressBar->setRange(0, m_duration);
    m_progressBar->setValue(m_duration);

    mainLayout->addWidget(container);
    mainLayout->addWidget(m_progressBar);

    // Determine colors based on type
    QString accentColor;
    switch (type) {
        case Success: accentColor = "#4CAF50"; break; // Green
        case Error:   accentColor = "#F44336"; break; // Red
        case Warning: accentColor = "#FF9800"; break; // Orange
        case Info:    accentColor = "#2196F3"; break; // Blue
    }

    // Stylesheet
    QString style = QString(
        "QWidget#toastContainer { "
        "   background-color: #2D2D30; "
        "   border-top-left-radius: 4px; "
        "   border-top-right-radius: 4px; "
        "   border-left: 4px solid %1; "
        "   color: #FFFFFF; "
        "}"
        "QProgressBar { "
        "   background-color: #2D2D30; "
        "   border: none; "
        "   border-bottom-left-radius: 4px; "
        "   border-bottom-right-radius: 4px; "
        "}"
        "QProgressBar::chunk { "
        "   background-color: %1; "
        "   border-bottom-left-radius: 4px; "
        "}"
    ).arg(accentColor);

    setStyleSheet(style);
    setFixedSize(300, 80);

    // Fade effect setup
    QGraphicsOpacityEffect *eff = new QGraphicsOpacityEffect(this);
    this->setGraphicsEffect(eff);
    m_opacityAnimation = new QPropertyAnimation(eff, "opacity");
    m_opacityAnimation->setDuration(500);
    m_opacityAnimation->setStartValue(1.0);
    m_opacityAnimation->setEndValue(0.0);
    connect(m_opacityAnimation, &QPropertyAnimation::finished, this, &QWidget::close);

    // Progress timer
    m_timer = new QTimer(this);
    connect(m_timer, &QTimer::timeout, this, &ToastNotification::updateProgress);

    int interval = 16; // ~60fps
    m_timer->start(interval);
}

void ToastNotification::updateProgress() {
    m_elapsed += 16;
    int remaining = m_duration - m_elapsed;
    if (remaining <= 0) {
        m_progressBar->setValue(0);
        m_timer->stop();
        startFadeOut();
    } else {
        m_progressBar->setValue(remaining);
    }
}

void ToastNotification::startFadeOut() {
    m_opacityAnimation->start();
}

void ToastNotification::paintEvent(QPaintEvent *event) {
    QWidget::paintEvent(event);
}

void ToastNotification::show(QWidget *parent, const QString &title, const QString &body, Type type, int durationMs) {
    ToastNotification *toast = new ToastNotification(parent, title, body, type, durationMs);

    // Position at bottom right of the parent widget, or screen if parent is null
    int margin = 20;
    QRect geometry;

    if (parent && parent->window()) {
        geometry = parent->window()->geometry();
    } else {
        geometry = QGuiApplication::primaryScreen()->availableGeometry();
    }

    int x = geometry.x() + geometry.width() - toast->width() - margin;
    int y = geometry.y() + geometry.height() - toast->height() - margin;

    toast->move(x, y);
    toast->QWidget::show();
}
