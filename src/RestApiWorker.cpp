#include <QtCore/QMetaMethod>
#include <QtCore/QJsonDocument>
#include "RestApiWorker.h"

RestApiWorker::RestApiWorker(QObject *parent)
  :QObject(parent),
   _manager(this){

}

void RestApiWorker::processRequest(QObject *reciever,
                                   const QByteArray &recieve_slot_signature,
                                   QNetworkRequest *request,
                                   RequestType type,
                                   QByteArray *data) {
  qRegisterMetaType<RequestType>();
  int metaIndex = reciever->metaObject()->indexOfMethod(recieve_slot_signature);
  QMetaMethod method{reciever->metaObject()->method(metaIndex)};
  QNetworkReply *reply = nullptr;
  switch(type){
    case RequestType::GET:
      reply = _manager.get(*request);
      break;
    case RequestType::POST:
      reply = _manager.post(*request,*data);
      break;
    case RequestType::PUT:
      reply = _manager.put(*request,*data);
      break;
    case RequestType::DELETE:
      reply = _manager.deleteResource(*request);
      break;
    default:
      return;
  }
  QObject::connect(reply,&QNetworkReply::finished,[reply,reciever,method]{
     method.invoke(reciever,Qt::QueuedConnection,Q_ARG(QNetworkReply*,reply));
  });
}

RestApiWorker::~RestApiWorker() {

}
