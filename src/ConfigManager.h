#ifndef YDS_CONFIGMANAGER_H
#define YDS_CONFIGMANAGER_H


#include <QtCore/QString>
#include <QtCore/QJsonObject>


//TODO: implement makeNewConfig() method for GUI
class ConfigManager {
public:
  ConfigManager(const QString &config_path);
  QString getUuid();
  QString getAppId() const;
  QString getDeviceName() const;
  void genUuid();
  void setDeviceName(const QString &device_name);
  void setAppId(const QString &app_id);
  void reloadConfig(const QString &new_config_path = "");
  void saveConfig();
private:
  QString _config_path;
  QJsonObject _config;
};


#endif //YDS_CONFIGMANAGER_H
