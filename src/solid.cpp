//
// Created by qwqb233 on 2025/5/5.
//

//TODO:智能指针重构
#include <iostream>
#include <cmath>
#include <Arduino.h>

#include <TFT_eSPI.h> // Graphics and font library for ST7735 driver chip
#include <SPI.h>

#include "../include/solid.h"
#include "../include/Logger.h"

/*
 * 初始化旋转矩阵和平移矩阵，默认不旋转
 * @param solid 要初始化的Solid对象
 * @param shape_center 形状的中心点
 * @return void
 */
void Solid::init_matrix(Solid * solid,point_3d shape_center)
{
    int * rotation_matrix_dims = new int[2]{3,3};
    int rotation_matrix_dim_size = 2;
    double * rotation_matrix_data = new double[9]{1,0,0,0,1,0,0,0,1};
    solid->rotation_matrix = new Matrix(rotation_matrix_dims,rotation_matrix_dim_size,rotation_matrix_data);

    int * translation_matrix_dims = new int[2]{3,1};
    int translation_matrix_dim_size = 2;
    double * translation_matrix_data = new double[3]{shape_center.x,shape_center.y,shape_center.z};
    solid->translation_matrix = new Matrix(translation_matrix_dims,translation_matrix_dim_size,translation_matrix_data);

    /*std::cout << "rotation_matrix:" << std::endl;
    solid->rotation_matrix->print();
    std::cout << "translation_matrix:" << std::endl;
    solid->translation_matrix->print();*/

    delete[] rotation_matrix_dims;
    delete[] rotation_matrix_data;
    delete[] translation_matrix_dims;
    delete[] translation_matrix_data;
}

/*
 * 将三维图像投影到二维画布
 * @param points 三维点集
 * @param points_2d 二维点集
 * @param num_points 点集大小
 * @return void
 */
void Solid::point_3d_to_2d(point_3d *points, point_2d *points_2d, int num_points) const
{
    int * dims = new int[2]{3,1};
    for(int i = 0;i < this->num_points;i++)
    {
        double * new_data = new double[3]{this->points[i].x,this->points[i].y,this->points[i].z};
        Matrix point_pos_matrix(dims,2,new_data);
        point_pos_matrix = *this->rotation_matrix * point_pos_matrix;
        point_pos_matrix = *this->translation_matrix + point_pos_matrix;
        std::vector<double> data = point_pos_matrix.get_data();
        this->points_temp[i] = {data[0],data[1],data[2]};
        delete[] new_data;
    }
    delete[] dims;
    for(int i = 0;i < num_points;i++)
    {
        points_2d[i].x = points[i].z;
        points_2d[i].y = points[i].y;
    }
}
/*
 * 使用类参数，将三维图像投影到二维画布
 */
void Solid::point_3d_to_2d() const
{
    int * dims = new int[2]{3,1};
    for(int i = 0;i < this->num_points;i++)
    {   
        // Serial.printf("start new matrix\r\n");
        double * new_data = new double[3]{this->points[i].x,this->points[i].y,this->points[i].z};
        Matrix point_pos_matrix(dims,2,new_data);
        // Serial.printf("new matrix\r\n");
        // Serial.printf("point_pos_matrix: %f %f %f\r\n",point_pos_matrix.get_data()[0],point_pos_matrix.get_data()[1],point_pos_matrix.get_data()[2]);
        // Serial.printf("rotation_matrix: %f %f %f\r\n",this->rotation_matrix->get_data()[0],this->rotation_matrix->get_data()[1],this->rotation_matrix->get_data()[2]);
        
        point_pos_matrix = *this->rotation_matrix * point_pos_matrix;
        point_pos_matrix = *this->translation_matrix + point_pos_matrix;
        std::vector<double> data = point_pos_matrix.get_data();
        this->points_temp[i] = {data[0],data[1],data[2]};
        delete[] new_data;
    }
    delete[] dims;
    for(int i = 0;i < this->num_points;i++)
    {
        this->points_2d[i].x = this->points_temp[i].z;
        this->points_2d[i].y = this->points_temp[i].y;
    }
}

/*
 * 构造函数，初始化Solid对象
 * @param points 三维点集
 * @param num_points 点集大小
 * @param color 颜色
 */
Solid::Solid(point_3d * points,int num_points,COLOR_TYPE color)
{
    this->init_matrix(this,point_3d{0,(double)SCREEN_HEIGHT/2,(double)SCREEN_WIDTH/2});
    this->center.x = (double)SCREEN_WIDTH/2;
    this->center.y = (double)SCREEN_HEIGHT/2;
    this->num_points = num_points;
    this->color = color;

    this->points = new point_3d[num_points];
    this->points_2d = new point_2d[num_points];
    this->points_temp = new point_3d[num_points];
    for(int i = 0;i < num_points;i++)
    {
        this->points[i] = {points[i].x,points[i].y,points[i].z};
        this->points_2d[i] = {points[i].z,points[i].y};
    }
}

