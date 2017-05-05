#ifndef YDS_YADRESTAPI_H
#define YDS_YADRESTAPI_H


#include <QtCore/QObject>
#include <QtNetwork/QNetworkAccessManager>
#include <QtCore/QSettings>
#include <QtCore/QUrl>
#include <QtQml/QtQml>
#include "JsonReplyWrapper.h"

///
/// \brief Class provides interface for Yandex Disk REST API
///
class YaDRestApi : public QObject {
Q_OBJECT
public:
  /// \brief Default constuctor for any QObject's inheritors
  /// \param parent parent of this object
  explicit YaDRestApi(QObject *parent = nullptr);

  /// \brief Construct YaDRestApi with explicit QNetworkAccessManager
  /// \param network_access manager fo dealing with http requests
  /// \param parent parent of this object
  YaDRestApi(QNetworkAccessManager *network_access, QObject *parent);

  /// \briefGetter for QSettings configuration object
  /// \return pointer to current configuration
  QSettings *getConfig() const;

  /// \brief Getter for QNetworkAccessManager network manager
  /// \return pointer to current network manager
  QNetworkAccessManager *getNetworkManager() const;

  /// \brief Metainformation about a file or folder
  ///
  /// \details To request metainformation about files and folders,
  ///          specify the path to the corresponding resource on Disk.
  ///          Metainformation includes the properties of files and folders,
  ///          and the properties and contents of subfolders.
  ///
  /// \param path path to resource
  /// \param params additional request params : sort, limit, offset, fields, preview_size
  /// \return when finished JsonReplyWrapper will contain
  ///          metainformation about the requested resource
  ///          avalible through getResponse() method and  jsonReply(const QJsonObect&) signal
  JsonReplyWrapper *getResourceInfo(const QString &path, const QUrlQuery &params) const;

  /// \brief Data about a user's Disk
  /// \details The API returns general information about a user's Disk:
  ///          the available space, addresses of system folders, and so on.
  /// \return  when finished JsonReplyWrapper will contain
  ///          data about the Disk in the
  ///          avalible through getResponse() method and  jsonReply(const QJsonObect&) signal
  JsonReplyWrapper *getDiskInfo() const;

  /// \brief Flat list of all files
  ///
  /// \details The API returns a flat list of all files on the Disk in alphabetical order.
  /// The flat list does not reflect the directory structure, so it is convenient
  /// for searching for files of a certain type that are spread across different folders.
  /// Yandex.Disk detects the file type when uploading each file.
  ///
  /// \param params additional request params : limit, media_type, offset, fields, preview_size, preview_crop
  /// \return when finished JsonReplyWrapper will contain
  /// metainformation about the requested number of files
  /// avalible through getResponse() method and  jsonReply(const QJsonObect&) signal
  JsonReplyWrapper *getFileList(const QUrlQuery &params) const;

  /// \brief Latest uploaded files
  ///
  /// \details The API returns a list of the files most recently uploaded to Yandex.Disk.
  /// The list can be filtered by file type (audio, video, image, and so on).
  ///  Each file type is detected by Disk when uploading.
  ///
  /// \param params additional request params : limit, media_type, fields, preview_size, preview_crop
  /// \return   when finished JsonReplyWrapper will contain metainformation about the requested number of files
  /// available through getResponse() method and jsonReply(const QJsonObect&) signal
  JsonReplyWrapper *getLastUploads(const QUrlQuery &params) const;

  ReplyWrapper *uploadFile(const QString &path, const QUrlQuery &params) const;

  ReplyWrapper *downloadFile(const QUrlQuery &params) const;

  JsonReplyWrapper *copyResource(const QUrlQuery &params) const;

  JsonReplyWrapper *moveResource(const QUrlQuery &params) const;

  JsonReplyWrapper *removeResource(const QUrlQuery &params) const;

  JsonReplyWrapper *createFolder(const QUrlQuery &params) const;

  JsonReplyWrapper *cleanTrash(const QUrlQuery &params) const;

  JsonReplyWrapper *restoreFromTrash(const QUrlQuery &params) const;


  JsonReplyWrapper *getOperationStatus(const QUrl &operation_url) const;

  void setConfig(QSettings *config);

  ~YaDRestApi();

public slots:

  void handleReply(QNetworkReply *reply);

signals:

  void replyNetworkError(const QString &error_msg, QNetworkReply::NetworkError error);

  void replyApiError(const QJsonObject &error);

  void error(const QString &error_msg) const;

private:
  JsonReplyWrapper *defaultGetRequest(const QUrl &url) const;

  JsonReplyWrapper *defaultPostRequest(const QUrl &url) const;

  JsonReplyWrapper *defaultDeleteRequest(const QUrl &url) const;

  JsonReplyWrapper *defaultPutRequest(const QUrl &url) const;

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
