#include <QObject>
#include <QQuickView>
#include <QString>
#include <QSettings>
#include <QtCore/QDir>
#include "Configurable.h"

class PreferencesController : public Configurable {
Q_OBJECT
protected:
  void handleConfigChange(QSettings *new_config) override;

  void loadConfigVariables() override;

private:

public:
  PreferencesController(QObject *parent = nullptr);

  void show();

  static void recursiveAddFolder(QStringList &foldersList, const QString &rootFolder);

  Q_INVOKABLE QString getRootPath() const;

  QQuickView *getView();

  const QQuickView *getView() const;

public slots:

  void log(const QString &msg) const;

  void changeRoot(const QUrl &url);

private:
  struct {
    QString local_root{QDir::cleanPath(QDir::homePath() + "/Yandex.Disk")};
    QString remote_root{"disk:"};
    QStringList watching_dirs;
  } _conf_vars;
  QQuickView _view;
};
