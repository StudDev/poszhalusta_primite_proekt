#include "PreferencesController.h"

PreferencesController::PreferencesController() {
    view.rootContext()->setContextProperty("controller", this);
    const QSize windowSize = QSize(640, 480);
    view.setMinimumSize(windowSize);
    view.setMaximumSize(windowSize);

    view.setSource(QUrl(QLatin1String("qrc:/PreferencesView.qml")));
    view.show();
}

void PreferencesController::log(const QString& msg) {
    qDebug() << msg;
}
