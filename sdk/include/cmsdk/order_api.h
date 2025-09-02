#pragma once

#include <QObject>
#include <QString>
#include <QStringList>
#include <QJsonObject>

namespace cmsdk {

class OrderApi : public QObject
{
    Q_OBJECT

public:
    explicit OrderApi(QObject *parent = nullptr);
    
    void setServer(const QString& host, quint16 port);
    
    int newOrder(const QString& title, const QString& desc, const QString& factoryUser);
    QStringList getOrders(const QString& role, const QString& username, 
                          const QString& keyword, const QString& status, 
                          bool* ok = nullptr, QString* msg = nullptr);
    bool updateOrder(int id, const QString& status);
    bool deleteOrder(int id, const QString& username);
    
    QString lastError() const;

private:
    QString host_;
    quint16 port_;
    QString lastError_;
    
    bool sendRequest(const QString& action, const QJsonObject& params, QJsonObject& reply);
};

} // namespace cmsdk