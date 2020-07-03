#include "quwiclient.h"

bool QuwiClient::auth(QString login, QString password){

    QEventLoop eventLoop;
    QObject::connect(manager, SIGNAL(finished(QNetworkReply*)), &eventLoop, SLOT(quit()));
    QUrl url(host + "auth/login");
    QNetworkRequest request(url);
    request.setSslConfiguration(QSslConfiguration::defaultConfiguration());
    request.setRawHeader("Content-Type", "application/json");
    request.setRawHeader("Client-Timezone-Offset", "60");
    request.setRawHeader("Client-Device", "desktop");

    QJsonObject jsonObj;
    jsonObj["email"] = login;
    jsonObj["password"] = password;

    QNetworkReply *reply = manager->post(request, QJsonDocument(jsonObj).toJson());

    eventLoop.exec(); // blocks stack until "finished()" has been called

    QString strReply = (QString)reply->readAll();
    QJsonDocument jsonResponse = QJsonDocument::fromJson(strReply.toUtf8());
    QJsonObject jsonObjRes = jsonResponse.object();

    if(reply->error() == QNetworkReply::NoError){

        if(jsonObjRes.find("token") != jsonObjRes.end()){

            token = jsonObjRes["token"].toString();

            delete reply;

            return true;

        }

    }else{

        if(jsonObjRes.find("first_errors") != jsonObjRes.end()){

            QErrorMessage errorMessage;
            errorMessage.showMessage(
                        jsonObjRes["first_errors"].toObject()["email"].toString() +
                    ", " + jsonObjRes["first_errors"].toObject()["password"].toString());
            errorMessage.exec();

        }else{

            //failure
            QErrorMessage errorMessage;
            errorMessage.showMessage("Failure: Can't parse json!");
            errorMessage.exec();

        }

    }

    delete reply;

    return false;

}

bool QuwiClient::logout(){

    if(this->token.size() > 0){

        QEventLoop eventLoop;
        QObject::connect(manager, SIGNAL(finished(QNetworkReply*)), &eventLoop, SLOT(quit()));
        QUrl url(host + "auth/logout");
        QNetworkRequest request(url);
        request.setSslConfiguration(QSslConfiguration::defaultConfiguration());
        request.setRawHeader("Content-Type", "application/json");
        request.setRawHeader("Client-Timezone-Offset", "60");
        request.setRawHeader("Client-Device", "desktop");
        request.setRawHeader(QByteArray("Authorization"), QString("Bearer " + this->token).toUtf8());

        QJsonObject jsonObj;
        jsonObj["anywhere"] = false;

        QNetworkReply *reply = manager->post(request, QJsonDocument(jsonObj).toJson());

        eventLoop.exec(); // blocks stack until "finished()" has been called

        QString strReply = (QString)reply->readAll();
        QJsonDocument jsonResponse = QJsonDocument::fromJson(strReply.toUtf8());
        QJsonObject jsonObjRes = jsonResponse.object();

        if(reply->error() == QNetworkReply::NoError){

            this->token = "";

            return true;

        }else{

            return false;

        }

    }

    return false;

}

QList<QuwiUser> QuwiClient::parseUsers(QJsonArray jsonObjUsers){

    QList<QuwiUser> users;

    foreach(const QJsonValue & jsonUser, jsonObjUsers){

        QJsonObject jsonObjUser = jsonUser.toObject();
        QuwiUser user;

        user.id = jsonObjUser["id"].toInt();
        user.id_company = jsonObjUser["id_company"].toInt();
        user.role = jsonObjUser["role"].toString();
        user.name = jsonObjUser["name"].toString();
        user.avatar_url = jsonObjUser["avatar_url"].toString();
        user.is_online = qint8(jsonObjUser["is_online"].toBool());
        user.dta_activity = jsonObjUser["dta_activity"].toString();
        user.is_timer_online = qint8(jsonObjUser["is_timer_online"].toBool());

        users.append(user);

    }

    return users;

}

