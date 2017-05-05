#include <QObject>
#include <QDebug>
#include <QString>
#include <QQuickView>
#include <QQmlContext>

class PreferencesController : public QObject {
    Q_OBJECT

public:
    PreferencesController();
        
public slots:
    void log(const QString& msg);

private:

    QQuickView view;
};
