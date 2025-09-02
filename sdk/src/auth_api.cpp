#include "cmsdk/auth_api.h"
#include <QTcpSocket>
#include <QJsonDocument>
#include <QJsonObject>
#include <QHostAddress>
#include <QDebug>

namespace cmsdk {

AuthApi::AuthApi(QObject *parent)
    : QObject(parent)
    , host_("127.0.0.1")
    , port_(5555)
{
}

void AuthApi::setServer(const QString& host, quint16 port)
{
    host_ = host;
    port_ = port;
}

bool AuthApi::registerUser(const QString& role, const QString& username, const QString& password)
{
    return sendRequest("register", role, username, password);
}

bool AuthApi::loginUser(const QString& role, const QString& username, const QString& password)
{
    return sendRequest("login", role, username, password);
}

QString AuthApi::lastError() const
{
    return lastError_;
}

bool AuthApi::sendRequest(const QString& action, const QString& role, 
                          const QString& username, const QString& password)
{
    lastError_.clear();
    
    QTcpSocket socket;
    socket.connectToHost(QHostAddress(host_), port_);
    
    if (!socket.waitForConnected(3000)) {
        lastError_ = "Failed to connect to server";
        return false;
    }
    
    QJsonObject request;
    request["action"] = action;
    request["role"] = role;
    request["username"] = username;
    request["password"] = password;
    
    QByteArray data = QJsonDocument(request).toJson(QJsonDocument::Compact) + "\n";
    
    if (socket.write(data) == -1 || !socket.waitForBytesWritten(2000)) {
        lastError_ = "Failed to send request";
        return false;
    }
    
    if (!socket.waitForReadyRead(5000)) {
        lastError_ = "Server did not respond";
        return false;
    }
    
    QByteArray response = socket.readAll();
    int newlinePos = response.indexOf('\n');
    if (newlinePos >= 0) {
        response = response.left(newlinePos);
    }
    
    QJsonParseError parseError;
    QJsonDocument doc = QJsonDocument::fromJson(response, &parseError);
    
    if (parseError.error != QJsonParseError::NoError || !doc.isObject()) {
        lastError_ = "Failed to parse server response";
        return false;
    }
    
    QJsonObject reply = doc.object();
    bool success = reply.value("ok").toBool(false);
    
    if (!success) {
        lastError_ = reply.value("msg").toString("Unknown error");
    }
    
    return success;
}

} // namespace cmsdk