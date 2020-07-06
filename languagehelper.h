#include <iostream>
#include <json/json.h>
#include <list>
#include <map>
#include <memory>
#include <sstream>
#include <string>

class LanguageHelper
{
private:
    /* data */
    std::string language_selected;
    Json::Value list_items;

public:
    LanguageHelper();
    LanguageHelper(std::string language_ident);
    ~LanguageHelper();
    void loadInMemory();
    std::string getItem(std::string token);
};