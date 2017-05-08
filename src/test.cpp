#include <QQuickView>
#include <QQmlContext>
#include <QGuiApplication>
#include "RestApiBase.h"
#include "YaDRestApi.h"

int main(int argc,char *argv[]){
  QGuiApplication app(argc,argv);
  app.setQuitOnLastWindowClosed(false);
  YaDRestApi api{&app};
  QUrlQuery params;
  params.addQueryItem("path","/ubuntu_serv_16_04.iso");
  QObject::connect(&api,&YaDRestApi::replyApiError,[](const QJsonObject &json){
    qDebug() << qPrintable(QJsonDocument(json).toJson());
  });
  ReplyWrapper* i = api.downloadFile("/home/danm/ubuntu_serv.iso",params);
  QObject::connect(i,&ReplyWrapper::finished,[i]{
    qDebug() << i->getReply()->bytesAvailable();
  });
  return app.exec();
}