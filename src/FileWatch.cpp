//
// Created by alting on 07.05.17.
//

#include "FileWatch.h"

FileWatch::FileWatch(int inotify_descriptor,
                     int control_pipe_descriptor,
                     QHash<int, QString> &hash_by_descriptor,
                     QThread *parent) :
        QThread(parent),
        hash_by_descriptor_(hash_by_descriptor),
        inotify_descriptor_(inotify_descriptor),
        control_pipe_descriptor_(control_pipe_descriptor) { }


FileWatch::~FileWatch() {
  qDebug() << "Watcher deleted";
}

bool FileWatch::initialize() {
  if(initialized_ == true)
    return true;

  epoll_descriptor_ = epoll_create1(0);
  if (-1 == epoll_descriptor_) {
    local_errno_ = errno;
    emit FileWatchInitError(local_errno_);
    return false;
  }

  epoll_event inotify_ev;
  epoll_event pipe_ev;

  inotify_ev.events = EPOLLIN;
  pipe_ev.events = EPOLLIN;

  inotify_ev.data.fd = inotify_descriptor_;
  pipe_ev.data.fd = control_pipe_descriptor_;

  if (-1 == epoll_ctl(epoll_descriptor_, EPOLL_CTL_ADD, inotify_descriptor_, &inotify_ev)) {
    qDebug() << "epoll_ctl inotify addition error";
    emit FileWatchError();
    return false;
  }

  if (-1 == epoll_ctl(epoll_descriptor_, EPOLL_CTL_ADD, pipe_ev.data.fd, &pipe_ev)) {
    qDebug() << "epoll_ctl pipe addition error";
    emit FileWatchError();
    return false;
  }

  initialized_ = true;
  return true;
}

void FileWatch::uninitialize() {

}

void FileWatch::run() {
  epoll_event events[MAX_EPOLL_EVENTS];

  bool exit_flag = false;
  while (true) {
    int events_available = epoll_wait(epoll_descriptor_, events, MAX_EPOLL_EVENTS, -1);

    if (-1 == events_available) {

      qDebug() << "epoll wait failed";
      return;
      emit FileWatchError();
    }
    for (int i = 0; i < events_available; ++i) {
      if (events[i].data.fd == inotify_descriptor_) {
        HandleEvents();
      }

      if (events[i].data.fd == control_pipe_descriptor_) {
        char temp_symbol;
        while (read(STDIN_FILENO, &temp_symbol, 1) > 0 && temp_symbol != '\n') {
          continue;
        }
        exit_flag = true;
        qDebug() << "Listening finished";
        emit watchFinished();
        break;
      }
    }

    if (exit_flag) {
      break;
    }
  }
}

void FileWatch::HandleEvents() {
  char buf[MAX_INOTIFY_EVENT_SIZE];
  ssize_t read_len;

  while (true) {
    read_len = read(inotify_descriptor_, buf, sizeof(buf));

    //eagain means "no data available" so we skip this error
    if (-1 == read_len && errno != EAGAIN) {
      qDebug() << "Inotify descriptor read failure";
      emit FileWatchError();
      return;
    }

    //we read the entire buffer
    if (read_len <= 0) {
      break;
    }

    const inotify_event *event;

    void *unread_events_ptr;

    //we can advance unread_events_ptr on event->len, because is happens only after the first iteration
    //so at the moment of advancing we have already initialized "event->len"
    for (unread_events_ptr = buf; unread_events_ptr < buf + read_len;
         unread_events_ptr += sizeof(inotify_event) + event->len) {

      event = static_cast<inotify_event *>(unread_events_ptr);

      //filter to avoid unix - temporary files beginning with "."
      if (event->len) {
        if(FilterByName(*event))
          continue;
      }

      HandleSingleEvent(*event);
    }
  }
}
//this function is currently in debug version: later all qDebug() calls will be replaced with
//signals in special form
void FileWatch::HandleSingleEvent(const inotify_event &event) const {
  if (event.mask & IN_CREATE) {
    qDebug() << "IN_CREATE: ";
  }
  if (event.mask & IN_DELETE) {
    qDebug() << "IN_DELETE: ";
  }
  if (event.mask & IN_DELETE_SELF) {
    qDebug() << "IN_DELETE_SELF: ";
  }
  if (event.mask & IN_MODIFY) {
    qDebug() << "IN_MODIFY: ";
  }
  if (event.mask & IN_MOVE_SELF) {
    qDebug() << "IN_MOVE_SELF: ";
  }
  if (event.mask & IN_MOVED_FROM) {
    qDebug() << "Cookie: " << event.cookie;
    qDebug() << "IN_MOVED_FROM: ";
  }
  if (event.mask & IN_MOVED_TO) {
    qDebug() << "Cookie: " << event.cookie;
    qDebug() << "IN_MOVED_TO: ";
  }

  qDebug() << hash_by_descriptor_.value(event.wd);

  if (event.len) {
    qDebug() << event.name;
  }

  qDebug() << "-------------------------------------------------------";
}

bool FileWatch::FilterByName(const inotify_event &event) const {
  QString name = event.name;
  QRegExp check_reg("^(?![.])(?!.*[-_.]$).+");
  if (!check_reg.exactMatch(name))
    return true;
}
