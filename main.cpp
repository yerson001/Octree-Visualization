#include "mainwindow.h"
#include <QApplication>
#include <QString>
#include <QTextStream>
#include "point.h"

//leer el archivo .obj
void ReadFile(QString Filename,QVector<float> &vector){
    QFile mFile(Filename);

    if(!mFile.open(QFile::ReadOnly | QFile:: Text)) {
        return;
    }

    QTextStream in(&mFile);
    QString mText = in.readAll();
    mText.remove('v');

    QStringList strList = mText.split(QRegExp("[ ]"),QString::SkipEmptyParts);
    float aum=0.5;

    for(int i=0; i<strList.length(); i++){
        vector.push_back(strList[i].toFloat()*aum);
    }

    mFile.close();

}

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    MainWindow w;

    QVector<float> puntos;

    //**************seleccina la ruta del proyecto del archivo obj
    QString mFilename = "E:/QT/OcTree/archivo/denber.txt";


    ReadFile(mFilename,puntos);

    for(int i=0; i<puntos.size()-3; i=i+3){
        w.qTree->insert(new Point(puntos[i],(puntos[i+1]-0.1),puntos[i+2]));
        w.pointsW.push_back(Point(puntos[i],(puntos[i+1]-0.1),puntos[i+2]));
    }



    w.paintGL();
    w.resize(680,680);
    w.show();

    return a.exec();
}
