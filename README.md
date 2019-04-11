# Image Factory `[v0.2.0]`

> 图像工厂 (Image Factory) 是基于Qt5和opencv的图像处理程序。

作者：`Kevin Lin [hayzlsls@whu.edu.cn]`

* [Image Factory [v0\.2\.0]](#image-factory-v020)
  * [一、技术栈](#%E4%B8%80%E6%8A%80%E6%9C%AF%E6%A0%88)
    * [1\. 用户界面](#1-%E7%94%A8%E6%88%B7%E7%95%8C%E9%9D%A2)
    * [2\. 图像处理](#2-%E5%9B%BE%E5%83%8F%E5%A4%84%E7%90%86)
  * [二、功能](#%E4%BA%8C%E5%8A%9F%E8%83%BD)
    * [v0\.1\.0](#v010)
    * [v0\.2\.0](#v020)
    * [TODO: v0\.3\.0](#todo-v030)

## 一、技术栈
### 1. 用户界面
- Qt 5.12.2

### 2. 图像处理
- OpenCV 4.0.1

## 二、功能
### v0.1.0
- 图像读取与显示
- Canny 算子边缘检测

### v0.2.0
添加边缘提取的其他算法：拉普拉斯（Laplacian）、高斯-拉普拉斯（LOG）、索贝尔（Sobel）、罗伯特（Robert）、普利伊特（Prewitt）、高斯差分算子（DOG）。

- 图像读取与显示
- 边缘检测
  - Canny
  - Laplacian
  - LOG
  - Sobel
  - Robert
  - Prewitt
  - DOG

### TODO: v0.3.0
- 点特征提取
  - Moravec
  - Harris
  - Forstner
  - SIFT
- 影像匹配
  - 相关系数
  - 最小二乘
  - SIFT
- 镜头检校 - 使用 OpenCV 的标准板
- 摄影机定标
 - 二维 DLT
- *三维点云提取与生成
