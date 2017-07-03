#include "TokenConfigLoader.h"

QSettings *TokenConfigLoader::createConfigInstance() {
  return new TokenConfig();
}
