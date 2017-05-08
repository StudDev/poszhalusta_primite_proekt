#ifndef YDS_FILEWATCH_H

#define YDS_FILEWATCH_H

#include <QDebug>

#include <QtCore/QObject>

#include <QString>
#include <QDir>
#include <vector>

//for storing   <directory, watch_descriptor> and
//              <watch_descriptor, directory>
#include <QHash>

//for inotify_init(..)
#include <sys/inotify.h>

//for "close(inotify_descriptor)
#include <unistd.h>

//for using ernno (last error)
#include <cerrno>

//
// Created by alting on 07.05.17.
//

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
                                      IN_MODIFY | IN_MOVE_SELF | IN_MOVE);
    QHash<QString, int> hash_by_directory_;
    QHash<int, QString> hash_by_descriptor_;
    int inotify_descriptor_;
    int local_errno_;
private:
    static const char* TransformQDirToChar(const QDir& arg) const;
    static const unsigned MAX_EVENTS = 4096;
    static const unsigned EVENT_SIZE = (sizeof(inotify_event));
    static const unsigned EVENT_BUF_LEN = (MAX_EVENTS * (EVENT_SIZE + 16));
};


#endif //YDS_FILEWATCH_H
