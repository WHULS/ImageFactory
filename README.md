# Image Factory `[v0.2.0]`

> 图像工厂 (Image Factory) 是基于Qt5和opencv的图像处理程序。

作者：`Kevin Lin [hayzlsls@whu.edu.cn]`

* [Image Factory [v0\.2\.0]](#image-factory-v020)
* [1\. Technology stack](#1-technology-stack)
  * [1\.1\. User Interface](#11-user-interface)
  * [1\.2\. Image Process](#12-image-process)
* [2\. User Guide](#2-user-guide)
  * [2\.1 General Function](#21-general-function)
  * [2\.2 Image Feature Extract/Matching](#22-image-feature-extractmatching)
  * [2\.3 Cloud Extraction](#23-cloud-extraction)

# 1. Technology stack
## 1.1. User Interface
- Qt 5.12.2
- Qt Creator 4.8.2

## 1.2. Image Process
- OpenCV 4.0.1

# 2. User Guide
## 2.1 General Function

`菜单栏 - 图像`提供了打开图像、相机检校的功能。

`菜单栏 - 特征提取` 提供了如 `Canny/Laplacian/LOG` 之类的经典且常见的特征提取算子。

## 2.2 Image Feature Extract/Matching

Using:

`菜单栏 - 特征提取/匹配 - 启动`

Code:

- `/ImageFactory/FeatureWin.cpp`
- `/ImageFactory/FeatureWin.h`


|Function|Index|C++ Function|
|---|---|---|
|打开图像|`图像 - 打开`|`on_open_image_triggered()`
|Wallis 滤波|`图像 - Wallis滤波`|`on_wallis_filter_triggered()`/`Wallis.cpp`/`Wallis.h`
|Moravec点特征提取|`点特征提取 - Moravec`|`on_moravec_fetch_triggered()` + `moravec()`
|Forstner点特征提取|`点特征提取 - Forstner`|`on_forstner_fetch_triggered()` + `forstner()`
|Harris点特征提取|`点特征提取 - Harris`|`on_harris_fetch_triggered()` + `harris()`
|相关系数图像匹配（需要提取角点）|`影像匹配 - 相关系数`|`on_correlation_index_triggered()` + `correlationMatch()` + `correlation()`
|最小二乘影像匹配（无需提取角点）|`影像匹配 - 最小二乘`|`on_least_square_triggered()` + `leastSquare()`(Overload 1)
|最小二乘影像匹配（需要提取角点）|`影像匹配 - 最小二乘plus`|`on_least_square_plus_triggered()` + `leastSquare()`(Overload 2)

## 2.3 Cloud Extraction

Developing...