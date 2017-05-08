#include "FileDownloader.h"

FileDownloader::FileDownloader(const QString &path, QObject *parent)
  :ReplyWrapper{parent},
   file{path}{
   file.setParent(_reply);
   file.open(QIODevice::WriteOnly);
}
void FileDownloader::handleFinishedReply() {
   file.close();
  _reply->close();
}

FileDownloader::FileDownloader(const QString &path, QNetworkReply *reply, QObject *parent)
  :FileDownloader{path,parent}{
  setReply(reply);
}

void FileDownloader::setReply(QNetworkReply *reply) {
  _reply = reply;
  QObject::connect(_reply,&QNetworkReply::readyRead,this,&FileDownloader::handleNewBytes);
  QObject::connect(_reply,&QNetworkReply::finished,this,&FileDownloader::watchReplyState);
  _reply->setParent(this);
}

void FileDownloader::handleNewBytes() {
  file.write(_reply->readAll());
}
