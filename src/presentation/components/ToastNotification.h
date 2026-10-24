#pragma once

#include <QWidget>
#include <QString>
#include <QTimer>
#include <QLabel>
#include <QProgressBar>
#include <QPropertyAnimation>

class ToastNotification : public QWidget {
    Q_OBJECT

public:
    enum Type { Success, Error, Warning, Info };

    explicit ToastNotification(QWidget *parent, const QString &title, const QString &body, Type type, int durationMs = 3000);

    static void show(QWidget *parent, const QString &title, const QString &body, Type type, int durationMs = 3000);

protected:
    void paintEvent(QPaintEvent *event) override;

private slots:
    void updateProgress();
    void startFadeOut();

private:
    QProgressBar *m_progressBar;
    QTimer *m_timer;
    int m_duration;
    int m_elapsed;
    QPropertyAnimation *m_opacityAnimation;
};
