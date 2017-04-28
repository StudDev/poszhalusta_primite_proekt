
#include <QObject>
#include <QDesktopServices>
#include <QUrl>
#include <QUuid>
#include <QtNetwork>

#include "YaDiskApiService.h"

void YaDiskApiService::handleReply(QNetworkReply *reply) {
  if(!reply->error()){
    qDebug() << "SUCCESS " << QString(reply->readAll()).toUtf8() << '\n';
  }else {
    qDebug() << "FAILT " << reply->errorString() << '\n';
  }
}
