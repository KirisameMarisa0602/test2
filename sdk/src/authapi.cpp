#include "cmsdk/authapi.h"
#include <QtNetwork/QTcpSocket>
#include <QtCore/QJsonDocument>
#include <QtCore/QJsonObject>
#include <QtCore/QEventLoop>
#include <QtCore/QTimer>

namespace cmsdk {

AuthApi::AuthApi(QObject *parent)
    : QObject(parent)
    , m_host("127.0.0.1")
    , m_port(5555)
{
}

void AuthApi::setServer(const QString &host, quint16 port)
{
    m_host = host;
    m_port = port;
}

bool AuthApi::registerUser(const QString &username, const QString &password, QString *errorMsg)
{
    QJsonObject params;
    params["username"] = username;
    params["password"] = password;
    
    QString response = sendCommand("register", params);
    if (response.isEmpty()) {
        if (errorMsg) *errorMsg = "Failed to connect to server";
        return false;
    }
    
    QJsonDocument doc = QJsonDocument::fromJson(response.toUtf8());
    QJsonObject obj = doc.object();
    
    if (obj["status"].toString() == "success") {
        return true;
    } else {
        if (errorMsg) *errorMsg = obj["message"].toString();
        return false;
    }
}

bool AuthApi::loginUser(const QString &username, const QString &password, QString *errorMsg)
{
    QJsonObject params;
    params["username"] = username;
    params["password"] = password;
    
    QString response = sendCommand("login", params);
    if (response.isEmpty()) {
        if (errorMsg) *errorMsg = "Failed to connect to server";
        return false;
    }
    
    QJsonDocument doc = QJsonDocument::fromJson(response.toUtf8());
    QJsonObject obj = doc.object();
    
    if (obj["status"].toString() == "success") {
        return true;
    } else {
        if (errorMsg) *errorMsg = obj["message"].toString();
        return false;
    }
}

QString AuthApi::sendCommand(const QString &command, const QJsonObject &params)
{
    QTcpSocket socket;
    socket.connectToHost(m_host, m_port);
    
    if (!socket.waitForConnected(5000)) {
        return QString();
    }
    
    QJsonObject message;
    message["command"] = command;
    message["params"] = params;
    
    QJsonDocument doc(message);
    QByteArray data = doc.toJson(QJsonDocument::Compact) + "\n";
    
    socket.write(data);
    if (!socket.waitForBytesWritten(5000)) {
        return QString();
    }
    
    if (!socket.waitForReadyRead(5000)) {
        return QString();
    }
    
    QByteArray response = socket.readLine();
    socket.disconnectFromHost();
    
    return QString::fromUtf8(response).trimmed();
}

}