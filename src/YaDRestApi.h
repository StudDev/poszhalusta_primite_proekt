#ifndef YDS_YADRESTAPI_H
#define YDS_YADRESTAPI_H


#include <QtCore/QObject>
#include <QtNetwork/QNetworkAccessManager>
#include <QtCore/QSettings>
#include <QtCore/QUrl>
#include <QtQml/QtQml>
#include "JsonReplyWrapper.h"
#include "RestApiBase.h"

//TODO: move QSettings to base class
class YaDRestApi : public RestApiBase {
Q_OBJECT
public:

  explicit YaDRestApi(QObject *parent = nullptr);

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

  ~YaDRestApi();

protected:
  void modifyRequest(QNetworkRequest &request) const override;

  void handleError(QNetworkReply *reply) override;

  void handleConfigChange(QSettings *new_config) override;

  void loadConfigVariables() override;


signals:

  void replyApiError(const QJsonObject &error) const;

private:
  void setHeaders(QNetworkRequest &request) const;

  void setAuthHeaders(QNetworkRequest &request) const;

  struct {
    QUrl main_url{"https://cloud-api.yandex.net:443/v1/"};
    QUrl resource_info_url{"./disk/resources/"};
    QUrl disk_info_url{"./disk/"};
    QUrl file_list_url{"./disk/resources/files"};
    QUrl upload_file_url{"./disk/resources/upload"};
    QUrl download_file_url{"./disk/resources/download"};
    QUrl copy_resource_url{"./disk/resources/copy"};
    QUrl move_resource_url{"./disk/resources/move"};
    QUrl remove_resource_url{"./disk/resources"};
    QUrl clean_trash_url{"./trash/resources"};
    QUrl restore_url{"./trash/resources/restore"};
    QUrl create_folder_url{"./disk/resources"};
    QUrl last_uploads_url{"./disk/resources/last-uploads"};
  } _conf_vars;
  QByteArray _accept;
  QByteArray _content_type;
  QUrl _main_url;
};


#endif //YDS_YADRESTAPI_H
