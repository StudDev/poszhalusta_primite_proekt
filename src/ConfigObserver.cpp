#include "ConfigObserver.h"

void ConfigObserver::pushNotification(QSettings *new_config) {
  emit notify(new_config);
}

void ConfigObserver::addConfigHolder(Configurable *config_holder) {
  _config_holders.insert(config_holder->getConfig(), config_holder);
  QObject::connect(config_holder, &Configurable::configChanged, &ConfigObserver::pushNotification);
  QObject::connect(this, &ConfigObserver::notify, config_holder, &Configurable::changeConfigSlot);
}

void ConfigObserver::removeConfigHolder(Configurable *config_holder) {
  _config_holders.remove(config_holder->getConfig());
}

ConfigObserver *ConfigObserver::getInstance() const {
  if (_self == nullptr) {
    _self = new ConfigObserver();
  } else {
    return _self;
  }
}
