#include <QObject>
#include <QDebug>
#include <QString>
#include <QQuickView>
#include <QtNetworkAuth>
#include <QNetworkAccessManager>

class AuthorizationController : public QObject {
    Q_OBJECT

public:
    AuthorizationController(const QString& clientIdentifier, QNetworkAccessManager& manager);

    bool openUrl(const QUrl& url);
    void grant();

    QUrl authUrl;
    bool isExpired();

signals:
    void authenticated();
        
public slots:

    void log(const QUrl &url);


private:
    QQuickView view;
    QOAuth2AuthorizationCodeFlow oauth2;
    QNetworkAccessManager& manager;
    QSettings settings;
};
