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
class FileWatch : public QThread {
Q_OBJECT
public:
  explicit FileWatch(int inotify_descriptor,
                     int control_pipe_descriptor,
                     QHash<int, QString> &hash_by_descriptor,
                     QThread *parent = nullptr);

  ~FileWatch();

  bool initialize();
  void uninitialize();

public slots:
  void run() Q_DECL_OVERRIDE;

signals:
  void watchFinished();

  void FileWatchInitError(int error_code);

  void FileWatchError();

  void FileWatchWrongArgument();

  void FileWatchDirectoryAlreadyAdded();

protected:
  const static auto WATCH_FLAGS_ = (IN_CREATE | IN_DELETE | IN_DELETE_SELF |
                                    IN_MODIFY | IN_MOVE_SELF | IN_MOVE | IN_DONT_FOLLOW);
  const static auto MAX_INOTIFY_EVENT_SIZE = sizeof(inotify_event) + NAME_MAX + 1;
  const static int MAX_EPOLL_EVENTS = 10;

  void HandleEvents();


  //we use 2 hashtables to have fast access both by directory and filewatch descriptor
  //this could be a single boost::bimap
  QHash<int, QString> &hash_by_descriptor_;

  //file descriptors, returned by inotify and epoll initialization functions
  int inotify_descriptor_;
  int epoll_descriptor_;
  int control_pipe_descriptor_;
  int local_errno_;

  bool initialized_ = false;
};


#endif //YDS_FILEWATCH_H
