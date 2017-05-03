#include <QGuiApplication>
#include <QQuickView>
#include <QQmlContext>

#include "PreferencesController.h"
#include "AuthorizationController.h"

int main(int argc, char *argv[]){
  QGuiApplication app(argc, argv);

  PreferencesController* preferencesController;
  AuthorizationController* authorizationController;
  
  const bool isAuthorized = false;

  if (isAuthorized) {
    preferencesController = new PreferencesController();
  } else {
    QNetworkAccessManager networkAccessManager;
    authorizationController = new AuthorizationController("20beb8f54f66490fa4f21b42f7af7145",
      networkAccessManager);

    authorizationController->grant();
  }

  int status = app.exec();

  authorizationController->deleteLater();
  preferencesController->deleteLater();

  return status;
}
