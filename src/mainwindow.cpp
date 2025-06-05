#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <string.h>
#include <cstring>

#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , context(NULL)
    , graphwidget(new GraphWidget(this))
{
    ui->setupUi(this);
    ui->gwlayout->addWidget(graphwidget);

    doOperation(CreateContext, &context, NULL);
    if (context != NULL) {
        graphwidget->setContext(context);
        ui->tableWidget->setColumnCount(7);
        QStringList headerLabels;
        headerLabels << "Year" << "Region" << "Nat. Pop. Growth"
                    << "Birth Rate" << "Death Rate" << "Dem. Weight" << "Urbanization";
        ui->tableWidget->setHorizontalHeaderLabels(headerLabels);
    } else {
        QMessageBox::critical(this, "Error", "Failed to initialize application context!");
    }
}

MainWindow::~MainWindow() {
    if (context != NULL) {
        doOperation(ClearContext, &context, NULL);
    }
    delete ui;
}

void MainWindow::on_openFileButton_clicked() {
    QString newFilename = QFileDialog::getOpenFileName(this, "Open CSV File", "", "CSV Files (*.csv)");
    if (!newFilename.isEmpty()) {
        filename = newFilename;
        ui->filePathLineEdit->setText(filename);
    }
}

void MainWindow::on_loadDataButton_clicked() {
    if (filename.isEmpty()) {
        QMessageBox::warning(this, "Warning", "Please select a CSV file first.");
    } else if (context == NULL) {
        QMessageBox::critical(this, "Error", "Application context is not initialized!");
    } else {
        Params param;
        strncpy(param.params.loadDataParams.filename, filename.toStdString().c_str(), sizeof(param.params.loadDataParams.filename) - 1);
        doOperation(LoadData, &context, &param);

        if (context->errorCode != OK) {
            QMessageBox::critical(this, "Error",
                QString("Error loading data: %1 %2").arg(context->errorMessage).arg(filename));
        } else {
            QMessageBox::information(this, "Information",
                QString("Total rows: %1\nValid rows: %2\nError rows: %3")
                .arg(context->totalRows)
                .arg(context->validRows)
                .arg(context->errorRows));
            displayDataInTable(ui->regionLineEdit->text());
        }
    }
}

void MainWindow::displayDataInTable(const QString& region) {
    ui->tableWidget->setRowCount(0);

    if (context == NULL || context->dataList == NULL) {
        QMessageBox::critical(this, "Error", "No data available to display!");
        return;
    }

    Iterator* iterator;
    createIterator(&iterator, context->dataList);
    if (iterator == NULL) {
        QMessageBox::critical(this, "Error", "Failed to create data iterator!");
        return;
    }

    DemographyData* currentData = NULL;
    while ((currentData = next(iterator)) != NULL) {
        if (currentData->valid && (region.isEmpty() || QString::fromStdString(currentData->region) == region)) {
            int row = ui->tableWidget->rowCount();
            ui->tableWidget->insertRow(row);

            ui->tableWidget->setItem(row, 0, new QTableWidgetItem(QString::number(currentData->year)));
            ui->tableWidget->setItem(row, 1, new QTableWidgetItem(QString::fromStdString(currentData->region)));
            ui->tableWidget->setItem(row, 2, new QTableWidgetItem(QString::number(currentData->natural_population_growth)));
            ui->tableWidget->setItem(row, 3, new QTableWidgetItem(QString::number(currentData->birth_rate)));
            ui->tableWidget->setItem(row, 4, new QTableWidgetItem(QString::number(currentData->death_rate)));
            ui->tableWidget->setItem(row, 5, new QTableWidgetItem(QString::number(currentData->general_demographic_weight)));
            ui->tableWidget->setItem(row, 6, new QTableWidgetItem(QString::number(currentData->urbanization)));
        }
    }
    freeIterator(iterator);
}

void MainWindow::on_calculateButton_clicked() {
    if (filename.isEmpty()) {
        QMessageBox::warning(this, "Warning", "Please select a CSV file first.");
    } else if (context == NULL) {
        QMessageBox::critical(this, "Error", "Application context is not initialized!");
    } else {
        int column = ui->columnSpinBox->value(); //колонки 1-7 индексы 0-6
        Params param;
        strncpy(param.params.calculateStatsParams.region, ui->regionLineEdit->text().toStdString().c_str(), sizeof(param.params.calculateStatsParams.region) - 1);
        param.params.calculateStatsParams.column = column;
        doOperation(CalcStats, &context, &param);

        if (context->errorCode != OK) {
            QMessageBox::critical(this, "Error",
                QString("Error calculating statistics: %1").arg(context->errorMessage));
        } else {
            ui->minLineEdit->setText(QString::number(context->calcValues->min));
            ui->maxLineEdit->setText(QString::number(context->calcValues->max));
            ui->medianLineEdit->setText(QString::number(context->calcValues->median));

            graphwidget->update();
        }
    }
}
