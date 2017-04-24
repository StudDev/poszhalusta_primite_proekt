#include <QObject>
#include <QDateTime>
#include <QDebug>
#include <QString>

class MainView : public QObject {
    Q_OBJECT
public:
    Q_INVOKABLE QDateTime getCurrentDateTime() const;

public slots:
    void log(const QString& msg) {
    	qDebug() << msg;
    }

};