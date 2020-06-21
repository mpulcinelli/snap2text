#include <curl/curl.h>
#include <string>

class CurlReader
{
private:
    /* data */
public:
    CurlReader(/* args */);
    ~CurlReader();

    std::string read_url(std::string url);

private:
    size_t write_function(void *ptr, size_t size, size_t nmemb, std::string *data);
};
