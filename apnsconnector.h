#ifndef APNSCONNECTOR_H
#define APNSCONNECTOR_H

#include <QObject>
#include <QString>
#include <QSslSocket>
#include <QAbstractSocket>

class APNSConnector : public QObject
{
    Q_OBJECT

public:
    APNSConnector(QString p12FilePath, QString p12FilePassword);
    ~APNSConnector();

    void sendPushByDeviceId(QString& deviceId);

private slots:
    void sslError(QList<QSslError> errors);
    void socketError(QAbstractSocket::SocketError);
    void socketReadyRead();

private:
    QSslSocket m_sslSocket;
};

#endif // APNSCONNECTOR_H
