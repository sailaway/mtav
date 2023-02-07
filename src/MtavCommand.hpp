//
//  mtav command class
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General PublicLicense as published by
// the Free Software Foundation, version 3 of the License
// Author:
//   sailaway <foxitwang@gmail.com>
//

#ifndef MtavCommand_hpp
#define MtavCommand_hpp

#include "cmdline.h"


class MtavCommand{
public:
    MtavCommand() {}
    virtual ~MtavCommand(){}
    virtual void process(int argc, const char **argv){
        throw "未实现的命令";
    }
};


#endif /* MtavCommand_hpp */
