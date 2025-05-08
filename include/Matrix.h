//
// Created by qwqb233 on 2025/5/5.
//

#ifndef CPP_TEST_MATRIX_H
#define CPP_TEST_MATRIX_H
#include <vector>

#include "thread_locker.h"

class Matrix : public  thread_locker{
    public:
        //使用double类型数组初始化矩阵
        Matrix(int * dims,int dim_size,double * data);
        //使用std::vector<double>类型数组初始化矩阵
        Matrix(int * dims,int dim_size,std::vector<double> data);
        ~Matrix();

        //在标准输出流中打印矩阵
        void print();

        std::vector<double> get_data();
        int* get_dims();
        int  get_dim_size();
        long long get_data_size();

        //矩阵转置
        void transpose();

        //矩阵加上标量
        Matrix operator+(const double scalar);
        //矩阵相加
        Matrix operator+(const Matrix& other);
        //矩阵减去标量
        Matrix operator-(const double scalar);
        //矩阵减去矩阵
        Matrix operator-(const Matrix& other);
        //矩阵乘以标量
        Matrix operator*(const double scalar);
        //矩阵相乘
        Matrix operator*(const Matrix& other);
        //矩阵除以标量
        Matrix operator/(const double scalar);
        //矩阵幂运算
        Matrix operator^(const double power);
        //矩阵赋值
        Matrix& operator=(const Matrix& other);
        //判断矩阵是否相等
        bool operator==(const Matrix& other);
    private:
        int * dims;
        int dim_size;
        long long data_size;
        std::vector<double> data;
};

#endif //CPP_TEST_MATRIX_H
