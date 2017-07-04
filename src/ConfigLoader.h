#ifndef YDS_CONFIGLOADER_H
#define YDS_CONFIGLOADER_H


#include <QtCore/QSettings>
#include "Configurable.h"
#include "ConfigObserver.h"

class Configurable;

class ConfigObserver;

class ConfigLoader {
public:
  ConfigLoader();

  virtual QSettings *createConfigInstance(const QString &organisation, const QString &application) = 0;

  void registerConfigHolder(Configurable *config_holder);

  void unregisterConfigHolder(Configurable *config_holder);

  virtual  ~ConfigLoader();

private:
  ConfigObserver *_observer;
};


#endif //YDS_CONFIGLOADER_H
