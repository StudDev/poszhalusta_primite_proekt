#ifndef YDS_FILEDOWNLOADER_H
#define YDS_FILEDOWNLOADER_H


#include <QtCore/QFile>
#include "ReplyWrapper.h"

class FileDownloader : public ReplyWrapper{
  Q_OBJECT
public:
  FileDownloader(const QString &path, QObject *parent = nullptr);
  FileDownloader(const QString &path, QNetworkReply* reply, QObject *parent = nullptr);


  void handleFinishedReply() override;
  void setReply(QNetworkReply *reply) override;

private slots:
  void handleNewBytes();
private:
  QFile file;
};


#endif //YDS_FILEDOWNLOADER_H
