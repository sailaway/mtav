![MTAV logo][logo]

# [MTAV - 多轨道动画视频生成器][mtav-git]

> MTAV 是一个多轨道视频内容合成工具，每条轨道在不同的时间点根据对应的属性生成视频画面，通过定义属性随着时间的变化过程生成动态视频内容。最终所有轨道内容组合生成最终的视频画面。

## 依赖
MTAV 依赖 OpenCV 库， makefile通过pkg-config管理OpenCV头文件和库文件位置。
```
CFLAGS+=`pkg-config --cflags opencv`
LDFLAGS+=`pkg-config --libs opencv`
```
如果未安装pkg-config或者自行编译OpenCV库,请替换Makefile上述配置，类似：
```
CFLAGS+=-I/自定义路径/opencv/x.x.x/include/opencv4
LDFLAGS+=`-L//自定义路径/opencv/x.x.x/lib -lopencv_imgproc -lopencv_core -lopencv_imgcodecs -lopencv_videoio -lopencv_highgui`
```
更多信息请参考 [OpenCV官网][opencv]


## 编译

可通过makefile直接编译
```
cd somewhere/mtav
make && make install 
```
将生成bin/mtav可执行文件

mtav.xcodeproj 是一个xcode示例工程，可通过xcode打开运行

## 运行
执行 `bin/mtav -c gen` 将看到下述输出
```
need option: --cfgFile
usage: bin/mtav --command=string --cfgFile=string [options] ... 
  根据MTAV配置JSON文件生成视频
options:
  -c, --command    图像命令 (string)
  -f, --cfgFile    MTAV配置JSON文件 (string)
  -?, --help       print this message

```

该命令需要一个视频生成配置文件`cfgFile`， 当输入自定义的配置文件 `bin/mtav -c gen -f xxx.cfg` 将会在指定位置生成视频

## 配置文件
[多轨道内容生成示意图](todo)



## 示例



## 贡献

MTAV生成库代码已经开源， 欢迎大家体验使用以及开发更多类型更丰富的轨道，也欢迎大家提issue, 完善文档和补充的更多特性。

## [协议][license]

MTAV is [licensed][license] under the GNU GENERAL PUBLIC LICENSE Version 3.

[logo]: https://open.zhiyanxx.com/mtav/images/logo.png
[mtav]: https://open.zhiyanxx.com/mtav
[mtav-git]: https://github.com/sailaway/mtav
[license]: LICENSE
[opencv]: https://opencv.org