/*
 * 构造函数，初始化Solid对象
 * @param shape_center 形状的中心点
 * @param points 三维点集
 * @param num_points 点集大小
 * @param color 颜色
 */
Solid::Solid(point_3d shape_center,point_3d * points, int num_points,COLOR_TYPE color)
{
    this->init_matrix(this,shape_center);

    this->center.x = shape_center.x;
    this->center.y = shape_center.y;
    this->num_points = num_points;
    this->color = color;

    this->points = new point_3d[num_points];
    this->points_2d = new point_2d[num_points];
    this->points_temp = new point_3d[num_points];
    for(int i = 0;i < num_points;i++)
    {
        this->points[i] = {points[i].x,points[i].y,points[i].z};
        this->points_2d[i] = {points[i].z,points[i].y};
    }
}


/*
 * 构造函数，初始化Solid对象
 * @param shape_center 形状的中心点
 * @param points 三维点集
 * @param num_points 点集大小
 * @param rotation 旋转矩阵
 * @param translation 平移矩阵
 * @param color 颜色
 */
Solid::Solid(point_3d shape_center,point_3d * points, int num_points,Matrix rotation,Matrix translation,COLOR_TYPE color)
{
    this->init_matrix(this,shape_center);
    this->center.x = shape_center.x;
    this->center.y = shape_center.y;
    this->num_points = num_points;
    this->color = color;
    *this->rotation_matrix = rotation;
    *this->translation_matrix = translation;

    this->points = new point_3d[num_points];
    this->points_2d = new point_2d[num_points];
    this->points_temp = new point_3d[num_points];
    for(int i = 0;i < num_points;i++)
    {
        this->points[i] = {points[i].x,points[i].y,points[i].z};
        this->points_2d[i] = {points[i].z,points[i].y};
    }
}

/*
 * 析构函数，释放内存
 */
Solid::~Solid() {
    delete this->rotation_matrix;
    delete this->translation_matrix;
    delete[] this->points;
    delete[] this->points_2d;
    delete[] this->points_temp;
}

/*
 * 虚函数，需要子类实现
 */
void Solid::draw() const
{
    std::cout << "Drawing a solid" << std::endl;
}

/*
 * 子类Sphere的构造函数，初始化Sphere对象
 * @param W 长度
 * @param D 厚度
 * @param H 高度
 * @param color 颜色
 */
Sphere::Sphere(double W,double D,double H,int color) : Solid(nullptr,0,color)
{
    this->num_points = 8;
    this->points = new point_3d[8];
    this->points_2d = new point_2d[8];
    this->points_temp = new point_3d[num_points];

//    point_3d * points = new point_3d[8];
    this->points[0] = point_3d{-W/2,-D/2,H/2};
    this->points[1] = point_3d{W/2,-D/2,H/2};
    this->points[2] = point_3d{W/2,D/2,H/2};
    this->points[3] = point_3d{-W/2,D/2,H/2};
    this->points[4] = point_3d{-W/2,-D/2,-H/2};
    this->points[5] = point_3d{W/2,-D/2,-H/2};
    this->points[6] = point_3d{W/2,D/2,-H/2};
    this->points[7] = point_3d{-W/2,D/2,-H/2};

    if(this->rotation_matrix == nullptr || this->translation_matrix == nullptr)
    {
        std::cout << "Error: rotation_matrix or translation_matrix is nullptr" << std::endl;
    }

    int * dims = new int[2]{3,1};
    for(int i = 0;i < this->num_points;i++)
    {
        double * new_data = new double[3]{this->points[i].x,this->points[i].y,this->points[i].z};
        Matrix point_pos_matrix(dims,2,new_data);
        point_pos_matrix = *this->rotation_matrix * point_pos_matrix;
        point_pos_matrix = *this->translation_matrix + point_pos_matrix;
        std::vector<double> data = point_pos_matrix.get_data();
        this->points_temp[i] = {data[0],data[1],data[2]};
        delete[] new_data;
    }
    delete[] dims;

    this->point_3d_to_2d();
}

/*
 * 图像在屏幕上移动
 * @param x x轴偏移
 * @param y y轴偏移
 */
