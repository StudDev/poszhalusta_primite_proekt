#include "RestApiConfigLoader.h"

QSettings *RestApiConfigLoader::createConfigInstance(const QString &organisation, const QString &application) {
  auto *rest_api_settings = new QSettings(organisation, application);
  rest_api_settings->beginGroup(getConfigBranch());
  return rest_api_settings;
}

RestApiConfigLoader::RestApiConfigLoader()
    : ConfigLoader{"api_settings/"} {
}
