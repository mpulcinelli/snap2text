#include "languagehelper.h"
#include "globals.h"
#include <fstream>

LanguageHelper::LanguageHelper()
{
}

LanguageHelper::LanguageHelper(std::string language_ident)
{
    language_selected = language_ident;
    loadInMemory();
}

LanguageHelper::~LanguageHelper()
{
}

void LanguageHelper::loadInMemory()
{
    Json::Reader json_reader;
    std::string exec_path = get_path_no_exe();
    std::ifstream file_conf(exec_path + "static/language/" + language_selected + ".json");

    if (file_conf.is_open())
    {
        Json::Reader json_reader;
        std::stringstream buffer;

        buffer << file_conf.rdbuf();
        file_conf.close();
        json_reader.parse(buffer, list_items);
    }
    //list_items
}

std::string LanguageHelper::getItem(std::string token)
{
    if (!list_items.isNull())
    {
        std::string value_member = list_items.get(token, "E").asString();
        return value_member;
    }
    else
    {
        return "";
    }
}