/*
 * @Author: qwqb233 57853293+qwqb233@users.noreply.github.com
 * @Date: 2025-05-01 21:18:14
 * @LastEditors: qwqb233 57853293+qwqb233@users.noreply.github.com
 * @FilePath: \C3_TFT\src\cube.cpp
 * @Description: 
 */
#include <TFT_eSPI.h> // Graphics and font library for ST7735 driver chip
#include <SPI.h>
#include <cmath>
#include "../include/cube.h"


void Cube::init_cube(int w,int h, int d)
{
    cube.d3_point[0].x = -w/2;
    cube.d3_point[0].y = -d/2;
    cube.d3_point[0].z = -h/2;
    
    cube.d3_point[1].x = -w/2;
    cube.d3_point[1].y = d/2;
    cube.d3_point[1].z = -h/2;

    cube.d3_point[2].x = w/2;
    cube.d3_point[2].y = -d/2;
    cube.d3_point[2].z = -h/2;

    cube.d3_point[3].x = w/2;
    cube.d3_point[3].y = d/2;
    cube.d3_point[3].z = -h/2;

    cube.d3_point[4].x = -w/2;
    cube.d3_point[4].y = -d/2;
    cube.d3_point[4].z = h/2;

    cube.d3_point[5].x = -w/2;
    cube.d3_point[5].y = d/2;
    cube.d3_point[5].z = h/2;

    cube.d3_point[6].x = w/2;
    cube.d3_point[6].y = -d/2;
    cube.d3_point[6].z = h/2;

    cube.d3_point[7].x = w/2;
    cube.d3_point[7].y = d/2;
    cube.d3_point[7].z = h/2;
}

Cube::Cube(int w,int h,int d,int x,int y,unsigned short color): rotation_matrix(nullptr)
{
    this->init_cube(w,h,d);
    cube.color = color;

    cube_position.x = x;
    cube_position.y = y;

    cube_rotation.x = 0.0f;
    cube_rotation.y = 0.0f;
    cube_rotation.z = 0.0f;
}

Cube::Cube(int w,int h,int d,int x,int y,rotation_3d rotation,unsigned short color): rotation_matrix(nullptr)

{
    this->init_cube(w,h,d);
    cube.color = color;

    cube_position.x = x;
    cube_position.y = y;

    cube_rotation.x = rotation.x;
    cube_rotation.y = rotation.y;
    cube_rotation.z = rotation.z;
}
Cube::~Cube()
{
    // Destructor implementation (if needed)
    // Currently, no dynamic memory allocation is used, so nothing to free
    // If you add dynamic memory allocation in the future, remember to free it here
}

void Cube::rotation()
{
    int rotation_dims[2] = {3,3};
    int dims_num = 2;

    double x_rotation_data[9] = {
        1,0,0,
        0,cos(cube_rotation.x),sin(cube_rotation.x),
        0,-sin(cube_rotation.x),cos(cube_rotation.x)
    };
    Matrix x_rotation_matrix(rotation_dims,dims_num,x_rotation_data);

    double y_rotation_data[9] = {
        cos(cube_rotation.y),0,-sin(cube_rotation.y),
        0,1,0,
        sin(cube_rotation.y),0,cos(cube_rotation.y)
    };
    Matrix y_rotation_matrix(rotation_dims,dims_num,y_rotation_data);

    double z_rotation_data[9] = {
        cos(cube_rotation.z),sin(cube_rotation.z),0,
        -sin(cube_rotation.z),cos(cube_rotation.z),0,
        0,0,1
    };
    Matrix z_rotation_matrix(rotation_dims,dims_num,z_rotation_data);

    matrix_t xy_rotation_matrix = x_rotation_matrix * y_rotation_matrix;
    Matrix xy_matrix = Matrix(xy_rotation_matrix.dims,xy_rotation_matrix.dims_num,xy_rotation_matrix.data);
    matrix_t xyz_rotaion_matrix = xy_matrix * z_rotation_matrix;
    rotation_matrix = new Matrix(xyz_rotaion_matrix.dims,xyz_rotaion_matrix.dims_num,xyz_rotaion_matrix.data);
}

void Cube::covert_3d_to_2d()
{
    for(int i = 0; i < 8; i++)
    {
        int dims[2] = {3,1};
        int dims_num = 2;
        double data[3] = {cube.d3_point[i].x,cube.d3_point[i].y,cube.d3_point[i].z};
        Matrix point_matrix(dims,dims_num,data);

        matrix_t result = *rotation_matrix * point_matrix;

        cube.d3_point[i].x = result.data[0];
        cube.d3_point[i].y = result.data[1];
        cube.d3_point[i].z = result.data[2];

        /* cube.d2_point[i].x = (cube.d3_point[i].x - cube_position.x) * 10 / (cube.d3_point[i].z - cube_position.z) + 30;
        cube.d2_point[i].y = (cube.d3_point[i].y - cube_position.y) * 10 / (cube.d3_point[i].z - cube_position.z) + 30; */
        cube.d2_point[i].x = cube.d3_point[i].z + cube_position.x;
        cube.d2_point[i].y = cube.d3_point[i].y + cube_position.y;
    }
}

void Cube::showCube(TFT_eSPI * tft)
{
    this->rotation();
    this->covert_3d_to_2d();
    tft->drawLine(cube.d2_point[0].x,cube.d2_point[0].y,cube.d2_point[1].x,cube.d2_point[1].y,cube.color);
    tft->drawLine(cube.d2_point[0].x,cube.d2_point[0].y,cube.d2_point[2].x,cube.d2_point[2].y,cube.color);
    tft->drawLine(cube.d2_point[0].x,cube.d2_point[0].y,cube.d2_point[4].x,cube.d2_point[4].y,cube.color);

    tft->drawLine(cube.d2_point[1].x,cube.d2_point[1].y,cube.d2_point[3].x,cube.d2_point[3].y,cube.color);
    tft->drawLine(cube.d2_point[1].x,cube.d2_point[1].y,cube.d2_point[5].x,cube.d2_point[5].y,cube.color);

    tft->drawLine(cube.d2_point[2].x,cube.d2_point[2].y,cube.d2_point[3].x,cube.d2_point[3].y,cube.color);
    tft->drawLine(cube.d2_point[2].x,cube.d2_point[2].y,cube.d2_point[6].x,cube.d2_point[6].y,cube.color);

    tft->drawLine(cube.d2_point[3].x,cube.d2_point[3].y,cube.d2_point[7].x,cube.d2_point[7].y,cube.color);

    tft->drawLine(cube.d2_point[4].x,cube.d2_point[4].y,cube.d2_point[5].x,cube.d2_point[5].y,cube.color);
    tft->drawLine(cube.d2_point[4].x,cube.d2_point[4].y,cube.d2_point[6].x,cube.d2_point[6].y,cube.color);

    tft->drawLine(cube.d2_point[5].x,cube.d2_point[5].y,cube.d2_point[7].x,cube.d2_point[7].y,cube.color);

    tft->drawLine(cube.d2_point[6].x,cube.d2_point[6].y,cube.d2_point[7].x,cube.d2_point[7].y,cube.color);
}


