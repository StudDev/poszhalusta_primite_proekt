#include <QObject>
#include <QDebug>
#include <QString>
#include <QQuickView>
#include <QtNetworkAuth>
#include <QNetworkAccessManager>

class AuthorizationController : public QObject {
  Q_OBJECT

public:
  AuthorizationController(const QString& clientIdentifier,
                          const QString& clientIdentifierSharedKey,
                          QNetworkAccessManager *manager,
                          QObject* parent = nullptr);
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
  QQuickView _view;
  QOAuth2AuthorizationCodeFlow* _oauth2;
  QSettings _settings;
};
