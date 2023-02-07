//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General PublicLicense as published by
// the Free Software Foundation, version 3 of the License
// Author:
//   sailaway <foxitwang@gmail.com>
//

#ifndef MutilTrackerGenCommand_hpp
#define MutilTrackerGenCommand_hpp

#include "../precomp.h"
#include "../cmdline.h"
#include "../Commands.hpp"


class MutilTrackerGenCommand: public MtavCommand{
public:
    void process(int argc, const char **argv);
private:
    cmdline::parser paramsCheck(int argc, const char **argv);
};


#endif /* MutilTrackerGenCommand_hpp */
