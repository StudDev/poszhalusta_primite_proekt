#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

#include "PreferencesController.h"
#include "AuthorizationController.h"

int main(int argc, char *argv[]){
  QGuiApplication app(argc, argv);
  
  QQmlApplicationEngine engine;
  
  
  bool isAuthorized = false;

  if (isAuthorized) {
    PreferencesController preferencesController;
    engine.rootContext()->setContextProperty("controller", &preferencesController);

    engine.load(QUrl(QLatin1String("qrc:/PreferencesView.qml")));
  } else {
    AuthorizationController authorizationView;
    engine.rootContext()->setContextProperty("controller", &authorizationView);

    engine.load(QUrl(QLatin1String("qrc:/AuthorizationView.qml")));
  }
  

  return app.exec();
}
