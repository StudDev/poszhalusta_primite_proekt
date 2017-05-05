#include <QtCore/QJsonDocument>
#include "JsonReplyWrapper.h"

JsonReplyWrapper::JsonReplyWrapper(QObject *parent)
  : ReplyWrapper{parent} {
}

JsonReplyWrapper::JsonReplyWrapper(QNetworkReply *reply, QObject *parent)
  : ReplyWrapper{reply, parent} {
}


void JsonReplyWrapper::watchReplyState() {
  QJsonDocument doc = QJsonDocument::fromJson(_reply->readAll());
  _jsonResponse = doc.object();
  emit finished();
  emit jsonReply(_jsonResponse);
}

JsonReplyWrapper::~JsonReplyWrapper() {
  _reply->deleteLater();
}

const QJsonObject &JsonReplyWrapper::getResponse() const {
  return _jsonResponse;
}

bool JsonReplyWrapper::isError() const {
  return _jsonResponse["error"].isNull() || _reply->error();
}
