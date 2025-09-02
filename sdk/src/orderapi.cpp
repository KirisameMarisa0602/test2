#include "cmsdk/orderapi.h"
#include <QtNetwork/QTcpSocket>
#include <QtCore/QJsonDocument>
#include <QtCore/QJsonObject>
#include <QtCore/QJsonArray>
#include <QtCore/QEventLoop>
#include <QtCore/QTimer>

namespace cmsdk {

OrderApi::OrderApi(QObject *parent)
    : QObject(parent)
    , m_host("127.0.0.1")
    , m_port(5555)
{
}

void OrderApi::setServer(const QString &host, quint16 port)
{
    m_host = host;
    m_port = port;
}

bool OrderApi::newOrder(const QJsonObject &orderData, int *orderId, QString *errorMsg)
{
    QJsonObject params = orderData;
    
    QString response = sendCommand("newOrder", params);
    if (response.isEmpty()) {
        if (errorMsg) *errorMsg = "Failed to connect to server";
        return false;
    }
    
    QJsonDocument doc = QJsonDocument::fromJson(response.toUtf8());
    QJsonObject obj = doc.object();
    
    if (obj["status"].toString() == "success") {
        if (orderId) *orderId = obj["orderId"].toInt();
        return true;
    } else {
        if (errorMsg) *errorMsg = obj["message"].toString();
        return false;
    }
}

QJsonArray OrderApi::getOrders(const QString &username, QString *errorMsg)
{
    QJsonObject params;
    if (!username.isEmpty()) {
        params["username"] = username;
    }
    
    QString response = sendCommand("getOrders", params);
    if (response.isEmpty()) {
        if (errorMsg) *errorMsg = "Failed to connect to server";
        return QJsonArray();
    }
    
    QJsonDocument doc = QJsonDocument::fromJson(response.toUtf8());
    QJsonObject obj = doc.object();
    
    if (obj["status"].toString() == "success") {
        return obj["orders"].toArray();
    } else {
        if (errorMsg) *errorMsg = obj["message"].toString();
        return QJsonArray();
    }
}

bool OrderApi::updateOrder(int orderId, const QJsonObject &updateData, QString *errorMsg)
{
    QJsonObject params = updateData;
    params["orderId"] = orderId;
    
    QString response = sendCommand("updateOrder", params);
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

bool OrderApi::deleteOrder(int orderId, QString *errorMsg)
{
    QJsonObject params;
    params["orderId"] = orderId;
    
    QString response = sendCommand("deleteOrder", params);
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

QString OrderApi::sendCommand(const QString &command, const QJsonObject &params)
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