QuwiProject QuwiClient::parseProject(QJsonObject jsonObjProject){

    QuwiProject quwiProject;

    quwiProject.id = jsonObjProject["id"].toInt();
    quwiProject.name = jsonObjProject["name"].toString();
    quwiProject.uid = jsonObjProject["uid"].toString();
    quwiProject.logo_url = jsonObjProject["logo_url"].toString();
    quwiProject.position = jsonObjProject["position"].toInt();
    quwiProject.is_active = jsonObjProject["is_active"].toInt();
    quwiProject.is_owner_watcher = jsonObjProject["is_owner_watcher"].toInt();
    quwiProject.spent_time_week = jsonObjProject["spent_time_week"].toInt();
    quwiProject.spent_time_month = jsonObjProject["spent_time_month"].toInt();
    quwiProject.spent_time_all = jsonObjProject["spent_time_all"].toInt();

    quwiProject.users = parseUsers(jsonObjProject["users"].toArray());

    quwiProject.logo = load(quwiProject.logo_url);//download logo

    return quwiProject;

}

QMap<qint32, QuwiProject> QuwiClient::getProjects(){

    if(this->token.size() > 0){

        QEventLoop eventLoop;
        QObject::connect(manager, SIGNAL(finished(QNetworkReply*)), &eventLoop, SLOT(quit()));
        QUrl url(host + "projects-manage/index");
        QNetworkRequest request(url);
        request.setSslConfiguration(QSslConfiguration::defaultConfiguration());
        request.setRawHeader("Content-Type", "application/json");
        request.setRawHeader("Client-Timezone-Offset", "60");
        request.setRawHeader("Client-Device", "desktop");
        request.setRawHeader(QByteArray("Authorization"), QString("Bearer " + this->token).toUtf8());
        QNetworkReply *reply = manager->get(request);

        eventLoop.exec(); // blocks stack until "finished()" has been called

        QString strReply = (QString)reply->readAll();
        QJsonDocument jsonResponse = QJsonDocument::fromJson(strReply.toUtf8());
        QJsonObject jsonObjRes = jsonResponse.object();

        if(reply->error() == QNetworkReply::NoError){

            if(jsonObjRes.find("projects") != jsonObjRes.end()){

                QJsonArray jsonObjProjects = jsonObjRes["projects"].toArray();

                QMap<qint32, QuwiProject> resProjects;

                foreach (const QJsonValue &value, jsonObjProjects) {

                    QuwiProject quwiProject = parseProject(value.toObject());

                    //get spent time
                    QuwiProject quwiProject_more = getProject(quwiProject.id);
                    quwiProject.spent_time_week = quwiProject_more.spent_time_week;
                    quwiProject.spent_time_month = quwiProject_more.spent_time_month;
                    quwiProject.spent_time_all = quwiProject_more.spent_time_all;

                    resProjects.insert(quwiProject.id,  quwiProject);

                }

                delete reply;

                return resProjects;

            }

        }else{

            if(jsonObjRes.find("message") != jsonObjRes.end()){

                QErrorMessage errorMessage;
                errorMessage.showMessage(
                            jsonObjRes["message"].toString());
                errorMessage.exec();

            }else{

                //failure
                QErrorMessage errorMessage;
                errorMessage.showMessage("Failure: Can't parse json!");
                errorMessage.exec();

            }

        }

        delete reply;

    }

    return {};

}

