#include "cmsdk/authapi.h"

#include <QTcpSocket>
#include <QJsonDocument>
#include <QJsonObject>

using namespace cmsdk;

AuthApi::AuthApi(QObject* parent)
    : QObject(parent), m_host("127.0.0.1"), m_port(5555) {}

void AuthApi::setServer(const QString& host, quint16 port) {
    m_host = host;
    m_port = port;
}

bool AuthApi::registerUser(const QString& username, const QString& password) {
    QJsonObject payload{{"username", username}, {"password", password}};
    QJsonObject reply;
    if (!sendRequest("auth.register", payload, reply)) return false;
    return reply.value("ok").toBool();
}

bool AuthApi::loginUser(const QString& username, const QString& password) {
    QJsonObject payload{{"username", username}, {"password", password}};
    QJsonObject reply;
    if (!sendRequest("auth.login", payload, reply)) return false;
    return reply.value("ok").toBool();
}

bool AuthApi::sendRequest(const QString& action, const QJsonObject& payload, QJsonObject& reply) const {
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
