#include <QtCore/QDir>
#include "FileDownloader.h"

FileDownloader::FileDownloader(const QString &path, QObject *parent)
    : ReplyWrapper{parent},
      file{path},
      _tempfile_suffix{".tmp"} {

  QFileInfo file_info{file};
  auto filepath = file_info.absolutePath() + '/';
  QDir().mkpath(filepath);
  filepath.append(file_info.fileName().prepend('.'));
  if (file.exists()) {
    file.setFileName(filepath + _tempfile_suffix);
  } else {
    file.setFileName(filepath);
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
  file.rename(QFileInfo(file).absolutePath().append(
      QFileInfo(file).fileName().remove(0, 1).remove(_tempfile_suffix).prepend('/')));
}
