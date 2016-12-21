#ifndef GCMCONNECTOR_H
#define GCMCONNECTOR_H

#include <QObject>
#include <QString>
#include <QtNetwork/QNetworkReply>

class GCMConnector : public QObject
{
    Q_OBJECT
public:
    GCMConnector();

    void sendPush(QString& appId, QString& json);
private slots:
    void onNetworkResponse(QNetworkReply* reply);
};

#endif // GCMCONNECTOR_H
