#include "googletranslator.h"
#include "apihelper.h"

GoogleTranslator::GoogleTranslator(Json::Value app_config)
{
    app_config_m = app_config;
}

GoogleTranslator::~GoogleTranslator()
{
}

Json::Value GoogleTranslator::translateContent(std::string content)
{
    ApiHelper api_reader = ApiHelper(app_config_m);
    Json::Value obj_json_result = api_reader.translateContentFromGoogleApi(content);
    return obj_json_result;
}

std::map<std::string, std::string> GoogleTranslator::listAllAvailableLanguages()
{
    ApiHelper api_reader = ApiHelper(app_config_m);
    std::map<std::string, std::string> languages_to_return;

    Json::Value obj_json_result;

    obj_json_result = api_reader.readAvailableLanguagesFromGoogleApi();

    auto data = obj_json_result["data"];
    auto languages = data["languages"];
    for (auto &i : languages)
    {
        languages_to_return.insert(std::pair<std::string, std::string>(i["language"].asString(), i["name"].asString()));
    }

    return languages_to_return;
}
