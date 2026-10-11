#pragma once

#include <QMainWindow>
#include <QListWidgetItem>
#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

    void addSubForm(QWidget *form);
    void closeSubForm(int index);

    template <typename T>
    bool existeFormulario();

private:
    Ui::MainWindow *ui;
    void verificarEmpresaInicial();
    void loadPluginButtons();
    void loadNativeButtons();

    void addActionToList(QAction *action);
    void onListClick(QListWidgetItem* item);

    void onClickEmpresas();
    void onClickPessoas();
    void onClickProcessarXml();
};