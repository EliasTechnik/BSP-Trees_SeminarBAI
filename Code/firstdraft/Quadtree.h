#pragma ONCE
#define MAX_POINTS_PER_NODE 4


class Point{
    protected:
        int x;
        int y;
    public:
        Point(int _x, int _y);
        int getX(){return x;};
        int getY(){return y;}; 
};
/*
struct PointArray{
    Point *points;
    unsigned int pointcount;
};

class Rectangle : public Point{
    protected:
        int width;
        int height;
    public:
        Rectangle(int _x, int _y, int _width, int _height):Point(_x,_y){};
        int getWidth(){return width;};
        int getHeight(){return height;};
};

class QuadNode: public Rectangle{
    protected:
        QuadNode *northwest;
        QuadNode *norteast;
        QuadNode *southeast;
        QuadNode *southwest;
        PointArray points;
        bool isLeave;//true if ther are no childs
        void Subdivide(); //checks pointcount, if over MAX_POINTS_PER_NODE --> subdivide by for and give Points to childs
    public:
        QuadNode(int _x, int _y, int _width, int _height):Rectangle(_x, _y, _width, _height){};
        void insertPoints(PointArray *_points, unsigned int max_points_per_node = MAX_POINTS_PER_NODE); //adds an array of points and subdivides if neccesarry
        unsigned int getPointCount(); //prototype for Pointcount
        PointArray getPoints(); //returns all Points under that node and its potential childs
        PointArray getPointsInRadius(Point _center, int radius); //signed int to not overshoot
};*/