//
// Created by danm on 19.04.17.
//

#ifndef YDS_NETWORKWORKER_H
#define YDS_NETWORKWORKER_H
#include <QtCore/QObject>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>
#include <QDebug>
#include <QJsonObject>

enum class RequestType{
  GET,
  POST,
  DELETE,
  PUT
};
Q_DECLARE_METATYPE(RequestType);

class RestApiWorker : public QObject {
  Q_OBJECT
public:
  RestApiWorker(QObject *parent = nullptr);
  ~RestApiWorker();
public slots:
  void processRequest(QObject *reciever,
                      const QByteArray &recieve_slot_signature,
                      QNetworkRequest  *request,
                      RequestType type,
                      QByteArray *data = nullptr);
private:
  QNetworkAccessManager _manager;
};


#endif //YDS_NETWORKWORKER_H
