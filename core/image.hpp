 /*
* @file Image_.hpp
* @brief tinycv::Image_ and Scalar_ class implementation
*/
#pragma once
#include <vector>
#include <stdarg.h>
#include "matrix.hpp"
#define TINY_IMG_MAX_DIM  4
namespace tinycv{
template <class DType=float32>
class Scalar_{
public:
    Scalar_()
    {
        _dim = TINY_IMG_MAX_DIM;
        for (int i = 0; i < TINY_IMG_MAX_DIM; i++)
        {
            _d[i] = (DType)0;
            _p[i] = &_d[i];
        }
    }
    Scalar_(DType s0)
    {
        _dim = 1;
        for (int i = 0; i < TINY_IMG_MAX_DIM; i++)
        {
            _d[i] = (DType)0;
            _p[i] = &_d[i];
        }
        _d[0] = s0;
    }

    Scalar_(DType s0, DType s1, DType s2)
    {
        _dim = 3;
        for (int i = 0; i < TINY_IMG_MAX_DIM; i++)
        {
            _d[i] = (DType)0;
            _p[i] = &_d[i];
        }
        _d[0] = s0; _d[1] = s1; _d[2] = s2;
        //_n_init(3, (DType)s0, (DType)s1, (DType)s2);
    }
    Scalar_(DType s0, DType s1, DType s2, DType s3)
    {
        _dim = 4;
        for (int i = 0; i < TINY_IMG_MAX_DIM; i++)
        {
            _d[i] = (DType)0;
            _p[i] = &_d[i];
        }
        _d[0] = s0; _d[1] = s1; _d[2] = s2; _d[3] = s3;
    }
    Scalar_(const Scalar_& s)
    {
        _dim = s.dim();
        for (int i = 0; i < TINY_IMG_MAX_DIM;i++)
        {
            _d[i] = s._d[i];
            _p[i] = s._p[i];
        }
    }
    Scalar_ & operator = (const Scalar_ &s)
    {
        assert(_dim == s.dim());
        for (int i = 0; i < _dim; i++)
        {
            _d[i] = s[i];
            *_p[i] = s[i];
        }
        return *this;
    }
    DType  operator[](int i) const
    {
        assert(i >=0 && i < _dim);
        return _d[i];
    }
    void set_data(int i,DType *p)
    {
        _p[i] = p;
        _d[i] = *p;
    }
    int dim() const  { return _dim; }
private:
    //void _n_init(uint n, ...)
    //{
    //	va_list args = NULL;
    //	va_start(args, n);
    //	_dim = n;
    //	for (uint i = 0; i < n; i++)
    //	{
    //		_d[i] = va_arg(args, DType);
    //		_p[i] = &_d[i];
    //	}
    //	va_end(args);
    //}
    template <class T> friend class Image_;
    int _dim;
    DType* _p[TINY_IMG_MAX_DIM]; // ��¼ֵ�ĵ�ַ
    DType _d[TINY_IMG_MAX_DIM]; //

};
template<typename  T>
std::ostream &operator<<(std::ostream &os, const Scalar_<T> &s)
{
    os << "[ ";
    for(int i=0;i<s.dim();i++)
    {
        os<< std::setprecision(5)<<s[i]<<"  ";
    }
    os << "]";
    return os;
}

//class Point_,we only support 2d point(_x,_y) because we may not use the depth infomation _z
template <class DType>
class Point_{
public:
    //default constructor
    Point_(DType x = 0,DType y = 0):_x(x),_y(y){}
    //copy constructor
    Point_(const Point_& pt):_x(pt.x()),_y(pt.y()){}
    //copy assign operator
	Point_ operator =(Point_ pt) { _x = pt.x(); _y = pt.y(); return *this; }
    int operator [](int i){
        assert(i == 0 || i == 1);
        return i?_y:_x;
    }
    //get the x coordinate
    DType x()const{return _x;}
    //get the y coordinate
    DType y()const{return _y;}
private:
    template <class T> friend class Image_;
    DType _x;
    DType _y;
};
typedef Point_<int> Point;
typedef Point_<float> Pointf;
typedef Point_<double> Pointd;

/*
     * support up to 4 channels Image_
     */
template <class DType>
class Image_
{
public:
    Image_(){_channels=0;_cols=0;_rows=0;}
    Image_(int rows,int cols,int channels, DType value=0, const DType*from=nullptr)
    {
        assert(channels<=TINY_IMG_MAX_DIM );
        assert(rows > 0 && cols > 0);
        _rows=rows;
        _cols=cols;
        _channels=channels;
        for(int i=0;i<_channels;i++)
        {
            _dvec.push_back(Matrix<DType>(_rows, _cols, value));
        }
		if (from != nullptr)
		{
			for (int i = 0; i < rows; i++)
				for (int j = 0; j < cols; j++)
					for (int c = 0; c < channels; c++)
						_dvec[c][i][j] = from[i*cols*channels + j*channels + c];
		}
    }
    Scalar_<DType> at(int row,int col)
    {
        assert(row >= 0 && row < _rows);
        assert(col >= 0 && col < _cols);        
        Scalar_<DType> s;
        s._dim = _channels;
        for(int i=0;i<_channels;i++)
            s.set_data(i, &_dvec[i][row][col]);
        return s;
    }
    template <class T>    
    Image_ roi(Point_<T> tl,Point_<T> br){
        assert(br._x > tl._x && br._y > tl._y);
        assert(tl._x >= 0 && br._x < _cols);
        assert(tl._y >= 0 && br._y < _rows);
        Image_<DType> roiImage(br._y - tl._y + 1,br._x - tl._x + 1,_channels);
        //for(int i = 0;i < _channels;i++){
            for(int j = 0;j < roiImage._rows;j++){
                for(int k = 0;k < roiImage._cols;k++){
                    roiImage.at(j,k) = this->at(j + tl._y,k + tl._x);
                }
            }
        //}
        return roiImage;
    }
    template <class T>        
    Image_ roi(T x,T y,T width,T height){
        Point_<T> tl = Point_<T>(x,y);
        Point_<T> br = Point_<T>(x + width - 1,y + height - 1);
        Image_ roiImage = roi(tl,br);
        return roiImage;
    }
    bool has_data()const {return _dvec.size() > 0;}
    int  rows() const  { return _rows; }
    int  cols() const  { return _cols; }
    int  channels() const  { return _channels; }
private:
    friend int imsave(const Image_<uchar> &image, const char *filename);
    std::vector<Matrix<DType>> _dvec;
    int _rows;
    int _cols;
    int _channels;
};
typedef  Image_<uchar> Image;
typedef  Scalar_<uchar> Scalar;

typedef  Image_<float> Image_f;
typedef  Scalar_<float> Scalar_f;

}// namespace tinycv

