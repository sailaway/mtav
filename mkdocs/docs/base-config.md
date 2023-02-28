# 基础属性配置

```
{
    "width": 428,
    "height": 428,
    "bgFile":"",
    "bgColor":"#fff",
    "frameCount":120,
    "fps":24,
    "outVideoFile":"/data/image-rotate.mp4",
    ...
}    
```

MTAV视频生成配置文件的基础属性主要用来指定输出视频的参数，参数有：



## width
输出视频宽度，整数类型

## height
输出视频高度，整数类型

## bgFile
输出视频的背景图片文件路径，字符类型。

**背景图片文件的尺寸必须和输出视频宽高一致**

## bgColor
输出视频的背景颜色，字符类型； 示例 "#000","#ff8899"

**如果有配置 `bgFile` 属性， bgColor将被忽略**

## fps
输出视频的帧率，整数类型； 通常情况下生成普通视频，指定为24即可

## frameCount
输出视频帧总数，整数类型； 用视频帧总数来指定视频时长，  视频时长 = frameCount / fps

## outVideoFile
输出视频路径 字符类型