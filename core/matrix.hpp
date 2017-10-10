/*
* @file matrix.hpp
* @brief tinycv::Matrix class implementation
*/
#pragma once
#ifndef  TINYCV_CORE_MATRIX_HPP
#define  TINYCV_CORE_MATRIX_HPP

#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <iostream>
#include <iomanip>
#include <vector>
#include <typeinfo>
#include <memory.h>

typedef unsigned char uchar;
typedef unsigned int uint;
typedef float float32;
typedef double float64;
namespace tinycv {


/*
 DType has 3 types:
 uchar for integral image,its range is [0,255]
 float for decimal image,its range is [0,1]
 double for cnn feature maps or other math matrix,it's range is double's range
*/
template<class DType>
class Matrix
{
public:
    Matrix()
    {
        _cols=0;
        _rows=0;
        _counts = 0;
        _data = nullptr;
    }

    Matrix(int rows, int cols)
    {
        default_init(rows, cols);
        memset((void*)_data, 0, _counts * sizeof(DType));
    }

    Matrix(int rows, int cols, const DType* data)
    {
        default_init(rows, cols);
        memcpy(_data, data, _counts * sizeof(DType));
    }


    Matrix(const std::vector<std::vector<DType> >  & dataVec)
    {
        assert(dataVec.size() > 0 && dataVec[0].size() > 0);
        default_init(dataVec.size(), dataVec[0].size());
        for (int i = 0; i < _rows; i++)
            for (int j = 0; j < _cols; j++)
                _data[i*_cols + j] = dataVec[i][j];
    }

    Matrix(int rows, int cols, DType value)
    {
        default_init(rows, cols);
        for (int i = 0; i<_counts; i++)
            _data[i] = value;
    }

    DType *row_ptr(int row) const
    {
        return &_data[row*_cols];
    }
    DType *col_ptr(int col) const
    {
        return &_data[col];
    }
    DType *get_ptr() const
    {
        return _data;
    }
    Matrix operator &(const Matrix &mat)
    {
        std::cout << "in & " << std::endl;
        //???what to do
        return *this;
    }

    DType data_at(int row, int col) const
    {
        assert(row >= 0 && row < _rows && col >= 0 && col < _cols);
        return _data[row*_cols + col];
    }
    void set_data(DType v, int row, int col)
    {
        _data[row*_cols + col] = v;
    }

    Matrix (const Matrix& m)
    {
        default_init(m.rows(), m.cols());
        memcpy(_data,m.get_ptr(),_counts  * sizeof(DType));
        std::cout << "in Matrix (const Matrix& m)" << std::endl;

    }

    Matrix& operator = (const Matrix& m)
    {
        if(this != &m){
            this->~Matrix();
            default_init(m.rows(), m.cols());
            memcpy(_data,m.get_ptr(),_counts  * sizeof(DType));
            return *this;
        }
        else{
            return *this;
        }
    }


    ~Matrix()
    {
        _rows = 0;
        _cols = 0;
        _counts = 0;
        if (_data != nullptr)
        {
            delete[] _data;
            _data = nullptr;
        }
        // unref
    }

    //some basic functionsfunctions
    //clone a Matrix and return a Matrix
    Matrix clone() const
    {
        Matrix m(_rows,_cols);
        memcpy(m.get_ptr(), get_ptr(), counts() * sizeof(DType));
        return m;
    }

    //copy the matrix to another matrix
    void copyTo(Matrix& m) const
    {
        assert(this != &m);
        if (_cols == m.cols() && _rows == m.rows() && typeid(_data) == typeid(m.get_ptr()))
        {
            memcpy(m.get_ptr(), get_ptr(),  counts() * sizeof(DType));
        }
        else
        {
            m.~Matrix();
            m._rows = _rows;
            m._cols = _cols;
            m._counts = _rows*_cols;
            m._data = new DType[_counts];
            memcpy(m.get_ptr(), get_ptr(), counts() * sizeof(DType));
        }

    }

    //returns a matrix's rows
    int rows() const
    {
        return _rows;
    }

    //returns a matrix's cols
    int cols() const
    {
        return _cols;
    }

    int counts() const
    {
        return _counts;
    }

