#ifndef YDS_JSONREPLYWRAPPER_H
#define YDS_JSONREPLYWRAPPER_H


#include <QtCore/QObject>
#include <QtNetwork/QNetworkReply>
#include <QtCore/QJsonObject>
#include "ReplyWrapper.h"

class JsonReplyWrapper : public ReplyWrapper {
Q_OBJECT
public:
  explicit JsonReplyWrapper(QObject *parent = nullptr);

  explicit JsonReplyWrapper(QNetworkReply *reply, QObject *parent = nullptr);

  const QJsonObject &getJsonResponse() const;

  bool isError() const override;

  void handleFinishedReply() override;

  ~JsonReplyWrapper();

signals:

  void jsonReply(const QJsonObject &obj) const;

private:
  QJsonObject _jsonResponse;
};


#endif //YDS_JSONREPLYWRAPPER_H
