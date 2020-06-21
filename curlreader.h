#include <curl/curl.h>
#include <string>

class CurlReader
{
private:
    /* data */
public:
    CurlReader(/* args */);
    ~CurlReader();

    std::string read_url(std::string url, char *data);

private:
};
