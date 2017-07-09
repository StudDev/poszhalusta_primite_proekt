#include "PreferencesController.h"
#include "PathConfigLoader.h"
#include <QDir>
#include <QQmlContext>
#include <QDebug>

namespace {
  constexpr unsigned WINDOW_WIDTH = 640;
  constexpr unsigned WINDOW_HEIGHT = 480;
}

PreferencesController::PreferencesController(QObject *parent)
    : Configurable{parent, new PathConfigLoader{}} {
  loadConfigVariables();
  QDir rootFolder(_conf_vars.local_root);
  if (!rootFolder.exists()) {
    rootFolder.mkpath(".");
  }
  confSetValue("localroot", _conf_vars.local_root);
  confSetValue("remoteroot", _conf_vars.remote_root);
  _view.rootContext()->setContextProperty("controller", this);
  const QSize windowSize = QSize(WINDOW_WIDTH, WINDOW_HEIGHT);
  _view.setMinimumSize(windowSize);
  _view.setMaximumSize(windowSize);
  _view.setSource(QUrl(QLatin1String("qrc:/PreferencesView.qml")));
}

void PreferencesController::changeRoot(const QUrl &url) {
  QDir root(url.path());
  root.setFilter(QDir::AllDirs | QDir::NoSymLinks | QDir::NoDotAndDotDot);
  _conf_vars.local_root = root.canonicalPath();
  confSetValue("localroot", _conf_vars.local_root);
  QStringList allFolders;
  recursiveAddFolder(allFolders, root.canonicalPath());
  confSetValue("watchingDirs", allFolders);
}

void PreferencesController::recursiveAddFolder(QStringList &foldersList, const QString &rootFolder) {
  QDir root(rootFolder);
  foldersList << root.canonicalPath();
  qDebug() << root.canonicalPath() + " is added to config";

  for (const QString &folderPath: root.entryList(QDir::AllDirs | QDir::NoSymLinks | QDir::NoDotAndDotDot))
    recursiveAddFolder(foldersList, root.filePath(folderPath));
}

void PreferencesController::show() { _view.show(); }

QString PreferencesController::getRootPath() const { return _conf_vars.local_root; }

QQuickView *PreferencesController::getView() { return &_view; }

const QQuickView *PreferencesController::getView() const { return &_view; }

void PreferencesController::log(const QString &msg) const { qDebug() << msg; }

void PreferencesController::handleConfigChange(QSettings *new_config) {
  auto oldVars = _conf_vars;
  loadConfigVariables();
  if (_conf_vars.local_root != oldVars.local_root) {
    changeRoot(QUrl{_conf_vars.local_root});
  }
}

void PreferencesController::loadConfigVariables() {
  _conf_vars.watching_dirs = getConfValue("watchingDirs", _conf_vars.watching_dirs).toStringList();
  _conf_vars.local_root = getConfValue("localroot", _conf_vars.local_root).toString();
}
