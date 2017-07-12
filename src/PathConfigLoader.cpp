#include "PathConfigLoader.h"

QSettings *PathConfigLoader::createConfigInstance(const QString &organisation, const QString &application) {
  auto *path_settings = new QSettings(organisation, application);
  path_settings->beginGroup(getConfigBranch());
  return path_settings;
}

PathConfigLoader::PathConfigLoader()
    : ConfigLoader{"path/"} {
}
