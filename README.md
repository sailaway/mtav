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
如下图为某视频编辑器页面，编辑区域主要有四部分，轨道区域，素材区域，属性区域，视频预览区域。
![多轨道内容生成示意图](video-edit.jpeg)

MTAV生成视频的思路与此类似。

```
{
    "width": 428,
    "height": 428,
    "bgFile":"",
    "bgColor":"#fff",
    "frameCount":120,
    "fps":24,
    "outVideoFile":"~/Downloads/image-rotate.mp4",
    "trackers":[{
            "trackType":"simpleImage",
            "startFrame":0,
            "endFrame":120,
            "input":{
                "image":"~/Downloads/6548-dst-50.jpg"
            },
            "initProperties":{
                "videoAlignPtX":0.5,
                "videoAlignPtY":0.5,
                "trackerAlignPtX":0.5,
                "trackerAlignPtY":0.5,
                "alpha":1,
                "scale":1,
                "rotate":0
            },
            "animation":[
                {
                    "property": "rotate",
                    "startVal": 0,
                    "endVal": 3600,
                    "startOffsetFrame":0,
                    "endOffsetFrame":120,
                    "interpolator":{
                        "interType":"linear"
                    }
                }
            ]
        }
    ]
}
```
如上述配置示例，width,height,bgFile,bgColor,frameCount,fps,outVideoFile 等基本属性定义了输出视频文件宽高的、全局背景、帧属性、文件输出位置等基本属性，然后是核心的轨道参数trackers.

trackers 是一个轨道列表，trackers中每一个元素定义一条轨道，每一条轨道定义了视频文件中指定区域图像内容，每一条轨道主要由三部分组成：素材，属性、动画。

### 轨道
轨道定义了视频文件中指定区域图像内容；每条轨道都需要定义轨道类型(trackType)、开始帧(startFrame)、结束帧(endFrame)。

- 轨道类型  不同类型的轨道定义了视频画面的生成逻辑，例如普通图像轨道(simpleImage)，支持输入图像文件根据属性在指定位置以指定的透明度、旋转、缩放展示在输出视频指定位置；
- 开始帧/结束帧 定义了轨道内容在视频中展示的开始/结束时间；

### 素材(input)
轨道在生成视频内容的时候，会用到外部输入文件；例如普通图像轨道(simpleImage)，需要输入一张图片，所有生成视频内容需要依赖到的文件都定义在轨道的input属性中。
不同类型的轨道使用到的素材都不一样，详情请阅读具体轨道的使用文档。

### 属性(property)
不同类型的轨道在生成视频内容的时候，会根据属性的不同改变图像/视频内容的展现。例如普通视频轨道(simpleVideo)的帧索引(videoFrameIdx), 当videoFrameIdx=0时输出的是视频第一帧图像内容。
轨道属性的初始值定义在轨道的initProperties属性中

所有轨道都支持基本属性：

- 对齐点(align point)
    
    对齐点是用来定义轨道图像内容在最终生成视频中的位置；对齐点采用相对坐标系，所有坐标取值范围都是[0,1]，采用原点(0,0)在左上角的坐标系。
    - 视频对齐点坐标(videoAlignPtX,videoAlignPtY)
        
        videoAlignPtX=0是生成视频的最左位置，videoAlignPtX=1 是生成视频的最右位置，videoAlignPtY=0是生成视频最上方，videoAlignPtY=1是生成视频最下方；
    - 轨道对齐点坐标(trackerAlignPtX,trackerAlignPtY)
        
        trackerAlignPtX =0是轨道图像内容的最左位置，trackerAlignPtX =1 是轨道图像内容的最右位置，trackerAlignPtY =0是轨道图像内容最上方，trackerAlignPtY =1是轨道图像内容最下方；
    
    对齐点通过定义生成视频中的点和轨道图像坐标系中的点的重合来定义轨道图像的展示位置。即(videoAlignPtX,videoAlignPtY)和(trackerAlignPtX,trackerAlignPtY) 是同一个位置，而(videoAlignPtX,videoAlignPtY)和(trackerAlignPtX,trackerAlignPtY)分别是各自坐标系下的坐标（视频图像和轨道图像）。
    通过对齐点很容易定义在视频指定位置让轨道居中/左下角/右上角对齐的效果；一般地对齐点也称为不动点。

    
- 大小(scale)
    
    scale=1 为图像内容不缩放， scale < 1 缩小； scale > 1 放大
    
- 旋转(rotate)

	rotate 是基于图像中心点的旋转， 单位是角度(degree)；顺时针方向旋转为正方向
	
- 透明度(alpha)
	
	alpha 取值范围[0,1] alpha=0：完全透明，轨道图像内容内容不可见， alpha=1：完全不透明

### 动画(animation)
轨道中的动画定义了轨道属性根据时间(帧)的变化而变化的过程。例如如下动画描述了在距离所在轨道展示帧偏移0～120帧的过程中属性rotate的变化： 以线性插值(linear)的方式从0变化到3600

```
{
    "property": "rotate",
    "startVal": 0,
    "endVal": 3600,
    "startOffsetFrame":0,
    "endOffsetFrame":120,
    "interpolator":{
        "interType":"linear"
    }
}

```
- 动画帧范围(startOffsetFrame/endOffsetFrame)
    
    动画的作用时间(起始帧/结束帧)，帧范围使用所在轨道的相对帧定义，例如轨道起始展示帧是生成视频的第100帧，动画的起始帧(startOffsetFrame)是320， 那么在动画会在视频的420帧开始进行

- 作用属性(property)
    
    动画过程描述的轨道属性的变化，作用属性(property)定义变换的具体属性，必须被所在轨道所支持
    
- 属性值(startVal/endVal)
	
	属性值变化，起始帧(startOffsetFrame)的时候展示起始值(startVal)，到结束帧(endOffsetFrame)展示结束值(endVal)

- 插值器(interpolator)
    
    描述属性值从开始值到结束值的变化过程。常见插值器如： 线性插值器(liear)、加速插值器(accelerate)、减速插值器(decelerate)等
    

## 文档



## 贡献

MTAV生成库代码已经开源， 欢迎大家体验使用以及开发更多类型更丰富的轨道，也欢迎大家提issue, 完善文档和补充的更多特性。

## [协议][license]

MTAV is [licensed][license] under the GNU GENERAL PUBLIC LICENSE Version 3.

[logo]: mtav-black.png
[mtav]: https://open.zhiyanxx.com/mtav
[mtav-git]: https://github.com/sailaway/mtav
[license]: LICENSE
[opencv]: https://opencv.org

