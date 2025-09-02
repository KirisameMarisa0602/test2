#include "cmsdk/order_api.h"
#include <QTcpSocket>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QHostAddress>
#include <QDebug>

namespace cmsdk {

OrderApi::OrderApi(QObject *parent)
    : QObject(parent)
    , host_("127.0.0.1")
    , port_(5555)
{
}

void OrderApi::setServer(const QString& host, quint16 port)
{
    host_ = host;
    port_ = port;
}

int OrderApi::newOrder(const QString& title, const QString& desc, const QString& factoryUser)
{
    QJsonObject params;
    params["title"] = title;
    params["desc"] = desc;
    params["factory_user"] = factoryUser;
    
    QJsonObject reply;
    if (!sendRequest("new_order", params, reply)) {
        return -1;
    }
    
    return reply.value("order_id").toInt(-1);
}

QStringList OrderApi::getOrders(const QString& role, const QString& username, 
                                const QString& keyword, const QString& status, 
                                bool* ok, QString* msg)
{
    QJsonObject params;
    params["role"] = role;
    params["username"] = username;
    if (!keyword.isEmpty()) {
        params["keyword"] = keyword;
    }
    if (!status.isEmpty()) {
        params["status"] = status;
    }
    
    QJsonObject reply;
    bool success = sendRequest("get_orders", params, reply);
    
    if (ok) *ok = success;
    if (msg) *msg = success ? QString() : lastError_;
    
    if (!success) {
        return QStringList();
    }
    
    QStringList result;
    QJsonArray orders = reply.value("orders").toArray();
    for (const QJsonValue& value : orders) {
        result.append(value.toString());
    }
    
    return result;
}

bool OrderApi::updateOrder(int id, const QString& status)
{
    QJsonObject params;
    params["id"] = id;
    params["status"] = status;
    
    QJsonObject reply;
    return sendRequest("update_order", params, reply);
}

bool OrderApi::deleteOrder(int id, const QString& username)
{
    QJsonObject params;
    params["id"] = id;
    params["username"] = username;
    
    QJsonObject reply;
    return sendRequest("delete_order", params, reply);
}

QString OrderApi::lastError() const
{
    return lastError_;
}

bool OrderApi::sendRequest(const QString& action, const QJsonObject& params, QJsonObject& reply)
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
    for (auto it = params.begin(); it != params.end(); ++it) {
        request[it.key()] = it.value();
    }
    
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
    
    reply = doc.object();
    bool success = reply.value("ok").toBool(false);
    
    if (!success) {
        lastError_ = reply.value("msg").toString("Unknown error");
    }
    
    return success;
}

} // namespace cmsdk