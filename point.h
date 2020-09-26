#ifndef POINT_H
#define POINT_H

class Point
{
public:
    Point() {
        x=0.0;
        y=0.0;
        z=0.0;
    }

    Point(float a,float b, float c){
        x=a;
        y=b;
        z=c;
    }

    Point(float a,float b){
        x=a;
        y=b;
        z=0.0;
    }

    float x,y,z;

};

#endif // POINT_H
