#include "cmsdk/orderapi.h"

#include <QTcpSocket>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

using namespace cmsdk;

OrderApi::OrderApi(QObject* parent)
    : QObject(parent), m_host("127.0.0.1"), m_port(5555) {}

void OrderApi::setServer(const QString& host, quint16 port) {
    m_host = host;
    m_port = port;
}

bool OrderApi::newOrder(const QJsonObject& order, QString* orderId) {
    QJsonObject reply;
    if (!sendRequest("order.new", order, reply)) return false;
    if (orderId) *orderId = reply.value("orderId").toString();
    return true;
}

bool OrderApi::getOrders(QJsonArray* orders) {
    QJsonObject reply;
    if (!sendRequest("order.list", QJsonObject{}, reply)) return false;
    if (orders) *orders = reply.value("orders").toArray();
    return true;
}

bool OrderApi::updateOrder(const QString& orderId, const QJsonObject& patch) {
    QJsonObject payload{{"orderId", orderId}, {"patch", patch}};
    QJsonObject reply;
    if (!sendRequest("order.update", payload, reply)) return false;
    return true;
}

bool OrderApi::deleteOrder(const QString& orderId) {
    QJsonObject payload{{"orderId", orderId}};
    QJsonObject reply;
    if (!sendRequest("order.delete", payload, reply)) return false;
    return true;
}

bool OrderApi::sendRequest(const QString& action, const QJsonObject& payload, QJsonObject& reply) const {
    m_lastError.clear();

    QTcpSocket sock;
    sock.connectToHost(m_host, m_port);
    if (!sock.waitForConnected(5000)) {
        m_lastError = sock.errorString();
        return false;
    }

    QJsonObject req{{"action", action}, {"payload", payload}};
    QByteArray line = QJsonDocument(req).toJson(QJsonDocument::Compact);
    line.append('\n');

    if (sock.write(line) == -1 || !sock.waitForBytesWritten(3000)) {
        m_lastError = sock.errorString();
        return false;
    }

    if (!sock.waitForReadyRead(5000)) {
        m_lastError = QStringLiteral("timeout waiting for reply");
        return false;
    }

    QByteArray resp = sock.readLine();
    QJsonParseError perr{};
    QJsonDocument doc = QJsonDocument::fromJson(resp, &perr);
    if (perr.error != QJsonParseError::NoError || !doc.isObject()) {
        m_lastError = QStringLiteral("invalid JSON reply");
        return false;
    }

    reply = doc.object();
    if (!reply.value("ok").toBool()) {
        m_lastError = reply.value("error").toString();
        return false;
    }
    return true;
}
