#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "apnsconnector.h"
#include "gcmconnector.h"

#include <QString>
#include <QJsonDocument>
#include <QMessageBox>
#include <QDir>
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->teJSON->setText(getiOSJson());
    ui->lbStatus->hide();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pbSend_clicked()
{
    if(m_OSTypeSelected == IOS) {
        APNSConnector connector(ui->leP12FileLocation->text(), ui->leP12FilePassword->text());
        connector.sendPushByDeviceId(ui->leId->text());
    } else {
        GCMConnector connector;
        connector.sendPush(ui->leId->text(), ui->teJSON->toPlainText());
    }
}

QString MainWindow::getiOSJson() {
    return "{\n    \"aps\": {\n        \"alert\": \"It's simple!\"\n    }\n}";
}

QString MainWindow::getAndroidJson() {
    return "{\n    \"registration_ids\": [\n\n    ],\n    \"data\": {\n    }\n}";
}

void MainWindow::on_pbBrowse_clicked()
{
    QString directory = QFileDialog::getOpenFileName(this,
                                tr("Find p12 file"), QDir::homePath(), "*.p12");

    if (!directory.isEmpty()) {
        ui->leP12FileLocation->setText(directory);
        ui->leP12FileLocation->setCursorPosition(0);
    }
}

void MainWindow::on_rbIos_clicked()
{
    m_OSTypeSelected = IOS;
    ui->lbId->setText("Device ID:");
    ui->lbAppleCertificate->show();
    ui->leP12FileLocation->show();
    ui->lbP12FilePassword->show();
    ui->leP12FilePassword->show();
    ui->pbBrowse->show();
    ui->teJSON->setText(getiOSJson());
    ui->leId->setText("");
}

void MainWindow::on_rbAndroid_clicked()
{
    m_OSTypeSelected = ANDROID;
    ui->lbId->setText("App ID:");
    ui->lbAppleCertificate->hide();
    ui->leP12FileLocation->hide();
    ui->lbP12FilePassword->hide();
    ui->leP12FilePassword->hide();
    ui->pbBrowse->hide();
    ui->teJSON->setText(getAndroidJson());
    ui->leId->setText("");
}
