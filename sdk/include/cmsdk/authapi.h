#pragma once

#include <QtCore/QObject>
#include <QtCore/QString>
#include <QtCore/QJsonObject>

namespace cmsdk {

class AuthApi : public QObject
{
    Q_OBJECT

public:
    explicit AuthApi(QObject *parent = nullptr);
    
    // Server configuration
    void setServer(const QString &host, quint16 port = 5555);
    QString serverHost() const { return m_host; }
    quint16 serverPort() const { return m_port; }
    
    // Authentication operations (synchronous)
    bool registerUser(const QString &username, const QString &password, QString *errorMsg = nullptr);
    bool loginUser(const QString &username, const QString &password, QString *errorMsg = nullptr);

private:
    QString sendCommand(const QString &command, const QJsonObject &params = QJsonObject());
    
    QString m_host;
    quint16 m_port;
};

}