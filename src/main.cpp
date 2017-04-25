#include <QGuiApplication>
#include <QQuickView>
#include <QQmlContext>

#include "YaDiskApiService.h"
#include "NetworkController.h"
#include "MainView.h"

int main(int argc, char *argv[]){
  QGuiApplication app(argc, argv);
  
  QQuickView view;
  MainView mainView;
  view.rootContext()->setContextProperty("mainView", &mainView);
  view.setSource(QUrl::fromLocalFile("res/main.qml"));
  view.show();

  return app.exec();
}
