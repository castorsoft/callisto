#include "InputLookup.h"
#include <QVBoxLayout>
#include <QKeyEvent>
#include <QStyle>
#include <QGuiApplication>
#include <QScreen>
#include <QCoreApplication>
#include <algorithm>

InputLookup::InputLookup(QWidget *parent)
    : InputBase(parent)
    , m_lineEdit(nullptr)
    , m_searchAction(nullptr)
    , m_listWidget(nullptr)
    , m_debounceTimer(new QTimer(this))
{
    setupUI();
}

InputLookup::~InputLookup()
{
    if (m_listWidget) {
        delete m_listWidget;
    }
}

void InputLookup::setupUI()
{
    m_lineEdit = new QLineEdit(this);
    m_lineEdit->setClearButtonEnabled(true);
    m_lineEdit->setPlaceholderText(QStringLiteral("Digite para buscar..."));

    // Ícone de seta para baixo / busca no final do lineEdit
    m_searchAction = m_lineEdit->addAction(
        m_lineEdit->style()->standardIcon(QStyle::SP_ArrowDown),
        QLineEdit::TrailingPosition
    );

    setInputWidget(m_lineEdit);

    // Lista suspensa flutuante como popup direto
    m_listWidget = new QListWidget(nullptr);
    m_listWidget->setWindowFlags(Qt::Popup | Qt::FramelessWindowHint);
    m_listWidget->setFocusPolicy(Qt::StrongFocus);
    m_listWidget->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    m_listWidget->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);

    m_listWidget->setStyleSheet(
        "QListWidget {"
        "   border: 1px solid palette(mid);"
        "   border-radius: 4px;"
        "   background-color: palette(base);"
        "}"
        "QListWidget::item {"
        "   padding: 6px 10px;"
        "}"
        "QListWidget::item:hover {"
        "   background-color: palette(highlight);"
        "   color: palette(highlighted-text);"
        "}"
        "QListWidget::item:selected {"
        "   background-color: palette(highlight);"
        "   color: palette(highlighted-text);"
        "}"
    );

    // Configuração do Timer para Debounce
    m_debounceTimer->setSingleShot(true);
    connect(m_debounceTimer, &QTimer::timeout, this, &InputLookup::onSearchTimeout);

    // Conexões do LineEdit e Ações
    connect(m_lineEdit, &QLineEdit::textChanged, this, &InputLookup::onTextChanged);
    connect(m_searchAction, &QAction::triggered, this, [this]() {
        if (m_listWidget->isVisible()) {
            hidePopup();
        } else {
            triggerSearch(m_lineEdit->text().trimmed());
        }
    });

    // Conexão da lista suspensa
    connect(m_listWidget, &QListWidget::itemClicked, this, &InputLookup::onItemClicked);

    // Filtros de eventos para controle refinado de foco e navegação por teclado
    m_lineEdit->installEventFilter(this);
    m_listWidget->installEventFilter(this);
}

QVariant InputLookup::currentData() const
{
    return m_currentData;
}

QString InputLookup::currentText() const
{
    return m_currentText;
}

bool InputLookup::hasSelection() const
{
    return m_currentData.isValid() && !m_currentData.isNull();
}

void InputLookup::setCurrentData(const QVariant &data, const QString &displayText)
{
    m_isSelecting = true;
    m_currentData = data;
    m_currentText = displayText;
    m_lineEdit->setText(displayText);
    m_isSelecting = false;
    hidePopup();
}

void InputLookup::clear()
{
    m_isSelecting = true;
    m_currentData.clear();
    m_currentText.clear();
    m_lineEdit->clear();
    m_isSelecting = false;
    hidePopup();
    emit selectionCleared();
    emit currentDataChanged(m_currentData);
}

void InputLookup::setResults(const QList<QPair<QString, QVariant>> &items)
{
    m_listWidget->clear();
    if (items.isEmpty()) {
        showNoResults();
        return;
    }

    for (const auto &pair : items) {
        addItem(pair.first, pair.second);
    }
    showPopup();
}

