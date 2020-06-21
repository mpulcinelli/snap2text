#include "curlreader.h"
#include <cstdint>
#include <curlpp/Easy.hpp>
#include <curlpp/Options.hpp>
#include <curlpp/cURLpp.hpp>
#include <iostream>
#include <json/json.h>
#include <list>
#include <memory>
#include <sstream>
#include <string>

CurlReader::CurlReader()
{
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

Json::Value CurlReader::read_url(std::string url_path, std::string jsondata)
{
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

void CurlReader::read_available_languages()
{
    //https: //www.googleapis.com/language/translate/v2/languages
}
