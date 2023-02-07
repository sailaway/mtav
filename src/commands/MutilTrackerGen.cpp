//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General PublicLicense as published by
// the Free Software Foundation, version 3 of the License
// Author:
//   sailaway <foxitwang@gmail.com>
//

#include "MutilTrackerGen.hpp"
#include <opencv2/video/video.hpp>
#include "../multiTracker/MultiTrackerVideoGen.hpp"
#include "../utils/cvutil.hpp"

cmdline::parser MutilTrackerGenCommand::paramsCheck(int argc, const char **argv){
    cmdline::parser cmdparser;
    cmdparser.add<string>("command"    , 'c', "图像命令", true);
    cmdparser.add<string>("cfgFile" , 'f', "MTAV配置JSON文件", true);
    string desc = "根据MTAV配置JSON文件生成视频";
    cmdparser.set_program_desc(desc);
    cmdparser.parse_check(argc, argv);
    return cmdparser;
}

void MutilTrackerGenCommand::process(int argc, const char **argv){
    cmdline::parser cmdparser = this->paramsCheck(argc,argv);
    string cfgFile   = cmdparser.get<string>("cfgFile");
    
    bool ok = genMultiTrackerVideo(cfgFile);
    if (ok) {
        cout<<"success"<<endl;
    } else {
        cout<<"failed"<<endl;
    }
    
}
