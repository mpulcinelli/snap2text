#include "curlreader.h"
#include <iostream>
#include <sstream>
#include <string>

#include <curlpp/Easy.hpp>
#include <curlpp/Options.hpp>
#include <curlpp/cURLpp.hpp>

CurlReader::CurlReader(/* args */)
{
}

CurlReader::~CurlReader()
{
}

std::string CurlReader::read_url(std::string url)
{
    curlpp::options::Url myUrl(url);
    curlpp::options::UserAgent ue("Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/83.0.4103.106 Safari/537.36");
    curlpp::Easy myRequest;

    myRequest.setOpt(ue);
    myRequest.setOpt(myUrl);

    std::ostringstream os;
    curlpp::options::WriteStream ws(&os);
    myRequest.setOpt(ws);
    myRequest.perform();

    os << myRequest;
    std::string s = os.str();
    return s;
}
