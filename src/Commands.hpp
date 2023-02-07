//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General PublicLicense as published by
// the Free Software Foundation, version 3 of the License
// Author:
//   sailaway <foxitwang@gmail.com>
//

#ifndef Commands_hpp
#define Commands_hpp

#include "precomp.h"
#include "cmdline.h"
#include "MtavCommand.hpp"

#define COMMAND_MTAV_GEN       "gen"
#define COMMAND_MTAV_FRAME     "frame"

vector<string> supportedCommand();
MtavCommand* getMtavCommandInstance(string commandName);


#endif /* Commands_hpp */
