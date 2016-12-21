#include "gcmconnector.h"

#include <QUrl>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkAccessManager>
#include <QSslConfiguration>
#include <QByteArray>
#include <QVariant>

GCMConnector::GCMConnector(){}

void GCMConnector::sendPush(QString& appId, QString& json) {
    QUrl url("https://gcm-http.googleapis.com/gcm/send");
    QNetworkRequest request(url);

    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    QString authorization = "key=" + appId;
    request.setRawHeader("Authorization", authorization.toUtf8());

    QSslConfiguration config = QSslConfiguration::defaultConfiguration();
     config.setProtocol(QSsl::TlsV1_2);
     request.setSslConfiguration(config);

    QNetworkAccessManager *manager = new QNetworkAccessManager(this);

    connect(manager, SIGNAL(finished(QNetworkReply*)),
            this, SLOT(onNetworkResponse(QNetworkReply*)));

    QByteArray data = json.toUtf8();

    qDebug() << "Sync" << QString::fromUtf8(data.data(), data.size());

    manager->post(request, data);
}

void GCMConnector::onNetworkResponse(QNetworkReply* reply) {
    QVariant statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
    if(statusCode.isValid()){
        if(statusCode.toInt() == 200) {
            qDebug() << "Success: " << reply->readAll().data();
        } else {
            qDebug() << "Error: " << reply->readAll().data();
        }
    }
}
