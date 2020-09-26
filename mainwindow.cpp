#include "mainwindow.h"
#include "point.h"
#include <QtMath>
#include <cmath>

MainWindow::MainWindow(QWidget *parent) {

    setSurfaceType(QWindow::OpenGLSurface);
    QSurfaceFormat format;
    format.setProfile(QSurfaceFormat::CompatibilityProfile);
    format.setVersion(2,1);

    setFormat(format);

    context = new QOpenGLContext;
    context->setFormat(format);
    context->create();

    context->makeCurrent(this);
    openGLFunctions = context->functions();

    //limites
    Point min(-1.0,-1.0,-1.0);
    Point max(1.0,1.0,1.0);

    qTree = nullptr;
    //instacia con los limites
    qTree = new OctTree(min,max);

    //****parametros de ventana*****
    cCircle.x = 0.0;
    cCircle.y = 0.0;
    cCircle.z = 0.0;

    cSphere.x = 0.0;
    cSphere.y = 0.0;
    cSphere.z = 0.0;
    angle= 0.8;
    zoomScale = 1.0;
    pressLB = false;
    radio = 0.5;
}

MainWindow::~MainWindow() {
    delete qTree;

}

void MainWindow::mouseMoveEvent(QMouseEvent *ev) {
        cCircle.x = float(ev->x())/float(this->width())-0.5;
        cCircle.y = float(ev->y())/float(this->height())-0.5;
        cCircle.x *= 2;
        cCircle.y *= -2;

        cSphere.x = cCircle.x ;
        cSphere.y = cCircle.y ;

    update();
}

void MainWindow::wheelEvent(QWheelEvent *ev) {
    QPoint numDegrees = ev->angleDelta();
    if (numDegrees.y() < 0)
        zoomScale = zoomScale/1.1;
    if (numDegrees.y() > 0)
        zoomScale = zoomScale*1.1;
    update();
}

void MainWindow::pushPoint(OctTree **p)
{
    if((*p)->leaf){
        for(int i=0; i<(*p)->data.size();i++) {
            pSelec.push_back((*p)->data[i]);
        }
    }else{
        //octantes
        pushPoint(&(*p)->northEastB);
        pushPoint(&(*p)->southEastB);
        pushPoint(&(*p)->northWestB);
        pushPoint(&(*p)->southWestB);
        pushPoint(&(*p)->northEastF);
        pushPoint(&(*p)->southEastF);
        pushPoint(&(*p)->northWestF);
        pushPoint(&(*p)->southWestF);
    }
}

void MainWindow::keyPressEvent(QKeyEvent *ev){
    if(ev->key() == 70){
        pSelec.clear();
        update();
    }
}

void MainWindow::mousePressEvent(QMouseEvent *ev) {
    float x_p = float(ev->x())/float(this->width())-0.5;
    float y_p = float(ev->y())/float(this->height())-0.5;

    pointsW.push_back(Point(x_p*2,y_p*-2));
    qTree->insert(new Point(x_p*2,y_p*-2));
    update();

}

void MainWindow::initializeGL() {
    resizeGL(this->width(),this->height());

}


void MainWindow::resizeGL(int w, int h) {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    glViewport(0,0,w,h);

   qreal aspectratio = qreal(w)/qreal(h);
   glOrtho(-1* aspectratio,1*aspectratio,-1,1,1,-1);


}



void MainWindow::paintGL() {

    glClearColor(255, 255, 255, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);

    glLoadIdentity();

    glScaled(zoomScale, zoomScale, zoomScale);
    //*******zoom scale***********
    glPointSize(3);
    gluPerspective(45.0,1.0,1.0,100.0);
    glTranslatef(0,0,-6);
    glRotatef(cCircle.x*100,0,1,0);
    glRotatef(cCircle.y*100,1,0,0);
    //********zoom******************

    pressLB = false;
    glBegin(GL_POINTS);
    glColor3d(0,0,100);
    //*****vertex**********
    for(int i=0;i<int(pointsW.size());i++){
        //qDebug() <<pointsW[i].first<< "paint " << pointsW[i].second<< "size" <<pointsW.size() ;
        glVertex3d(pointsW[i].x,pointsW[i].y,pointsW[i].z);
    }
    glEnd();
    glFlush();


}

void MainWindow::resizeEvent(QResizeEvent *event) {
    resizeGL(this->width(),this->height());
    this->update();
}

void MainWindow::paintEvent(QPaintEvent *event) {
    paintGL();

}

