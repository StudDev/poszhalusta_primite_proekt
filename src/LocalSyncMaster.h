#ifndef YDS_LOCALSYNCMASTER_H
#define YDS_LOCALSYNCMASTER_H


#include <QtCore/QObject>

#include "YaDRestApi.h"

struct ResourceRow {
  QString path;
  QByteArray md5;
  QDateTime last_modified;
};

class LocalSyncMaster : public Configurable {
Q_OBJECT
public:

  LocalSyncMaster(YaDRestApi *api, QObject *parent = nullptr);

  void forceSync();

  ~LocalSyncMaster();

protected:
  virtual void loadConfigVariables() override;

  void handleConfigChange() override;

private slots:

  void parseFileResourceList(const QJsonObject &jsonObject);

  void mergeResources();

private:

  JsonReplyWrapper *getRemoteResources();

  void getLocalResources();

  YaDRestApi *_api;
  QList<ResourceRow> _remote_resources;
  QList<ResourceRow> _local_resources;
  struct {
    QString remote_root;
    QString local_root;
    QStringList watching_dirs;
  } _conf_vars;
};


#endif //YDS_LOCALSYNCMASTER_H
