#ifndef YDS_YADRESTAPI_H
#define YDS_YADRESTAPI_H


#include <QtCore/QObject>
#include <QtNetwork/QNetworkAccessManager>
#include <QtCore/QSettings>
#include <QtCore/QUrl>
#include <QtQml/QtQml>
#include "JsonReplyWrapper.h"
#include "RestApiBase.h"


class YaDRestApi : public RestApiBase {
Q_OBJECT
public:

  explicit YaDRestApi(QObject *parent = nullptr);

  YaDRestApi(QNetworkAccessManager *network_access, QObject *parent = nullptr);

  QSettings *getConfig() const;

  JsonReplyWrapper *getResourceInfo(const QString &path, const QUrlQuery &params);

  JsonReplyWrapper *getDiskInfo();

  JsonReplyWrapper *getFileList(const QUrlQuery &params);

  JsonReplyWrapper *getLastUploads(const QUrlQuery &params);

  ReplyWrapper *uploadFile(const QString &path, const QUrlQuery &params);

  ReplyWrapper *downloadFile(const QString &localpath, const QUrlQuery &params);

  JsonReplyWrapper *copyResource(const QUrlQuery &params);

  JsonReplyWrapper *moveResource(const QUrlQuery &params);

  JsonReplyWrapper *removeResource(const QUrlQuery &params);

  JsonReplyWrapper *createFolder(const QUrlQuery &params);

  JsonReplyWrapper *cleanTrash(const QUrlQuery &params);

  JsonReplyWrapper *restoreFromTrash(const QUrlQuery &params);

  JsonReplyWrapper *getOperationStatus(const QUrl &operation_url);

  void setConfig(QSettings *config);

  ~YaDRestApi();

protected:
  void modifyRequest(QNetworkRequest &request) const override;
  void handleError(QNetworkReply *reply) const override;

signals:
  void replyApiError(const QJsonObject &error) const;

private:
  void setHeaders(QNetworkRequest &request) const;
  void setAuthHeaders(QNetworkRequest &request) const;
  QByteArray _accept;
  QByteArray _content_type;
  QUrl _main_url;
  QSettings *_config;
};


#endif //YDS_YADRESTAPI_H
