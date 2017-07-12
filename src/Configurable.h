#ifndef YDS_CONFIGURABLE_H
#define YDS_CONFIGURABLE_H

#include <QObject>
#include <QtCore/QSettings>
#include "ConfigLoader.h"

class ConfigLoader;

class Configurable : public QObject {
Q_OBJECT
public:
  Configurable(QObject *parent = nullptr);

  Configurable(QObject *parent, ConfigLoader *loader);

  void loadConfig(ConfigLoader *customLoader);


  void reloadConfig(ConfigLoader *customLoader);

  QSettings *getConfig() const;

  void confSetValue(const QString &key, const QVariant &value);

  void confRemoveKey(const QString &key);

  bool confContains(const QString &key);

  QVariant getConfValue(const QString &key, const QVariant &defaultValue = QVariant()) const;

  virtual ~Configurable();

public slots:

  void changeConfigSlot(QSettings *new_config);

signals:

  void configChanged(QSettings *_config);

protected:
  virtual void handleConfigChange(QSettings *new_config);

  virtual void loadConfigVariables() = 0;

private:
  static const QString application_name;
  static const QString organization_name;
  QSettings *_config;
  ConfigLoader *_loader;
};



#endif //YDS_CONFIGURABLE_H
