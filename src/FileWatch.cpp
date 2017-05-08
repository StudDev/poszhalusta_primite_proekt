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
}


FileWatch::~FileWatch() {
    close(inotify_descriptor_);
}

void FileWatch::AddDirectory(const QDir &arg) {
    if(!arg.exists()) {
        qDebug() << "Directory doesn't exist: " << arg.canonicalPath();
        emit FileWatchWrongArgument();
        return;
    }
    if(hash_by_directory_.contains(arg.canonicalPath())) {
        qDebug() << "Adding contained directory: " << arg.canonicalPath();
        emit FileWatchDirectoryAlreadyAdded();
        return;
    }
    int temp_watch_descriptor = inotify_add_watch(inotify_descriptor_, TransformQDirToChar(arg), WATCH_FLAGS_);
    if(-1 == temp_watch_descriptor) {
        local_errno_ = errno;
        qDebug() << "Adding watch failed";
        emit FileWatchError();
        return;
    }
    hash_by_directory_.insert(arg.canonicalPath(), temp_watch_descriptor);
    hash_by_descriptor_.insert(temp_watch_descriptor, arg.canonicalPath());
}

void FileWatch::RemoveDirectory(const QDir &arg) {

    //check for existance argument directory in list of watched directories
    if(!hash_by_directory_.contains(arg.canonicalPath())) {
        qDebug() << "Removing directory doesn't exists: " << arg.canonicalPath();
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
    
}

const char *FileWatch::TransformQDirToChar(const QDir &arg) const {
    return arg.canonicalPath().toStdString().c_str();
}
