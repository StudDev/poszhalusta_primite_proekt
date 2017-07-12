#ifndef YDS_RESTAPICONFIGLOADER_H
#define YDS_RESTAPICONFIGLOADER_H


#include "ConfigLoader.h"

class RestApiConfigLoader : public ConfigLoader {
public:
  RestApiConfigLoader();

  QSettings *createConfigInstance(const QString &organisation, const QString &application) override;

};


#endif //YDS_RESTAPICONFIGLOADER_H
