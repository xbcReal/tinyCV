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
			_refN = 1;
			_data = nullptr;
			_refptr = nullptr;
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
			assert(dataVec.size() > 0&& dataVec[0].size() > 0);
			default_init(dataVec.size(), dataVec[0].size())
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
			return *this;
		}

		DType data_at(int row, int col) const
		{
			return _data[row*_cols + col];
		}
		void set_data(DType v, int row, int col)
		{
			_data[row*cols() + col] = v;
		}

		Matrix (const Matrix& m)
		{
			_cols = m.cols();
			_rows = m.rows();
			_counts = m.counts();
			_data = m.get_ptr();
			m._refN++;
			_refptr = nullptr;
			m._refptr = this;
			_refN = m._refN;
			
		}

		Matrix& operator = (const Matrix& m)
		{
			this->~Matrix();
			_cols = m.cols();
			_rows = m.rows();
			_counts = m.counts();
			_data = m.get_ptr();
			m._refN++;
			_refptr = nullptr;
			m._refptr = this;
			_refN = m._refN;
			return *this;
		}
		

		~Matrix()
		{
			_rows = 0;
			_cols = 0;
			_counts = 0;
			if (_data != nullptr && (--_refN)==0)
			{
				delete[] _data;
				_data = nullptr;
			}
			// unref
			const Matrix *p = _refptr;
			while (p != nullptr)
			{
				p->_refN--;
				p = p->_refptr;
			}
		}

		//some basic functions
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
			if (cols() == m.cols() && rows() == m.rows() && typeid(_data) == typeid(m.get_ptr()))
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
			for (int i = 0; i<sub.rows(); i++)
				for (int j = 0; j<sub.cols(); j++)
					sub[i][j] = this->data_at(i, j) - mat[i][j];
			return sub;
		}
		Matrix operator +(const Matrix& mat)
		{
			assert(cols() == mat.cols() && rows() == mat.rows());
			Matrix add(mat.rows(), mat.cols());
			for (int i = 0; i<add.rows(); i++)
				for (int j = 0; j<add.cols(); j++)
					add[i][j] = this->data_at(i, j) + mat[i][j];
			return add;
		}

		Matrix operator -(void)
		{
			Matrix ret(rows(), cols());
			for (int i = 0; i<counts(); i++)
				ret.get_ptr()[i] = -(this->get_ptr()[i]);
			return ret;

		}
		
		//Matrix operator -(int v)
		//{
		//	Matrix sub(rows(), cols());
		//	for (int i = 0; i<sub.rows(); i++)
		//		for (int j = 0; j<sub.cols(); j++)
		//			sub[i][j] = this->data_at(i, j) - v;
		//	return sub;
		//}

		Matrix operator -(const DType v)
		{
			Matrix sub(rows(), cols());
			for (int i = 0; i<sub.rows(); i++)
				for (int j = 0; j<sub.cols(); j++)
					sub[i][j] = this->data_at(i, j) - v;
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
			for (int i = 0; i<sub.rows(); i++)
				for (int j = 0; j<sub.cols(); j++)
					sub[i][j] = this->data_at(i, j) + v;
			return sub;
		}
		Matrix operator *(const DType v) {
			Matrix sub(rows(), cols());
			for (int i = 0; i < sub.rows(); i++) {
				for (int j = 0; j < sub.cols(); j++) {
					sub[i][j] = this->data_at(i, j) * v;
				}
			}
			return sub;
		}
		Matrix operator /(const DType v)
		{
			return 1.0 / v*(*this);
		}


	private:
		void default_init(int rows,int cols)
		{
			_rows = rows;
			_cols = cols;
			_refN = 1;
			_counts = rows*cols;
			_refptr = nullptr;
			try
			{
				_data = new DType[_counts];
			}
			catch (const std::bad_alloc &e)
			{
				std::cerr << e.what() << std::endl;
			}
		}
		int _cols;             
		int _rows;             
		int _counts;
		DType* _data;
		mutable int _refN;//  引用计数
		mutable const Matrix *_refptr;// 记录谁引用了此对象
	};


	template<typename  T>
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
		};
		os << "]" << std::endl;
		return os;

	}
	std::ostream &operator<<(std::ostream &os, uchar c)
	{
		os << (int)c;
		return os;
	}
>>>>>>> lhw

}//namespace tinycv

#endif // TINYCV_CORE_MATRIX_HPP
