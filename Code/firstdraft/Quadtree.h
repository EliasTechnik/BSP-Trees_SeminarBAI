
class Point{
    protected:
        int x;
        int y;
    public:
        Point(int _x, int _y);
        int getX(){return x;};
        int getY(){return y;}; 
};

class Rectangle : public Point{
    protected:
        int width;
        int height;
    public:
        Rectangle(int _x, int _y, int _width, int _height);
        int getWidth(){return width;};
        int getHeight(){return height;};
};

class QuadRect : public Rectangle{
    protected:
        int childs[4];
        bool issubdivided;
        Point points[];
        int pointcount;
        void subdivide(int maxPoints);
    public:
        QuadRect(int _x, int _y, int _width, int _height);
        void insertPoint(Point _p, int maxPoints);
        Point *getPointsInRadius(Point center); //returns the pointer to an array of points which lay inside the radius
        int getPointCountInradius(Point center);    //returns the number of points which lay inside the radius
        
}