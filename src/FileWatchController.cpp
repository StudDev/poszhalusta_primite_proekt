#include "FileWatchController.h"

FileWatchController::FileWatchController(QObject *parent)
    : QObject(parent) {
  inotify_descriptor_ = inotify_init1(IN_NONBLOCK);
}

FileWatchController::~FileWatchController() {
  close(inotify_descriptor_);
  worker_thread.quit();
  worker_thread.wait();
}

void FileWatchController::AddDirectory(const QDir &arg) {
  qDebug() << "Add Directory";
  if (!arg.exists()) {
    qDebug() << "Directory doesn't exist: " << arg;
    emit WrongArgument();
    return;
  }

  if (hash_by_directory_.contains(arg.canonicalPath())) {
    qDebug() << "Adding already contained directory: " << arg;
    //emit WrongArgument();
    return;
  }

  int temp_watch_descriptor = inotify_add_watch(inotify_descriptor_,
                                                arg.canonicalPath().toLocal8Bit().data(), WATCH_FLAGS_);
  if (-1 == temp_watch_descriptor) {
    local_errno_ = errno;
    qDebug() << "Adding watch failed: unknown error";
    emit FileWatchControllerError(local_errno_);
    return;
  }

  hash_by_directory_.insert(arg.canonicalPath(), temp_watch_descriptor);
  hash_by_descriptor_.insert(temp_watch_descriptor, arg.canonicalPath());
  for (auto &i :hash_by_descriptor_) {
    qDebug() << i.toLocal8Bit();
  }
}

void FileWatchController::RemoveDirectory(const QDir &arg) {

  //check for existence argument directory in list of watched directories
  if (!hash_by_directory_.contains(arg.canonicalPath())) {
    qDebug() << "Removing directory doesn't exists: " << arg;
    emit WrongArgument();
    return;
  }

  //resolving watch descriptor from path
  int temp_watch_descriptor = hash_by_directory_[arg.canonicalPath()];

  //receiving rm_result from removing watch function
  int rm_result = inotify_rm_watch(inotify_descriptor_, temp_watch_descriptor);

  //checking validity
  if (-1 == rm_result) {
    local_errno_ = errno;
    qDebug() << "Removing directory error";
    emit FileWatchControllerError(local_errno_);
    return;
  }

  //remove path from both indexes
  hash_by_descriptor_.remove(temp_watch_descriptor);
  hash_by_directory_.remove(arg.canonicalPath());
}

void FileWatchController::StartWatch() {
  //protection against second run over the current
  if (process_status_) {
    qDebug() << "Warning: watcher already started";
    emit WrongArgument();
    return;
  }

  //initializing pipe
  if (-1 == pipe2(pipe_descriptors_, O_NONBLOCK)) {
    qDebug() << "Error: pipe creation failed";
    local_errno_ = errno;
    emit FileWatchControllerError(local_errno_);
    return;
    //return
  }
  watcher = new FileWatch(inotify_descriptor_, pipe_descriptors_[0], hash_by_descriptor_);
  connect(&worker_thread, &QThread::finished, watcher, &QObject::deleteLater);
  connect(this, &FileWatchController::startWatchingProcess, watcher, &FileWatch::StartWatch);
  connect(watcher, &FileWatch::movedToEvent, this, &FileWatchController::movedToEvent);
  connect(watcher, &FileWatch::movedFromEvent, this, &FileWatchController::movedFromEvent);
  connect(watcher, &FileWatch::createdEvent, this, &FileWatchController::createdEvent);
  connect(watcher, &FileWatch::modifiedEvent, this, &FileWatchController::modifiedEvent);
  watcher->moveToThread(&worker_thread);
  worker_thread.start();
  process_status_ = true;
  emit startWatchingProcess();
}

void FileWatchController::StopWatch() {
  qDebug() << "Trying to stop watch";
  if (!process_status_) {
    qDebug() << "Warning: watcher already stopped";
    emit WrongArgument();
    return;
  }
  char buf = '\n';
  write(pipe_descriptors_[1], &buf, 1);
  process_status_ = false;
}