    DType *  operator[](int k) const
    {
        return &_data[k * _cols];
    }

    //returns true if matrix data is NULL
    bool isEmpty() const
    {
        return (_data == nullptr);
    }


    Matrix operator -(const Matrix & mat)
    {
        assert(cols() == mat.cols() && rows() == mat.rows());
        Matrix sub(mat.rows(), mat.cols());
        if(!strcmp(typeid(DType).name(),"h")){
            for (int i = 0; i<sub.rows(); i++)
                for (int j = 0; j<sub.cols(); j++)
                    sub[i][j] = std::min(std::max(DType(this->data_at(i, j) - mat[i][j]),DType(0)),DType(255));
        }
        else if(!strcmp(typeid(DType).name(),"f")){
            for (int i = 0; i<sub.rows(); i++)
                for (int j = 0; j<sub.cols(); j++)
                    sub[i][j] = std::min(std::max(DType(this->data_at(i, j) - mat[i][j]),DType(0)),DType(1));
        }
        else if(!strcmp(typeid(DType).name(),"d")){
            //just sub without other limits
            for (int i = 0; i<sub.rows(); i++)
                for (int j = 0; j<sub.cols(); j++)
                    sub[i][j] = this->data_at(i, j) - mat[i][j];
        }
        else{
            std::cout << "typeid:" << typeid(DType).name() << std::endl;
            abort();
        }
        return sub;
    }
    Matrix operator +(const Matrix& mat)
    {
        assert(cols() == mat.cols() && rows() == mat.rows());
        //assert(typeid(DType))
        Matrix add(mat.rows(), mat.cols());
        if(!strcmp(typeid(DType).name(),"h")){
            std::cout << "uchar" <<std::endl;
            for (int i = 0; i<add.rows(); i++)
                for (int j = 0; j<add.cols(); j++)
                    add[i][j] = std::min(std::max(DType(this->data_at(i, j) + mat[i][j]),DType(0)),DType(255));
        }
        else if(!strcmp(typeid(DType).name(),"f")){
            for (int i = 0; i<add.rows(); i++)
                for (int j = 0; j<add.cols(); j++)
                    add[i][j] = std::min(std::max(DType(this->data_at(i, j) + mat[i][j]),DType(0)),DType(1));
        }
        else if(!strcmp(typeid(DType).name(),"d")){
            //just add without other limits
            for (int i = 0; i<add.rows(); i++)
                for (int j = 0; j<add.cols(); j++)
                    add[i][j] = DType(this->data_at(i, j) + mat[i][j]);
        }
        else{
            std::cout << typeid(DType).name() << std::endl;
            abort();
        }
        return add;
    }

    Matrix operator -(void)
    {
        assert(!strcmp(typeid(DType).name(),"d"));
        Matrix ret(rows(), cols());
        for (int i = 0; i< _counts; i++)
            ret.get_ptr()[i] = -(this->get_ptr()[i]);
        return ret;
    }

    Matrix operator -(const DType v)
    {
        Matrix sub(rows(), cols());
        if(!strcmp(typeid(DType).name() ,"h")){
            for (int i = 0; i<sub.rows(); i++)
                for (int j = 0; j<sub.cols(); j++)
                    sub[i][j] = std::min(std::max(DType(this->data_at(i, j) - v),DType(0)),DType(255));
        }
        else if(!strcmp(typeid(DType).name(),"f")){
            for (int i = 0; i<sub.rows(); i++)
                for (int j = 0; j<sub.cols(); j++)
                    sub[i][j] = std::min(std::max(DType(this->data_at(i, j) - v),DType(0)),DType(1));
        }
        else if(!strcmp(typeid(DType).name(),"d")){
            //just minus without other limits
            for (int i = 0; i<sub.rows(); i++)
                for (int j = 0; j<sub.cols(); j++)
                    sub[i][j] = this->data_at(i, j) - v;
        }
        else{
             std::cout << "typeid:" << typeid(DType).name() << std::endl;
             abort();
        }
        return sub;
    }

    //Matrix operator +(int v)
    //{
    //	Matrix sub(rows(), cols());
    //	for (int i = 0; i<sub.rows(); i++)
    //		for (int j = 0; j<sub.cols(); j++)
    //			sub[i][j] = this->data_at(i, j) + v;
    //	return sub;
    //}

