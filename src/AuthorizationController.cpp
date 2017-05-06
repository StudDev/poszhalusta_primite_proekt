#include "AuthorizationController.h"

#include <QtWebEngine/qtwebengineglobal.h>
#include <QQmlContext>
#include <QUrlQuery>

AuthorizationController::AuthorizationController(QNetworkAccessManager *manager, QObject *parent = nullptr)
  : QObject(parent), _oauth2(new QOAuth2AuthorizationCodeFlow(manager, this)) {
  oauthAutoInit();
  QtWebEngine::initialize();
  connect(_oauth2, &QOAuth2AuthorizationCodeFlow::authorizeWithBrowser,
      this, &AuthorizationController::openUrl);
}


AuthorizationController::~AuthorizationController() {}

bool AuthorizationController::openUrl(const QUrl& url) {  _auth_url = url;

  _view.rootContext()->setContextProperty("controller", this);
  _view.rootContext()->setContextProperty("authUrl", _auth_url);
  const QSize window_size = QSize(640, 660);
  _view.setMinimumSize(window_size);
  _view.setMaximumSize(window_size);
  _view.setSource(QUrl(QLatin1String("qrc:/AuthorizationView.qml")));

  _view.show();

  return 1;
}

void AuthorizationController::grant() {
  if (_settings.allKeys().contains("token")) {
    qDebug() << _settings.value("token").toString();
    _oauth2->setToken(_settings.value("token").toString());
  } else {
    _oauth2->grant();
  }
}

QOAuth2AuthorizationCodeFlow* AuthorizationController::getOAuth2AuthorizationCodeFlow() const {
  return _oauth2;
}

void AuthorizationController::log(const QUrl& url) {
  QUrlQuery url_query(url);
  if (url_query.hasQueryItem("code"))
    qDebug() << url_query.queryItemValue("code").toInt();
}

bool AuthorizationController::isExpired() {
  return _oauth2->expirationAt() <= QDateTime::currentDateTime();
}

AuthorizationController::AuthorizationController(QObject *parent = nullptr)
  :AuthorizationController{new QNetworkAccessManager(this),parent}{

}

AuthorizationController::AuthorizationController(QOAuth2AuthorizationCodeFlow *oauth, QObject *parent = nullptr)
  :QObject(parent), _oauth2(oauth){
  oauthAutoInit();
  QtWebEngine::initialize();
  connect(_oauth2, &QOAuth2AuthorizationCodeFlow::authorizeWithBrowser,
          this, &AuthorizationController::openUrl);
}

void AuthorizationController::oauthAutoInit() {
  auto reply_handler = new QOAuthHttpServerReplyHandler(9980, this);
  _oauth2->setReplyHandler(reply_handler);
  connect(_oauth2, &QOAuth2AuthorizationCodeFlow::statusChanged,
          [this](QAbstractOAuth::Status status) {
            if (status == QAbstractOAuth::Status::Granted) {
              _settings.setValue("token", _oauth2->token());
              qDebug() << _oauth2->token();
              emit authenticated();
              _view.close();
            }
          });
  _oauth2->setAuthorizationUrl(QUrl("https://oauth.yandex.ru/authorize"));
  _oauth2->setAccessTokenUrl(QUrl("https://oauth.yandex.ru/token"));
  _oauth2->setClientIdentifier("20beb8f54f66490fa4f21b42f7af7145");
  _oauth2->setClientIdentifierSharedKey("45493f2fdac944f994b891995db2a305");
}
