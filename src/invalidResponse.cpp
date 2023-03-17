//
// Created by xiaoxuan on 3/16/23.
//

#ifndef MOODME_CONSOLE_INVALIDRESPONSE_H
#define MOODME_CONSOLE_INVALIDRESPONSE_H

#include <string>

class invalid_response : public std::exception{
private:
    std::string message;
public:
    invalid_response(std::string msg):message(msg){}
    std::string getMessage(){
        return message;
    }
};


#endif //MOODME_CONSOLE_INVALIDRESPONSE_H
