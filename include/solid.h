//
// Created by qwqb233 on 2025/5/5.
//

#ifndef CPP_TEST_SOLID_H
#define CPP_TEST_SOLID_H

#include "Matrix.h"

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 160

#define COLOR_TYPE int

typedef struct point_2d
{
    double x;
    double y;
}point_2d;

typedef struct point_3d
{
    double x;
    double y;
    double z;
}point_3d;

class Solid  {
    public:
        Solid() = default;
        Solid(point_3d * points,int num_points,COLOR_TYPE color);
        Solid(point_3d shape_center,point_3d * points, COLOR_TYPE num_points,int color);
        Solid(point_3d shape_center,point_3d * points, COLOR_TYPE num_points,Matrix rotation,Matrix translation,int color);
        void point_3d_to_2d(point_3d * points,point_2d * points_2d,int num_points) const;
        void point_3d_to_2d() const;
        ~Solid();
        virtual void draw() const;

    protected:
        void init_matrix(Solid * solid,point_3d shape_center);

        Matrix * rotation_matrix;
        Matrix * translation_matrix;
        point_3d center;
        point_3d *  points;
        point_3d * points_temp;
        point_2d *  points_2d;
        int num_points;
    COLOR_TYPE color;
};

class Sphere : private Solid {
    public:
        Sphere() = default;
        Sphere(double W,double D,double H,COLOR_TYPE color);
        ~Sphere() = default;
        void draw() const override;
        void tft_draw(TFT_eSPI tft);
        void rotation_to(double x_angle,double y_angle,double z_angle);
        void moving_to(double x,double y);

};

//TODO:添加其他形状
//TODO:添加工厂接口


#endif //CPP_TEST_SOLID_H
