#include "TokenConfigLoader.h"

QSettings *TokenConfigLoader::createConfigInstance(const QString &organisation, const QString &application) {
  auto *token_settings = new QSettings(organisation, application);
  token_settings->beginGroup(getConfigBranch());
  return token_settings;
}

TokenConfigLoader::TokenConfigLoader()
    : ConfigLoader{"token/"} {
}
