#ifndef YDS_QOAUTH2_H
#define YDS_QOAUTH2_H
#include <QtNetworkAuth/QOAuth2AuthorizationCodeFlow>


class QOAuth2 : public QOAuth2AuthorizationCodeFlow {
public:
  QOAuth2(QObject *parent = nullptr);
  QOAuth2(QNetworkAccessManager *manager, QObject *parent = nullptr);
  void setToken(const QString &token) override;
};


#endif //YDS_QOAUTH2_H
