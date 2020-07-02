#include <cstdint>
#include <curlpp/Easy.hpp>
#include <curlpp/Options.hpp>
#include <curlpp/cURLpp.hpp>
#include <iostream>
#include <json/json.h>
#include <list>
#include <map>
#include <memory>
#include <sstream>
#include <string>

class GoogleTranslator
{
private:
    /* data */
    Json::Value app_config_m;

public:
    GoogleTranslator(Json::Value app_config);

    ~GoogleTranslator();

    Json::Value translateContent(std::string content);
    std::map<std::string, std::string> listAllAvailableLanguages();
};
