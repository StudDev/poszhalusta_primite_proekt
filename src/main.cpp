#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <src/NetworkController.h>
#include "YaDiskApiService.h"
#include "NetworkController.h"

int main(int argc, char *argv[]){
  QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
  QGuiApplication app(argc, argv);
  QQmlApplicationEngine engine;
  engine.load(QUrl(QLatin1String("qrc:/main.qml")));
  QNetworkAccessManager manager(&app);
  NetworkController *worker = new NetworkController(&manager);
  YaDiskApiService service{ "/etc/yds/yds_config.json" };
  QThread thread;
  worker->moveToThread(&thread);
  connect(&thread, &QThread::finished, worker, &QObject::deleteLater);
  QObject::connect(&service, &YaDiskApiService::requestToken, worker, &NetworkController::processPostRequest);
  thread.start();
  service.getAuthCode();
  return app.exec();
}
