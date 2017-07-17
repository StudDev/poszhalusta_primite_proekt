#include "SyncMaster.h"
#include "PathConfigLoader.h"
#include <QMutexLocker>
#include <QDebug>

SyncMaster::SyncMaster(QObject *object)
    : Configurable{object, new PathConfigLoader{}},
      _shared_res{new SyncSharedRes{}},
      _file_watcher{new FileWatchController{this}} {

  loadConfigVariables();

  for (const auto &i : _conf_vars.waching_dirs) {
    _file_watcher->AddDirectory(i);
  }

  QObject::connect(_file_watcher, &FileWatchController::movedToEvent, this, &SyncMaster::moveToHandler);
  QObject::connect(_file_watcher, &FileWatchController::movedFromEvent, this, &SyncMaster::moveFromHandler);
  QObject::connect(_file_watcher, &FileWatchController::modifiedEvent, this, &SyncMaster::modifyHandler);
  QObject::connect(_file_watcher, &FileWatchController::createdEvent, this, &SyncMaster::createdHandler);


  auto *sync_worker = new SyncWorker{_shared_res};

  QObject::connect(&worker_thread, &QThread::finished, sync_worker, &QObject::deleteLater);
  QObject::connect(this, &SyncMaster::startSyncWorker, sync_worker, &SyncWorker::start);
  sync_worker->moveToThread(&worker_thread);

}

void SyncMaster::moveToHandler(unsigned cookie, const QString &name) {
  QMutexLocker locker{&_shared_res->move_events_mutex};
  if (_shared_res->move_events.contains(cookie)) {

    auto renamed = _shared_res->move_events.find(cookie);
    renamed->move_to_recieved = true;
    renamed->new_path = name;
  } else {
    MoveEventContext ev_context{
        false, //move_from_recieved
        true, // move_to_recieved
        "",   // old_name
        name // new_name
    };
    _shared_res->move_events.insert(cookie, ev_context);
  }
}

void SyncMaster::moveFromHandler(unsigned cookie, const QString &name) {
  QMutexLocker locker{&_shared_res->move_events_mutex};
  if (_shared_res->move_events.contains(cookie)) {
    qDebug() << "Unexpectable move events order";
    return;
  } else {
    MoveEventContext ev_context{
        true, //move_from_recieved
        false, // move_to_recieved
        name,   // old_name
        "" // new_name
    };
    _shared_res->move_events.insert(cookie, ev_context);
  }
}

void SyncMaster::modifyHandler(const QString &name) {
  QMutexLocker locker{&_shared_res->modify_events_mutex};
  if (_shared_res->modify_events.contains(name)) {
    return;
  } else {
    _shared_res->modify_events.insert(name);
  }
}

SyncMaster::~SyncMaster() {
  worker_thread.quit();
  worker_thread.wait();
}

void SyncMaster::startSync() {
  worker_thread.start();
  emit startSyncWorker();
  _file_watcher->StartWatch();
}

void SyncMaster::handleConfigChange() {
  auto _old_watching = _conf_vars.waching_dirs;
  loadConfigVariables();
  auto &_curr_watching = _conf_vars.waching_dirs;
  for (auto &i:_curr_watching) {
    if (qBinaryFind(_curr_watching, i) == _curr_watching.end()) {
      _file_watcher->RemoveDirectory(i);
    }
  }
  for (auto &i:_curr_watching) {
    if (qBinaryFind(_old_watching, i) == _old_watching.end()) {
      _file_watcher->AddDirectory(i);
    }
  }
}

void SyncMaster::loadConfigVariables() {
  _conf_vars.waching_dirs = getConfValue("watchingDirs", _conf_vars.waching_dirs).toStringList();
}

void SyncMaster::createdHandler(const QString &name, bool is_directory) {
  if (is_directory) {
    _file_watcher->AddDirectory(name);
  }
  QMutexLocker locker{&_shared_res->modify_events_mutex};
  if (_shared_res->modify_events.contains(name)) {
    return;
  } else {
    _shared_res->modify_events.insert(name);
  }
}

MoveEventContext::MoveEventContext(bool move_from, bool move_to, const QString &old_name, const QString &new_name)
    : move_from_recieved{move_from},
      move_to_recieved{move_to},
      old_path{old_name},
      new_path{new_name} {
}

MoveEventContext::MoveEventContext() {

}
