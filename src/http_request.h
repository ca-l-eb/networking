#ifndef CMD_HTTP_REQUEST_H
#define CMD_HTTP_REQUEST_H

#include <map>
#include <memory>
#include <regex>
#include <string>
#include <vector>

#include <openssl/ssl.h>

#include "http_response.h"
#include "socket.h"
#include "stream.h"

namespace cmd
{
class http_request
{
public:
    explicit http_request(const std::string &url);
    explicit http_request(cmd::stream &stream);
    void set_request_method(const std::string &method);
    void set_header(const std::string &header, const std::string &value);
    void set_body(const std::string &body);
    void set_resource(const std::string &resource);
    void connect();
    cmd::http_response response();

private:
    std::string request_method, resource, body;
    std::map<std::string, std::string> headers;
    cmd::stream *stream;
    cmd::http_response resp;
    bool connected;
};
}  // namespace cmd

#endif
