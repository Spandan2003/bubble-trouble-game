#include <simplecpp>

/* Bubble Vars */
const int BUBBLE_START_X = 250;
const int BUBBLE_START_Y = 100;
const int BUBBLE_DEFAULT_RADIUS = 10;
const int BUBBLE_RADIUS_THRESHOLD = 10;
const int BUBBLE_DEFAULT_VX = 100;
const int BUBBLE_DEFAULT_VY = 100;
const double GRAVITY = 100;

/* Game Vars */
const int PLAY_Y_HEIGHT = 450;
const int LEFT_MARGIN = 30;
const int TOP_MARGIN = 20;
const int BOTTOM_MARGIN = (PLAY_Y_HEIGHT+TOP_MARGIN);

class Bubble
{
private:
    Circle circle;  // the circle representing the bubble
    Circle border;  // the border of circle
    double vx, vy;  // velocity in x and y direction
    Color color;    // color of the bubble
    int order;      // Order i.e how many times the bubble can split. Order 1 means it will dissappear.

public:
    Bubble(double cx, double cy, double r, double vx_, double vy_,int order_=1, Color color_=COLOR(0, 0, 255))
    {
        // Bubble constructor
        color = color_;
        circle = Circle(cx, cy, r);
        border = Circle(cx, cy, r);
        circle.setColor(color);
        border.setColor(COLOR(0,0,0));
        circle.setFill(true);

        vx = vx_;
        vy = vy_;
        order = order_;
    }

    void nextStep(double t)
    {
        // move the bubble
        double new_x = circle.getX() + vx*t;
        double new_y = circle.getY() + vy*t;

        if ((vx < 0 && new_x < (0 + circle.getRadius())) // bounce along X direction at left border
            ||
            (vx > 0 && new_x > (WINDOW_X - circle.getRadius()))) // bounce along X direction at right border
        {

            vx = -vx;
            new_x = circle.getX() + vx*t;

        }
        if ((vy < 0 && new_y < (0 + circle.getRadius())) // bounce along Y direction at upper border
            ||
            (vy > 0 && new_y > (PLAY_Y_HEIGHT - circle.getRadius()))) // bounce along Y direction at lower border
        {

            vy = -vy;
            new_y = circle.getY() + vy*t;

        }

        vy = vy + GRAVITY*t;

        circle.moveTo(new_x, new_y);
        border.moveTo(new_x, new_y);
    }

    double get_radius()
    {
        // return the radius of the bubble
        return circle.getRadius();
    }

    double get_center_x()
    {
        // return the x coordinate of the center of the bubble
        return circle.getX();
    }

    double get_center_y()
    {
        // return the y coordinate of the center of the bubble
        return circle.getY();
    }

    double get_vx()
    {
        // return the x velocity of the bubble
        return vx;
    }
    double get_vy()
    {
        // return the y velocity of the bubble
        return vy;
    }
    int get_order()
    {
        // return the order of the bubble
        return order;
    }

    Color get_color()
    {
        // return the color of the bubble
        return color;
    }
};
