
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
    /* data */
    EAppIntegrityCheck CheckAppConfigContent();

public:
    AppIntegrity(/* args */);
    ~AppIntegrity();
    EAppIntegrityCheck CheckConfigFilesIntegrity();
};
