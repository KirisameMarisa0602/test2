#pragma once

#include <QObject>
#include <QString>

namespace cmsdk {

class AuthApi : public QObject
{
    Q_OBJECT

public:
    explicit AuthApi(QObject *parent = nullptr);
    
    void setServer(const QString& host, quint16 port);
    
    bool registerUser(const QString& role, const QString& username, const QString& password);
    bool loginUser(const QString& role, const QString& username, const QString& password);
    
    QString lastError() const;

private:
    QString host_;
    quint16 port_;
    QString lastError_;
    
    bool sendRequest(const QString& action, const QString& role, 
                     const QString& username, const QString& password);
};

} // namespace cmsdk