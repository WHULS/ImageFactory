#include "Matrix.h"

Matrix::Matrix()
{
    row = 0;
    col = 0;
}

Matrix::Matrix(size_t n)
{
    row = n;
    col = n;
    vector<double> tempData(size_t(n), 0);
    for (size_t i = 0; i < n; i++)
    {
        data.push_back(tempData);
    }
}

Matrix::Matrix(size_t mRow, size_t mCol)
{
    row = mRow;
    col = mCol;
    vector<double> tempData(size_t(mCol), 0);
    for (size_t i = 0; i < mRow; i++)
    {
        data.push_back(tempData);
    }
}

Matrix::Matrix(size_t mRow, size_t mCol, vector<double> mData)
{
    size_t i, j;
    if (mData.size() != mRow*mCol)
    {
        cout << "数据大小错误" << endl;
        exit(0);
    }
    row = mRow; col = mCol;
    vector<double> oneRow;
    for (i = 0; i < mRow; i++)
    {
        for (j = 0; j < mCol; j++)
        {
            oneRow.push_back(mData[i*mCol + j]);
        }
        data.push_back(oneRow);
        oneRow.clear();
    }
}

Matrix::Matrix(vector<vector<double>> mData)
{
    row = mData.size();
    col = mData[0].size();
    data = mData;
}

// 输出结果
void Matrix::print()
{
    size_t i, j;
    for (i = 0; i < row; i++)
    {
        cout << i << "\t" << "|";
        for (j = 0; j < col; j++)
        {
            cout << fixed << data[i][j] << "\t|";
        }
        cout << endl;
    }

    cout << endl;
}

// 转置
Matrix Matrix::transposition()
{
    size_t i, j;
    Matrix outTransMat(col, row);
    for (i = 0; i < row; i++)
    {
        for (j = 0; j < col; j++)
        {
            outTransMat[j][i] = data[i][j];
        }
    }

    return outTransMat;
}

// 求逆
Matrix Matrix::reverse()
{
    if (row != col)
    {
        cout << "矩阵非方阵，不存在逆矩阵" << endl;
        exit(-1);
    }

    size_t i, j;

    // reverse: 利用变换矩阵
    vector<vector<double>> outData = Matrix(row, row).data; //单位阵
    for (i = 0; i < row; i++)
    {
        outData[i][i] = 1;
    }

    vector<vector<double>> tData = data;                 //原矩阵
    // 若首元素为0，则做初等变换
    if (tData[0][0] == 0.0)
    {
        for (i = 0; i < row; i++)
        {
            if (tData[i][0] != 0.0)
            {
                vector<double> tempRow1 = tData[i];
                tData[i] = tData[0];
                tData[0] = tempRow1;

                vector<double> tempRow2 = outData[i];
                outData[i] = outData[0];
                outData[0] = tempRow2;
                break;
            }
            if (i == row - 1)
            {
                cout << "矩阵首列为零，无法计算逆矩阵" << endl;
                exit(0);
            }
        }
    }

    if (row == 1)
    {
        outData[0][0] = 1 / data[0][0];
    }
    else
    {
        // 比例系数k
        double k;
        // 先把左下三角化为0
        for (i = 1; i < row; i++)
        {
            for (j = 0; j < i; j++)
            {
                k = tData[i][j] / tData[j][j];
                tData[i] = tData[i] + tData[j] * (-k);
                outData[i] = outData[i] + outData[j] * (-k);
            }
        }
        // 对角元素归一化
        for (i = 0; i < row; i++)
        {
            k = 1 / tData[i][i];
            tData[i] = tData[i] * k;
            outData[i] = outData[i] * k;
        }

        // 再把右上三角化为0
        for (i = 0; i < row - 1; i++)
        {
            for (j = i + 1; j < row; j++)
            {
                k = tData[i][j];
                tData[i] = tData[i] + tData[j] * (-k);
                outData[i] = outData[i] + outData[j] * (-k);
            }
        }
    }//else

    return outData;
}

// 求行列式
double Matrix::det()
{
    if (row != col)
    {
        cout << "不是方阵无法求解行列式" << endl;
        exit(-1);
    }
    vector<vector<double>> a = data;
    size_t i, j, N, m, n, b, c; // 计数用
    int sign = 1; // 行列式符号
    double t, f = 1, sum;
    N = row;

    for (i = 0, j = 0; i < N && j < N; i++, j++)
    {	// 判断对角线上的项是否为0,如果是则进行换行处理
        if (abs(a[i][j])<1e-5)
        {
            for (m = i; m<N && abs(a[m][j])<1e-5; m++);
            if (m == N)
                cout << "行列式的值为0";
            else
            {	// 进行换行处理
                for (n = 0; n < N; n++)
                {
                    t = a[i][n];
                    a[i][n] = a[m][n];
                    a[m][n] = t;
                }
                // 换行行列式变号
                sign = (-1)*sign;
            }
        }
        else
        { // 把行列式化为三角行列式
            for (b = i + 1; b < N; b++)
            {
                t = (-1)*(a[b][j]) / (a[i][j]);
                for (c = j; c < N; c++)
                    a[b][c] = a[b][c] + t*a[i][c];
            }
        }
    }
    // 计算行列式
    for (i = 0; i < N; i++)
        f *= a[i][i];
    // 确定符号
    sum = sign*f;
    return sum;
}


// 转换成向量（一维情况下）
bool Matrix::toVector(vector<double> *outputVec)
{
    if (row != 1 && col != 1)
    {
        qDebug() << "无法转换成向量: " << row << "," << col;
        return false;
    }
    size_t i;
    if (row == 1)
    {
        for (i=0; i<col; i++)
        {
            outputVec->push_back(data[0][i]);
        }
        return true;
    }
    else if (col == 1)
    {
        for (i=0; i<row; i++)
        {
            outputVec->push_back(data[i][0]);
        }
        return true;
    }
    return false;
}

// 括号运算符重载(output)
vector<double>& Matrix::operator[](size_t row)
{
    return this->data[row];
}

void Matrix::normalize()
{
    size_t i, j;
    Matrix out(row, col);
    double sum = 0.0;
    for (j=0; j<col; j++)
    {
        for (i=0; i<row; i++)
        {
            sum += data[i][j];
        }
        for (i=0; i<row; i++)
        {
            data[i][j] /= sum;
        }
    }
}
