#include "Matrix.h"

Matrix::Matrix()
{
    row = 0;
    col = 0;
}

Matrix::Matrix(int n)
{
    row = n;
    col = n;
    vector<double> tempData(size_t(n), 0);
    for (int i = 0; i < n; i++)
    {
        data.push_back(tempData);
    }
}

Matrix::Matrix(int mRow, int mCol)
{
    row = mRow;
    col = mCol;
    vector<double> tempData(size_t(mCol), 0);
    for (int i = 0; i < mRow; i++)
    {
        data.push_back(tempData);
    }
}

Matrix::Matrix(int mRow, int mCol, vector<double> mData)
{
    int i, j;
    if (mData.size() != size_t(mRow*mCol))
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
            oneRow.push_back(mData[size_t(i*mCol + j)]);
        }
        data.push_back(oneRow);
        oneRow.clear();
    }
}

Matrix::Matrix(vector<vector<double>> mData)
{
    row = int(mData.size());
    col = int(mData[0].size());
    data = mData;
}

// 输出结果
void Matrix::print()
{
    int i, j;
    for (i = 0; i < row; i++)
    {
        cout << i << "\t" << "|";
        for (j = 0; j < col; j++)
        {
            cout << fixed << data[size_t(i)][size_t(j)] << "\t|";
        }
        cout << endl;
    }

    cout << endl;
}

// 转置
Matrix Matrix::transposition()
{
    int i, j;
    Matrix outTransMat(col, row);
    for (i = 0; i < row; i++)
    {
        for (j = 0; j < col; j++)
        {
            outTransMat[j][size_t(i)] = data[size_t(i)][size_t(j)];
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

    int i, j;

    // reverse: 利用变换矩阵
    vector<vector<double>> outData = Matrix(row, row).data; //单位阵
    for (i = 0; i < row; i++)
    {
        outData[size_t(i)][size_t(i)] = 1;
    }

    vector<vector<double>> tData = data;                 //原矩阵
    // 若首元素为0，则做初等变换
    if (tData[0][0] == 0.0)
    {
        for (i = 0; i < row; i++)
        {
            if (tData[size_t(i)][0] != 0.0)
            {
                vector<double> tempRow1 = tData[size_t(i)];
                tData[size_t(i)] = tData[0];
                tData[0] = tempRow1;

                vector<double> tempRow2 = outData[size_t(i)];
                outData[size_t(i)] = outData[0];
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
                k = tData[size_t(i)][size_t(j)] / tData[size_t(j)][size_t(j)];
                tData[size_t(i)] = tData[size_t(i)] + tData[size_t(j)] * (-k);
                outData[size_t(i)] = outData[size_t(i)] + outData[size_t(j)] * (-k);
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
    int i, j, N, m, n, b, c; // 计数用
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
    int i;
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
vector<double>& Matrix::operator[](int row)
{
    return this->data[row];
}

void Matrix::normalize()
{
    int i, j;
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

Matrix Matrix::operator()(int r1, int r2, int c1, int c2)
{
    int newHeight, newWidth;
    newHeight = abs(r1 - r2);
    newWidth = abs(c1 - c2);
    if (newHeight == 0 || newWidth == 0)
    {
        qDebug() << "非有效矩形";
        exit(-1);
    }
    if (r1 > r2)
    {
        int temp;
        temp = r1;
        r1 = r2;
        r2 = temp;
    }
    if (c1 > c2)
    {
        int temp;
        temp = c1;
        c1 = c2;
        c2 = temp;
    }
    if (c2>=col || r2>=row)
    {
        qDebug() << "超界";
        exit(-1);
    }
    Matrix newMatrix(newHeight, newWidth);
    int i, j;
    for (i=0; i<newHeight; i++)
    {
        for (j=0; j<newWidth; j++)
        {
            newMatrix[i][j] = data[r1+i][c1+j];
        }
    }
    return newMatrix;
}

double Matrix::max()
{
    double out = data[0][0];
    int i, j;
    for (i=0; i<row; i++)
    {
        for (j=0; j<col; j++)
        {
            if (data[i][j]>out)
                out = data[i][j];
        }
    }
    return out;
}
double Matrix::min()
{
    double out = data[0][0];
    int i, j;
    for (i=0; i<row; i++)
    {
        for (j=0; j<col; j++)
        {
            if (data[i][j]<out)
                out = data[i][j];
        }
    }
    return out;
}
double Matrix::mean()
{
    int i, j;
    double sum = 0.0;
    for (i = 0; i<row; ++i)
    {
        for (j=0; j<col; j++)
        {
            sum += data[i][j];
        }
    }
    return sum/(row*col);
}

Matrix Matrix::operator^(int n)
{
    if (n!=-1 && n!=1)
    {
        qDebug() << "目前只支持求逆";
        exit(-1);
    }
    return n==1 ? transposition() : reverse();
}

double Matrix::tr()
{
    if (col != row)
    {
        qDebug() << "不是方阵，无法求迹";
        exit(-1);
    }
    double out = data[0][0];
    for (size_t i=0; i<col; i++)
    {
        out += data[i][i];
    }
    return out;
}
