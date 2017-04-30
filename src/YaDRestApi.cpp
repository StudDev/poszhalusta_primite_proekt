#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>
#include "YaDRestApi.h"

YaDRestApi::YaDRestApi(QNetworkAccessManager *network_access, QObject *parent = nullptr)
:QObject(parent),
 _accept{"application/json"},
 _content_type{"application/json"},
 _main_url{"https://cloud-api.yandex.net/v1/"},
 _network_manager(network_access),
 _config{nullptr}{
  parent->connect(_network_manager, &QNetworkAccessManager::finished, this, &YaDRestApi::handleReply);
}
YaDRestApi::YaDRestApi(QObject *parent)
  : YaDRestApi{new QNetworkAccessManager(this),parent}{
}

QSettings *YaDRestApi::getConfig() const {
  return _config;
}

QNetworkAccessManager *YaDRestApi::getNetworkManager() const {
  return _network_manager;
}

JsonReplyWrapper * YaDRestApi::getDiskInfo() const {
  QNetworkRequest request{_main_url.resolved(QUrl("./disk"))};
  setHeaders(request);
  setAuthHeaders(request);
  auto jsonReplyWrapper = new JsonReplyWrapper(_network_manager->get(request));
  return jsonReplyWrapper;
}

JsonReplyWrapper * YaDRestApi::getResourceInfo(const QString &path, const QUrlQuery &params) const{
  auto target_url = _main_url.resolved(QUrl("./disk/resources/"));
  target_url.setQuery(params);
  QNetworkRequest request{target_url};
  setHeaders(request);
  setAuthHeaders(request);
  auto jsonReplyWrapper = new JsonReplyWrapper(_network_manager->get(request));
  return jsonReplyWrapper;
}


JsonReplyWrapper* YaDRestApi::getFileList(unsigned limit, const QUrlQuery &params) const {
  auto target_url = _main_url.resolved(QUrl("./disk/resources/files"));
  target_url.setQuery(params);
  QNetworkRequest request(target_url);
  setHeaders(request);
  setAuthHeaders(request);
  auto jsonReplyWrapper = new JsonReplyWrapper(_network_manager->get(request));
  return jsonReplyWrapper;
}

void YaDRestApi::setHeaders(QNetworkRequest &request) const{
  request.setHeader(QNetworkRequest::ContentTypeHeader, _content_type);
  request.setRawHeader("Accept",_accept);
}

void YaDRestApi::setAuthHeaders(QNetworkRequest &request) const{
  request.setRawHeader("Authorization",_config->value("token/token").toByteArray());
}

void YaDRestApi::handleReply(QNetworkReply *reply) {
  if(reply->error() != QNetworkReply::NetworkError::NoError){
    emit(reply->errorString(),reply->error());
    reply->deleteLater();
  }
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
  QNetworkRequest request(target_url);
  setHeaders(request);
  setAuthHeaders(request);
  auto jsonReply = new JsonReplyWrapper(_network_manager->get(request));
  return jsonReply;
}

JsonReplyWrapper * YaDRestApi::uploadFile(const QString &path, const QUrlQuery &params) const {
  auto target_url = _main_url.resolved(QUrl("./disk/resources/upload"));
  target_url.setQuery(params);
  QNetworkRequest request(target_url);
  setHeaders(request);
  setAuthHeaders(request);
  QNetworkReply *upload_url_reply = _network_manager->get(request);
  JsonReplyWrapper* handler{new JsonReplyWrapper};
  QObject::connect(upload_url_reply,&QNetworkReply::finished,[this, &path, handler, upload_url_reply]{
    QJsonDocument doc = QJsonDocument::fromJson(upload_url_reply->readAll());
    QJsonObject obj = doc.object();
    QUrl upload_url = obj["href"].toString();
    QNetworkRequest upload_request{ upload_url };
    QFile *file_to_upload{ new QFile(path)};
    file_to_upload->open(QIODevice::ReadOnly);
    QNetworkReply *upload_reply = _network_manager->put(upload_request,file_to_upload);
    handler->setReply(upload_reply);
    file_to_upload->setParent(handler);
  });
  return handler;
}

void YaDRestApi::processUploading(QNetworkReply *uploadUrlReply, JsonReplyWrapper *result_handler) {

}


