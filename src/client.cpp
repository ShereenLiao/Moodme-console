#include "../include/client.h"

client::client(boost::asio::io_service & io_service): socket(io_service), resolver(io_service){
    //look up the domain name
    auto const results = resolver.resolve(host_name, port);
    net::connect(socket, results.begin(), results.end());
}

/**
 * command: search <query>
 * Send HTTP get request.
 * Parse the body & header, and print out results.
 * */
void client::search(std::string query, int offset){
    this->offset = offset;
    this->query = query;

    std::string target = stickers_api + "?api_key=" + api_key + "&q=" + query +"&offset=" + std::to_string(offset);

    // Set up an HTTP GET request message
    http::request<http::string_body> req{http::verb::get, target, version};
    req.set(http::field::host, host_name);
    req.set(http::field::user_agent, BOOST_BEAST_VERSION_STRING);

    //Send the http request to the remote host
    http::write(socket, req);

    //This buffer is used for reading and must be persisted
    beast::flat_buffer buffer;

    // Declare a container to hold the response and receive the response
    http::response<http::dynamic_body> res;
    http::read(socket, buffer, res);

    // Parse the http header and body
    try{
        parse_header(res);
    }
    catch(invalid_response e){
        std::cout<<e.getMessage()<<std::endl;
        return;
    }
    parse_body(res);
}

/**
 * command: next
 * Presents the next data page.
 * If there is no data for next page, the client will go to waiting mode.
 * The client will be active until a new cancel command.
 * */
void client::next() {
    this->search(query, offset);
}

/**
 * command: cancel
 * Cancel ongoing search and waits for a next command.
 * If the client is in waiting mode, activate the client and cancel the ongoing search.
 * Activate the client and cancel ongoing search.
 * */
void client::cancel(){
    if(this->status == WAITING){
        this->status = ACTIVE;
        std::cout<<"The console is in active mode."<<std::endl;
    }
    else{
        std::cout<<"There is not ongoing search."<<std::endl;
    }
}

/**
 * command: Rank <rating>
 * Get how many stickers with the same rank are queried so far.
 * */
void client::countByRating(std::string rating) {
    int count = 0;
    for(auto vec : images){
        for(auto p : vec){
            if(p.second == rating){
                count++;
            }
        }
    }
    std::cout<<"The number of images of rating " + rating +" is " + std::to_string(count)<<std::endl;
}

/**
 * Launch the interactive console.
 * Parse commands and validate input.
 * Evoke corresponding functions.
 * */
void client::run() {
    std::string input;
    do{
        std::cout << "prompt> ";
        std::getline(std::cin, input);
        std::vector<std::string> commands = splitString(input);
        std::string command = make_lowercase(commands.front());
        if(command.compare("exit") == 0 && commands.size() == 1){
            exit();
        }
        else if(this->status == WAITING){
            if(command.compare("cancel") == 0 && commands.size() == 1){
                cancel();
            }
            else{
                std::cout<<"The console is in waiting mode"<<std::endl;
            }
        }
        else if(command.compare("search")== 0 && commands.size() == 2){
            search(commands[1], 0);
        }
        else if(command.compare("rank") == 0 && commands.size() == 2){
            countByRating(commands[1]);
        }
        else if(command.compare("next") == 0 && commands.size() == 1){
            next();
        }
        else{
            std::cout<<"Invalid input.\n"
                     <<"Valid Usages: \n"
                     <<"\tsearch <criteria>\n"
                     << "\tnext\n"
                     << "\trank <rating>\n"
                     <<"\tcancel\n"
                     <<"\texit\n"
                     <<std::endl;
        }
    }
    while(input != "exit");
}

/**
 * command: exit
 * When user close the client, disconnect the socket and free space.
 * */
void client::exit() {
    this->status = TERMINATED;
    // Gracefully close the socket
    beast::error_code ec;
    socket.shutdown(tcp::socket::shutdown_both, ec);
    if(ec && ec != boost::system::errc::not_connected)
        throw boost::system::system_error{ec};

    delete this;
}

/**
 * Parse the http response body.
 * Parse the url in response body, store in images, and update the offset.
 * */
void client::parse_body(http::response<http::dynamic_body> res){
    std::string body = beast::buffers_to_string(res.body().data());
    std::vector<std::pair<std::string, std::string>> urls;
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
        std::cout<<"No data. "<<std::endl;
        std::cout<<"The console is in waiting mode."<<std::endl;
        this->status = WAITING;
    }
    else{
        this->images.push_back(urls);
    }
    offset += count;
}

/**
 * Parse the http header and validate the status code.
 * */
void client::parse_header(http::response<http::dynamic_body> res){
    // status code as integer
    int result_int = res.result_int();
    // text reason for failure
    boost::beast::string_view reason = res.reason();
    std::string msg {reason};
    if (result_int != 200)
    {
        throw new invalid_response("Invalid Response with status code" + std::to_string(result_int) + ". Error Message : " + msg);
    }
}

/**
 * Split string into vector<string> delimiting by space
 * */
std::vector<std::string> client::splitString(const std::string & str) {
    std::istringstream iss(str);
    std::vector<std::string> result;
    for(std::string s; iss >> s; )
        result.push_back(s);
    return result;
}

/**
 * Convert characters in string into lower cases.
 * */
std::string client::make_lowercase(const std::string& in)
{
    std::string out;
    std::transform( in.begin(), in.end(), std::back_inserter( out ), ::tolower);
    return out;
}



