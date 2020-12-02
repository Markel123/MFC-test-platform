# MFC-test-platform
This is an vision algrithom test platform based on OpenCV and MFC.
---------------------------------------------------------------
##### 需要注意
* 环境为Windows，VisualStudio2017，编译器版本vc141
* OpenCV版本3.4.10, vc14
* 解决方案配置为 Debug，x64
* 解决方案被加密了，所以需要自己新建解决方案，然后将相关文件放到对应位置

##### 关于初版测试环境 <界面> 的使用说明
* 点击按钮OPEN，打开图片
* 打开图片之后，需要左键先点击一下图片所在的窗口，以启用ROI画框功能
* 画好ROI框后，点击按钮Reset ROI
* 最后点击按钮Analyze，输出分析结果
* 鼠标右键按住不放并拖拽，可以放大当前点击的区域，显示到左上角的小窗口
* Bug1：刚打开exe的时候，左上角的小窗口尺寸没初始化，显得很难看
* Bug2：小窗口应该能够跟随鼠标移动才对

##### 关于初版测试环境 <程序> 部分的说明
###### 目前图像处理逻辑函数: 
 * 用类JINOItem1
 * 声明在JINOImageProcess.h，定义放在JINOImageProcess.cpp

###### 大部分变量放在结构体: 
 * struJINOFeature
 * 若要添加自定义变量(和控件无关的变量)，可以在此结构体内添加，
        (或者其他更好的方法)

###### 点击按钮后执行的函数为:
 * void CJINORunDlg::OnClickedButtonAnalyzePic()
 * 位于文件 “JINORunDlg.cpp”

-------------------------------------------------------------
