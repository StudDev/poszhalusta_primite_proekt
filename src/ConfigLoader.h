#ifndef YDS_CONFIGLOADER_H
#define YDS_CONFIGLOADER_H


#include <QtCore/QSettings>
#include "Configurable.h"
#include "ConfigObserver.h"

class Configurable;

class ConfigObserver;


class ConfigLoader {
public:

  ConfigLoader(const QString &conf_branch);

  virtual QSettings *createConfigInstance(const QString &organisation = default_org_name,
                                          const QString &application = default_app_name) = 0;

  void registerConfigHolder(Configurable *config_holder);

  void unregisterConfigHolder(Configurable *config_holder);

  const QString &getConfigBranch() const;


  virtual  ~ConfigLoader();

private:
  static QString default_org_name;
  static QString default_app_name;
  QString _config_branch;
  ConfigObserver *_observer;
};


#endif //YDS_CONFIGLOADER_H
