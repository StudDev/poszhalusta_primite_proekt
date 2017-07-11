//
// Created by alting on 10.05.17.
//

#ifndef YDS_FILEWATCHCONTROLLER_H
#define YDS_FILEWATCHCONTROLLER_H

#include "FileWatch.h"

#include <QtCore/QObject>
#include <QDir>
#include <QHash>
#include <QThread>

#include <QDebug>

//for inotify_init(..)
#include <sys/inotify.h>

//obtain O_* constant definitions
#include <fcntl.h>

//for "close(inotify_descriptor)
#include <unistd.h>

//for using ernno (last error)
#include <cerrno>


//TODO: this class
class FileWatchController : public QObject {
Q_OBJECT

public:
  explicit FileWatchController(QObject *parent = nullptr);

  ~FileWatchController();

  void AddDirectory(const QDir &arg);

  void RemoveDirectory(const QDir &arg);

  void StopWatch();

  void StartWatch();

signals:

  void FileWatchControllerError(int error);

  void WrongArgument();

private:
  bool initialize();

  const static auto WATCH_FLAGS_ = (IN_CREATE | IN_DELETE | IN_DELETE_SELF |
                                    IN_MODIFY | IN_MOVE_SELF | IN_MOVE | IN_DONT_FOLLOW);
  const static auto MAX_INOTIFY_EVENT_SIZE = sizeof(inotify_event) + NAME_MAX + 1;


  FileWatch *watcher = nullptr;

  //we use 2 hashtables to have fast access both by directory and filewatch descriptor
  //this provides a simple safety for operations "add directory" "remove directory"
  QHash<QString, int> hash_by_directory_;
  QHash<int, QString> hash_by_descriptor_;

  //file descriptors, returned by inotify initialization functions
  int inotify_descriptor_;
  int local_errno_;
  int pipe_descriptors_[2];
  bool process_status_ = false;
  bool initialized_ = false;

};


#endif //YDS_FILEWATCHCONTROLLER_H
