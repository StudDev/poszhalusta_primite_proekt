#ifndef YDS_FILEWATCH_H

#define YDS_FILEWATCH_H

#include <QDebug>
#include <string.h>

#include <QtCore/QObject>

#include <QThread>

#include <QString>
#include <QRegExp>
#include <QDir>
#include <QQueue>

//for storing   <directory, watch_descriptor> and
//              <watch_descriptor, directory>
#include <QHash>

//for inotify_init(..)
#include <sys/inotify.h>

//for using polls (blocking wait in watch)
#include <sys/epoll.h>

//for "close(inotify_descriptor)
#include <unistd.h>

//for using ernno (last error)
#include <cerrno>


//TODO: FSEvent signal and lazy_initialization
class FileWatch : public QObject {
Q_OBJECT
public:
  explicit FileWatch(int inotify_descriptor,
                     int control_pipe_descriptor,
                     QHash<int, QString> &hash_by_descriptor,
                     QObject *parent = nullptr);

  ~FileWatch();



public slots:
  void StartWatch();

signals:
  void WatchFinished();

  void FileWatchError(QString error_description);

  void movedToEvent(unsigned cookie, const QString &name) const;

  void movedFromEvent(unsigned cookie, const QString &name) const;

  void modifiedEvent(const QString &name) const;

  void createdEvent(const QString &name, bool is_directory) const;
protected:


  const static auto WATCH_FLAGS_ = (IN_CREATE | IN_DELETE | IN_DELETE_SELF |
                                    IN_MODIFY | IN_MOVE_SELF | IN_MOVE | IN_DONT_FOLLOW);
  const static auto MAX_INOTIFY_EVENT_SIZE = sizeof(inotify_event) + NAME_MAX + 1;
  const static int MAX_EPOLL_EVENTS = 10;

  void HandleEvents();
  void HandleSingleEvent(const inotify_event& event) const;
  bool FilterByName(const inotify_event& event) const;

  void Initialize();
  void Uninitialize();

private:
  QString GetEventStr(const inotify_event &event) const;

  QHash<int, QString> &hash_by_descriptor_;

  //file descriptors, returned by inotify and epoll initialization functions
  int inotify_descriptor_;
  int epoll_descriptor_;
  int control_pipe_descriptor_;
  int local_errno_;

  bool initialized_;
};


#endif //YDS_FILEWATCH_H
