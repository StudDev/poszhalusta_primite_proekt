#include "ReplyWrapper.h"

ReplyWrapper::ReplyWrapper(QObject *parent)
  : QObject(parent) {

}

ReplyWrapper::ReplyWrapper(QNetworkReply *reply, QObject *parent)
  : _reply{reply} {
  QObject::connect(_reply, &QNetworkReply::finished, this, &ReplyWrapper::watchReplyState);

}

void ReplyWrapper::watchReplyState() {
  handleFinishedReply();
  emit finished();
}

QNetworkReply *ReplyWrapper::getReply() const {
  return _reply;
}

//TODO: check reply for nullptr
void ReplyWrapper::setReply(QNetworkReply *reply) {
  _reply = reply;
  _reply->setParent(this);
  QObject::connect(_reply, &QNetworkReply::readyRead, this, &ReplyWrapper::watchReplyState);
}

ReplyWrapper::~ReplyWrapper() {
  _reply->deleteLater();
}

bool ReplyWrapper::isError() const {
  return _reply->error() != QNetworkReply::NoError;
}

void ReplyWrapper::handleFinishedReply() {
  return;
}
