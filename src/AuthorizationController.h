#ifndef AUTHORIZATION_CONTROLLER_H
#define AUTHORIZATION_CONTROLLER_H

#include <QObject>
#include <QDebug>
#include <QString>
#include <QQuickView>
#include <QtNetworkAuth>
#include <QNetworkAccessManager>
#include <QtCore/QSettings>
#include "Configurable.h"

namespace {

}
class AuthorizationController : public Configurable {
Q_OBJECT
public:
  AuthorizationController(QObject *parent = nullptr);

  AuthorizationController(QNetworkAccessManager *manager, QObject *parent = nullptr);

  AuthorizationController(QOAuth2AuthorizationCodeFlow *oauth, QObject *parent = nullptr);

  ~AuthorizationController();

  bool openUrl(const QUrl &url);

  void grant(bool cleanup);

  QOAuth2AuthorizationCodeFlow *getOAuth2AuthorizationCodeFlow() const;

  bool isExpired();

protected:
  void handleConfigChange() override;

  void loadConfigVariables() override;
signals:

  void authenticated();

public slots:

  void log(const QUrl &url);

private:
  struct {
    QString authorization_url{"https://oauth.yandex.ru/authorize"};
    QString token_url{"https://oauth.yandex.ru/token"};
    unsigned oauth_port{9980};
  } _conf_vars;
  void oauthAutoInit();

  bool time_set{false};
  bool token_set{false};
  QQuickView _view;
  QOAuth2AuthorizationCodeFlow *_oauth2;

};

#endif // AUTHORIZATION_CONTROLLER_H