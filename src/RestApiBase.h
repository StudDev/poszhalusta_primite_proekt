#ifndef YDS_RESTAPIBASE_H
#define YDS_RESTAPIBASE_H


#include <QtCore/QObject>
#include <QtCore/QUrlQuery>
#include "ReplyWrapper.h"
#include "AuthorizationController.h"
#include "Configurable.h"

//TODO: add QSettings management , implement interface for it
class RestApiBase : public Configurable {
Q_OBJECT
public:
  explicit RestApiBase(QObject *parent = nullptr);


  RestApiBase(QOAuth2AuthorizationCodeFlow *authorizer, QObject *parent = nullptr);

  bool isTokenFresh() const;

  void grantAccess(bool forced = false);

  QString token() const;

  ~RestApiBase();

signals:

  void accessGranted() const;

  void replyNetworkError(const QString &error_msg, QNetworkReply::NetworkError error) const;

  void error(const QString &error_msg) const;

  void authorized() const;

protected slots:

  void handleReply(QNetworkReply *reply);

protected:
  QNetworkReply *get(const QUrl &request, QUrlQuery params = QUrlQuery());

  QNetworkReply *get(const QNetworkRequest &request);

  QNetworkReply *deleteResource(const QUrl &request, const QUrlQuery params = QUrlQuery());

  QNetworkReply *post(const QUrl &request, QIODevice *data, const QUrlQuery params = QUrlQuery());

  QNetworkReply *put(const QUrl &request, QIODevice *data, const QUrlQuery params = QUrlQuery());

  QNetworkReply *post(const QUrl &request, const QByteArray &data, const QUrlQuery params = QUrlQuery());

  QNetworkReply *put(const QUrl &request, const QByteArray &data, const QUrlQuery params = QUrlQuery());

  QNetworkRequest createRequest(const QUrl &request, const QUrlQuery &params = QUrlQuery()) const;

  virtual void modifyRequest(QNetworkRequest &request) const;

  virtual void handleError(QNetworkReply *reply);

  virtual void handleConfigChange() override;

private:
  template<typename InputData>
  QNetworkReply *performRequest(const QNetworkRequest &request,
                                InputData &&data,
                                QNetworkAccessManager::Operation request_type = QNetworkAccessManager::GetOperation) const;


  QNetworkReply *defaultRequest(const QUrl &request,
                                QIODevice *data = nullptr,
                                const QUrlQuery params = QUrlQuery(),
                                QNetworkAccessManager::Operation request_type = QNetworkAccessManager::GetOperation);

  QNetworkReply *defaultRequest(const QUrl &request,
                                const QByteArray &data,
                                const QUrlQuery params = QUrlQuery(),
                                QNetworkAccessManager::Operation request_type = QNetworkAccessManager::GetOperation);

  mutable std::atomic_flag is_auth_process_started;
  QOAuth2AuthorizationCodeFlow *_oauth;
};

#endif //YDS_RESTAPIBASE_H
