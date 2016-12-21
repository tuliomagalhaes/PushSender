#include "apnsconnector.h"
#include <QDebug>
#include <QSslKey>
#include <QByteArray>
#include <QDataStream>
#include <QFile>
#include <QSslCertificate>
#include <QCoreApplication>
#include <QList>
#include <QSslConfiguration>
#include <QtCrypto>

APNSConnector::APNSConnector(QString p12FilePath, QString p12FilePassword) {
//    WITH QT CRYPTO (EXTERNAL LIBRARY)
    QCA::init();

    QCA::KeyBundle bundle(p12FilePath, QCA::SecureArray(p12FilePassword.toLocal8Bit()));

    QList<QSslCertificate> CACerts;
    QSslConfiguration sslConfig = QSslConfiguration::defaultConfiguration();

    foreach (QCA::Certificate cert, bundle.certificateChain()) {
        if (cert.isCA()) {
            QSslCertificate CACertificate(cert.toPEM().toLocal8Bit(), QSsl::Pem);
            CACerts.append(CACertificate);
        } else {
            sslConfig.setLocalCertificate(QSslCertificate(cert.toPEM().toLocal8Bit(), QSsl::Pem));
        }
    }

    sslConfig.setCaCertificates(CACerts);
    QSslKey key(bundle.privateKey().toDER().toByteArray(), QSsl::Rsa, QSsl::Der);
    sslConfig.setPrivateKey(key);
    m_sslSocket.setSslConfiguration(sslConfig);

//    WITH IMPORTPKCS12 (QT NATIVE METHOD)

//    QFile certFile(p12FilePath);
//    certFile.open(QIODevice::ReadOnly);

//    QSslKey sslKey;
//    QSslCertificate sslCert;
//    QSslCertificate::importPkcs12(&certFile, &sslKey, &sslCert, Q_NULLPTR, p12FilePassword.toUtf8());

//    QSslConfiguration sslConfig = QSslConfiguration::defaultConfiguration();
//    sslConfig.setLocalCertificate(sslCert);
//    sslConfig.setPrivateKey(sslKey);
//    m_sslSocket.setSslConfiguration(sslConfig);

    connect(&m_sslSocket, SIGNAL(sslErrors(QList<QSslError>)),
                this, SLOT(sslError(QList<QSslError>)) );
    connect(&m_sslSocket, SIGNAL(error(QAbstractSocket::SocketError)),
          this, SLOT(socketError(QAbstractSocket::SocketError)));
    connect(&m_sslSocket, SIGNAL(encrypted()), this, SLOT(socketReadyRead()));

    m_sslSocket.connectToHostEncrypted("gateway.sandbox.push.apple.com", 2195);
}

APNSConnector::~APNSConnector() {
    m_sslSocket.disconnectFromHost();
    m_sslSocket.waitForDisconnected();
}

void APNSConnector::sslError(QList<QSslError> errors) {
    QString erroStr="";
    foreach (const QSslError &e, errors)
        erroStr.append(e.errorString()).append("\n");

    qDebug() << "Error: " << erroStr;

    m_sslSocket.ignoreSslErrors();
}

void APNSConnector::socketError(QAbstractSocket::SocketError) {
    qDebug() << "Connection error: " << m_sslSocket.errorString();
}

void APNSConnector::socketReadyRead() {
    qDebug() << "ready";
}

void APNSConnector::sendPushByDeviceId(QString& deviceId) {
    QByteArray deviceToken = QByteArray::fromHex(deviceId.toUtf8());
    QByteArray payload = "{\"aps\":{\"alert\":\"It's simple!\"}}";

    QByteArray content;
    QDataStream out(&content, QIODevice::WriteOnly);
    out << (quint8) 1;
    out << (quint32) 1;
    out << (quint32) 2147483647;
    out << (quint16) deviceToken.size();
    out.writeRawData(deviceToken, deviceToken.size());
    out << (quint16) payload.size();
    out.writeRawData(payload, deviceToken.size());

    m_sslSocket.write(content);
    m_sslSocket.flush();
}
