#ifndef CMSDK_ORDERAPI_H
#define CMSDK_ORDERAPI_H

#include <QObject>
#include <QString>
#include <QJsonObject>
#include <QJsonArray>

namespace cmsdk {

class OrderApi : public QObject {
    Q_OBJECT
public:
    explicit OrderApi(QObject* parent = nullptr);

    void setServer(const QString& host, quint16 port = 5555);

    bool newOrder(const QJsonObject& order, QString* orderId = nullptr);
    bool getOrders(QJsonArray* orders);
    bool updateOrder(const QString& orderId, const QJsonObject& patch);
    bool deleteOrder(const QString& orderId);

    QString lastError() const { return m_lastError; }

private:
    QString m_host;
    quint16 m_port;
    mutable QString m_lastError;

    bool sendRequest(const QString& action, const QJsonObject& payload, QJsonObject& reply) const;
};

} // namespace cmsdk

#endif // CMSDK_ORDERAPI_H
