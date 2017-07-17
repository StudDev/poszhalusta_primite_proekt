#ifndef YDS_CONFIGOBSERVER_H
#define YDS_CONFIGOBSERVER_H


#include <QtCore/QObject>
#include "Configurable.h"
#include <QHash>

class Configurable;

class ConfigObserver : public QObject {
Q_OBJECT
public:

  static ConfigObserver *getInstance();

  void addConfigHolder(Configurable *config_holder);

  void removeConfigHolder(Configurable *config_holder);

  virtual  ~ConfigObserver();

public slots:

  void pushNotification(const QString &new_config);

signals:

  void notify(const QString &new_config);

private:
  ConfigObserver(QObject *parent = nullptr);

  static ConfigObserver *_self;

  QHash<QSettings *, Configurable *> _config_holders;
};


#endif //YDS_CONFIGOBSERVER_H
