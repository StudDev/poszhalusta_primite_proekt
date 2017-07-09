#include "RestApiBase.h"
#include "QOAuth2.h"
#include "RestApiConfigLoader.h"


template<>
QNetworkReply *RestApiBase::performRequest<std::nullptr_t>(const QNetworkRequest &request,
                                                           std::nullptr_t &&data,
                                                           QNetworkAccessManager::Operation request_type) const {
  performRequest(request, "", request_type);
}


//TODO: check manager for nullptr
RestApiBase::RestApiBase(QObject *parent)
    : RestApiBase{new QOAuth2{}, parent} {

}


RestApiBase::RestApiBase(QOAuth2AuthorizationCodeFlow *authorizer, QObject *parent)
    : Configurable{parent, new RestApiConfigLoader{}},
      _oauth{authorizer},
      is_auth_process_started{false} {
  qDebug() << "api base constructor";
  auto *network_manager = _oauth->networkAccessManager();
  QObject::connect(network_manager, &QNetworkAccessManager::finished, this, &RestApiBase::handleReply);
  _oauth->setParent(this);
}

void RestApiBase::grantAccess(bool forced) {
  if (is_auth_process_started.test_and_set(std::memory_order_acquire)) {
    return;
  }
  AuthorizationController *auth = new AuthorizationController{_oauth, this};
  auth->grant(forced);
  QObject::connect(auth, &AuthorizationController::authenticated, [this, auth] {
    qDebug() << _oauth->token() << ' ' << _oauth->expirationAt();
    auth->deleteLater();
    is_auth_process_started.clear(std::memory_order_release);
    emit authorized();
  });
}

RestApiBase::~RestApiBase() {
  _oauth->deleteLater();
}

QNetworkRequest RestApiBase::createRequest(const QUrl &url, const QUrlQuery &params) const {
  auto target_url = url;
  target_url.setQuery(params);
  QNetworkRequest request{target_url};
  modifyRequest(request);
  return request;
}

QNetworkReply *RestApiBase::get(const QUrl &url, QUrlQuery params) {
  return defaultRequest(url, nullptr, params, QNetworkAccessManager::GetOperation);
}

QNetworkReply *RestApiBase::get(const QNetworkRequest &request) {
  return performRequest(request, nullptr, QNetworkAccessManager::GetOperation);
}


QNetworkReply *RestApiBase::deleteResource(const QUrl &url, const QUrlQuery params) {
  return defaultRequest(url, nullptr, params, QNetworkAccessManager::DeleteOperation);
}

QNetworkReply *RestApiBase::post(const QUrl &url, QIODevice *data, const QUrlQuery params) {
  return defaultRequest(url, data, params, QNetworkAccessManager::PostOperation);
}

QNetworkReply *RestApiBase::put(const QUrl &url, QIODevice *data, const QUrlQuery params) {
  return defaultRequest(url, data, params, QNetworkAccessManager::PutOperation);
}

QNetworkReply *RestApiBase::post(const QUrl &url, const QByteArray &data, const QUrlQuery params) {
  return defaultRequest(url, data, params, QNetworkAccessManager::PostOperation);
}

QNetworkReply *RestApiBase::put(const QUrl &url, const QByteArray &data, const QUrlQuery params) {
  return defaultRequest(url, data, params, QNetworkAccessManager::PutOperation);
}

bool RestApiBase::isTokenFresh() const {
  return _oauth->token() != "" && _oauth->expirationAt() > QDateTime::currentDateTime();
}

QString RestApiBase::token() const {
  return _oauth->token();
}

void RestApiBase::handleReply(QNetworkReply *reply) {
  qDebug() << reply->attribute(QNetworkRequest::HttpReasonPhraseAttribute).toString()
           << reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toString()
           << reply->bytesAvailable();
  if (reply->error() != QNetworkReply::NetworkError::NoError) {
    emit replyNetworkError(reply->errorString(), reply->error());
    handleError(reply);
  }
  return;
}

void RestApiBase::handleError(QNetworkReply *reply) {
  return;
}

void RestApiBase::modifyRequest(QNetworkRequest &request) const {
  return;
}

QNetworkReply *RestApiBase::defaultRequest(const QUrl &url,
                                           QIODevice *data,
                                           const QUrlQuery params,
                                           QNetworkAccessManager::Operation request_type) {
  QNetworkReply *reply = nullptr;
  auto request = createRequest(url, params);
  if (_oauth->status() != QAbstractOAuth::Status::Granted || !isTokenFresh()) {
    grantAccess();
    QObject::connect(this, &RestApiBase::accessGranted, [request, data, &reply, request_type, this] {
      reply = performRequest(request, data, request_type);
    });
    return reply;
  }
  return performRequest(request, data, request_type);
}

QNetworkReply *RestApiBase::defaultRequest(const QUrl &url, const QByteArray &data, const QUrlQuery params,
                                           QNetworkAccessManager::Operation request_type) {
  QNetworkReply *reply = nullptr;
  auto request = createRequest(url, params);
  if (_oauth->status() != QAbstractOAuth::Status::Granted || !isTokenFresh()) {
    grantAccess();
    QObject::connect(this, &RestApiBase::accessGranted, [request, &data, &reply, request_type, this] {
      reply = performRequest(request, data, request_type);
    });
    return reply;
  }
  return performRequest(request, data, request_type);
}


template<typename InputData>
QNetworkReply *RestApiBase::performRequest(const QNetworkRequest &request,
                                           InputData &&data,
                                           QNetworkAccessManager::Operation request_type) const {
  auto *manager = _oauth->networkAccessManager();
  switch (request_type) {
    case QNetworkAccessManager::GetOperation:
      return manager->get(request);
    case QNetworkAccessManager::PutOperation:
      return manager->put(request, std::forward<InputData>(data));
    case QNetworkAccessManager::PostOperation:
      return manager->post(request, data);
    case QNetworkAccessManager::DeleteOperation:
      return manager->deleteResource(request);
    default:
      return nullptr;
  }
}

void RestApiBase::handleConfigChange(QSettings *new_config) {
  //Do nothing
}

