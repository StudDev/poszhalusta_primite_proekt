#include "AuthorizationController.h"

#include <QtWebEngine/qtwebengineglobal.h>
#include <QQmlContext>
#include <QUrlQuery>

AuthorizationController::AuthorizationController(const QString& clientIdentifier,
                        const QString& clientIdentifierSharedKey,
                        QNetworkAccessManager *manager,
                        QObject* parent)
    : QObject(parent), _oauth2(new QOAuth2AuthorizationCodeFlow(manager, this)) {

  QtWebEngine::initialize();

  auto reply_handler = new QOAuthHttpServerReplyHandler(9980, this);
  _oauth2->setReplyHandler(reply_handler);
  _oauth2->setAuthorizationUrl(QUrl("https://oauth.yandex.ru/authorize"));
  _oauth2->setAccessTokenUrl(QUrl("https://oauth.yandex.ru/token"));
  _oauth2->setClientIdentifier(clientIdentifier);
  _oauth2->setClientIdentifierSharedKey(clientIdentifierSharedKey);

  connect(_oauth2, &QOAuth2AuthorizationCodeFlow::statusChanged,
          [this](QAbstractOAuth::Status status) {

            if (status == QAbstractOAuth::Status::Granted) {
              _settings.setValue("token", _oauth2->token());
              qDebug() << _oauth2->token();
              emit authenticated();
              _view.close();
            }

          });

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
