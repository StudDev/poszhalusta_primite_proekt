#ifndef AUTHORIZATION_CONTROLLER_H
#define AUTHORIZATION_CONTROLLER_H

#include <QObject>
#include <QDebug>
#include <QString>
#include <QQuickView>
#include <QtNetworkAuth>
#include <QNetworkAccessManager>
#include <QtCore/QSettings>


class AuthorizationController : public QObject {
  Q_OBJECT
public:
  AuthorizationController(QObject *parent);
  AuthorizationController(QNetworkAccessManager *manager, QObject *parent);

  AuthorizationController(QOAuth2AuthorizationCodeFlow *oauth, QObject *parent);
  ~AuthorizationController();

  bool openUrl(const QUrl& url);
  void grant();
  QOAuth2AuthorizationCodeFlow* getOAuth2AuthorizationCodeFlow() const;

  QUrl _auth_url;
  bool isExpired();

signals:
  void authenticated();
public slots:
  void log(const QUrl &url);

private:
  void oauthAutoInit();
  QQuickView _view;
  QOAuth2AuthorizationCodeFlow* _oauth2;
  QSettings _settings;
};

#endif // AUTHORIZATION_CONTROLLER_H