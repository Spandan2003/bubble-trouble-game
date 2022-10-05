#include <simplecpp>
#include "bubble.h"
class Freeze
{
private:
    Circle circlef;  // the circle representing the freeze ball
    Circle circleb;  // the circle representing the border of freeze ball
    double vx, vy;  // velocity in x and y direction
    Color colorf;    // fill color of the freeze ball
    Color colorb;    // border color of the freeze ball


public:
    Freeze(double cx, double cy, double r, double vx_, double vy_, Color colorf_=COLOR(175, 242, 255),Color colorb_=COLOR(8,76,236))
    {
        // Freeze constructor
        colorf = colorf_;
        colorb = colorb_;
        circlef = Circle(cx, cy, r);
        circleb = Circle(cx, cy, r);    //border circle with different color
        circlef.setColor(colorf);
        circleb.setColor(colorb);
        circlef.setFill(true);

        vx = vx_;
        vy = vy_;
    }

    bool nextStep(double t)
    {
        // move the freeze ball
        vy = vy + GRAVITY*t;
        double new_y = circlef.getY() + vy*t;
        if(new_y > (PLAY_Y_HEIGHT-circlef.getRadius()))   // out of canvas, so delete it
        {
            return false;
        }
        circlef.moveTo(circlef.getX(), new_y);
        circleb.moveTo(circleb.getX(), new_y);
        return true;
    }

    double get_radius()
    {
        // return the radius of the freeze ball
        return circlef.getRadius();
    }

    double get_center_x()
    {
        // return the x coordinate of the center of the freeze ball
        return circlef.getX();
    }

    double get_center_y()
    {
        // return the y coordinate of the center of the freeze ball
        return circlef.getY();
    }

    double get_vx()
    {
        // return the x velocity of the freeze ball
        return vx;
    }
    double get_vy()
    {
        // return the y velocity of the freeze ball
        return vy;
    }

    Color get_color_fill()
    {
        // return the color of the freeze ball
        return colorf;
    }

    Color get_color_border()
    {
        // return the color of the freeze ball
        return colorb;
    }

};


class Heart
{
private:
    Rectangle body; // Rect and circle make up heart
    Circle lc,rc;
    double vy;      //y-velocity of heart
    Color color;    // Color of heart

public:
    Heart(double px, double py, double vy_, double size = 5, Color color_=COLOR(255,0,0))
    {   //Heart constructor. size refers to half of width of body
        color = color_;

        body = Rectangle(px, py, size*2, size*2);
        body.setColor(color);
        body.setFill(true);
        lc = Circle(px-size,py-size,size);
        rc = Circle(px+size,py-size,size);
        lc.setColor(color);
        lc.setFill(true);
        rc.setColor(color);
        rc.setFill(true);

        vy = vy_;
    }

    bool nextStep(double t)
    {
        // move the heart
        double new_y = body.getY() + vy*t;
        if(new_y > PLAY_Y_HEIGHT-(body.getHeight()/2.0))   // out of canvas, so delete it
        {
            return false;
        }
        body.moveTo(body.getX(), new_y);
        lc.moveTo(body.getX()-body.getWidth()/2, new_y-body.getWidth()/2);
        rc.moveTo(body.getX()+body.getWidth()/2, new_y-body.getWidth()/2);
        return true;
    }

    double get_size()
    {
        // return the size of the heart which is half of length of square of heart
        return body.getWidth()/2;
    }

    double get_center_x()
    {
        // return the x coordinate of the center of the heart centre
        return body.getX();
    }

    double get_center_y()
    {
        // return the y coordinate of the center of the heart centre
        return body.getY();
    }
};
