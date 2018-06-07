#ifndef ZABBIX_H
#define ZABBIX_H

#include "libqzabbix_global.h"
#include <QString>
#include <QNetworkRequest>
#include <QNetworkAccessManager>

class LIBQZABBIXSHARED_EXPORT QZabbix : public QObject
{

public:
    QZabbix(QString user, QString password, QString zabbixURL);
    QString Hello();
    bool login();
    bool logout();
    QJsonObject *zabbixRequest(const char* method, QJsonObject *params = NULL);
    inline QString getAuthStr() {return auth;}
    inline bool isLoggedOn() { return loggedOn;}
private:
    bool loggedOn = false;
    QString user;
    QString password;
    QString zabbixURL;
    QNetworkRequest* networkRequest;
    QNetworkAccessManager* nam;
    unsigned long _requestID;
    QString auth;
};

#endif // ZABBIX_H
