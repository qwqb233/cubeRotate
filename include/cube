/*
 * @Author: qwqb233 57853293+qwqb233@users.noreply.github.com
 * @Date: 2025-05-01 21:17:53
 * @LastEditors: qwqb233 57853293+qwqb233@users.noreply.github.com
 * @FilePath: \C3_TFT\include\cube.h
 * @Description: 
 */
#pragma once

#include "../include/linera.h"

//三维坐标点
typedef struct point_3d
{
    int x;
    int y;
    int z;
}point_3d;

//三维旋转角度
typedef struct rotation_3d
{
    float x;
    float y;
    float z;
}rotation_3d;

//二维坐标点
typedef struct point_2d
{
    int x;
    int y;
}point_2d;

//立方体结构体
typedef struct cube
{
    point_3d d3_point[8];
    point_2d d2_point[8];
    unsigned short color;
}cube_t;

class Cube
{
    public:
        Cube(int w,int h,int d,int x,int y,unsigned short color);
        Cube(int w,int h,int d,int x,int y,rotation_3d rotation,unsigned short color);
        ~Cube();
        void covert_3d_to_2d();
        void rotation();
        void showCube(TFT_eSPI * tft);
        cube_t cube; 
    protected:
        
    private:
        point_2d cube_position;
        rotation_3d cube_rotation;
        Matrix * rotation_matrix;

        void init_cube(int w,int h,int d);
};
