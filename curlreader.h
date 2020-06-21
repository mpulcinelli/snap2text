#include <curl/curl.h>
#include <json/json.h>
#include <string>

class CurlReader
{
private:
    /* data */
public:
    CurlReader(/* args */);
    ~CurlReader();

    Json::Value read_url(std::string url, std::string);
    void read_available_languages();

private:
};
