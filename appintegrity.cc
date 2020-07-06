#include "appintegrity.h"
#include "globals.h"

#include <sstream>
#include <filesystem>
#include <fstream>
#include <sys/stat.h>
#include <unistd.h>
#include <string>
#include <fstream>
#include <json/json.h>

AppIntegrity::AppIntegrity(/* args */)
{
}

AppIntegrity::~AppIntegrity()
{
}

EAppIntegrityCheck AppIntegrity::CheckConfigFilesIntegrity()
{
    bool exist = false;

    exist = std::filesystem::exists("app_config.json");

    if (!exist)
        return EAppIntegrityCheck::AppConfigMissing;
    else
    {
        EAppIntegrityCheck retval = CheckAppConfigContent();
        if (retval != EAppIntegrityCheck::AppGoodToGo)
        {
            return retval;
        }
    }
    std::string exec_path = get_path_no_exe();

    exist = std::filesystem::exists(exec_path + "static/style.css");

    if (!exist)
        return EAppIntegrityCheck::AppStyleMissing;

    exist = std::filesystem::exists(exec_path + "static/ui-window.glade");

    if (!exist)
        return EAppIntegrityCheck::AppUIWindowGladeMissing;

    return EAppIntegrityCheck::AppGoodToGo;
}

EAppIntegrityCheck AppIntegrity::CheckAppConfigContent()
{
    std::string exec_path = get_path_no_exe();
    std::ifstream file_conf(exec_path + "app_config.json");

    if (file_conf.is_open())
    {
        Json::Value app_config_params;
        Json::Reader json_reader;
        std::stringstream buffer;

        buffer << file_conf.rdbuf();
        file_conf.close();
        json_reader.parse(buffer, app_config_params);

        if (!app_config_params.isNull())
        {
            std::string check_member = "";

            check_member = app_config_params.get("api_google_translator", "E").asString();
            if (check_member.compare("E") == 0)
                return EAppIntegrityCheck::AppMemberApiGoogleTranslatorMissing;

            check_member = app_config_params.get("api_google_translator_list_language", "E").asString();
            if (check_member.compare("E") == 0)
                return EAppIntegrityCheck::AppMemberApiGoogleLanguageListMissing;

            check_member = app_config_params.get("app_language", "E").asString();
            if (check_member.compare("E") == 0)
                return EAppIntegrityCheck::AppMemberLanguageMissing;
        }
    }

    return EAppIntegrityCheck::AppGoodToGo;
}