#include <QObject>
#include <QQuickView>
#include <QString>
#include <QSettings>
#include <QtCore/QDir>

class PreferencesController : public QObject {
  Q_OBJECT

 public:
  PreferencesController(QObject* parent = nullptr);

  void show() { _view.show(); }
  static void recursiveAddFolder(QStringList& foldersList, const QString& rootFolder);

  Q_INVOKABLE QString getRootPath() const { return _root_path; }
  QSettings* getSettings() { return &_settings; }
  QQuickView* getView() { return &_view; }
  const QQuickView* getView() const { return &_view; }
 public slots:
  void log(const QString& msg) const { qDebug() << msg; }
  void changeRoot(const QUrl& url);

 private:
  QString _root_path = QDir::cleanPath(QDir::homePath() + "/Yandex.Disk");
  QSettings _settings;
  QQuickView _view;
};
