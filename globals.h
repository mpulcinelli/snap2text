#include <algorithm>
#include <cctype>
#include <functional>
#include <limits.h>
#include <locale>
#include <sstream>
#include <string>
#include <unistd.h>

const inline std::size_t _ID = 15669471125426487840UL;
const inline std::size_t _TITLE = 2085137023996725595UL;
const inline std::size_t _DESCRIPTION = 3086331403157438677UL;
const inline std::size_t _CREATEDDATE = 12442762642115171650UL;

// trim from start (in place)
static inline void ltrim(std::string &s)
{
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](int ch) { return !std::isspace(ch); }));
}

// trim from end (in place)
static inline void rtrim(std::string &s)
{
    s.erase(std::find_if(s.rbegin(), s.rend(), [](int ch) { return !std::isspace(ch); }).base(), s.end());
}

// trim from both ends (in place)
static inline void trim(std::string &s)
{
    ltrim(s);
    rtrim(s);
}

// trim from start (copying)
static inline std::string ltrim_copy(std::string s)
{
    ltrim(s);
    return s;
}

// trim from end (copying)
static inline std::string rtrim_copy(std::string s)
{
    rtrim(s);
    return s;
}

// trim from both ends (copying)
static inline std::string trim_copy(std::string s)
{
    trim(s);
    return s;
}

static inline std::string get_path_with_exe()
{
    char result[PATH_MAX];
    ssize_t count = readlink("/proc/self/exe", result, PATH_MAX);
    return std::string(result, (count > 0) ? count : 0);
}

static inline std::string get_path_no_exe()
{
    char result[PATH_MAX];
    ssize_t count = readlink("/proc/self/exe", result, PATH_MAX);
    std::string strPath = std::string(result, (count > 0) ? count : 0);
    std::string::size_type t = strPath.find_last_of("/");
    strPath = strPath.substr(0, t + 1);
    return strPath;
}

static inline void findAndReplaceAll(std::string &data, std::string toSearch, std::string replaceStr)
{
    size_t pos = data.find(toSearch);

    while (pos != std::string::npos)
    {
        data.replace(pos, toSearch.size(), replaceStr);
        pos = data.find(toSearch, pos + replaceStr.size());
    }
}

static inline unsigned long converttoASCII(std::string s)
{
    std::hash<std::string> strHash;
    unsigned long ret = strHash(s);
    return ret;
}