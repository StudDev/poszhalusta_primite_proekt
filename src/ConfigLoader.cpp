#include "ConfigLoader.h"
#include <QDebug>

QString ConfigLoader::default_org_name = "TopProgrammersTeam";
QString ConfigLoader::default_app_name = "yds";

void ConfigLoader::registerConfigHolder(Configurable *config_holder) {
  _observer->addConfigHolder(config_holder);
}

void ConfigLoader::unregisterConfigHolder(Configurable *config_holder) {
  _observer->removeConfigHolder(config_holder);
}

ConfigLoader::ConfigLoader(const QString &config_branch)
    : _observer{ConfigObserver::getInstance()},
      _config_branch{config_branch} {
  qDebug() << "loader created";
}

ConfigLoader::~ConfigLoader() {
  qDebug() << "loader deleted";
}

const QString &ConfigLoader::getConfigBranch() const {
  return _config_branch;
}


