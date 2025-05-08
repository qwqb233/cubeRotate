/*
 * @Author: qwqb233 57853293+qwqb233@users.noreply.github.com
 * @Date: 2025-05-02 21:49:05
 * @LastEditors: qwqb233 57853293+qwqb233@users.noreply.github.com
 * @FilePath: \C3_TFT\include\linera.h
 * @Description: 
 */
#pragma once

#include <vector>

typedef struct matrix
{
    int * dims;
    int dims_num;
    std::vector<double> data;
    int data_size;
}matrix_t;

class Matrix
{
    public:
        Matrix(int * dims, int dims_num, double * data);
        Matrix(int * dims, int dims_num, std::vector<double> data);
        ~Matrix();
        matrix_t operator+(const Matrix &other);
        matrix_t operator+(const double &other);
        matrix_t operator-(const Matrix &other);
        matrix_t operator-(const double &other);
        matrix_t operator*(const Matrix &other);
        matrix_t operator*(const double &other);
        matrix_t operator/(const double &other);
    protected:
        matrix_t * matrix;

    private:
        bool matrix_equal(matrix_t * matrix1, matrix_t * matrix2);
        bool matrix_can_multiply(matrix_t * matrix1, matrix_t * matrix2);
};