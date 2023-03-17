//
// Created by xiaoxuan on 3/16/23.
//

#include "Console.h"
#include "InvalidResponse.h"


void Console::search(std::string criteria){
    this->criteria = criteria;
    this->offset = 0;
    std::string target = stickers_api + "?api_key=" + api_key + "&q=" + criteria+"&offset=" + std::to_string(offset);
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
        return;
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
    int count = 0;
    for(auto data : json["data"]){
        std::string url = data["images"]["original"]["url"];
        std::string rating = data["rating"];
        urls.emplace_back(url, rating);
        count += 1;
        std::cout<<url<<" "<<rating<<std::endl;
    }
    if(count == 0){
        std::cout<<"No data"<<std::endl;
    }
//    int count = json["pagination"]["count"];
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

void Console::next() {
    this->search(criteria);
}

void Console::cancel(){

}

void Console::countByRating(std::string rating) {
    int count = 0;
    for(auto p : urls){
        if(p.second == rating){
            count++;
        }
    }
    std::cout<<"The number of images of rating " + rating +" is " + std::to_string(count)<<std::endl;
}



void Console::connect_handler(const boost::system::error_code & ec) {
}

void Console::run() {
    std::string input;
    do{
        std::cout << "prompt> ";
        std::getline(std::cin, input);
        std::vector<std::string> commands = splitString(input, " ");
        std::string command = commands.front();
        if(command.compare("search") == 0 && commands.size() == 2){
            search(commands[1]);
        }
        else if(command.compare("rank") == 0 && commands.size() == 2){
            countByRating(commands[1]);
        }
        else if(command.compare("next") == 0){
            next();
        }
        else if(command.compare("cancel") == 0){
            cancel();
        }
        else{
            std::cout<<"Invalid input.\n"
            <<"Valid Usages: \n"
            <<"\tsearch <criteria>\n"
            << "\tnext\n"
            << "\trank <rating>\n"
            <<"\tcancel\n"<<std::endl;
        }
    }
    while(input != "exit");
}



std::vector<std::string> Console::splitString(std::string str, std::string delimiter) {
    int start = 0;
    int end = str.find(delimiter);
    std::vector<std::string> parsed_command ;
    while (end != -1) {
        parsed_command.push_back(str.substr(start, end - start));
        start = end + delimiter.size();
        end = str.find(delimiter, start);
    }
    parsed_command.push_back(str.substr(start, end - start));
    return parsed_command;
}



