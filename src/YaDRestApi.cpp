#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>
#include "YaDRestApi.h"

YaDRestApi::YaDRestApi(QNetworkAccessManager *network_access, QObject *parent = nullptr)
  : QObject(parent),
    _accept{"application/json"},
    _content_type{"application/json"},
    _main_url{"https://cloud-api.yandex.net:443/v1/"},
    _network_manager(network_access),
    _config{nullptr} {
  parent->connect(_network_manager, &QNetworkAccessManager::finished, this, &YaDRestApi::handleReply);
}

YaDRestApi::YaDRestApi(QObject *parent)
  : YaDRestApi{new QNetworkAccessManager(this), parent} {
}

QSettings *YaDRestApi::getConfig() const {
  return _config;
}

QNetworkAccessManager *YaDRestApi::getNetworkManager() const {
  return _network_manager;
}

JsonReplyWrapper *YaDRestApi::getDiskInfo() const {
  auto target_url = _main_url.resolved(QUrl{"./disk"});
 return defaultGetRequest(target_url);
}

JsonReplyWrapper *YaDRestApi::getResourceInfo(const QString &path, const QUrlQuery &params) const {
  auto target_url = _main_url.resolved(QUrl("./disk/resources/"));
  target_url.setQuery(params);
  return defaultGetRequest(target_url);
}

JsonReplyWrapper *YaDRestApi::getFileList(unsigned limit, const QUrlQuery &params) const {
  auto target_url = _main_url.resolved(QUrl("./disk/resources/files"));
  target_url.setQuery(params);
  return defaultGetRequest(target_url);
}

void YaDRestApi::setHeaders(QNetworkRequest &request) const {
  request.setHeader(QNetworkRequest::ContentTypeHeader, _content_type);
  request.setRawHeader("Accept", _accept);
}

void YaDRestApi::setAuthHeaders(QNetworkRequest &request) const {
  request.setRawHeader("Authorization", _config->value("token/token").toByteArray());
}

void YaDRestApi::handleReply(QNetworkReply *reply) {
  qDebug() << reply->attribute(QNetworkRequest::HttpReasonPhraseAttribute);
  if (reply->error() != QNetworkReply::NetworkError::NoError) {
    emit replyNetworkError(reply->errorString(), reply->error());
    QJsonDocument doc = QJsonDocument::fromJson(reply->readAll());
    QJsonObject obj = doc.object();
    if(!obj["error"].isNull()){
      emit replyApiError(obj);
    }
  }
  return;
}

YaDRestApi::~YaDRestApi() {
  _network_manager->deleteLater();
}

void YaDRestApi::setConfig(QSettings *config) {
  _config = config;
}

JsonReplyWrapper * YaDRestApi::getLastUploads(unsigned limit, const QUrlQuery &params) const {
  auto target_url = _main_url.resolved(QUrl("./disk/resources/"));
  target_url.setQuery(params);
  return defaultGetRequest(target_url);
}

ReplyWrapper * YaDRestApi::uploadFile(const QString &path, const QUrlQuery &params) const {
  if(!QFile::exists(path)){
    QString error_msg{"File not found: "};
    error_msg.append(path);
    emit error(error_msg);
    return nullptr;
  }
  auto target_url = _main_url.resolved(QUrl("./disk/resources/upload"));
  target_url.setQuery(params);
  QNetworkRequest request(target_url);
  setHeaders(request);
  setAuthHeaders(request);
  QNetworkReply *upload_url_reply = _network_manager->get(request);
  ReplyWrapper *handler{new ReplyWrapper};
  QObject::connect(upload_url_reply, &QNetworkReply::finished, [this, &path, handler, upload_url_reply] {
    if(upload_url_reply->bytesAvailable() == 0){
      upload_url_reply->close();
      handler->setReply(upload_url_reply);
      return;
    }
    QJsonDocument doc = QJsonDocument::fromJson(upload_url_reply->readAll());
    QJsonObject obj = doc.object();
    QUrl upload_url = obj["href"].toString();
    QNetworkRequest upload_request{upload_url};
    QFile *file_to_upload{new QFile(path)};
    file_to_upload->open(QIODevice::ReadOnly);
    QNetworkReply *upload_reply = _network_manager->put(upload_request, file_to_upload);
    handler->setReply(upload_reply);
    file_to_upload->setParent(handler);
  });
  return handler;
}

