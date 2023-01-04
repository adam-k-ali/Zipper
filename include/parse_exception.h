//
// Created by Adam Ali on 04/01/2023.
//

#ifndef ZIPPER_PARSE_EXCEPTION_H
#define ZIPPER_PARSE_EXCEPTION_H

#include <iostream>

class parse_exception : public std::exception {
private:
    std::string message;
public:
    parse_exception(const std::string &message) : message(message) {}
    const char *what() const noexcept override {
        return message.c_str();
    }
};
#endif //ZIPPER_PARSE_EXCEPTION_H