QuwiProject QuwiClient::getProject(qint32 id){

    if(this->token.size() > 0){

        QEventLoop eventLoop;
        QObject::connect(manager, SIGNAL(finished(QNetworkReply*)), &eventLoop, SLOT(quit()));
        QUrl url(host + "projects-manage/" + QString::number(id));
        QNetworkRequest request(url);
        request.setSslConfiguration(QSslConfiguration::defaultConfiguration());
        request.setRawHeader("Content-Type", "application/json");
        request.setRawHeader("Client-Timezone-Offset", "60");
        request.setRawHeader("Client-Device", "desktop");
        request.setRawHeader(QByteArray("Authorization"), QString("Bearer " + this->token).toUtf8());
        QNetworkReply *reply = manager->get(request);

        eventLoop.exec(); // blocks stack until "finished()" has been called

        QString strReply = (QString)reply->readAll();
        QJsonDocument jsonResponse = QJsonDocument::fromJson(strReply.toUtf8());
        QJsonObject jsonObjRes = jsonResponse.object();

        if(reply->error() == QNetworkReply::NoError){

            if(jsonObjRes.find("project") != jsonObjRes.end()){

                delete reply;

                return parseProject(jsonObjRes["project"].toObject());

            }

        }else{

            if(jsonObjRes.find("message") != jsonObjRes.end()){

                QErrorMessage errorMessage;
                errorMessage.showMessage(
                            jsonObjRes["message"].toString());
                errorMessage.exec();

            }else{

                //failure
                QErrorMessage errorMessage;
                errorMessage.showMessage("Failure: Can't parse json!");
                errorMessage.exec();

            }

        }

        delete reply;

    }

    return QuwiProject();

}

QuwiProject QuwiClient::upgradeProject(QuwiProject quwiProject){

    if(this->token.size() > 0){

        QEventLoop eventLoop;
        QObject::connect(manager, SIGNAL(finished(QNetworkReply*)), &eventLoop, SLOT(quit()));
        QUrl url(host + "projects-manage/update?id=" + QString::number(quwiProject.id));
        QNetworkRequest request(url);
        request.setSslConfiguration(QSslConfiguration::defaultConfiguration());
        request.setRawHeader("Content-Type", "application/json");
        request.setRawHeader("Client-Timezone-Offset", "60");
        request.setRawHeader("Client-Device", "desktop");
        request.setRawHeader(QByteArray("Authorization"), QString("Bearer " + this->token).toUtf8());

        QJsonObject jsonObj;
        jsonObj["is_active"] = QString::number(qint32(quwiProject.is_active));
        jsonObj["name"] = quwiProject.name;
        jsonObj["is_owner_watcher"] = QString::number(qint32(quwiProject.is_owner_watcher));

        QNetworkReply *reply = manager->post(request, QJsonDocument(jsonObj).toJson());

        eventLoop.exec(); // blocks stack until "finished()" has been called

        QString strReply = (QString)reply->readAll();
        QJsonDocument jsonResponse = QJsonDocument::fromJson(strReply.toUtf8());
        QJsonObject jsonObjRes = jsonResponse.object();

        if(reply->error() == QNetworkReply::NoError){

            if(jsonObjRes.find("project") != jsonObjRes.end()){

                delete reply;

                return parseProject(jsonObjRes["project"].toObject());

            }

        }else{

            if(jsonObjRes.find("message") != jsonObjRes.end()){

                QErrorMessage errorMessage;
                errorMessage.showMessage(
                            jsonObjRes["message"].toString());
                errorMessage.exec();

            }else{

                //failure
                QErrorMessage errorMessage;
                errorMessage.showMessage("Failure: Can't parse json!");
                errorMessage.exec();

            }

        }

        delete reply;

    }

    return QuwiProject();

}

QByteArray QuwiClient::load(QString url){

    if(this->token.size() > 0){

        QEventLoop eventLoop;
        QObject::connect(manager, SIGNAL(finished(QNetworkReply*)), &eventLoop, SLOT(quit()));
        QUrl qurl = QUrl(url);
        QNetworkRequest request(qurl);
        request.setSslConfiguration(QSslConfiguration::defaultConfiguration());
        request.setRawHeader("Content-Type", "application/json");
        request.setRawHeader("Client-Timezone-Offset", "60");
        request.setRawHeader("Client-Device", "desktop");
        request.setRawHeader(QByteArray("Authorization"), QString("Bearer " + this->token).toUtf8());
        QNetworkReply *reply = manager->get(request);

        eventLoop.exec(); // blocks stack until "finished()" has been called

        if(reply->error() == QNetworkReply::NoError){

            return reply->readAll();

        }else{

            return QByteArray();

        }

    }

    return QByteArray();
}

QuwiClient::QuwiClient(QString server):
    host(server)
{
    manager = new QNetworkAccessManager;
}

QuwiClient::~QuwiClient()
{

    delete manager;

}
