#include "AuthorizationController.h"
#include "QOAuth2.h"
#include "TokenConfigLoader.h"

#include <QtWebEngine/qtwebengineglobal.h>
#include <QQmlContext>

//TODO move constants to configration file
namespace {
  QUrl AUTHORIZATION_VIEW_SOURCE{QLatin1String{"qrc:/AuthorizationView.qml"}};
  const unsigned short WINDOW_HEIGHT = 660;
  const unsigned short WINDOW_WIDTH = 640;
  const QString CLIENT_IDENTIFIER{"20beb8f54f66490fa4f21b42f7af7145"};
  const QString SHARED_KEY{"45493f2fdac944f994b891995db2a305"};
}

//TODO check manager for nullptr
AuthorizationController::AuthorizationController(QNetworkAccessManager *manager, QObject *parent)
    : Configurable{parent, new TokenConfigLoader{}},
      _oauth2{new QOAuth2{manager, this}} {
  loadConfigVariables();
  oauthAutoInit();
  QtWebEngine::initialize();
  connect(_oauth2, &QOAuth2AuthorizationCodeFlow::authorizeWithBrowser,
          this, &AuthorizationController::openUrl);
}

AuthorizationController::AuthorizationController(QObject *parent)
    : AuthorizationController{new QNetworkAccessManager{this}, parent} {

}

//TODO: check oauth fro nullptr
AuthorizationController::AuthorizationController(QOAuth2AuthorizationCodeFlow *oauth, QObject *parent)
    : Configurable{parent, new TokenConfigLoader{}},
      _oauth2{oauth} {
  loadConfigVariables();
  oauthAutoInit();
  QtWebEngine::initialize();
  connect(_oauth2, &QOAuth2AuthorizationCodeFlow::authorizeWithBrowser,
          this, &AuthorizationController::openUrl);
}

AuthorizationController::~AuthorizationController() {
  qDebug() << "AC destroyed";
}


bool AuthorizationController::openUrl(const QUrl &url) {
  _view.rootContext()->setContextProperty("controller", this);
  _view.rootContext()->setContextProperty("authUrl", url);

  const QSize window_size = QSize(WINDOW_WIDTH, WINDOW_HEIGHT);
  _view.setMinimumSize(window_size);
  _view.setMaximumSize(window_size);
  _view.setSource(AUTHORIZATION_VIEW_SOURCE);

  _view.show();
  return true;
}

void AuthorizationController::grant(bool forced = false) {
  if (confContains("token") && forced) {
    _oauth2->setToken(getConfValue("token").toString());
    qDebug() << _oauth2->token() << ' ' << _oauth2->expirationAt();
  } else {
    _oauth2->grant();
  }
}

QOAuth2AuthorizationCodeFlow *AuthorizationController::getOAuth2AuthorizationCodeFlow() const {
  return _oauth2;
}

void AuthorizationController::log(const QUrl &url) {
  QUrlQuery url_query(url);
  if (url_query.hasQueryItem("code"))
    qDebug() << url_query.queryItemValue("code").toInt();
}

bool AuthorizationController::isExpired() {
  return _oauth2->expirationAt() <= QDateTime::currentDateTime();
}

void AuthorizationController::oauthAutoInit() {
  auto reply_handler = new QOAuthHttpServerReplyHandler(_conf_vars.oauth_port, this);
  _oauth2->setReplyHandler(reply_handler);
  connect(_oauth2, &QOAuth2AuthorizationCodeFlow::statusChanged, [this, reply_handler](QAbstractOAuth::Status status) {
    if (status == QAbstractOAuth::Status::Granted) {
      confSetValue("token", _oauth2->token());
      if (time_set) {
        emit authenticated();
        _view.close();
        reply_handler->deleteLater();
        token_set = false;
      }
      token_set = true;
    }
  });
  connect(_oauth2, &QOAuth2AuthorizationCodeFlow::expirationAtChanged,
          [this, reply_handler](const QDateTime &expiration) {
            confSetValue("expires", _oauth2->expirationAt());
            if (token_set) {
              emit authenticated();
              _view.close();
              reply_handler->deleteLater();
              time_set = false;
            }
            time_set = true;
          });
  _oauth2->setAuthorizationUrl(_conf_vars.authorization_url);
  _oauth2->setAccessTokenUrl(_conf_vars.token_url);
  _oauth2->setClientIdentifier(CLIENT_IDENTIFIER);
  _oauth2->setClientIdentifierSharedKey(SHARED_KEY);
}

void AuthorizationController::loadConfigVariables() {
  _conf_vars.authorization_url = getConfValue("auth_url", _conf_vars.authorization_url).toString();
  _conf_vars.token_url = getConfValue("token_url", _conf_vars.token_url).toString();
  _conf_vars.oauth_port = getConfValue("oauth_port", _conf_vars.oauth_port).toUInt();

}

void AuthorizationController::handleConfigChange(QSettings *new_config) {
  qDebug() << "AC conf change handler";
}
