#include <string>

enum class EAppIntegrityCheck
{
    AppConfigMissing,
    AppStyleMissing,
    AppUIWindowGladeMissing,
    AppMemberApiGoogleTranslatorMissing,
    AppMemberApiGoogleLanguageListMissing,
    AppMemberLanguageMissing,
    AppGoodToGo
};

class AppIntegrity
{
private:
    const std::string _APP_CONFIG = "static/app_config.json";
    const std::string _APP_STYLE_CSS = "static/style.css";
    const std::string _APP_UI_GLADE = "static/ui-window.glade";

    std::string app_path;

    EAppIntegrityCheck CheckAppConfigContent();

public:
    AppIntegrity(/* args */);
    ~AppIntegrity();
    EAppIntegrityCheck CheckConfigFilesIntegrity();
};
