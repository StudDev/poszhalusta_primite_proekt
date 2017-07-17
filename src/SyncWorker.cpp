#include "SyncWorker.h"
#include <QDebug>


void SyncWorker::pullEvents() {
  QMutexLocker move_events_locker{&_shared_res->move_events_mutex};
  for (auto &key : _shared_res->move_events.keys()) {
    auto event_context = _shared_res->move_events.take(key);
    switch (getEventType(event_context)) {
      case HandlerReaction::Move:
        qDebug() << "Reaction: Move " << event_context.old_path << " to " << event_context.new_path;
        break;
      case HandlerReaction::Upload:
        qDebug() << "Reaction:Upload " << event_context.new_path;
        break;
      case HandlerReaction::Remove:
        qDebug() << "Reaction:Remove " << event_context.old_path;
        break;
    }
  }
  move_events_locker.unlock();
  QMutexLocker modify_events_locker{&_shared_res->modify_events_mutex};
  for (auto &modified : _shared_res->modify_events.values()) {
    _shared_res->modify_events.remove(modified);
    qDebug() << "Reaction:Upload" << modified;

  }
  modify_events_locker.unlock();
}

void SyncWorker::start() {
  event_pull_timer = new QTimer{this};
  QObject::connect(event_pull_timer, &QTimer::timeout, this, &SyncWorker::pullEvents);
  event_pull_timer->start(1000);
}

SyncWorker::SyncWorker(const QSharedPointer<SyncSharedRes> &shared_res, QObject *parent)
    : QObject{parent},
      _shared_res{shared_res},
      event_pull_timer{nullptr} {

}

SyncWorker::HandlerReaction SyncWorker::getEventType(const MoveEventContext &context) {
  if (context.move_from_recieved && context.move_to_recieved) {
    return HandlerReaction::Move;
  } else if (context.move_to_recieved && !context.move_from_recieved) {
    return HandlerReaction::Upload;
  } else {
    return HandlerReaction::Remove;
  }
}
