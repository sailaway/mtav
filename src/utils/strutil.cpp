//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General PublicLicense as published by
// the Free Software Foundation, version 3 of the License
// Author:
//   sailaway <foxitwang@gmail.com>
//

#include "strutil.hpp"

/*
 * string 被单字符分割
 */
vector<string> strSplit(const string& str, const char delim) {
    vector<string> res;
    if("" == str) return res;
    int s_pos = 0;
    int i = 0;
    while (i < str.length()) {
        if (str[i] == delim) {
            ostringstream oss;
            for (int p = s_pos; p < i; p++) {
                oss<<str[p];
            }
            res.push_back(oss.str());
            s_pos = i + 1;
        }
        i++;
    }
    ostringstream oss;
    for (int p = s_pos; p < str.length(); p++) {
        oss<<str[p];
    }
    res.push_back(oss.str());
    return res;
}

bool strContains(string str,string seg){
    long idx = str.find(seg);
    return idx >= 0;
}
bool strStartWith(string str,string seg){
    if (seg.size() > str.size()) {
        return false;
    }
    long idx = str.find(seg);
    return idx == 0;
}
bool strEndWith(string str,string seg){
    if (seg.size() > str.size()) {
        return false;
    }
    long idx = str.rfind(seg);
    return idx == str.size() - seg.size();
}

// 第一个字符大写
string ucfirst(string str){
    if(str.size() <= 0){
        return str;
    }
    char uc = toupper(str.at(0));
    str.replace(0, 1, 1,uc);
    return str;
}
