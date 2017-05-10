#ifndef YDS_FILEWATCH_H

#define YDS_FILEWATCH_H

#include <QDebug>

#include <QtCore/QObject>

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


//TODO: FSEvent signal
class FileWatch : public QObject {
    Q_OBJECT
public:
    explicit FileWatch(QObject *parent = nullptr);
    ~FileWatch();
    //void AddRecursiveDirectory(QDir& arg);
    void AddDirectory(const QDir& arg);
    //void AddFile(QDir& arg);
    void RemoveDirectory(const QDir& arg);
    void StartWatch();
signals:
    void FileWatchInitError();
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
    QHash<QString, int> hash_by_directory_;
    QHash<int, QString> hash_by_descriptor_;

    //file descriptors, returned by inotify and epoll initialization functions
    int inotify_descriptor_;
    int epoll_descriptor_;
    int local_errno_;


};


#endif //YDS_FILEWATCH_H
