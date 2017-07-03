#ifndef YDS_TOKENCONFIGLOADER_H
#define YDS_TOKENCONFIGLOADER_H

#include "ConfigLoader.h
class TokenConfigLoader : public ConfigLoader {
public:
  QSettings* createConfigInstance() override ;
};


#endif //YDS_TOKENCONFIGLOADER_H
