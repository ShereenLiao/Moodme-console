//
// Created by xiaoxuan on 3/16/23.
//

#ifndef CONSOLE_INVALIDRESPONSE_H
#define CONSOLE_INVALIDRESPONSE_H

#include <string>

class InvalidResponse : public std::exception{
private:
    std::string message;
public:
    InvalidResponse(std::string msg):message(msg){}
    std::string getMessage(){
        return message;
    }
};


#endif //CONSOLE_INVALIDRESPONSE_H
