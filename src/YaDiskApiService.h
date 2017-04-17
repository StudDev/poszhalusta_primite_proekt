#ifndef YDS_YADISKAPISERVICE_H
#define YDS_YADISKAPISERVICE_H

#include <QObject>
#include <QtNetwork>
#include "ConfigManager.h"

class YaDiskApiService : public QObject {
  Q_OBJECT;
public:
  YaDiskApiService(const QString &config_path, QObject *parent = nullptr);
void getAuthCode();
void getToken(const QString &auth_code);
public slots:
  void storeToken(QString token);
signals:
  void requestToken(QNetworkRequest *, QByteArray *request_data);
  void requestAuthCode();
private:
  const QString _authorization = "https://oauth.yandex.ru/authorize";
  QUrl _token_path;
  ConfigManager _config_manager;
};


#endif //YDS_YADISKAPISERVICE_H
