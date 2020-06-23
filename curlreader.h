#include <curl/curl.h>
#include <json/json.h>
#include <string>

class CurlReader
{
private:
    /* data */
public:
    CurlReader(Json::Value app_config);
    ~CurlReader();

    Json::Value translate_content(std::string jsondata);
    std::map<std::string, std::string> read_available_languages();

private:
    Json::Value app_config_m;
};