    Matrix operator +(const DType v)
    {
        Matrix sub(rows(), cols());
        if(!strcmp(typeid(DType).name() ,"h")){
            for (int i = 0; i<sub.rows(); i++)
                for (int j = 0; j<sub.cols(); j++)
                    sub[i][j] = std::min(std::max(DType(this->data_at(i, j) + v),DType(0)),DType(255));
        }
        else if(!strcmp(typeid(DType).name(),"f")){
            for (int i = 0; i<sub.rows(); i++)
                for (int j = 0; j<sub.cols(); j++)
                    sub[i][j] = std::min(std::max(DType(this->data_at(i, j) + v),DType(0)),DType(1));
        }
        else if(!strcmp(typeid(DType).name(),"d")){
            for (int i = 0; i<sub.rows(); i++)
                for (int j = 0; j<sub.cols(); j++)
                    sub[i][j] = this->data_at(i, j) + v;
        }
        else{
            std::cout << "typeid:" << typeid(DType).name() << std::endl;
            abort();
        }
        return sub;
    }
    Matrix operator *(const DType v) {
        Matrix sub(rows(), cols());
        if(!strcmp(typeid(DType).name(),"h")){
            for (int i = 0; i < sub.rows(); i++) {
                for (int j = 0; j < sub.cols(); j++) {
                    sub[i][j] = std::min(std::max(DType(this->data_at(i, j) * v),DType(0)),DType(255));
                }
            }
        }
        else if(!strcmp(typeid(DType).name(),"f")){
            for (int i = 0; i < sub.rows(); i++) {
                for (int j = 0; j < sub.cols(); j++) {
                    sub[i][j] = std::min(std::max(DType(this->data_at(i, j) * v),DType(0)),DType(1));
                }
            }
        }
        else if(!strcmp(typeid(DType).name(),"d")){
            for (int i = 0; i < sub.rows(); i++) {
                for (int j = 0; j < sub.cols(); j++) {
                    sub[i][j] = this->data_at(i, j) * v;
                }
            }
        }
        else{
            std::cout << "typeid:" << typeid(DType).name() << std::endl;
            abort();
        }

        return sub;
    }
    Matrix operator /(const DType v)
    {
        return (*this) *  DType(1.0 / v);
    }

    //matrix *
    Matrix operator *(const Matrix& mat){
        //std::cout << "_cols is:" << _cols << std::endl;
        //std::cout << "mat.rows() is:" << mat.rows() << std::endl;
        assert(_cols == mat.rows() && !strcmp(typeid(DType).name(),"d"));
        Matrix sub(_rows,mat.cols());
        for(int i = 0;i < sub._rows;i++){
            for(int j = 0;j < sub._cols;j++){
                sub[i][j] = 0.0;
                std::vector<DType> rowData = this->get_row(i);
                std::vector<DType> colData = mat.get_col(j);
                assert(rowData.size() == colData.size());
                for(int k = 0;k < rowData.size();k++){
                    sub[i][j] += rowData[k] * colData[k];
                }
            }
        }
        return sub;

    }

    //matrix .*(replace it with mul)
    Matrix mul(const Matrix& mat){
        assert(_cols == mat.cols() && _rows == mat.rows() && !strcmp(typeid(DType).name(),"d"));
        Matrix sub(_rows,_cols);
        for(int i = 0;i < sub._rows;i++){
            for(int j = 0;j < sub._cols;j++){
                sub[i][j] = data_at(i,j) * mat.data_at(i,j);
            }
        }
        return sub;
    }

    //matrix ./(replace it with div)
    Matrix div(const Matrix& mat){
        assert(_cols == mat.cols() && _rows == mat.rows() && !strcmp(typeid(DType).name(),"d"));
        Matrix sub(_rows,_cols);
        for(int i = 0;i < sub._rows;i++){
            for(int j = 0;j < sub._cols;j++){
                sub[i][j] = data_at(i,j) / (double)mat.data_at(i,j);
            }
        }
        return sub;
    }

