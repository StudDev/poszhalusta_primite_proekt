#include "Configurable.h"


Configurable::Configurable(ConfigLoader *loader)
  : _config(nullptr),
    _loader(nullptr) {
  loadConfig(loader);
}

Configurable::Configurable()
  : _config(nullptr),
    _loader(nullptr) {

}

void Configurable::changeConfigSlot(QSettings *new_config) {
  handleConfigChange(new_config);
}

Configurable::~Configurable() {

}

QSettings *Configurable::getConfig() const {
  return _config;
}

void Configurable::loadConfig(ConfigLoader *loader) {
  if (_config != nullptr) {
    _loader->unregisterConfigHolder(this);
  }
  _loader = loader;
  _config = _loader->createConfigInstance(organization_name, application_name);
  _loader->registerConfigHolder(this);
  _config->setParent(this);
}

void Configurable::reloadConfig() {
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
