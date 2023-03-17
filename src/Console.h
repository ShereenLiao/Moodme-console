//
// Created by xiaoxuan on 3/16/23.
//

#ifndef CONSOLE_CONSOLE_H
#define CONSOLE_CONSOLE_H

#include <boost/asio.hpp>
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/version.hpp>
#include <boost/asio/connect.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <nlohmann/json.hpp>

#include <string>
#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>
#include <strings.h>

namespace beast = boost::beast;     // from <boost/beast.hpp>
namespace http = beast::http;       // from <boost/beast/http.hpp>
namespace net = boost::asio;        // from <boost/asio.hpp>
using tcp = net::ip::tcp;           // from <boost/asio/ip/tcp.hpp>
using json = nlohmann::json;        //from <nlohmann/json.hpp>

enum ConsoleStatus{
    ACTIVE, WAITING
};

class Console {
private:
    const std::string host_name = "api.giphy.com";
    const std::string port = "80";
    const std::string stickers_api = "/v1/stickers/search";
    const std::string api_key = "aSw3Zz2JXx9oKftl4wPGqBwt6jaj1j21";
    const int version = 11;

    ConsoleStatus status = ACTIVE;
    std::string criteria;
    int offset = 0;

    std::vector<std::vector<std::pair<std::string, std::string>>> images;

    void parse_body(http::response<http::dynamic_body> res);
    void parse_header(http::response<http::dynamic_body> res);
    std::vector<std::string>  splitString(const std::string & str, std::string delimiter = " ");
    std::string make_lowercase(const std::string& in);

public:
    void search(std::string criteria, int offset);
    void next();
    void cancel();
    void countByRating(std::string rating);
    void run();
};


#endif //CONSOLE_CONSOLE_H