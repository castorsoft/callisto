#pragma once

#include "InputBase.h"
#include <QLineEdit>
#include <QListWidget>
#include <QTimer>
#include <QVariant>
#include <QPair>
#include <QList>
#include <functional>

class InputLookup : public InputBase {
    Q_OBJECT
    Q_PROPERTY(QString placeholderText READ placeholderText WRITE setPlaceholderText)
    Q_PROPERTY(int minSearchChars READ minSearchChars WRITE setMinSearchChars)
    Q_PROPERTY(int searchDebounceMs READ searchDebounceMs WRITE setSearchDebounceMs)

public:
    using SearchCallback = std::function<QList<QPair<QString, QVariant>>(const QString &)>;

    explicit InputLookup(QWidget *parent = nullptr);
    ~InputLookup() override;

    // Acesso aos dados selecionados
    QVariant currentData() const;
    QString currentText() const;
    bool hasSelection() const;

    void setCurrentData(const QVariant &data, const QString &displayText);
    void clear();

    // Alimentação dos resultados de busca
    void setResults(const QList<QPair<QString, QVariant>> &items);
    void addItem(const QString &text, const QVariant &data = QVariant());
    void showLoading();
    void showNoResults(const QString &message = QStringLiteral("Nenhum registro encontrado"));

    // Callback de busca direto (opcional, como alternativa a signals)
    void setSearchCallback(SearchCallback callback);

    // Propriedades
    QString placeholderText() const;
    void setPlaceholderText(const QString &text);

    int minSearchChars() const;
    void setMinSearchChars(int chars);

    int searchDebounceMs() const;
    void setSearchDebounceMs(int ms);

    QLineEdit* lineEdit() const { return m_lineEdit; }
    QListWidget* listWidget() const { return m_listWidget; }

signals:
    void searchRequested(const QString &query);
    void itemSelected(const QVariant &data, const QString &text);
    void currentDataChanged(const QVariant &data);
    void selectionCleared();

protected:
    bool eventFilter(QObject *watched, QEvent *event) override;

private slots:
    void onTextChanged(const QString &text);
    void onSearchTimeout();
    void onItemClicked(QListWidgetItem *item);

private:
    void setupUI();
    void showPopup();
    void hidePopup();
    void updatePopupGeometry();
    void triggerSearch(const QString &query);

    QLineEdit *m_lineEdit;
    QAction *m_searchAction;

    QListWidget *m_listWidget;

    QTimer *m_debounceTimer;
    SearchCallback m_searchCallback;

    QVariant m_currentData;
    QString m_currentText;

    int m_minSearchChars = 2;
    int m_searchDebounceMs = 300;
    bool m_isSelecting = false;
};
