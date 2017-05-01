#ifndef YDS_YADRESTAPI_H
#define YDS_YADRESTAPI_H


#include <QtCore/QObject>
#include <QtNetwork/QNetworkAccessManager>
#include <QtCore/QSettings>
#include <QtCore/QUrl>
#include <QtQml/QtQml>
#include "JsonReplyWrapper.h"

class YaDRestApi : public QObject {
Q_OBJECT
public:
  explicit YaDRestApi(QObject *parent = nullptr);
  YaDRestApi(QNetworkAccessManager *network_access, QObject *parent);
  QSettings *getConfig() const;
  QNetworkAccessManager *getNetworkManager() const;
  JsonReplyWrapper * getResourceInfo(const QString &path, const QUrlQuery &params) const;
  JsonReplyWrapper * getDiskInfo() const;
  JsonReplyWrapper * getFileList(unsigned limit, const QUrlQuery &params) const;
  JsonReplyWrapper * getLastUploads(unsigned limit, const QUrlQuery &params) const;

  JsonReplyWrapper * uploadFile(const QString &path, const QUrlQuery &params) const;
  JsonReplyWrapper * downloadFile(const QUrlQuery &params) const;
  JsonReplyWrapper * copyResource(const QUrlQuery &params) const;
  JsonReplyWrapper * moveResource(const QUrlQuery &params) const;
  JsonReplyWrapper * removeResource(const QUrlQuery &params) const;
  JsonReplyWrapper * createFolder(const QUrlQuery &params) const;
  JsonReplyWrapper * getOperationStatus(const QUrl &operation_url) const;
  void setConfig(QSettings *config);
  ~YaDRestApi();

public slots:
  void handleReply(QNetworkReply *reply);

signals:
  void replyNetworkError(const QString &error_msg, QNetworkReply::NetworkError error);
  void replyApiError(const QJsonObject &error);
  void error(const QString &error_msg) const;

private:
  JsonReplyWrapper* defaultGetRequest(const QUrl &url) const;
  JsonReplyWrapper* defaultPostRequest(const QUrl &url) const;
  JsonReplyWrapper* defaultDeleteRequest(const QUrl &url) const;
  void setDefaultRequestOptions(QNetworkRequest &request, const QUrl &url) const;
  void setHeaders(QNetworkRequest &request) const;
  void setAuthHeaders(QNetworkRequest &request) const;
  QByteArray _accept;
  QByteArray _content_type;
  QUrl _main_url;
  QNetworkAccessManager *_network_manager;
  QSettings *_config;
};


#endif //YDS_YADRESTAPI_H
