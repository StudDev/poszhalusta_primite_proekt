#include <QGuiApplication>
#include <QQuickView>
#include <QQmlContext>

#include "YaDiskApiService.h"
#include "NetworkController.h"
#include "MainView.h"

int main(int argc, char *argv[]){
  // QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
  QGuiApplication app(argc, argv);
  // QQmlApplicationEngine engine;
  // engine.load(QUrl(QLatin1String("qrc:/main.qml")));

  QQuickView view;
  MainView mainView;
  view.rootContext()->setContextProperty("mainView", &mainView);
  view.setSource(QUrl::fromLocalFile("res/main.qml"));
  view.show();

  return app.exec();
}
