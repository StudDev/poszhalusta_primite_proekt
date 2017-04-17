
#include <QObject>
#include <QDesktopServices>
#include <QUrl>
#include <QUuid>
#include <QtNetwork>
#include "YaDiskApiService.h"

YaDiskApiService::YaDiskApiService(const QString &config_path, QObject *parent)
  :QObject(parent),
   _config_manager(config_path){
}

void YaDiskApiService::getAuthCode() {
  QUrl user_redirect{ _authorization };
  user_redirect.setQuery("response_type=code&client_id=" + _config_manager.getAppId());
  QDesktopServices::openUrl(user_redirect);
  emit requestAuthCode();
}

void YaDiskApiService::getToken(const QString &auth_code) {
  QNetworkRequest *token_request{ new QNetworkRequest() };
  token_request->setRawHeader(QByteArray("Host"),QByteArray("oauth.yandex.ru"));
  token_request->setHeader(QNetworkRequest::KnownHeaders::ContentTypeHeader,
                           "application/x-www-form-urlencoded");
  QByteArray request_data;
  request_data.append("grant_type=authorization_code");
  request_data.append("?code=" + auth_code);

  emit requestToken(token_request);
}
