//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General PublicLicense as published by
// the Free Software Foundation, version 3 of the License
// Author:
//   sailaway <foxitwang@gmail.com>
//

#ifndef strutil_hpp
#define strutil_hpp

#include <iostream>
#include <sstream>
#include <stdio.h>
#include <vector>
#include <string>
#include <algorithm>
using namespace std;

vector<string> strSplit(const string& str, const char delim);

bool strEndWith(string str,string seg);
bool strStartWith(string str,string seg);
bool strContains(string str,string seg);
string ucfirst(string str);

inline std::string str2LowerCase(const std::string& str){
    std::string result(str);
    transform(result.begin(), result.end(), result.begin(), ::tolower);
    return result;
}

inline std::string str2UpperCase(const std::string& str){
    std::string result(str);
    transform(result.begin(), result.end(), result.begin(), ::toupper);
    return result;
}


#endif /* strutil_hpp */
