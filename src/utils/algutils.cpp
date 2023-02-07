//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General PublicLicense as published by
// the Free Software Foundation, version 3 of the License
// Author:
//   sailaway <foxitwang@gmail.com>
//

#include "algutils.hpp"

vector<double> calcuMap(double multiply,int len){
    vector<double> map;
    for (int i = 0; i < len; i++) {
        double val = multiply * i;
        map.push_back(val);
    }
    return map;
}
