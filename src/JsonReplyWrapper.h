#ifndef YDS_JSONREPLYWRAPPER_H
#define YDS_JSONREPLYWRAPPER_H


#include <QtCore/QObject>
#include <QtNetwork/QNetworkReply>
#include <QtCore/QJsonObject>

class JsonReplyWrapper : public QObject {
  Q_OBJECT
public:
  JsonReplyWrapper(QObject *parent = nullptr);
  ~JsonReplyWrapper();
  QNetworkReply *getReply() const;
  const QJsonObject& getResponse() const;
  void setReply(QNetworkReply *reply);
private slots:
  void watchReplyState();
signals:
  void finished();
private:
  QNetworkReply *_reply;
  QJsonObject _jsonResponse;
};


#endif //YDS_JSONREPLYWRAPPER_H
