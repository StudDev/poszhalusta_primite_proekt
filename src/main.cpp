#include <QGuiApplication>
#include <QQuickView>
#include <QQmlContext>

#include "PreferencesController.h"
#include "AuthorizationController.h"

int main(int argc, char *argv[]){
  QGuiApplication app(argc, argv);
  app.setQuitOnLastWindowClosed(false);

  //PreferencesController* preferencesController;
  AuthorizationController* authorizationController;
  
  const bool isAuthorized = false;

  if (isAuthorized) {
    //preferencesController = new PreferencesController();
  } else {
    authorizationController = new AuthorizationController("20beb8f54f66490fa4f21b42f7af7145",
      "45493f2fdac944f994b891995db2a305",
      &app);

    authorizationController->grant();
  }

  return app.exec();
}
