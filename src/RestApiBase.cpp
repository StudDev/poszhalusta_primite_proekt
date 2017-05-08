#include "RestApiBase.h"

RestApiBase::RestApiBase(QNetworkAccessManager *manager, QObject *parent)
  :QObject(parent),
  _manager{manager},
  _oauth{new QOAuth2AuthorizationCodeFlow{_manager,this}},
   is_auth_process_started{false}{
  grantAccess();
  QObject::connect(_manager,&QNetworkAccessManager::finished,this,&RestApiBase::handleReply);
}
RestApiBase::RestApiBase(QObject *parent)
  :RestApiBase(new QNetworkAccessManager,parent){
  _manager->setParent(this);
}

void RestApiBase::grantAccess() {
  if(is_auth_process_started){
    return;
  }
  is_auth_process_started = true;
  auto *auth = new AuthorizationController{_oauth,this};
  auth->grant();
  QObject::connect(auth,&AuthorizationController::authenticated,[this,auth]{
    qDebug() << _oauth->token() << ' ' << _oauth->expirationAt();
    auth->deleteLater();
    is_auth_process_started = false;
  });
}

RestApiBase::~RestApiBase() {
  _manager->deleteLater();
  _oauth->deleteLater();
}

QNetworkRequest RestApiBase::createRequest(const QUrl &url,const QUrlQuery &params) const {
  auto target_url = url;
  target_url.setQuery(params);
  QNetworkRequest request{target_url};
  modifyRequest(request);
  return request;
}

QNetworkReply *RestApiBase::get(const QUrl &url, QUrlQuery params) {
  return defaultRequest(url, nullptr,params,QNetworkAccessManager::GetOperation);
}

QNetworkReply *RestApiBase::get(const QNetworkRequest &request) {
  return performRequest(request, nullptr, QNetworkAccessManager::GetOperation);
}


QNetworkReply * RestApiBase::deleteResource(const QUrl &url, const QUrlQuery params) {
  return defaultRequest(url, nullptr,params,QNetworkAccessManager::DeleteOperation);
}

QNetworkReply * RestApiBase::post(const QUrl &url, QIODevice *data, const QUrlQuery params) {
  return defaultRequest(url,data,params,QNetworkAccessManager::PostOperation);
}

QNetworkReply * RestApiBase::put(const QUrl &url, QIODevice *data, const QUrlQuery params) {
  return defaultRequest(url,data,params,QNetworkAccessManager::PutOperation);
}

QNetworkReply * RestApiBase::post(const QUrl &url, const QByteArray &data, const QUrlQuery params) {
  return defaultRequest(url,data,params,QNetworkAccessManager::PostOperation);
}

QNetworkReply * RestApiBase::put(const QUrl &url, const QByteArray &data, const QUrlQuery params) {
  return defaultRequest(url,data,params,QNetworkAccessManager::PutOperation);
}

bool RestApiBase::isTokenFresh() const {
  return _oauth->token() != ""  && _oauth->expirationAt() < QDateTime::currentDateTime();
}

QString RestApiBase::token() const {
  return _oauth->token();
}

void RestApiBase::handleReply(QNetworkReply * reply) {
  qDebug() << reply->attribute(QNetworkRequest::HttpReasonPhraseAttribute).toString() << reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toString();
  if (reply->error() != QNetworkReply::NetworkError::NoError) {
    emit replyNetworkError(reply->errorString(), reply->error());
    handleError(reply);
  }
  return;
}

void RestApiBase::handleError(QNetworkReply *reply) const {
  return;
}

void RestApiBase::modifyRequest(QNetworkRequest &request) const{
  return;
}

QNetworkReply *RestApiBase::defaultRequest(const QUrl &url,
                                           QIODevice *data,
                                           const QUrlQuery params,
                                           QNetworkAccessManager::Operation request_type) {
  QNetworkReply *reply = nullptr;
  auto request = createRequest(url,params);
  if(_oauth->property("Status") != "Granted" || !isTokenFresh()){
    grantAccess();
    QObject::connect(this,&RestApiBase::accessGranted,[request,data,&reply,request_type,this]{
       reply = performRequest(request,data,request_type);
    });
    return reply;
  }
  return  performRequest(request,data,request_type);
}

QNetworkReply *RestApiBase::defaultRequest(const QUrl &url, const QByteArray &data, const QUrlQuery params,
                                           QNetworkAccessManager::Operation request_type) {
  QNetworkReply *reply = nullptr;
  auto request = createRequest(url,params);
  if(_oauth->property("Status") != "Granted" || !isTokenFresh()){
    grantAccess();
    QObject::connect(this,&RestApiBase::accessGranted,[request,&data,&reply,request_type,this]{
      reply = performRequest(request,data,request_type);
    });
    return reply;
  }
  return  performRequest(request,data,request_type);
}

QNetworkReply * RestApiBase::performRequest(const QNetworkRequest &request,
                                            QIODevice *data,
                                            QNetworkAccessManager::Operation request_type) const {
  switch (request_type) {
    case QNetworkAccessManager::GetOperation:
      return _manager->get(request);
    case QNetworkAccessManager::PutOperation:
      return _manager->put(request, data);
    case QNetworkAccessManager::PostOperation:
      return _manager->post(request, data);
    case QNetworkAccessManager::DeleteOperation:
      return _manager->deleteResource(request);
    default:
      return nullptr;
  }
}

QNetworkReply *RestApiBase::performRequest(const QNetworkRequest &request,
                                             const QByteArray &data,
                                             QNetworkAccessManager::Operation request_type) const {
  switch (request_type) {
    case QNetworkAccessManager::GetOperation:
      return _manager->get(request);
    case QNetworkAccessManager::PutOperation:
      return _manager->put(request, data);
    case QNetworkAccessManager::PostOperation:
      return _manager->post(request, data);
    case QNetworkAccessManager::DeleteOperation:
      return _manager->deleteResource(request);
    default:
      return nullptr;
  }
}


