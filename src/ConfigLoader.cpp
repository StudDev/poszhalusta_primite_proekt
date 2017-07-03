#include "ConfigLoader.h"


void ConfigLoader::registerConfigHolder(Configurable *config_holder) {
  _observer->addConfigHolder(config_holder);
}

void ConfigLoader::unregisterConfigHolder(Configurable *config_holder) {
  _observer->removeConfigHolder(config_holder);
}

ConfigLoader::ConfigLoader()
  : _observer{ConfigObserver::getInstance()} {
}

ConfigLoader::~ConfigLoader() {

}
