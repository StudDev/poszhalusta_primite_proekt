#include <QtCore/QJsonDocument>
#include "JsonReplyWrapper.h"

JsonReplyWrapper::JsonReplyWrapper(QObject *parent)
  : ReplyWrapper{parent} {
}

JsonReplyWrapper::JsonReplyWrapper(QNetworkReply *reply, QObject *parent)
  : ReplyWrapper{reply, parent} {

}
JsonReplyWrapper::~JsonReplyWrapper() {
  _reply->deleteLater();
}

const QJsonObject &JsonReplyWrapper::getJsonResponse() const {
  return _jsonResponse;
}

bool JsonReplyWrapper::isError() const {
  return _jsonResponse["error"].isNull() || getReply()->error();
}

void JsonReplyWrapper::handleFinishedReply() {
  QJsonDocument doc = QJsonDocument::fromJson(getReply()->readAll());
  _jsonResponse = doc.object();
  emit jsonReply(_jsonResponse);
}
