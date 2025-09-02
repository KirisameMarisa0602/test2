#ifndef CMSDK_AUTHAPI_H
#define CMSDK_AUTHAPI_H

#include <QObject>
#include <QString>
#include <QJsonObject>

namespace cmsdk {

class AuthApi : public QObject {
    Q_OBJECT
public:
    explicit AuthApi(QObject* parent = nullptr);

    void setServer(const QString& host, quint16 port = 5555);

    // Returns true on success; false on error. Inspect lastError() on failure.
    bool registerUser(const QString& username, const QString& password);
    bool loginUser(const QString& username, const QString& password);

    QString lastError() const { return m_lastError; }

private:
    QString m_host;
    quint16 m_port;
    mutable QString m_lastError;

    bool sendRequest(const QString& action, const QJsonObject& payload, QJsonObject& reply) const;
};

} // namespace cmsdk

#endif // CMSDK_AUTHAPI_H
