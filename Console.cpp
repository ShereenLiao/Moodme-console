//
// Created by xiaoxuan on 3/16/23.
//

#include "Console.h"
#include "InvalidResponse.h"


void Console::search(std::string criteria) {
    std::string target = stickers_api + "?api_key=" + api_key + "&q="+criteria;
    // The io_context is required for all I/O
    net::io_context ioc;

    // These objects perform our I/O
    tcp::resolver resolver(ioc);
    beast::tcp_stream stream(ioc);

    // Look up the domain name
    auto const results = resolver.resolve(host_name, port);

    // Make the connection on the IP address we get from a lookup
    stream.connect(results);

    // Set up an HTTP GET request message
    http::request<http::string_body> req{http::verb::get, target, version};
    req.set(http::field::host, host_name);
    req.set(http::field::user_agent, BOOST_BEAST_VERSION_STRING);


    // Send the HTTP request to the remote host
    http::write(stream, req);

    // This buffer is used for reading and must be persisted
    beast::flat_buffer buffer;

    // Declare a container to hold the response
    http::response<http::dynamic_body> res;

    // Receive the HTTP response
    http::read(stream, buffer, res);

    try{
        parse_header(res);
    }
    catch(InvalidResponse e){
        std::cout<<e.getMessage()<<std::endl;
    }

    // Write the message to standard out
    parse_body(res);

    // Gracefully close the socket
    beast::error_code ec;
    stream.socket().shutdown(tcp::socket::shutdown_both, ec);
}

void Console::parse_body(http::response<http::dynamic_body> res){
    std::string body = beast::buffers_to_string(res.body().data());
    auto json = json::parse(body);
    for(auto data : json["data"]){
        std::string url = data["images"]["original"]["url"];
        urls.push_back(url);
//        std::cout<<url<<std::endl;
    }
    int count = json["pagination"]["count"];
    offset += count;
}


void Console::parse_header(http::response<http::dynamic_body> res){
    // status code as integer
    int result_int = res.result_int();
    // text reason for failure
    boost::beast::string_view reason = res.reason();
    std::string msg {reason};
    if (result_int != 200)
    {
        throw new InvalidResponse("Invalid Response with status code" + std::to_string(result_int) + ". Error Message : " + msg);
    }
}




