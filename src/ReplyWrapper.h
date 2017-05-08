#ifndef YDS_REPLYWRAPPER_H
#define YDS_REPLYWRAPPER_H


#include <QtCore/QObject>
#include <QtNetwork/QNetworkReply>

class ReplyWrapper : public QObject {
Q_OBJECT
public:
  explicit ReplyWrapper(QObject *parent = nullptr);

  explicit ReplyWrapper(QNetworkReply *reply, QObject *parent = nullptr);

  QNetworkReply *getReply() const;

  virtual void setReply(QNetworkReply *reply);

  virtual bool isError() const;

  virtual void handleFinishedReply();

  virtual ~ReplyWrapper();

signals:

  void finished() const;
protected slots:
  void watchReplyState();
protected:
  QNetworkReply *_reply;
};


#endif //YDS_REPLYWRAPPER_H
