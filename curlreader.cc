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

std::string CurlReader::read_url(std::string url, char *data)
{

    std::istringstream myStream(data);
    int size = myStream.str().size();
    char buf[50];
    std::list<std::string> headers;
    headers.push_back("Content-Type: application/json");

    sprintf(buf, "Content-Length: %d", size);
    headers.push_back(buf);

    curlpp::Cleanup cleaner;
    curlpp::Easy request;

    using namespace curlpp::Options;

    request.setOpt(new Verbose(true));
    request.setOpt(new UserAgent("Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/83.0.4103.106 Safari/537.36"));
    request.setOpt(new ReadStream(&myStream));
    request.setOpt(new InfileSize(size));
    request.setOpt(new Upload(true));
    request.setOpt(new HttpHeader(headers));

    request.setOpt(new Url(url));

    request.perform();

    std::ostringstream os;
    curlpp::options::WriteStream ws(&os);

    request.setOpt(ws);
    request.perform();

    os << request;
    std::string s = os.str();
    return s;
}
