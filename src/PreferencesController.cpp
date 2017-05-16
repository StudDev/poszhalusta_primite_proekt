#include "PreferencesController.h"
#include <QDir>
#include <QQmlContext>
#include <QDebug>

PreferencesController::PreferencesController(QObject* parent)
    :QObject(parent) {
  if (_settings.allKeys().contains("paths/root"))
    _root_path = QDir::cleanPath(_settings.value("paths/root").toString());

  QDir rootFolder(_root_path);
  if (!rootFolder.exists())
    rootFolder.mkpath(".");

  _settings.setValue("paths/root", _root_path);
  

  _view.rootContext()->setContextProperty("controller", this);

  const QSize windowSize = QSize(640, 480);
  _view.setMinimumSize(windowSize);
  _view.setMaximumSize(windowSize);

  _view.setSource(QUrl(QLatin1String("qrc:/PreferencesView.qml")));
}

void PreferencesController::changeRoot(const QUrl& url) {
  QDir root(url.path());
  root.setFilter(QDir::AllDirs | QDir::NoSymLinks | QDir::NoDotAndDotDot);
  _root_path = root.canonicalPath();
  _settings.setValue("paths/root", _root_path);

  QStringList allFolders;
  recursiveAddFolder(allFolders, root.canonicalPath());

  _settings.setValue("paths/watchingDirs", allFolders);
}

void PreferencesController::recursiveAddFolder(QStringList& foldersList, const QString& rootFolder) {
  QDir root(rootFolder);
  foldersList << root.canonicalPath();
  qDebug() << root.canonicalPath() + " is added to config";

  for (const QString& folderPath: root.entryList(QDir::AllDirs | QDir::NoSymLinks | QDir::NoDotAndDotDot))
    recursiveAddFolder(foldersList, root.filePath(folderPath));
}