void InputLookup::addItem(const QString &text, const QVariant &data)
{
    auto *item = new QListWidgetItem(text, m_listWidget);
    item->setData(Qt::UserRole, data);
}

void InputLookup::showLoading()
{
    m_listWidget->clear();
    auto *item = new QListWidgetItem(QStringLiteral("Buscando..."), m_listWidget);
    item->setFlags(Qt::NoItemFlags);
    item->setForeground(palette().color(QPalette::Disabled, QPalette::Text));
    QFont font = item->font();
    font.setItalic(true);
    item->setFont(font);
    showPopup();
}

void InputLookup::showNoResults(const QString &message)
{
    m_listWidget->clear();
    auto *item = new QListWidgetItem(message, m_listWidget);
    item->setFlags(Qt::NoItemFlags);
    item->setForeground(palette().color(QPalette::Disabled, QPalette::Text));
    QFont font = item->font();
    font.setItalic(true);
    item->setFont(font);
    showPopup();
}

void InputLookup::setSearchCallback(SearchCallback callback)
{
    m_searchCallback = std::move(callback);
}

QString InputLookup::placeholderText() const
{
    return m_lineEdit->placeholderText();
}

void InputLookup::setPlaceholderText(const QString &text)
{
    m_lineEdit->setPlaceholderText(text);
}

int InputLookup::minSearchChars() const
{
    return m_minSearchChars;
}

void InputLookup::setMinSearchChars(int chars)
{
    m_minSearchChars = chars;
}

int InputLookup::searchDebounceMs() const
{
    return m_searchDebounceMs;
}

void InputLookup::setSearchDebounceMs(int ms)
{
    m_searchDebounceMs = ms;
}

void InputLookup::onTextChanged(const QString &text)
{
    if (m_isSelecting) {
        return;
    }

    if (text.trimmed().isEmpty()) {
        m_currentData.clear();
        m_currentText.clear();
        hidePopup();
        emit selectionCleared();
        emit currentDataChanged(m_currentData);
        return;
    }

    // Se o usuário digitou e o texto diverge do selecionado anteriormente, invalida a seleção
    if (m_currentData.isValid() && text != m_currentText) {
        m_currentData.clear();
        emit currentDataChanged(m_currentData);
    }

    m_debounceTimer->start(m_searchDebounceMs);
}

void InputLookup::onSearchTimeout()
{
    triggerSearch(m_lineEdit->text().trimmed());
}

void InputLookup::triggerSearch(const QString &query)
{
    if (m_isSelecting) {
        return;
    }

    if (query.length() < m_minSearchChars) {
        hidePopup();
        return;
    }

    if (m_searchCallback) {
        showLoading();
        auto results = m_searchCallback(query);
        setResults(results);
    } else {
        showLoading();
        emit searchRequested(query);
    }
}

void InputLookup::onItemClicked(QListWidgetItem *item)
{
    if (!item) {
        return;
    }

    QVariant data = item->data(Qt::UserRole);
    if (!data.isValid()) {
        return; // Item de aviso (Buscando... ou Nenhum registro)
    }

    m_isSelecting = true;
    m_currentData = data;
    m_currentText = item->text();
    m_lineEdit->setText(m_currentText);
    m_isSelecting = false;

    hidePopup();
    m_lineEdit->setFocus();
    emit itemSelected(m_currentData, m_currentText);
    emit currentDataChanged(m_currentData);
}

void InputLookup::showPopup()
{
    if (m_listWidget->count() == 0) {
        return;
    }
    updatePopupGeometry();

    bool listHadFocus = m_listWidget->hasFocus();

    m_listWidget->show();
    m_listWidget->raise();

    // Mantém o foco no input enquanto digita e após a busca, a menos que a lista já tivesse foco
    if (!listHadFocus) {
        m_lineEdit->setFocus(Qt::OtherFocusReason);
    }
}

void InputLookup::hidePopup()
{
    if (m_listWidget && m_listWidget->isVisible()) {
        m_listWidget->hide();
    }
}

