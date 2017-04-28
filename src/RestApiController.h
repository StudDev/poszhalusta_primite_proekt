#ifndef YDS_NETWORKCONTROLLER_H
#define YDS_NETWORKCONTROLLER_H
#include <QObject>
#include <QtNetwork/QNetworkAccessManager>
#include <memory>
#include <QThread>
#include "RestApiWorker.h"

class RestApiController:public QObject{
  Q_OBJECT
public:
  RestApiController(QObject *parent = nullptr);
  ~RestApiController();
public:
  void sendGetRequest(QObject *reciever,
                      const QByteArray &recieve_slot,
                      QNetworkRequest  *request);
  void sendPostRequest(QObject *reciever,
                       const QByteArray &recieve_slot,
                       QNetworkRequest  *request,
                       QByteArray *data);
  void sendPutRequest(QObject *reciever, const QByteArray &recieve_slot,
                   QNetworkRequest  *request, QByteArray *data);

signals:
  void proxyRequest(QObject *reciever,
                    const QByteArray &recieve_slot,
                    QNetworkRequest  *request,
                    RequestType type,
                    QByteArray *data = nullptr);

private:
  RestApiWorker *worker;
  QThread worker_thread;
};




#endif //YDS_NETWORKCONTROLLER_H
