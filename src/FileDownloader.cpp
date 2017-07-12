#include <QtCore/QDir>
#include "FileDownloader.h"

FileDownloader::FileDownloader(const QString &path, QObject *parent)
  : ReplyWrapper{parent},
    file{path},
    _tempfile_suffix{".tmp"}{
  QDir().mkpath(QFileInfo(file).absolutePath());
  if (file.exists()) {
    qDebug() << file.fileName() << "exists";
    file.setFileName(path + _tempfile_suffix);
  }
  file.open(QIODevice::WriteOnly);
}

FileDownloader::FileDownloader(const QString &path, QNetworkReply *reply, QObject *parent)
  : FileDownloader{path, parent} {
  setReply(reply);
}


//TODO: check reply for nullptr
void FileDownloader::setReply(QNetworkReply *reply) {
  _reply = reply;
  QObject::connect(_reply, &QNetworkReply::readyRead, this, &FileDownloader::handleNewBytes);
  QObject::connect(_reply, &QNetworkReply::finished, this, &FileDownloader::watchReplyState);
  _reply->setParent(this);
}

void FileDownloader::handleNewBytes() {
  if (!file.isOpen()) {
    qDebug() << file.fileName() << "error";
    QObject::disconnect(_reply, &QNetworkReply::readyRead, this, &FileDownloader::handleNewBytes);
    return;
  }
  file.write(_reply->readAll());
}

FileDownloader::~FileDownloader() {
  file.close();
}

//TODO: make ".tmp" const
void FileDownloader::handleFinishedReply() {
  file.rename(file.fileName().remove(_tempfile_suffix));
}
