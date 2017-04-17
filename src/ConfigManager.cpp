#include "ConfigManager.h"
#include <QFile>
#include <QtCore/QUuid>
#include <QtCore/QJsonDocument>

ConfigManager::ConfigManager(const QString &config_path)
  :_config_path(config_path)
{
  if(!QFile::exists(config_path)){
    setAppId("20beb8f54f66490fa4f21b42f7af7145");
    genUuid();
    saveConfig();
    return;
  }
  reloadConfig();
}

QString ConfigManager::getUuid() {
  QJsonValue uuid{ _config["UUID"] };
  if(uuid.type() == QJsonValue::Undefined){
    genUuid();
  }
  return uuid.toString();
}

QString ConfigManager::getAppId() const {
  return _config["AppID"].toString();

}

QString ConfigManager::getDeviceName() const {
  return _config["DeviceName"].toString();
}

void ConfigManager::genUuid() {
  auto uuid = QUuid::createUuid().toString();
  _config.insert("UUID", uuid);
}

void ConfigManager::setDeviceName(const QString &device_name) {
  _config["DeviceName"] = device_name;
}

void ConfigManager::setAppId(const QString &app_id) {
  _config["AppID"] = app_id;
}

void ConfigManager::reloadConfig(const QString &new_config_path) {
  if(!new_config_path.isEmpty()){
    _config_path = new_config_path;
  }
  QFile conf_file{ _config_path.toUtf8() };
  conf_file.open(QIODevice::ReadOnly);
  QString file_content{ conf_file.readAll() };
  conf_file.close();
  QJsonDocument document{ QJsonDocument::fromJson(file_content.toUtf8()) };
  _config = document.object();
}

void ConfigManager::saveConfig() {
  QJsonDocument document{ _config };
  QFile conf_file{ _config_path.toUtf8() };
  conf_file.open(QFile::WriteOnly);
  conf_file.write(document.toJson());
  conf_file.close();
}
