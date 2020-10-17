#include <curl/curl.h>
#include <jsoncpp/json/json.h>
#include <string>

class ApiHelper
{
private:
    /* data */
public:
    ApiHelper(Json::Value app_config);
    ~ApiHelper();

    Json::Value translateContentFromGoogleApi(std::string jsondata);
    Json::Value readAvailableLanguagesFromGoogleApi();

private:
    Json::Value app_config_m;
};
