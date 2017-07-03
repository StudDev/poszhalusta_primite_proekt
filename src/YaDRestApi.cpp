#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>
#include "YaDRestApi.h"
#include "FileDownloader.h"

namespace {
  const char *const ACCEPT{"application/json"};
  const char *const CONTENT_TYPE{"application/json"};
  const char *const MAIN_URL{"https://cloud-api.yandex.net:443/v1/"};
}

YaDRestApi::YaDRestApi(QSettings *config, QObject *parent)
  : RestApiBase{parent},
    _accept{ACCEPT},
    _content_type{CONTENT_TYPE},
    _main_url{MAIN_URL},
    _config{config} {

}

YaDRestApi::YaDRestApi(QObject *parent)
  : YaDRestApi{nullptr, parent} {

}

QSettings *YaDRestApi::getConfig() const {
  return _config;
}

JsonReplyWrapper *YaDRestApi::getDiskInfo() {
  auto target_url = _main_url.resolved(QUrl{"./disk"});
  return new JsonReplyWrapper{get(target_url)};
}

JsonReplyWrapper *YaDRestApi::getResourceInfo(const QString &path, const QUrlQuery &params) {
  auto target_url = _main_url.resolved(QUrl("./disk/resources/"));
  return new JsonReplyWrapper{get(target_url, params)};
}

JsonReplyWrapper *YaDRestApi::getFileList(const QUrlQuery &params) {
  auto target_url = _main_url.resolved(QUrl("./disk/resources/files"));
  return new JsonReplyWrapper{get(target_url, params)};
}

void YaDRestApi::setHeaders(QNetworkRequest &request) const {
  request.setHeader(QNetworkRequest::ContentTypeHeader, _content_type);
  request.setRawHeader("Accept", _accept);
}

void YaDRestApi::setAuthHeaders(QNetworkRequest &request) const {
  request.setRawHeader("Authorization", token().toLocal8Bit());
}

YaDRestApi::~YaDRestApi() {

}

void YaDRestApi::setConfig(QSettings *config) {
  _config = config;
}

JsonReplyWrapper *YaDRestApi::getLastUploads(const QUrlQuery &params) {
  auto target_url = _main_url.resolved(QUrl("./disk/resources/last-uploads"));
  return new JsonReplyWrapper{get(target_url, params)};
}

ReplyWrapper *YaDRestApi::uploadFile(const QString &path, const QUrlQuery &params) {
  if (!QFile::exists(path)) {
    QString error_msg{"File not found: "};
    error_msg.append(path);
    emit error(error_msg);
    return nullptr;
  }
  auto target_url = _main_url.resolved(QUrl("./disk/resources/upload"));
  QNetworkReply *upload_url_reply = get(target_url, params);
  ReplyWrapper *handler{new ReplyWrapper};
  QObject::connect(upload_url_reply, &QNetworkReply::finished, [this, path, handler, upload_url_reply] {
    if (upload_url_reply->bytesAvailable() == 0) {
      upload_url_reply->close();
      return;
    }
    QJsonDocument doc = QJsonDocument::fromJson(upload_url_reply->readAll());
    QJsonObject obj = doc.object();
    QUrl upload_url = obj["href"].toString();
    QFile *file_to_upload{new QFile(path)};
    file_to_upload->open(QIODevice::ReadOnly);
    if (!file_to_upload->isOpen()) {
      upload_url_reply->close();
      handler->setReply(upload_url_reply);
      return;
    }
    QNetworkReply *upload_reply = put(upload_url, file_to_upload);
    handler->setReply(upload_reply);
    file_to_upload->setParent(handler);
    upload_url_reply->deleteLater();
  });
  return handler;
}

ReplyWrapper *YaDRestApi::downloadFile(const QString &localpath, const QUrlQuery &params) {
  auto target_url = _main_url.resolved(QUrl("./disk/resources/download"));
  QNetworkReply *download_url_reply = get(target_url, params);
  FileDownloader *handler{new FileDownloader{localpath}};
  QObject::connect(download_url_reply, &QNetworkReply::finished, [this, handler, download_url_reply] {
    if (download_url_reply->bytesAvailable() == 0) {
      handler->setReply(download_url_reply);
      download_url_reply->close();
      return;
    }
    QJsonDocument doc = QJsonDocument::fromJson(download_url_reply->readAll());
    QJsonObject obj = doc.object();
    QUrl download_url = obj["href"].toString();
    QNetworkRequest req{download_url};
    req.setAttribute(QNetworkRequest::FollowRedirectsAttribute, true);
    QNetworkReply *download_reply = get(req);
    handler->setReply(download_reply);
    download_url_reply->deleteLater();
  });
  return handler;
}

JsonReplyWrapper *YaDRestApi::copyResource(const QUrlQuery &params) {
  auto target_url = _main_url.resolved(QUrl("./disk/resources/copy"));
  return new JsonReplyWrapper{post(target_url, "", params)};
}

JsonReplyWrapper *YaDRestApi::moveResource(const QUrlQuery &params) {
  auto target_url = _main_url.resolved(QUrl("./disk/resources/move"));
  return new JsonReplyWrapper{post(target_url, "", params)};
}

JsonReplyWrapper *YaDRestApi::removeResource(const QUrlQuery &params) {
  auto target_url = _main_url.resolved(QUrl("./disk/resources"));
  return new JsonReplyWrapper{deleteResource(target_url, params)};
}

JsonReplyWrapper *YaDRestApi::createFolder(const QUrlQuery &params) {
  auto target_url = _main_url.resolved(QUrl("./disk/resources"));
  return new JsonReplyWrapper{put(target_url, "", params)};
}

JsonReplyWrapper *YaDRestApi::getOperationStatus(const QUrl &operation_url) {
  return new JsonReplyWrapper{get(operation_url)};
}

JsonReplyWrapper *YaDRestApi::cleanTrash(const QUrlQuery &params) {
  auto target_url = _main_url.resolved(QUrl{"./trash/resources"});
  return new JsonReplyWrapper{deleteResource(target_url, params)};
}

JsonReplyWrapper *YaDRestApi::restoreFromTrash(const QUrlQuery &params) {
  auto target_url = _main_url.resolved(QUrl{"./trash/resources/restore"});
  return new JsonReplyWrapper{put(target_url, "", params)};
}

void YaDRestApi::modifyRequest(QNetworkRequest &request) const {
  setHeaders(request);
  setAuthHeaders(request);
}

void YaDRestApi::handleError(QNetworkReply *reply) const {
  QJsonDocument json = QJsonDocument::fromJson(reply->readAll());
  auto json_obj = json.object();
  if (json_obj["error"] != "") {
    emit replyApiError(json_obj);
  }
}





