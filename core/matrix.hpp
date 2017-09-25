/*
* @file matrix.hpp
* @brief tinycv::Matrix class implementation
*/
#pragma once
#ifndef  TINYCV_CORE_MATRIX_HPP
#define  TINYCV_CORE_MATRIX_HPP

#include <stdlib.h>
#include <assert.h>
#include <iostream>
#include <iomanip>
#include <vector>
#include <typeinfo>

typedef unsigned char uchar;
typedef float float32;
typedef double float64;

namespace tinycv {

	/*
	 */
	template<class Dtype=uchar>
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
		}
		
		Matrix(int rows, int cols)
		{
			_rows = rows;
			_cols = cols;
			_refN = 1;
			_counts = rows*cols;
			_data = new Dtype[_counts];
			memset((void*)_data, 0, _counts * sizeof(Dtype));
		}

		Matrix(int rows, int cols, const Dtype* data)
		{
			_rows = rows;
			_cols = cols;
			_refN = 1;
			_counts = rows*cols;
			_data = new Dtype[counts_];
			memcpy(data_, data, counts_ * sizeof(Dtype));

		}
		Matrix(const std::vector<std::vector<Dtype> >  & dataVec)
		{
			assert(dataVec.size() > 0&& dataVec[0].size() > 0);
			_rows = dataVec.size();
			_cols = dataVec[0].size();
			_refN = 1;
			_counts = _rows*_cols;
			_data = new Dtype[_counts];
			for (int i = 0; i < _rows; i++)
				for (int j = 0; j < _cols; j++)
					_data[i*_cols + j] = dataVec[i][j];
		}

		Matrix(int rows, int cols, Dtype value)
		{
			_rows = rows;
			_cols = cols;
			_refN = 1;
			_counts = rows*cols;
			_data = new Dtype[_counts];
			for (int i = 0; i<_counts; i++)
				_data[i] = value;
		}

		Dtype *row_ptr(int row) const
		{
			return &_data[row*_cols];
		}
		Dtype *col_ptr(int col) const
		{
			return &_data[col];
		}
		Dtype *get_ptr() const
		{
			return _data;
		}
		Matrix operator &(const Matrix &mat)
		{
			return *this;
		}

		Dtype data_at(int row, int col) const
		{
			return _data[row*_cols + col];
		}
		void set_data(Dtype v, int row, int col)
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
		}

		Matrix& operator = (const Matrix& m)
		{
			this->~Matrix();
			_cols = m.cols();
			_rows = m.rows();
			_counts = m.counts();
			_data = m.get_ptr();
			m._refN++;
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
		}

		//some basic functions
		//clone a Matrix and return a Matrix
		Matrix clone() const
		{
			Matrix m(_rows,_cols);
			memcpy(m.get_ptr(), get_ptr(), counts() * sizeof(Dtype));
			return m;
		}

		//copy the matrix to another matrix
		void copyTo(Matrix& m) const
		{
			if (cols() == m.cols() && rows() == m.rows() && typeid(_data) == typeid(m.get_ptr()))
			{
				memcpy(m.get_ptr(), get_ptr(),  counts() * sizeof(Dtype));
			}
			else
			{
				m.~Matrix();
				m._rows = _rows;
				m._cols = _cols;
				m._counts = _rows*_cols;
				m._data = new Dtype[_counts];
				memcpy(m.get_ptr(), get_ptr(), counts() * sizeof(Dtype));
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

		Dtype *  operator[](int k) const
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
		
		Matrix operator -(int v)
		{
			Matrix sub(rows(), cols());
			for (int i = 0; i<sub.rows(); i++)
				for (int j = 0; j<sub.cols(); j++)
					sub[i][j] = this->data_at(i, j) - v;
			return sub;
		}

		Matrix operator -(const Dtype v)
		{
			Matrix sub(rows(), cols());
			for (int i = 0; i<sub.rows(); i++)
				for (int j = 0; j<sub.cols(); j++)
					sub[i][j] = this->data_at(i, j) - v;
			return sub;
		}

		Matrix operator +(int v)
		{
			Matrix sub(rows(), cols());
			for (int i = 0; i<sub.rows(); i++)
				for (int j = 0; j<sub.cols(); j++)
					sub[i][j] = this->data_at(i, j) + v;
			return sub;
		}

		Matrix operator +(const Dtype v)
		{
			Matrix sub(rows(), cols());
			for (int i = 0; i<sub.rows(); i++)
				for (int j = 0; j<sub.cols(); j++)
					sub[i][j] = this->data_at(i, j) + v;
			return sub;
		}

		Matrix operator /(const Dtype v)
		{
			return 1.0 / v*(*this);
		}


	private:
		friend class Matrix;
		int _cols;             
		int _rows;             
		int _counts;
		mutable int _refN;
		Dtype* _data;
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

}//namespace tinycv

#endif // TINYCV_CORE_MATRIX_HPP
