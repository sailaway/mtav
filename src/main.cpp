//
// Multi Tracker Animation Video
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General PublicLicense as published by
// the Free Software Foundation, version 3 of the License
// Author:
//   sailaway <foxitwang@gmail.com>
//


#include "cmdline.h"
#include "Commands.hpp"

#define MTAV_VERSION "1.0.0"

cmdline::parser help(int argc, const char *argv[]){
    
    cmdline::parser cmdparser;
    cmdparser.add<string>("command", 'c', "图像命令", true, "");
    vector<string> commands = supportedCommand();
    ostringstream oss;
    oss<<"-c command 参数是必填的参数 -c command 必须在所有其他参数的前面 "<<endl;
    oss<<"支持的图像命令:"<<endl;
    for (int i = 0; i < commands.size(); i++) {
        oss<<"      "<<commands[i]<<endl;
    }
    cmdparser.set_program_desc(oss.str());
    if (argc > 3) {
        argc = 3;
    }
    cmdparser.parse_check(argc, argv);
    return cmdparser;
}

int main(int argc, const char * argv[]) {
    cmdline::parser cmdparser = help(argc, argv);
    string commandName = cmdparser.get<string>("command");
    MtavCommand* command = getMtavCommandInstance(commandName);
    if (command == NULL) {
        cout<<cmdparser.usage();
        return -1;
    }
    command->process(argc, argv);
    
    delete command;
    return 0;
}
