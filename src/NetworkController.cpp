#include "NetworkController.h"
#include <QObject>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>
#include <QThread>

NetworkController::NetworkController(QObject *parent)
  :QObject(parent),
   _manager(){

}

NetworkController::NetworkController(QNetworkAccessManager *manager, QObject *parent)
  :QObject(parent),
   _manager(manager)
{
  QObject::connect(_manager,&QNetworkAccessManager::finished,
                   this,&NetworkController::handleReply);
}

NetworkController::~NetworkController() {

}

void NetworkController::processPostRequest(QNetworkRequest *request, QIODevice *data) {
  _manager->post(*request, data);
}

void NetworkController::processGetRequest(QNetworkRequest *request) {
  _manager->get(*request);
}

void NetworkController::handleReply(QNetworkReply *reply) {
  emit requestDone(reply);
}
