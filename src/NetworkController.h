#ifndef YDS_NETWORKCONTROLLER_H
#define YDS_NETWORKCONTROLLER_H


#include <QObject>
#include <QtNetwork/QNetworkAccessManager>

class NetworkController: public QObject {
  Q_OBJECT
  QNetworkAccessManager *_manager;
public:
  NetworkController(QObject *parent = nullptr);
  NetworkController(QNetworkAccessManager* manager, QObject *parent = nullptr);
  ~NetworkController();
public slots:
  void processPostRequest(QNetworkRequest *request, QByteArray *data);

  void processGetRequest(QNetworkRequest *request);


private slots:
  void handleReply(QNetworkReply *reply);;
signals:
  void requestDone(QNetworkReply*);
};



#endif //YDS_NETWORKCONTROLLER_H
