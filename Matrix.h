#ifndef MATRIX_H
#define MATRIX_H

#include <iostream>
#include <vector>
// 调试输出用
#include <QDebug>
#include <QString>
using namespace std;

class Matrix
{
// Method
public:
    Matrix();
    Matrix(size_t n);
    Matrix(size_t mRow, size_t mCol);
    Matrix(size_t mRow, size_t mCol, vector<double> mData);
    Matrix(vector<vector<double>> mData);

    // 输出结果
    void print();
    // 转置
    Matrix transposition();
    // 求逆
    Matrix reverse();
    // 求行列式
    double det();
    // 转换成向量（一维情况下）
    bool toVector(vector<double>*);
    // 归一化
    void normalize();

// Attribute
public:
    size_t row;
    size_t col;
    vector<vector<double>> data;

public:
    // 括号运算符重载(返回引用才能进行赋值操作)
    vector<double>& operator[](size_t row);
};

// Operator
/* 1. operator: 向量乘某个数 */
static vector<double> operator*(vector<double> v1, double n)
{
    vector<double> outV(v1.size());
    for (size_t i = 0; i < v1.size(); i++)
    {
        outV[i] = v1[i] * n;
    }
    return outV;
}
/* 2. operator: 向量相加 */
static vector<double> operator+(vector<double> v1, vector<double> v2)
{
    if (v1.size() != v2.size())
    {
        qDebug() << "向量长度不匹配";
        exit(-1);
    }
    vector<double> outV(v1.size());
    for (size_t i = 0; i < v1.size(); i++)
    {
        outV[i] = v1[i] + v2[i];
    }

    return outV;
}
/* 3. operator: 矩阵相乘 */
static Matrix operator*(Matrix m1, Matrix m2)
{
    size_t i, j, k;
    size_t h1 = m1.row;
    size_t h2 = m2.row;
    size_t w1 = m1.col;
    size_t w2 = m2.col;

    if (w1 != h2)
    {
        qDebug() << "矩阵行列不匹配";
        exit(-1);
    }

    // 计算及输出
    Matrix outMat(h1, w2);
    double tResult;
    for (i = 0; i < h1; i++)
    {
        for (j = 0; j < w2; j++)
        {
            tResult = 0.0;
            for (k = 0; k < w1; k++)
            {
                tResult += m1[i][k] * m2[k][j];
            }
            outMat[i][j] = tResult;
        }
    }

    return outMat;
}

/* 4. operator: 矩阵相加*/
static Matrix operator+(Matrix m1, Matrix m2)
{
    size_t r, r2, c, c2;
    r = m1.row; c = m1.col;
    r2 = m2.row; c2 = m2.col;
    if (r != r2 || c != c2)
    {
        qDebug() << "两个矩阵尺寸不一致";
        exit(-1);
    }
    Matrix outMat(r, c);
    size_t i, j;
    for (i=0; i<r; i++)
    {
        for (j=0; j<c; j++)
        {
            outMat[i][j] = m1[i][j] + m2[i][j];
        }
    }
    return outMat;
}
/* 5. operator: 矩阵相减*/
static Matrix operator-(Matrix m1, Matrix m2)
{
    size_t r, r2, c, c2;
    r = m1.row; c = m1.col;
    r2 = m2.row; c2 = m2.col;
    if (r != r2 || c != c2)
    {
        qDebug() << "两个矩阵尺寸不一致";
        exit(-1);
    }
    Matrix outMat(r, c);
    size_t i, j;
    for (i=0; i<r; i++)
    {
        for (j=0; j<c; j++)
        {
            outMat[i][j] = m1[i][j] - m2[i][j];
        }
    }
    return outMat;
}
/* 6. operator: 矩阵除一个数*/
static Matrix operator/(Matrix m, double n)
{
    if (n < 1e-5)
    {
        qDebug() << "除数为0";
        exit(-1);
    }
    size_t r, c;
    r = m.row; c = m.col;
    Matrix outMat = m;
    size_t i, j;
    for (i=0; i<r; i++)
    {
        for (j=0; j<c; j++)
        {
            outMat[i][j] /= n;
        }
    }
    return outMat;
}

#endif // MATRIX_H

