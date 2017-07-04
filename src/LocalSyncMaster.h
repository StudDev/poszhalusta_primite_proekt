#ifndef YDS_LOCALSYNCMASTER_H
#define YDS_LOCALSYNCMASTER_H


#include <QtCore/QObject>

#include "YaDRestApi.h"

struct ResourceRow {
  QString path;
  QByteArray md5;
  QDateTime last_modified;
};

class LocalSyncMaster : public QObject {
Q_OBJECT
public:

  LocalSyncMaster(YaDRestApi *api, QObject *parent = nullptr);


  void forceSync();

  ~LocalSyncMaster();

private slots:

  void parseFileResourceList(const QJsonObject &jsonObject);

  void mergeResources();

private:
  JsonReplyWrapper *getRemoteResources();

  void getLocalResources();

  YaDRestApi *_api;
  QSettings *_config;
  QList<ResourceRow> _remote_resources;
  QList<ResourceRow> _local_resources;
};


#endif //YDS_LOCALSYNCMASTER_H
