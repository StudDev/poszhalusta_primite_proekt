#ifndef YDS_RESTAPIBASE_H
#define YDS_RESTAPIBASE_H


#include <QtCore/QObject>
#include <QtCore/QUrlQuery>
#include "ReplyWrapper.h"
#include "AuthorizationController.h"

class RestApiBase: public QObject {
 Q_OBJECT
public:
  RestApiBase(QNetworkAccessManager *manager = nullptr, QObject *parent = nullptr);
  RestApiBase(QObject *parent);

  bool isTokenFresh() const;
  void grantAccess();
  QString token() const;
  ~RestApiBase();
signals:
  void accessGranted();
  void replyNetworkError(const QString &error_msg, QNetworkReply::NetworkError error);
protected slots:
  void handleReply(QNetworkReply * reply);
protected:

  QNetworkReply* get(const QUrl &request, QUrlQuery params = QUrlQuery());
  QNetworkReply* get(const QNetworkRequest &request);
  QNetworkReply* deleteResource(const QUrl &request,const QUrlQuery params = QUrlQuery());

  QNetworkReply* post(const QUrl &request,QIODevice *data, const QUrlQuery params = QUrlQuery());

  QNetworkReply* put(const QUrl &request, QIODevice *data, const QUrlQuery params = QUrlQuery());

  QNetworkReply* post(const QUrl &request,const QByteArray &data,const QUrlQuery params = QUrlQuery());

  QNetworkReply* put(const QUrl &request, const QByteArray &data, const QUrlQuery params = QUrlQuery());

  QNetworkRequest createRequest(const QUrl &request,const QUrlQuery &params = QUrlQuery()) const;

  virtual void modifyRequest(QNetworkRequest &request) const;
  virtual void handleError(QNetworkReply *reply);
private:
  QNetworkReply* performRequest(const QNetworkRequest &request,
                                QIODevice *data,
                                QNetworkAccessManager::Operation request_type = QNetworkAccessManager::GetOperation) const;
  QNetworkReply* performRequest(const QNetworkRequest &request,
                                const QByteArray &data,
                                QNetworkAccessManager::Operation request_type = QNetworkAccessManager::GetOperation) const;
  QNetworkReply* defaultRequest(const QUrl &request,
                                QIODevice *data = nullptr,
                                const QUrlQuery params = QUrlQuery(),
                                QNetworkAccessManager::Operation request_type = QNetworkAccessManager::GetOperation);
  QNetworkReply* defaultRequest(const QUrl &request,
                                 const QByteArray &data,
                                 const QUrlQuery params = QUrlQuery(),
                                 QNetworkAccessManager::Operation request_type = QNetworkAccessManager::GetOperation);
  mutable bool is_auth_process_started;
  QNetworkAccessManager *_manager;
  QOAuth2AuthorizationCodeFlow *_oauth;
};

#endif //YDS_RESTAPIBASE_H
