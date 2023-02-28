# 配置文件

如下是一个MTAV配置JSON示例，它主要包含输出视频基础属性以及轨道列表两部分

基础属性例如输出视频宽度(width)，输出视频高度(height)等属性，它定义了输出视频的基本参数配置；
轨道列表是视频中展现出来的所有视觉元素集合，每一个轨道代表一块独立的视觉展示元素。

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

