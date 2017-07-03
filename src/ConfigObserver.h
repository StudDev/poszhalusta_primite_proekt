#ifndef YDS_CONFIGOBSERVER_H
#define YDS_CONFIGOBSERVER_H


#include <QtCore/QObject>
#include "Configurable.h"
#include <QHash>

class Configurable;

class ConfigObserver : public QObject {
Q_OBJECT
public:
  static ConfigObserver *getInstance() const;

  void addConfigHolder(Configurable *config_holder);

  void removeConfigHolder(Configurable *config_holder);

public slots:

  void pushNotification(QSettings *new_config);

signals:

  void notify(QSettings *new_config);

private:
  static ConfigObserver *_self;

  ConfigObserver(QObject *parent = nullptr);

  QHash<QSettings *, Configurable *> _config_holders;
};


#endif //YDS_CONFIGOBSERVER_H
