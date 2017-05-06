#ifndef YDS_RESTAPIBASE_H
#define YDS_RESTAPIBASE_H


#include <QtCore/QObject>
#include <QtCore/QUrlQuery>
#include "ReplyWrapper.h"
#include "AuthorizationController.h"

class RestApiBase: public QObject {
  Q_OBJECT
public:
  RestApiBase(QNetworkAccessManager *manager = nullptr, QObject *parent = nullptr);
  RestApiBase(QObject *parent);
  enum class TokenState{
    Expired,
    Normal,
    Empty
  };

  void grantAccess();

  TokenState checkTokenState();

public:
  virtual ReplyWrapper* get(const QUrl &request, QUrlQuery params = QUrlQuery());

  virtual ReplyWrapper* deleteResource(const QUrl &request, QUrlQuery params);

  virtual ReplyWrapper* post(const QUrl &request, QUrlQuery params);

  virtual ReplyWrapper* put(const QUrl &request, QUrlQuery params);

private:
  QOAuth2AuthorizationCodeFlow *oauth;
};


#endif //YDS_RESTAPIBASE_H
