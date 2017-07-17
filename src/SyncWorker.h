#ifndef YDS_SYNCWORKER_H
#define YDS_SYNCWORKER_H


#include <QtCore/QObject>
#include <QtCore/QTimer>
#include "SyncMaster.h"

struct SyncSharedRes;
struct MoveEventContext;

class SyncWorker : public QObject {
Q_OBJECT
public:
  enum class HandlerReaction {
    Upload = 0,
    Move,
    Remove
  };

  SyncWorker(const QSharedPointer<SyncSharedRes> &shared_res, QObject *parent = nullptr);

  void start();

  HandlerReaction getEventType(const MoveEventContext &context);

public slots:

  void pullEvents();

signals:

  void stop();

private:
  QTimer *event_pull_timer;

  QSharedPointer<SyncSharedRes> _shared_res;
};


#endif //YDS_SYNCWORKER_H
