#include "curlreader.h"
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

CurlReader::CurlReader(Json::Value app_config)
{
    app_config_m = app_config;
}

CurlReader::~CurlReader()
{
}

inline size_t WriteCallback(const char *in,
                            std::size_t size,
                            std::size_t num,
                            std::string *out)
{
    const std::size_t totalBytes(size * num);
    out->append(in, totalBytes);
    return totalBytes;
}

Json::Value CurlReader::translate_content(std::string jsondata)
{
    std::string url_path = app_config_m["api_google_translator"].asString();

    std::list<std::string> header;

    Json::Value obj_json;
    Json::Reader json_reader;

    if (!json_reader.parse(jsondata, obj_json))
        return nullptr;

    header.push_back("Content-Type: application/json");
    curlpp::Cleanup clean;
    curlpp::Easy r;

    r.setOpt(new curlpp::options::Url(url_path));
    r.setOpt(new curlpp::options::HttpHeader(header));
    r.setOpt(new curlpp::options::PostFields(obj_json.toStyledString()));
    r.setOpt(new curlpp::options::PostFieldSize(obj_json.toStyledString().length()));

    std::ostringstream response;
    r.setOpt(new curlpp::options::WriteStream(&response));

    r.perform();
    std::string result = std::string(response.str());

    Json::Value obj_json_result;

    json_reader.parse(result, obj_json_result);

    return obj_json_result;
}

std::map<std::string, std::string> CurlReader::read_available_languages()
{
    std::string url_path = app_config_m["api_google_translator_list_language"].asString();

    std::list<std::string> header;
    std::map<std::string, std::string> languages_to_return;

    Json::Value obj_json;
    Json::Reader json_reader;

    header.push_back("Content-Type: application/json");
    curlpp::Cleanup clean;
    curlpp::Easy r;

    obj_json["target"] = "pt-br";

    r.setOpt(new curlpp::options::Url(url_path));
    r.setOpt(new curlpp::options::HttpHeader(header));
    r.setOpt(new curlpp::options::PostFields(obj_json.toStyledString()));
    r.setOpt(new curlpp::options::PostFieldSize(obj_json.toStyledString().length()));

    std::ostringstream response;
    r.setOpt(new curlpp::options::WriteStream(&response));

    r.perform();
    std::string result = std::string(response.str());

    Json::Value obj_json_result;

    if (json_reader.parse(result, obj_json_result))
    {
        auto data = obj_json_result["data"];
        auto languages = data["languages"];
        for (auto &i : languages)
        {
            languages_to_return.insert(std::pair<std::string, std::string>(i["language"].asString(), i["name"].asString()));
        }
    }

    return languages_to_return;
}
