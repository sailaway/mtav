//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General PublicLicense as published by
// the Free Software Foundation, version 3 of the License
// Author:
//   sailaway <foxitwang@gmail.com>
//


#include "cmdline.h"
#include "Commands.hpp"
#include "./utils/ImageProcess.hpp"
#include "commands/MutilTrackerGen.hpp"

vector<string> supportedCommand(){
    vector<string> commands;
    commands.push_back(COMMAND_MTAV_GEN);
    //commands.push_back(COMMAND_MTAV_FRAME);
    
    return commands;
}

MtavCommand* getMtavCommandInstance(string commandName){
    if (commandName == COMMAND_MTAV_GEN) {
        MutilTrackerGenCommand *command = new MutilTrackerGenCommand();
        return static_cast<MtavCommand *>(command);
    } else if (commandName == COMMAND_MTAV_FRAME) {
//        MutilTrackerGenCommand *command = new MutilTrackerGenCommand();
//        return static_cast<SubCommand *>(command);
    }
    
    return NULL;
}


