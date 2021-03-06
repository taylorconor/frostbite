//
//  Utils.cpp
//  frostbite
//
//  Created by Conor Taylor on 14/11/2014.
//  Copyright (c) 2014 Frostplant. All rights reserved.
//

#include "Utils.h"

void Utils::error(std::string msg) {
    perror(msg.c_str());
}

std::string Utils::dump_map(const std::map<std::string, std::string>& map) {
    std::string ret;
    for (std::map<std::string,std::string>::const_iterator it = map.begin();
         it != map.end(); it++) {
        ret += it->first + ": " + it->second + '\n';
    }
    return ret;
}

std::string& trim_right_inplace(std::string& s,
                                const std::string& delimiters = " \f\n\r\t\v") {
    return s.erase(s.find_last_not_of(delimiters) + 1);
}

std::string& trim_left_inplace(std::string& s,
                               const std::string& delimiters = " \f\n\r\t\v") {
    return s.erase(0, s.find_first_not_of(delimiters));
}

std::string& Utils::trim(std::string& s) {
    return trim_left_inplace(trim_right_inplace(s, " \f\n\r\t\v"), " \f\n\r\t\v");
}

std::vector<std::string> Utils::explode(std::string const &s, char delim) {
    std::vector<std::string> result;
    std::istringstream iss(s);
    
    for (std::string token; std::getline(iss, token, delim); )
        result.push_back(std::move(token));
    
    return result;
}

std::vector<std::string> Utils::explode(std::string const &s,
                                        char delim,
                                        int amt) {
    std::vector<std::string> result;
    std::istringstream iss(s);
    std::string remainder, token;
    int i = 0;
    
    for (; std::getline(iss, token, delim); i++) {
        if (i < amt-1)
            result.push_back(std::move(token));
        else
            remainder += delim + std::move(token);
    }
    
    if (i >= amt) {
        // remove the delimiter from the very start of the string
        if (remainder[0] == delim)
            remainder.erase(0,1);
        // push the remainder, trimming whitespace
        result.push_back(trim(remainder));
    }
    
    return result;
}

bool Utils::exists(const std::string& name) {
    struct stat buffer;
    return (stat (name.c_str(), &buffer) == 0);
}

std::string Utils::time_string(time_t t) {
    tm *ltm = localtime(&t);
    
    std::string s;
    if (ltm->tm_hour < 10)
        s += "0";
    s += std::to_string(ltm->tm_hour)+":";
    
    if (ltm->tm_min < 10)
        s += "0";
    s += std::to_string(ltm->tm_min)+":";
    
    if (ltm->tm_sec < 10)
        s += "0";
    s += std::to_string(ltm->tm_sec)+" ";
    
    if (ltm->tm_mday < 10)
        s += "0";
    s += std::to_string(ltm->tm_mday)+"/";
    
    if (ltm->tm_mon < 10)
        s += "0";
    s += std::to_string(ltm->tm_mon)+"/";
    
    s += std::to_string(ltm->tm_year+1900);
    
    return s;
}