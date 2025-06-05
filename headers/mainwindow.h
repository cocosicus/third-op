#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QMessageBox>
#include <QTableWidget>

#include "graphwidget.h"

extern "C" {
    #include "entryPoint.h"
    #include "linkedList.h"
}

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_openFileButton_clicked();
    void on_loadDataButton_clicked();
    void on_calculateButton_clicked();

private:
    Ui::MainWindow *ui;
    Context* context;
    QString filename;
    GraphWidget* graphwidget;

    void displayDataInTable(const QString& region = "");
};
#endif // MAINWINDOW_H
