#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

#include "PreferencesController.h"
#include "YaDRestApi.h"


int main(int argc, char *argv[]) {
  QSettings sett{"O", "P"};
  sett.setValue("token/token", "AQAAAAAc9NGzAAQ0sp2NDon360V7rG_tply2j2U");

  QGuiApplication app(argc, argv);

  QQmlApplicationEngine engine;
  bool isAuthorized = false;
  QNetworkAccessManager manager(&app);

  YaDRestApi api_service(&manager, &app);
  QObject::connect(&api_service, &YaDRestApi::replyApiError, [](const QJsonObject &obj) {
    QJsonDocument doc(obj);
    qDebug() << qPrintable(doc.toJson());
  });

  QObject::connect(&api_service, &YaDRestApi::replyNetworkError, [](const QString &error_msg,
                                                                    QNetworkReply::NetworkError error) {
    qDebug() << error_msg << error;
  });

  QObject::connect(&api_service, &YaDRestApi::error, [](const QString &error_msg) {
    qDebug() << error_msg;
  });
  api_service.setConfig(&sett);
  QUrlQuery query;
  query.addQueryItem("path", "/Foo/maxresdefault.png");
  query.addQueryItem("overwrite", "true");
  ReplyWrapper *r = api_service.downloadFile(query);
  QObject::connect(r, &ReplyWrapper::finished, [r] {
    qDebug() << r->getReply()->bytesAvailable();
    QFile file{"/home/danm/filee.png"};
    file.open(QIODevice::WriteOnly);
    file.write(r->getReply()->readAll());
    file.close();
    r->deleteLater();
  });
  return app.exec();
}
