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

  void setConfig(QSettings *config);
  ~YaDRestApi();
public slots:
  void handleReply(QNetworkReply *reply);
  void processUploading(QNetworkReply *uploadUrlReply,JsonReplyWrapper *result_handler);
signals:
  void replyError(QString error_msg, QNetworkReply::NetworkError error);
private:
  void setHeaders(QNetworkRequest &request) const;
  void setAuthHeaders(QNetworkRequest &request) const;

  QByteArray _accept;
  QByteArray _content_type{"application/json"};
  QUrl _main_url{"https://cloud-api.yandex.net/v1/"};
  QNetworkAccessManager *_network_manager;
  QSettings *_config;
};


#endif //YDS_YADRESTAPI_H
