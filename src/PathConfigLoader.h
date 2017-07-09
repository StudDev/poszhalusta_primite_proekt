#ifndef YDS_PATHCONFIGLOADER_H
#define YDS_PATHCONFIGLOADER_H


#include "ConfigLoader.h"

class PathConfigLoader : public ConfigLoader {
public:
  PathConfigLoader();

  QSettings *createConfigInstance(const QString &organisation, const QString &application) override;
};


#endif //YDS_PATHCONFIGLOADER_H
