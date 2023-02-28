# 插值器

## accelerate 插值器
加速插值器
形状是抛物线的右半部分，起点缓慢，然后加速

### 参数 factor
抛物线参数, 默认为1

### 参数 doubleFactor
抛物线参数, 默认为2

两个抛物线参数，控制抛物线的陡缓程度

## accelerateDecelerate 插值器
先加速后减速插值器
余弦函数的半个周期，起点和终点增长缓慢，而中间快速增长

## anticipate 插值器
起始的时候往回一定值，而后再往前。

### 参数 tension
默认为2.0

## anticipateOvershoot 插值器
起点往回一定值，然后往前，到终点再超出一定值，然后返回

### 参数 tension
默认为3.0

## bounce 插值器
类似于球掉落地面的效果

## cycle
正弦曲线，循环播放cycles次。
cycles取值为2，那么动画时间内函数有2个周期，也就是动画属性值重复变化2次

### 参数 cycles
默认为2

## decelerate 插值器
减速插值器

### 参数 factor
默认为1

## overshoot 插值器
到终点再超出一定值，然后返回

### 参数 tension
默认值为2.0

## linear 插值器
线性插值器

## shake 插值器
震荡插值
类似悬挂小球的摆动效果

### 参数 shakeCycle
震荡周期，默认值为4

### 参数 friction
默认值为 0.857
一般 friction 设置为 ((2\*shakeCycle-2)/(2\*shakeCycle-1))
