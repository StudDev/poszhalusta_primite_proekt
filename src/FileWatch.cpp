//
// Created by alting on 07.05.17.
//

#include "FileWatch.h"

FileWatch::FileWatch(QObject *parent) : QObject(parent) {
    inotify_descriptor_ = inotify_init1(IN_NONBLOCK);
    if(-1 == inotify_descriptor_) {
        local_errno_ = errno;
        emit FileWatchInitError();
    }

    epoll_descriptor_ = epoll_create1(0);
    if(-1 == epoll_descriptor_) {
        local_errno_ = errno;
        emit FileWatchInitError();
    }
}


FileWatch::~FileWatch() {
    close(inotify_descriptor_);
    close(epoll_descriptor_);
}

void FileWatch::AddDirectory(const QDir &arg) {
    if(!arg.exists()) {
        qDebug() << "Directory doesn't exist: " << arg;
        emit FileWatchWrongArgument();
        return;
    }

    if(hash_by_directory_.contains(arg.canonicalPath())) {
        qDebug() << "Adding already contained directory: " << arg;
        emit FileWatchDirectoryAlreadyAdded();
        return;
    }

    int temp_watch_descriptor = inotify_add_watch(inotify_descriptor_,
                                                  arg.canonicalPath().toStdString().c_str(), WATCH_FLAGS_);
    if(-1 == temp_watch_descriptor) {
        local_errno_ = errno;
        qDebug() << "Adding watch failed: unknown error";
        emit FileWatchError();
        return;
    }

    hash_by_directory_.insert(arg.canonicalPath(), temp_watch_descriptor);
    hash_by_descriptor_.insert(temp_watch_descriptor, arg.canonicalPath());
}

void FileWatch::RemoveDirectory(const QDir &arg) {

    //check for existence argument directory in list of watched directories
    if(!hash_by_directory_.contains(arg.canonicalPath())) {
        qDebug() << "Removing directory doesn't exists: " << arg;
        emit FileWatchWrongArgument();
        return;
    }

    //resolving watch descriptor from path
    int temp_watch_descriptor = hash_by_directory_[arg.canonicalPath()];

    //receiving rm_result from removing watch function
    int rm_result = inotify_rm_watch(inotify_descriptor_, temp_watch_descriptor);

    //checking validity
    if(-1 == rm_result) {
        local_errno_ = errno;
        qDebug() << "Removing directory error";
        emit FileWatchError();
        return;
    }

    //remove path from both indexes
    hash_by_descriptor_.remove(temp_watch_descriptor);
    hash_by_directory_.remove(arg.canonicalPath());

}

void FileWatch::StartWatch() {
    epoll_event inotify_ev;
    epoll_event console_ev;
    epoll_event events[MAX_EPOLL_EVENTS];

    inotify_ev.events = EPOLLIN;
    console_ev.events = EPOLLIN;

    inotify_ev.data.fd = inotify_descriptor_;
    console_ev.data.fd = STDIN_FILENO;

    if(-1 == epoll_ctl(epoll_descriptor_, EPOLL_CTL_ADD, inotify_descriptor_, &inotify_ev)) {
        qDebug() << "epoll add error";
        emit FileWatchError();
    }
    if(-1 == epoll_ctl(epoll_descriptor_, EPOLL_CTL_ADD, console_ev.data.fd, &console_ev)) {
        qDebug() << "epoll add error";
        emit FileWatchError();
    }

    qDebug() << "Listening for events";

    bool exit_flag = false;
    while(true) {
        int events_available = epoll_wait(epoll_descriptor_, events, MAX_EPOLL_EVENTS, -1);
        if(-1 == events_available) {
            qDebug() << "epoll wait failed";
            emit FileWatchError();
        }
        for(int i = 0; i < events_available; ++i) {
            if(events[i].data.fd == STDIN_FILENO) {
                char temp_symbol;
                while(read(STDIN_FILENO, &temp_symbol, 1) > 0 && temp_symbol != '\n') {
                    continue;
                }
                exit_flag = true;
                qDebug() << "Listening finished";
                break;
            }

            if(events[i].data.fd == inotify_descriptor_) {
                HandleEvents();
            }
        }
        if(exit_flag) {
            break;
        }
    }
}

void FileWatch::HandleEvents() {
    char buf[MAX_INOTIFY_EVENT_SIZE];
    ssize_t read_len;

    while(true) {
        read_len = read(inotify_descriptor_, buf, sizeof(buf));

        //eagain means "no data available" so we skip this error
        if(-1 == read_len && errno != EAGAIN) {
            qDebug() << "Inotify descriptor read failure";
            emit FileWatchError();
            return;
        }

        //we read the entire buffer
        if(read_len <= 0) {
            break;
        }

        const inotify_event* event;

        void* unread_events_ptr;

        //we can advance unread_events_ptr on event->len, because is happens only after the first iteration
        //so at that moment we have initialized "event->len"
        for(unread_events_ptr = buf; unread_events_ptr < buf + read_len;
            unread_events_ptr += sizeof(inotify_event) + event->len) {

            event = static_cast<inotify_event*>(unread_events_ptr);


            //filter to avoid unix - temporary files beginning with "."
            if(event->len) {
                QString name = event->name;
                QRegExp check_reg("^(?![.])(?!.*[-_.]$).+");
                if(!check_reg.exactMatch(name))
                    continue;
            }


            if(event->mask & IN_CREATE) {
                qDebug() << "IN_CREATE: ";
            }
            if(event->mask & IN_DELETE) {
                qDebug() << "IN_DELETE: ";
            }
            if(event->mask & IN_DELETE_SELF) {
                qDebug() << "IN_DELETE_SELF: ";
            }
            if(event->mask & IN_MODIFY) {
                qDebug() << "IN_MODIFY: ";
            }
            if(event->mask & IN_MOVE_SELF) {
                qDebug() << "IN_MOVE_SELF: ";
            }
            if(event->mask & IN_MOVED_FROM) {
                qDebug() << "IN_MOVED_FROM: ";
            }
            if(event->mask & IN_MOVED_TO) {
                qDebug() << "IN_MOVED_TO: ";
            }

            qDebug() << hash_by_descriptor_.value(event->wd);

            if(event->len) {
                qDebug() << event->name;
            }

            qDebug() << "-------------------------------------------------------";
        }
    }
}

