#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>
#include "YaDRestApi.h"
#include "FileDownloader.h"

namespace {
  const char *const ACCEPT{"application/json"};
  const char *const CONTENT_TYPE{"application/json"};
  const char *const MAIN_URL{"https://cloud-api.yandex.net:443/v1/"};
}

YaDRestApi::YaDRestApi(QObject *parent)
  : RestApiBase{parent},
    _accept{ACCEPT},
    _content_type{CONTENT_TYPE},
    _main_url{MAIN_URL} {
  qDebug() << "Api constructor";
}



JsonReplyWrapper *YaDRestApi::getDiskInfo() {
  auto target_url = _main_url.resolved(_conf_vars.disk_info_url);
  return new JsonReplyWrapper{get(target_url)};
}

JsonReplyWrapper *YaDRestApi::getResourceInfo(const QString &path, const QUrlQuery &params) {
  auto target_url = _main_url.resolved(_conf_vars.resource_info_url);
  return new JsonReplyWrapper{get(target_url, params)};
}

JsonReplyWrapper *YaDRestApi::getFileList(const QUrlQuery &params) {
  auto target_url = _main_url.resolved(_conf_vars.file_list_url);
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


JsonReplyWrapper *YaDRestApi::getLastUploads(const QUrlQuery &params) {
  auto target_url = _main_url.resolved(_conf_vars.last_uploads_url);
  return new JsonReplyWrapper{get(target_url, params)};
}

ReplyWrapper *YaDRestApi::uploadFile(const QString &path, const QUrlQuery &params) {
  if (!QFile::exists(path)) {
    QString error_msg{"File not found: "};
    error_msg.append(path);
    emit error(error_msg);
    return nullptr;
  }
  auto target_url = _main_url.resolved(_conf_vars.upload_file_url);
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
  auto target_url = _main_url.resolved(_conf_vars.download_file_url);

  QNetworkReply *download_url_reply = get(target_url, params);
  download_url_reply->setParent(this);
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
  auto target_url = _main_url.resolved(_conf_vars.copy_resource_url);
  return new JsonReplyWrapper{post(target_url, "", params)};
}

JsonReplyWrapper *YaDRestApi::moveResource(const QUrlQuery &params) {
  auto target_url = _main_url.resolved(_conf_vars.move_resource_url);
  return new JsonReplyWrapper{post(target_url, "", params)};
}

JsonReplyWrapper *YaDRestApi::removeResource(const QUrlQuery &params) {
  auto target_url = _main_url.resolved(_conf_vars.remove_resource_url);
  return new JsonReplyWrapper{deleteResource(target_url, params)};
}

JsonReplyWrapper *YaDRestApi::createFolder(const QUrlQuery &params) {
  auto target_url = _main_url.resolved(_conf_vars.create_folder_url);
  return new JsonReplyWrapper{put(target_url, "", params)};
}

JsonReplyWrapper *YaDRestApi::getOperationStatus(const QUrl &operation_url) {
  return new JsonReplyWrapper{get(operation_url)};
}

JsonReplyWrapper *YaDRestApi::cleanTrash(const QUrlQuery &params) {
  auto target_url = _main_url.resolved(_conf_vars.clean_trash_url);
  return new JsonReplyWrapper{deleteResource(target_url, params)};
}

JsonReplyWrapper *YaDRestApi::restoreFromTrash(const QUrlQuery &params) {
  auto target_url = _main_url.resolved(_conf_vars.restore_url);
  return new JsonReplyWrapper{put(target_url, "", params)};
}

void YaDRestApi::modifyRequest(QNetworkRequest &request) const {
  setHeaders(request);
  setAuthHeaders(request);
}

void YaDRestApi::handleError(QNetworkReply *reply) {
  QJsonDocument json = QJsonDocument::fromJson(reply->readAll());
  qDebug() << json;
  auto json_obj = json.object();
  if (json_obj["error"] != "") {
    if (json_obj["error"] == "UnauthorizedError") {
      grantAccess(true);
    }
    emit replyApiError(json_obj);
  }
}

void YaDRestApi::handleConfigChange() {
  qDebug() << "yad_conf_change_handler\n";
  loadConfigVariables();
}

void YaDRestApi::loadConfigVariables() {
  _conf_vars.main_url = getConfValue("main_url", _conf_vars.main_url).toUrl();

  _conf_vars.resource_info_url = getConfValue("resource_info_url", _conf_vars.resource_info_url).toUrl();

  _conf_vars.disk_info_url = getConfValue("disk_info_url", _conf_vars.disk_info_url).toUrl();

  _conf_vars.file_list_url = getConfValue("file_list_url", _conf_vars.file_list_url).toUrl();

  _conf_vars.upload_file_url = getConfValue("upload_file_url", _conf_vars.upload_file_url).toUrl();

  _conf_vars.download_file_url = getConfValue("download_file_url", _conf_vars.download_file_url).toUrl();

  _conf_vars.copy_resource_url = getConfValue("copy_resource_url", _conf_vars.copy_resource_url).toUrl();

  _conf_vars.move_resource_url = getConfValue("move_resource_url", _conf_vars.move_resource_url).toUrl();

  _conf_vars.remove_resource_url = getConfValue("remove_resource_url", _conf_vars.remove_resource_url).toUrl();

  _conf_vars.clean_trash_url = getConfValue("clean_trash_url", _conf_vars.clean_trash_url).toUrl();

  _conf_vars.restore_url = getConfValue("restore_url", _conf_vars.restore_url).toUrl();

  _conf_vars.create_folder_url = getConfValue("create_folder", _conf_vars.create_folder_url).toUrl();

  _conf_vars.last_uploads_url = getConfValue("main_url", _conf_vars.last_uploads_url).toUrl();
}