void Sphere::moving_to(double x,double y)
{
    int * dims = new int[3]{3,1};
    double * new_data = new double[3]{0,y,x};
    Matrix translation_matrix(dims,2,new_data);
    *this->translation_matrix = translation_matrix;
    delete[] new_data;
    delete[] dims;
}
void Sphere::rotation_to(double x_angle,double y_angle,double z_angle)
{
    double pi = 3.14159265358979323846;
    static double x_angle_sum = 0 ,y_angle_sum = 0, z_angle_sum = 0;
    x_angle_sum += x_angle;
    y_angle_sum += y_angle;
    z_angle_sum += z_angle;

    if(x_angle_sum > 2*pi) x_angle_sum -= 2*pi;
    if(x_angle_sum < -2*pi) x_angle_sum += 2*pi;
    if(y_angle_sum > 2*pi) y_angle_sum -= 2*pi;
    if(y_angle_sum < -2*pi) y_angle_sum += 2*pi;
    if(z_angle_sum > 2*pi) z_angle_sum -= 2*pi;
    if(z_angle_sum < -2*pi) z_angle_sum += 2*pi;

    // Serial.printf("x_angle_sum: %f\r\n",x_angle_sum);
    int * dims = new int[2]{3,3};
    double * new_x_data = new double[9]{1,0,0,0,cos(x_angle_sum),-sin(x_angle_sum),0,sin(x_angle_sum),cos(x_angle_sum)};
    Matrix x_rotation_matrix(dims,2,new_x_data);
    delete[] new_x_data;
    // Serial.printf("y_angle_sum: %f\r\n",y_angle_sum);
    double * new_y_data = new double[9]{cos(y_angle_sum),0,sin(y_angle_sum),0,1,0,-sin(y_angle_sum),0,cos(y_angle_sum)};
    Matrix y_rotation_matrix(dims,2,new_y_data);
    delete[] new_y_data;
    double * new_z_data = new double[9]{cos(z_angle_sum),-sin(z_angle_sum),0,sin(z_angle_sum),cos(z_angle_sum),0,0,0,1};
    Matrix z_rotation_matrix(dims,2,new_z_data);
    delete[] new_z_data;
    delete[] dims;
    // Serial.printf("rotation_matrix: %f %f %f\r\n",this->rotation_matrix->get_data()[0],this->rotation_matrix->get_data()[1],this->rotation_matrix->get_data()[2]);
    
    *this->rotation_matrix = x_rotation_matrix * y_rotation_matrix * z_rotation_matrix;
    // Serial.printf("z_angle_sum: %f\r\n",z_angle_sum);
    //*this->rotation_matrix = *this->rotation_matrix * x_rotation_matrix * y_rotation_matrix * z_rotation_matrix;
}

/*
 * 子类Sphere的绘制函数，将Sphere对象绘制到屏幕上
 */
void Sphere::draw() const
{
    //计算平移旋转，并投影到二维画布
    this->point_3d_to_2d();

    //TODO:连接各点
    //TODO:在屏幕上绘制图像

    /* for(int i = 0;i < this->num_points;i++)
    {
        Serial.printf("Point %d: (%f,%f,%f)\r\n",i,this->points[i].x,this->points[i].y,this->points[i].z);
        // std::cout << "Point " << i << ": (" << this->points[i].x << "," << this->points[i].y << "," << this->points[i].z << ")" << std::endl;
    }
    // std::cout << std::endl;
    for(int i = 0;i < this->num_points;i++)
    {
        Serial.printf("Point %d: (%f,%f)\r\n",i,this->points_2d[i].x,this->points_2d[i].y);
        // std::cout << "Point " << i << ": (" << this->points_2d[i].x << "," << this->points_2d[i].y << ")" << std::endl;
    } */

    
}

void Sphere::tft_draw(TFT_eSPI tft)
{
    tft.fillScreen(TFT_BLACK);

    this->draw();
    tft.drawLine(this->points_2d[0].x,this->points_2d[0].y,this->points_2d[1].x,this->points_2d[1].y,TFT_WHITE);
    tft.drawLine(this->points_2d[0].x,this->points_2d[0].y,this->points_2d[3].x,this->points_2d[3].y,TFT_WHITE);
    tft.drawLine(this->points_2d[0].x,this->points_2d[0].y,this->points_2d[4].x,this->points_2d[4].y,TFT_WHITE);

    tft.drawLine(this->points_2d[1].x,this->points_2d[1].y,this->points_2d[2].x,this->points_2d[2].y,TFT_WHITE);
    tft.drawLine(this->points_2d[1].x,this->points_2d[1].y,this->points_2d[5].x,this->points_2d[5].y,TFT_WHITE);

    tft.drawLine(this->points_2d[2].x,this->points_2d[2].y,this->points_2d[6].x,this->points_2d[6].y,TFT_WHITE);
    tft.drawLine(this->points_2d[2].x,this->points_2d[2].y,this->points_2d[3].x,this->points_2d[3].y,TFT_WHITE);

    tft.drawLine(this->points_2d[3].x,this->points_2d[3].y,this->points_2d[7].x,this->points_2d[7].y,TFT_WHITE);

    tft.drawLine(this->points_2d[4].x,this->points_2d[4].y,this->points_2d[5].x,this->points_2d[5].y,TFT_WHITE);
    tft.drawLine(this->points_2d[4].x,this->points_2d[4].y,this->points_2d[7].x,this->points_2d[7].y,TFT_WHITE);

    tft.drawLine(this->points_2d[5].x,this->points_2d[5].y,this->points_2d[6].x,this->points_2d[6].y,TFT_WHITE);

    tft.drawLine(this->points_2d[6].x,this->points_2d[6].y,this->points_2d[7].x,this->points_2d[7].y,TFT_WHITE);

}
