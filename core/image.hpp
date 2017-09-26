/*
* @file image.hpp
* @brief tinycv::Image class implementation
*/
#pragma once
#include <vector>
#include "matrix.hpp"
#define TINY_IMG_MAX_DIM  4
namespace tinycv{

    template <class DType>
    class Scalar{
        public:
            Scalar()
            {
                for(int i=0;i<TINY_IMG_MAX_DIM ;i++)
                    _p[i]=nullptr;
            }
        //private:
            int _dim;
            DType* _p[TINY_IMG_MAX_DIM ]; 

    };
    template<typename  T>
	std::ostream &operator<<(std::ostream &os, const Scalar<T> &s)
	{
		os << "[ ";
        for(int i=0;i<s._dim;i++)
        {
            os<<*s._p[i]<<" ";
        }
		os << "]";
		return os;

    }
    std::ostream &operator<<(std::ostream &os, unsigned char &c)
    {
        os<<(int)c;
        return os;

    }


    /*
     * support up to 4 channels image
     */
    template <class DType>
    class Image
    {
    public:
        Image(){_channels=0;_cols=0;_rows=0;}
        Image(int rows,int cols,int channels)
        {
            assert(channels<=TINY_IMG_MAX_DIM );
            _rows=rows;
            _cols=cols;
            _channels=channels;
            for(int i=0;i<_channels;i++)
            {
                _dvec.push_back(Matrix<DType>(_rows,_cols,(DType)0));
            }
        }
        Scalar<DType> at(int row,int col)
		{
            Scalar<DType> s;
            s._dim = _channels;
            for(int i=0;i<_channels;i++)
            {
                std::cout<<_dvec[i]<<std::endl;
                s._p[i] = (DType *)&_dvec[i][row][col];
               
            }
            return s;
		}
    private:
        std::vector<Matrix<DType>> _dvec;
        int _rows;
        int _cols;
        int _channels;
        

    };
}// namespace tinycv