    //matrix transponse
    Matrix trans(){
        Matrix sub(_cols,_rows);
        for(int i = 0;i < sub._rows;i++){
            for(int j = 0;j < sub._cols;j++){
                sub[i][j] = this->data_at(j,i);
            }
        }
        return sub;
    }
    //matrix inv,until now,we only support square matrix to get its inv matrix and if the dimension is a little bigger ,the program would be pretty slow
    //because until now we use adjoint matrix to get its inv matrix.And we will try to use other methods to get the inv matrix to support the none square
    //and biiger matrix to get its inv matrixt
    //program cant get the right inverse matrix,need to be fixed...
    /*Matrix inv(){
        assert(_cols == _rows && !strcmp(typeid(DType).name(),"d"));
        int n = _cols;
        int i,j,k,m=2*n;
        Matrix a(_cols,2 * _cols);
        Matrix B(_cols,_cols);
        double mik,temp;
        for(i=0;i<n;i++)
        {
            for(j=0;j<n;j++)
            {
                if(i==j)
                    B[i][j]=1.0;
                else
                    B[i][j]=0.0;
            }
        }                                //初始化B=E

        int iii= 0;
        for(i=0;i<n;i++){
            for(j=0;j<n;j++){
                a[i][j]=(*this)[i][j];  //复制A到a，避免改变A的值
                //iii++;
            }
        }

        for(i=0;i<n;i++)
            for(j=n;j<m;j++)
                a[i][j]=B[i][j-n];      //复制B到a，增广矩阵

        for(k=1;k<=n-1;k++)
        {
            for(i=k+1;i<=n;i++)
            {
                mik=a[i-1][k-1]/a[k-1][k-1];
                for(j=k+1;j<=m;j++)
                {
                    a[i-1][j-1]-=mik*a[k-1][j-1];
                }
            }
        }                              //顺序高斯消去法化左下角为零

        for(i=1;i<=n;i++)
        {
            temp=a[i-1][i-1];
            for(j=1;j<=m;j++)
            {
                a[i-1][j-1]/=temp;
            }
        }                             //归一化

        for(k=n-1;k>=1;k--)
        {
            for(i=k;i>=1;i--)
            {
                mik=a[i-1][k];
                for(j=k+1;j<=m;j++)
                {
                    a[i-1][j-1]-=mik*a[k][j-1];
                }
            }
        }                            //逆序高斯消去法化增广矩阵左边为单位矩阵

        for(i=0;i<n;i++){
            for(j=0;j<n;j++){
                if(abs(a[i][j+n]) < 0.0001){
                    B[i][j]=0.0;
                }
                else{
                    B[i][j] = a[i][j+n];
                }
            }
        }
        return B;
    }*/

private:
    void default_init(int rows,int cols)
    {
        _rows = rows;
        _cols = cols;
        _counts = rows*cols;
        try
        {
            _data = new DType[_counts];
        }
        catch (const std::bad_alloc &e)
        {
            std::cerr << e.what() << std::endl;
        }
    }
    //index from 0 to _rows-1
    std::vector<DType> get_row(const int row) const{
        std::vector<DType> rowData;
        int base = row * _cols;
        for(int i = 0;i < _cols;i++){
            rowData.push_back(_data[base+i]);
        }
        return rowData;
    }
    //index from 0 to _cols-1    
    std::vector<DType> get_col(const int col) const{
        std::vector<DType> colData;   
        for(int i = col;i < _counts;i+=_cols){
            colData.push_back(_data[i]);
        }     
        return colData;
    }
private:
    int _cols;
    int _rows;
    int _counts;
    DType* _data;
};


template<typename T>
std::ostream &operator<<(std::ostream &os, const Matrix<T> &mat)
{
    os << "[ " << std::endl;
    for (int i = 0; i<mat.rows(); i++)
    {
        for (int j = 0; j<mat.cols(); j++)
        {
            os << std::setprecision(5) << mat[i][j];
            if (j != mat.cols() - 1)
                os << " ,";
        }
        os << ";" << std::endl;
    }
    os << "]" << std::endl;
    return os;
}
}//namespace tinycv
typedef tinycv::Matrix<double> Matrixd;
typedef tinycv::Matrix<uchar> Matu;
typedef tinycv::Matrix<float> Matf;
#endif // TINYCV_CORE_MATRIX_HPP
