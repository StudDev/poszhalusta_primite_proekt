#include "AuthorizationController.h"

#include <QtWebEngine/qtwebengineglobal.h>
#include <QQmlContext>

namespace {
  const QUrl AUTHORIZATION_URL{"https://oauth.yandex.ru/authorize"};
  const QUrl TOKEN_URL{"https://oauth.yandex.ru/token"};
  const QUrl AUTHORIZATION_VIEW_SOURCE{QLatin1String{"qrc:/AuthorizationView.qml"}};
  const unsigned short WINDOW_HEIGHT = 660;
  const unsigned short WINDOW_WIDTH = 640;
  const QString CLIENT_IDENTIFIER{"20beb8f54f66490fa4f21b42f7af7145"};
  const QString SHARED_KEY{"20beb8f54f66490fa4f21b42f7af7145"};

}

AuthorizationController::AuthorizationController(QNetworkAccessManager *manager, QObject *parent)
  : QObject(parent),
    _oauth2(new QOAuth2AuthorizationCodeFlow(manager, this)) {
  oauthAutoInit();
  QtWebEngine::initialize();
  connect(_oauth2, &QOAuth2AuthorizationCodeFlow::authorizeWithBrowser,
      this, &AuthorizationController::openUrl);
}


AuthorizationController::~AuthorizationController() {}

bool AuthorizationController::openUrl(const QUrl& url) {
  _auth_url = url;
  _view.rootContext()->setContextProperty("controller", this);
  _view.rootContext()->setContextProperty("authUrl", _auth_url);
  const QSize window_size = QSize(WINDOW_WIDTH,WINDOW_HEIGHT);
  _view.setMinimumSize(window_size);
  _view.setMaximumSize(window_size);
  _view.setSource(AUTHORIZATION_VIEW_SOURCE);
  _view.show();

  return true;
}

void AuthorizationController::grant() {
  if (_settings.allKeys().contains("token/token")) {
    _oauth2->setToken(_settings.value("token/token").toString());
    qDebug() << _oauth2->token() << ' ' << _oauth2->expirationAt();
  } else {
    _oauth2->grant();
  }

  _oauth2->setProperty("Status","Granted");
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

AuthorizationController::AuthorizationController(QObject *parent)
  :AuthorizationController{new QNetworkAccessManager(this),parent}{

}

AuthorizationController::AuthorizationController(QOAuth2AuthorizationCodeFlow *oauth, QObject *parent)
  :QObject(parent), _oauth2(oauth){
  oauthAutoInit();
  QtWebEngine::initialize();
  connect(_oauth2, &QOAuth2AuthorizationCodeFlow::authorizeWithBrowser,
          this, &AuthorizationController::openUrl);
}

void AuthorizationController::oauthAutoInit() {
  auto reply_handler = new QOAuthHttpServerReplyHandler(9980, this);
  _oauth2->setReplyHandler(reply_handler);
  connect(_oauth2, &QOAuth2AuthorizationCodeFlow::statusChanged, [this, reply_handler](QAbstractOAuth::Status status) {
            if (status == QAbstractOAuth::Status::Granted) {
              _settings.setValue("token/token", _oauth2->token());
              _settings.setValue("token/expires",_oauth2->expirationAt().toLocalTime());
               emit authenticated();
              _view.close();
              reply_handler->deleteLater();
            }
          });
  _oauth2->setAuthorizationUrl(AUTHORIZATION_URL);
  _oauth2->setAccessTokenUrl(TOKEN_URL);
  _oauth2->setClientIdentifier(CLIENT_IDENTIFIER);
  _oauth2->setClientIdentifierSharedKey(SHARED_KEY);
  _oauth2->setProperty("Status","NotGranted");
}
