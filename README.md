# Ukive
一个简单的UI框架，动态布局模仿 Android 系统的机制。
采用 Google 的 Material Design 设计标准。
使用 DirectX 11.1 渲染。

![Demo](https://github.com/ucclkp/Ukive/tree/master/Screenshot/demo.png)

DirectXMath 文件夹下为微软的开源数学库。用于支持使用 SSE 指令集。

此为 Windows 应用程序项目，使用 Visual Studio Community 2017 编写。
最低运行环境为 Windows 7 SP1 with DirectX 11.1。
程序中使用了 C++ 的新标准中的特性，如 nullptr，lambda 表达式。
程序中使用 C++ 标准库。

程序入口 WinMain 位于 Main.cpp 文件中。
程序主循环位于 UApplication.cpp 中，窗体也在此定义。
除 Windows 的消息队列外，程序自身维护一消息队列，该消息队列处理由 UCycler 发出的消息。参见 UMessageLooper 类和 UCycler 类。
UCycler 对应于 Android 中的 Handler。

UDeviceManager 类中定义 DirectX 设备。
DirectX 的交换链定义在 URenderer 类中，URenderer 类包含创建一系列与设备相关的资源的方法，也包含绘制方法。每一个窗体都拥有一个 URenderer 类实例。

UWindow 类是窗体的基类，代表一个典型的 Windows 窗体。其子类 LodWindow 为 LOD 相关的窗体。
UWidget 类是控件的基类。控件即是UI中与用户进行具体交互的元素，如按钮，文本框等。
UWidgetGroup 类继承于 UWidget 类，其中可添加 UWidget 和 UWidgetGroup，形成树形结构。

目前有如下预定义控件：
* UTextView: 文本框。支持常规键盘。支持编辑和查看模式。由 TSF 支持输入法输入， Windows10 自带输入法测试通过。暂不支持 RTL。
* UButton: 按钮，使用 Ripple 特效。
* UImageView: 图片框，可显示图像。
* USeekBar: 拖动条。
* UDirect3DView: 3D视图控件。其位置及大小，即是3D视图在窗体上的位置和大小。每个窗体上最多只能有一个。
* USpinnerView: 下拉选择控件（未完成）。
* UBaseLayout: 每个窗体有且只有一个，为控件树的根。
* UFrameLayout: 最简单的布局。功能对应于 Android 的 FrameLayout。
* ULinearLayout: 线性排列布局。功能对应于 Android 的 LinearLayout。
* UListView: 列表（未完成）。
* UScrollView: 滚动布局。功能对应于 Android 的 ScrollView。
* RestrainLayout: 基于关系的布局，较为复杂。功能对应于 Android 的 ConstraintLayout。

## Licenses
* GPL v3.0
* DirectXMath: MIT