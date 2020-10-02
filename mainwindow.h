#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QMainWindow>
#include <QOpenGLWindow>
#include <QSurfaceFormat>
#include <QOpenGLFunctions>
#include <QtOpenGL>
#include <GL/glu.h>
#include <QMouseEvent>


#include "point.h"
#include "octree.h"

using namespace std;
class MainWindow : public QOpenGLWindow {
    Q_OBJECT

public:
    //************funciones de vista ******
    MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void mousePressEvent(QMouseEvent *);
    void mouseMoveEvent(QMouseEvent *);
    void wheelEvent(QWheelEvent *);
    void keyPressEvent(QKeyEvent *);
    //*************************************
     virtual void paintGL();

    bool pointIN(Point a);
    float zoomScale;
    void printQT(OctTree **quad);

    OctTree *qTree ;// instancia del arbol
    vector<Point> pointsW;
    vector<Point*> pSelec;

    vector<Point> linesA;
    vector<Point> linesB;


protected:
    virtual void initializeGL();
    virtual void resizeGL(int w, int h);

    void resizeEvent(QResizeEvent *event);
    void paintEvent(QPaintEvent *event);
    void pushPoint(OctTree **p);
   //manipular vantana
    Point cCircle;
    float angle;
    bool pressLB;
    Point cSphere;
    float radio;
private:
    QOpenGLContext *context;
    QOpenGLFunctions *openGLFunctions;
};

#endif // MAINWINDOW_H
