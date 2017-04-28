#include <QObject>
#include <QDebug>
#include <QString>

class PreferencesController : public QObject {
    Q_OBJECT
        
public slots:
    void log(const QString& msg);
};
