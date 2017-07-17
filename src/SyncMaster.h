#ifndef YDS_SYNCMASTER_H
#define YDS_SYNCMASTER_H


#include <QtCore/QObject>
#include <QtCore/QHash>
#include <QtCore/QSet>
#include <QtCore/QMutex>
#include <QtCore/QSharedPointer>
#include <QtCore/QThread>
#include "SyncWorker.h"
#include "Configurable.h"
#include "FileWatchController.h"


struct MoveEventContext {
  MoveEventContext();

  MoveEventContext(bool move_from, bool move_to, const QString &old_name, const QString &new_name);

  bool move_from_recieved{false};
  bool move_to_recieved{false};
  QString old_path{""};
  QString new_path{""};
};

struct SyncSharedRes {
  QHash<unsigned, MoveEventContext> move_events;
  QSet<QString> modify_events;
  QMutex move_events_mutex;
  QMutex modify_events_mutex;
};


class SyncMaster : public Configurable {
Q_OBJECT
protected:
  void handleConfigChange() override;

  void loadConfigVariables() override;

private:
public:
  SyncMaster(QObject *object = nullptr);

  ~SyncMaster();

public slots:

  void moveToHandler(unsigned cookie, const QString &name);

  void moveFromHandler(unsigned cookie, const QString &name);

  void modifyHandler(const QString &name);

  void createdHandler(const QString &name, bool is_directory);

  void startSync();

signals:

  void startSyncWorker();

private:
  QSharedPointer<SyncSharedRes> _shared_res;
  QThread worker_thread;
  struct {
    QStringList waching_dirs;
  } _conf_vars;
  FileWatchController *_file_watcher;
};


#endif //YDS_SYNCMASTER_H
