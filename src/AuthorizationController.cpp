#include "AuthorizationController.h"

#include <QUrlQuery>
#include <QtWebEngine/qtwebengineglobal.h>
#include <QQmlContext>

AuthorizationController::AuthorizationController(const QString& clientIdentifier,
    QNetworkAccessManager& manager): manager(manager) {
    QtWebEngine::initialize();

    auto replyHandler = new QOAuthHttpServerReplyHandler(9980, this);
    oauth2.setReplyHandler(replyHandler);
    oauth2.setAuthorizationUrl(QUrl("https://oauth.yandex.ru/authorize"));
    oauth2.setAccessTokenUrl(QUrl("https://oauth.yandex.ru/token"));
    oauth2.setClientIdentifier(clientIdentifier);
    oauth2.setClientIdentifierSharedKey("45493f2fdac944f994b891995db2a305");

    connect(&oauth2, &QOAuth2AuthorizationCodeFlow::statusChanged,
        [=](QAbstractOAuth::Status status) {

        if (status == QAbstractOAuth::Status::Granted) {
            settings.setValue("token", oauth2.token());
            qDebug() << oauth2.token();
            emit authenticated();
            view.close();
        }

    });

    connect(&oauth2, &QOAuth2AuthorizationCodeFlow::authorizeWithBrowser,
            this, &AuthorizationController::openUrl);
}

bool AuthorizationController::openUrl(const QUrl& url) {
    authUrl = url;

    view.rootContext()->setContextProperty("controller", this);
    view.rootContext()->setContextProperty("authUrl", authUrl);
    const QSize windowSize = QSize(640, 660);
    view.setMinimumSize(windowSize);
    view.setMaximumSize(windowSize);
    view.setSource(QUrl(QLatin1String("qrc:/AuthorizationView.qml")));
    
    view.show();

    return 1;
}

void AuthorizationController::grant() {
    if (settings.allKeys().contains("token")) {
        oauth2.setToken(settings.value("token").toString());
    } else {
        oauth2.grant();
    }
}

void AuthorizationController::log(const QUrl &url) {
    QUrlQuery urQuery(url);
    if (urQuery.hasQueryItem("code")) {
        qDebug() << urQuery.queryItemValue("code").toInt();
    }
}

bool AuthorizationController::isExpired() {
    return oauth2.expirationAt() <= QDateTime::currentDateTime();
}
