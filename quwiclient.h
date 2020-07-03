#ifndef QUWICLIENT_H
#define QUWICLIENT_H

#include <QString>
#include <QtNetwork>
#include <QErrorMessage>
#include <QList>
#include <QUuid>

class QuwiUser{

public:

    qint32 id;
    qint32 id_company;
    QString role;
    QString name;
    QString avatar_url;
    qint8 is_online;
    QString dta_activity;
    qint8 is_timer_online;

    QuwiUser() = default;

};

class QuwiProject{

public:

    qint32 id;
    QString name;
    QString uid;
    QString logo_url;
    QByteArray logo;
    qint32 position;
    qint32 is_active;
    qint32 is_owner_watcher;
    QList<QuwiUser> users;

    qint32 spent_time_week;
    qint32 spent_time_month;
    qint32 spent_time_all;

    QuwiProject() = default;

};

class QuwiClient
{
public:

    QuwiClient(QString server = "https://api.quwi.com/v2/");

    ~QuwiClient();

    bool auth(QString login, QString password);

    bool logout();

private:

    QList<QuwiUser> parseUsers(QJsonArray jsonObjUsers);

    QuwiProject parseProject(QJsonObject jsonObjProject);

public:

    QMap<qint32, QuwiProject> getProjects();

    QuwiProject getProject(qint32 id);

    QuwiProject upgradeProject(QuwiProject quwiProject);

    QByteArray load(QString url);

private:

    QString host;
    QNetworkAccessManager *manager;

    QString token;

};

#endif // QUWICLIENT_H
