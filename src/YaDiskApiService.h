#ifndef YDS_YADISKAPISERVICE_H
#define YDS_YADISKAPISERVICE_H

#include <memory>
#include <QObject>
#include <QtNetwork>
#include "ConfigManager.h"

class YaDiskApiService : public QObject {
  Q_OBJECT;
public:
  YaDiskApiService(QObject *parnet = nullptr)
    :QObject(parnet){

  }
  ~YaDiskApiService(){}
public slots:
  void handleReply(QNetworkReply *reply);
};


#endif //YDS_YADISKAPISERVICE_H
