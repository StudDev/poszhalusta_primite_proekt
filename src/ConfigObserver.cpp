#include "ConfigObserver.h"
#include <QDebug>

ConfigObserver *ConfigObserver::_self = nullptr;

void ConfigObserver::pushNotification(const QString &config_branch) {
  emit notify(config_branch);
}

void ConfigObserver::addConfigHolder(Configurable *config_holder) {
  auto *config_instance = config_holder->getConfig();
  auto self_instance = getInstance();
  auto &config_branch = config_holder->getConfigBranch();
  self_instance->_config_holders.insert(config_instance, config_holder);
  QObject::connect(config_holder, &Configurable::configChanged, self_instance, &ConfigObserver::pushNotification);
  QObject::connect(self_instance, &ConfigObserver::notify, config_holder, &Configurable::changeConfigSlot);
}

void ConfigObserver::removeConfigHolder(Configurable *config_holder) {
  auto *config = config_holder->getConfig();
  auto *self_instance = getInstance();
  self_instance->_config_holders.remove(config);
  QObject::disconnect(config_holder, &Configurable::configChanged, self_instance, &ConfigObserver::pushNotification);
  QObject::disconnect(self_instance, &ConfigObserver::notify, config_holder, &Configurable::changeConfigSlot);
  if (self_instance->_config_holders.isEmpty()) {
    _self->deleteLater();
    _self = nullptr;
  }
}

ConfigObserver *ConfigObserver::getInstance() {
  if (_self == nullptr) {
    _self = new ConfigObserver();
  } else {
    return _self;
  }
}

ConfigObserver::ConfigObserver(QObject *parent)
    : QObject{parent} {
}

ConfigObserver::~ConfigObserver() {
  _self->deleteLater();
  _self = nullptr;
}
