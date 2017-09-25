#ifndef MATRIX_H
#define MATRIX_H
#include <iostream>
#include <string>
#include <sys/types.h>
namespace tinycv{

//only supprot .jpg image with 1 channels or 3 channels
class Matrix
{
public:
    //default constructor
    Matrix();
    //constructor with parameters
    Matrix(int rows,int cols,int channels);
    //copy constructor
    Matrix(const Matrix& m);
    //copy assign operator
    Matrix& operator = (const Matrix& m);
    //deconstructor
    ~Matrix();

    //some basic functions
    //clone a Matrix and return a Matrix
    Matrix clone() const;
    //copy the matrix to another matrix
    void copyTo(Matrix& m) const;
    //create a matrix
    void create(int rows, int cols, int channels);

    //returns a matrix's row number
    int row() const;
    //returns a matrix's col number
    int col() const;
    //returns true iff the matrix data is continuous
    bool isContinuous() const;
    //returns true if matrix data is NULL
    bool isEmpty() const;
private:
    int cols;             //col of an image
    int rows;             //row of an image
    int channels;         //channel of an image
    unsigned char* data;  //pointer to the data
};

}
#endif
