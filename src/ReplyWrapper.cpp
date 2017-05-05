#include "ReplyWrapper.h"

ReplyWrapper::ReplyWrapper(QObject *parent)
  : QObject(parent) {

}

ReplyWrapper::ReplyWrapper(QNetworkReply *reply, QObject *parent)
  : _reply{reply} {
  QObject::connect(_reply, &QNetworkReply::finished, this, &ReplyWrapper::watchReplyState);
  _reply->setParent(this);
}

void ReplyWrapper::watchReplyState() {
  emit finished();
}

QNetworkReply *ReplyWrapper::getReply() const {
  return _reply;
}

void ReplyWrapper::setReply(QNetworkReply *reply) {
  _reply = reply;
  QObject::connect(_reply, &QNetworkReply::finished, this, &ReplyWrapper::watchReplyState);
  _reply->setParent(this);
}

ReplyWrapper::~ReplyWrapper() {
  _reply->deleteLater();
}

bool ReplyWrapper::isError() const {
  return _reply->error() != QNetworkReply::NoError;
}
