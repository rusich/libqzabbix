#include "qzabbix.h"
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <QNetworkReply>
#include <QApplication>

QZabbix::QZabbix(QString user, QString password, QString zabbixURL)
{
    _requestID = 0;
    this->user = user;
    this->password = password;
    this->zabbixURL = zabbixURL;
    networkRequest = new QNetworkRequest(QUrl(zabbixURL));
    networkRequest->setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    nam = new QNetworkAccessManager();
}

QString QZabbix::Hello()
{
    return "Hello!";
}

bool QZabbix::login()
{
    QJsonObject userAuth;
    userAuth["user"] = this->user;
    userAuth["password"] = this->password;
    //userAuth["userData"] = "false";
    QJsonArray params;
    params.append(userAuth);

    QJsonObject* response = zabbixRequest("user.login",&userAuth);
    if(!response->contains("error") && response->contains("result"))
    {
        this->auth = response->value("result").toString();
        loggedOn = true;
        return true;
    }
    return false;
}

bool QZabbix::logout()
{
    QJsonObject* response = zabbixRequest("user.logout");
    auth = "";
    loggedOn = false;
    return true;
}

QJsonObject* QZabbix::zabbixRequest(const char* method, QJsonObject* params)
{

    _requestID++;
    QJsonObject jsonRequest;
    jsonRequest["id"] = QString::number(_requestID);
    jsonRequest["jsonrpc"] = QString("2.0");
    jsonRequest["method"] = method;

    if(loggedOn)
        jsonRequest["auth"] = this->auth;

    if(params!=NULL)
        jsonRequest["params"] = *params;
    else
    {
        QJsonObject emptyParams;
        jsonRequest["params"] = emptyParams;
    }

    QNetworkReply *reply = nam->post(*networkRequest, QJsonDocument(jsonRequest).toJson());

    while(!reply->isFinished())
    {
      qApp->processEvents();
    }

    QByteArray rawReply = reply->readAll();
    QJsonDocument jsonReply  = QJsonDocument::fromJson(rawReply);
    QJsonObject* jsonObj =  new QJsonObject(jsonReply.object());
    reply->deleteLater();
    return jsonObj;
}