ReplyWrapper * YaDRestApi::downloadFile(const QUrlQuery &params) const {
  auto target_url = _main_url.resolved(QUrl("./disk/resources/download"));
  target_url.setQuery(params);
  QNetworkRequest request(target_url);
  setHeaders(request);
  setAuthHeaders(request);
  QNetworkReply *download_url_reply = _network_manager->get(request);
  ReplyWrapper *handler{new ReplyWrapper};
  QObject::connect(download_url_reply, &QNetworkReply::finished, [this, handler, download_url_reply] {
    if(download_url_reply->bytesAvailable() == 0){
      download_url_reply->close();
      handler->setReply(download_url_reply);
      return;
    }
    QJsonDocument doc = QJsonDocument::fromJson(download_url_reply->readAll());
    QJsonObject obj = doc.object();
    QUrl download_url = obj["href"].toString();
    QNetworkRequest download_request{download_url};
    setAuthHeaders(download_request);
    QNetworkReply *download_reply = _network_manager->get(download_request);
    handler->setReply(download_reply);
  });
  return handler;
}

JsonReplyWrapper *YaDRestApi::copyResource(const QUrlQuery &params) const {
  auto target_url = _main_url.resolved(QUrl("./disk/resources/copy"));
  target_url.setQuery(params);
  return defaultPostRequest(target_url);
}

JsonReplyWrapper *YaDRestApi::moveResource(const QUrlQuery &params) const {
  auto target_url = _main_url.resolved(QUrl("./disk/resources/move"));
  target_url.setQuery(params);
  return defaultPostRequest(target_url);
}

JsonReplyWrapper *YaDRestApi::removeResource(const QUrlQuery &params) const {
  auto target_url = _main_url.resolved(QUrl("./disk/resources"));
  target_url.setQuery(params);
  return defaultDeleteRequest(target_url);
}

JsonReplyWrapper *YaDRestApi::createFolder(const QUrlQuery &params) const {
  auto target_url = _main_url.resolved(QUrl("./disk/resources"));
  target_url.setQuery(params);
  QNetworkRequest request(target_url);
  setHeaders(request);
  setAuthHeaders(request);
  auto jsonReplyWrapper = new JsonReplyWrapper(_network_manager->put(request,""));
  return jsonReplyWrapper;
}

JsonReplyWrapper *YaDRestApi::getOperationStatus(const QUrl &operation_url) const {
  return defaultGetRequest(operation_url);
}

JsonReplyWrapper *YaDRestApi::defaultGetRequest(const QUrl &url) const {
  QNetworkRequest request;
  setDefaultRequestOptions(request, url);
  auto jsonReplyWrapper = new JsonReplyWrapper(_network_manager->get(request));
  return jsonReplyWrapper;
}

JsonReplyWrapper *YaDRestApi::defaultPostRequest(const QUrl &url) const {
  QNetworkRequest request;
  setDefaultRequestOptions(request, url);
  auto jsonReplyWrapper = new JsonReplyWrapper(_network_manager->post(request,""));
  return jsonReplyWrapper;
}

JsonReplyWrapper *YaDRestApi::defaultDeleteRequest(const QUrl &url) const {
  QNetworkRequest request;
  setDefaultRequestOptions(request, url);
  auto jsonReplyWrapper = new JsonReplyWrapper(_network_manager->deleteResource(request));
  return jsonReplyWrapper;
}

void YaDRestApi::setDefaultRequestOptions(QNetworkRequest &request, const QUrl &url) const {
  setHeaders(request);
  setAuthHeaders(request);
  request.setUrl(url);
}


