#ifndef YDS_FILEDOWNLOADER_H
#define YDS_FILEDOWNLOADER_H


#include <QtCore/QFile>
#include "ReplyWrapper.h"

class FileDownloader : public ReplyWrapper {
Q_OBJECT
public:
  FileDownloader(const QString &path, QObject *parent = nullptr);

  FileDownloader(const QString &path, QNetworkReply *reply, QObject *parent = nullptr);

  void setReply(QNetworkReply *reply) override;

  void setTempfileSuffix(const QString &suffix);

  void handleFinishedReply() override;

  ~FileDownloader();

private slots:

  void handleNewBytes();

private:
  QFile file;
  QString _tempfile_suffix;
};


#endif //YDS_FILEDOWNLOADER_H