void InputLookup::updatePopupGeometry()
{
    if (!m_listWidget || !m_lineEdit) {
        return;
    }

    QPoint globalPos = m_lineEdit->mapToGlobal(QPoint(0, m_lineEdit->height() + 2));
    int width = std::max(m_lineEdit->width(), 320);

    int rowHeight = m_listWidget->sizeHintForRow(0);
    if (rowHeight <= 0) {
        rowHeight = 32;
    }
    int count = std::max(1, m_listWidget->count());
    int height = std::min(rowHeight * count + 8, 240);

    QScreen *screen = QGuiApplication::screenAt(globalPos);
    if (!screen) {
        screen = QGuiApplication::primaryScreen();
    }

    if (screen) {
        QRect screenGeo = screen->availableGeometry();
        if (globalPos.y() + height > screenGeo.bottom()) {
            globalPos = m_lineEdit->mapToGlobal(QPoint(0, -height - 2));
        }
        if (globalPos.x() + width > screenGeo.right()) {
            globalPos.setX(screenGeo.right() - width - 4);
        }
    }

    m_listWidget->setGeometry(globalPos.x(), globalPos.y(), width, height);
}

bool InputLookup::eventFilter(QObject *watched, QEvent *event)
{
    // 1. Eventos no campo de digitação (m_lineEdit)
    if (watched == m_lineEdit && event->type() == QEvent::KeyPress) {
        auto *keyEvent = static_cast<QKeyEvent*>(event);
        int key = keyEvent->key();

        if (key == Qt::Key_Down) {
            // Seta para baixo: foco sai do input e vai para a lista
            if (m_listWidget->isVisible() && m_listWidget->count() > 0) {
                m_listWidget->setFocus(Qt::TabFocusReason);
                int nextRow = m_listWidget->currentRow() + 1;
                if (m_listWidget->currentRow() < 0) {
                    nextRow = 0;
                } else if (nextRow >= m_listWidget->count()) {
                    nextRow = 0;
                }
                m_listWidget->setCurrentRow(nextRow);
                return true;
            } else {
                triggerSearch(m_lineEdit->text().trimmed());
                return true;
            }
        } else if (key == Qt::Key_Return || key == Qt::Key_Enter) {
            if (m_listWidget->isVisible() && m_listWidget->currentRow() >= 0) {
                auto *item = m_listWidget->currentItem();
                if (item && item->data(Qt::UserRole).isValid()) {
                    onItemClicked(item);
                    return true;
                }
            }
            m_debounceTimer->stop();
            triggerSearch(m_lineEdit->text().trimmed());
            return true;
        } else if (key == Qt::Key_Escape) {
            if (m_listWidget->isVisible()) {
                hidePopup();
                return true;
            }
        }
    }
    // 2. Eventos na lista suspensa quando ela possui foco (m_listWidget)
    else if (watched == m_listWidget && event->type() == QEvent::KeyPress) {
        auto *keyEvent = static_cast<QKeyEvent*>(event);
        int key = keyEvent->key();

        if (key == Qt::Key_Return || key == Qt::Key_Enter) {
            auto *item = m_listWidget->currentItem();
            if (item && item->data(Qt::UserRole).isValid()) {
                onItemClicked(item);
                return true;
            }
        } else if (key == Qt::Key_Escape) {
            hidePopup();
            m_lineEdit->setFocus();
            return true;
        } else if (key == Qt::Key_Up) {
            // Se estiver no primeiro item e pressionar seta para cima, volta o foco para o input
            if (m_listWidget->currentRow() <= 0) {
                m_listWidget->setCurrentRow(-1);
                m_lineEdit->setFocus();
                return true;
            }
        } else if (key == Qt::Key_Tab || key == Qt::Key_Backtab) {
            hidePopup();
            return false;
        } else if (!keyEvent->text().isEmpty() && (key != Qt::Key_Down && key != Qt::Key_Up)) {
            // Se o usuário começar a digitar texto enquanto está na lista, devolve o foco para o input
            m_lineEdit->setFocus();
            QCoreApplication::sendEvent(m_lineEdit, event);
            return true;
        }
    }

    return InputBase::eventFilter(watched, event);
}
