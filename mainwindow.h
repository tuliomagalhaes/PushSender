#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTextEdit>
#include <QLineEdit>
#include <QPushButton>
#include <QtNetwork/QNetworkReply>

enum OSTypeSelected {
    IOS, ANDROID
};

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_pbSend_clicked();
    void on_pbBrowse_clicked();
    void on_rbIos_clicked();
    void on_rbAndroid_clicked();

private:
    Ui::MainWindow *ui;

    QString getiOSJson();
    QString getAndroidJson();

    OSTypeSelected m_OSTypeSelected = IOS;
};

#endif // MAINWINDOW_H
