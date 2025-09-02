#pragma once

#include <QtCore/QObject>
#include <QtCore/QString>
#include <QtCore/QJsonObject>
#include <QtCore/QJsonArray>

namespace cmsdk {

class OrderApi : public QObject
{
    Q_OBJECT

public:
    explicit OrderApi(QObject *parent = nullptr);
    
    // Server configuration
    void setServer(const QString &host, quint16 port = 5555);
    QString serverHost() const { return m_host; }
    quint16 serverPort() const { return m_port; }
    
    // Order operations (synchronous)
    bool newOrder(const QJsonObject &orderData, int *orderId = nullptr, QString *errorMsg = nullptr);
    QJsonArray getOrders(const QString &username = QString(), QString *errorMsg = nullptr);
    bool updateOrder(int orderId, const QJsonObject &updateData, QString *errorMsg = nullptr);
    bool deleteOrder(int orderId, QString *errorMsg = nullptr);

private:
    QString sendCommand(const QString &command, const QJsonObject &params = QJsonObject());
    
    QString m_host;
    quint16 m_port;
};

}