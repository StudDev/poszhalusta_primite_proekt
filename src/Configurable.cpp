#include "Configurable.h"
#include <QDebug>

const QString Configurable::application_name = "yds";
const QString Configurable::organization_name = "TopCodersTeam";

Configurable::Configurable(QObject *parent, ConfigLoader *loader)
    : QObject{parent},
      _config{nullptr},
      _loader{nullptr} {
  loadConfig(loader);
}

Configurable::Configurable(QObject *parent)
    : QObject{parent},
      _config{nullptr},
      _loader{nullptr} {
}

void Configurable::changeConfigSlot(QSettings *new_config) {
  if (getConfig() != new_config) {
    handleConfigChange(new_config);
  }
}

Configurable::~Configurable() {
  if (_loader != nullptr) {
    _loader->unregisterConfigHolder(this);
  }
  delete _loader;
}

QSettings *Configurable::getConfig() const {
  return _config;
}

void Configurable::loadConfig(ConfigLoader *customLoader) {
  if (_config != nullptr) {
    _config->deleteLater();
  }
  if (customLoader != nullptr) {
    if (_loader != nullptr) {
      delete _loader;
    }
    _loader = customLoader;
  }
  _config = _loader->createConfigInstance(organization_name, application_name);
  _config->setParent(this);
  _loader->registerConfigHolder(this);
}

void Configurable::reloadConfig(ConfigLoader *customLoader) {
  delete _loader;
  _loader = customLoader;
  _config->deleteLater();
  _config = _loader->createConfigInstance(organization_name, application_name);
  _config->setParent(this);
}

void Configurable::confSetValue(const QString &key, const QVariant &value) {
  _config->setValue(key, value);
  emit configChanged(_config);
}

void Configurable::confRemoveKey(const QString &key) {
  _config->remove(key);
  emit configChanged(_config);
}

bool Configurable::confContains(const QString &key) {
  return _config->contains(key);
}

QVariant Configurable::getConfValue(const QString &key, const QVariant &defaultValue) const {
  return _config->value(key, defaultValue);
}

void Configurable::handleConfigChange(QSettings *new_config) {
  throw std::runtime_error("Configurable::handleConfigChange Called");
}
