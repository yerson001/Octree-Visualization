#ifndef OCTREE_H
#define OCTREE_H

#include "point.h"
#include <vector>
#include <QtDebug>
using namespace std;
class OctTree {

private:

    bool parar(){
        if(data.size() >= 100){
            return true;
        }
        return false;

    }
public:
    OctTree(){

        dimMin = Point(-1,-1,-1);
        dimMax = Point(1,1,1);
        leaf = true;
    }
    OctTree(Point min, Point max){
        dimMax = max;
        dimMin = min;
        leaf = true;
    }
    //****datos*******
    bool leaf;
    Point dimMax;
    Point dimMin;
    vector<Point*> data;

    OctTree *northWestF;
    OctTree *southWestF;
    OctTree *northEastF;
    OctTree *southEastF;
    OctTree *northWestB;
    OctTree *southWestB;
    OctTree *northEastB;
    OctTree *southEastB;
    //*******************

    bool find(Point *pt, OctTree **pOT ){
        if(leaf){
            *pOT = this;
            for(int i=0;i<data.size();i++){

                if(data[i]->x == pt->x && data[i]->y == pt->y && data[i]->z == pt->z){
                    return true;
                }
            }
            return false;
        }
        return FindOctan(pt)->find(pt,pOT);
    }

    OctTree* FindOctan(Point *p){
        float midX = (dimMax.x + dimMin.x)/2;
        float midY = (dimMax.y + dimMin.y)/2;
        float midZ = (dimMax.z + dimMin.z)/2;


        if(p->x<= midX){
            if(p->y <= midY){
                if(p->z <= midZ){
                    return southWestB;
                }else{
                    return southWestF;
                }
            }else{
                if(p->z <= midZ){
                    return northWestB;
                }else{
                    return northWestF;
                }
            }
        }else{
            if(p->y <= midY){
                if(p->z <= midZ){
                    return southEastB;
                }else{
                    return southEastF;
                }
            }else{
                if(p->z <= midZ){
                    return northEastB;
                }else{
                    return northEastF;
                }
            }

        }
    }

    bool insert(Point *pt) {
        OctTree *pOT = nullptr;

        if(find(pt,&pOT)){
            return false;
        }
        if(pOT==nullptr) return false;
        if( pOT->parar() ) {
            pOT->leaf = false;

            float midX = (pOT->dimMax.x + pOT->dimMin.x)/2;
            float midY = (pOT->dimMax.y + pOT->dimMin.y)/2;
            float midZ = (pOT->dimMax.z + pOT->dimMin.z)/2;

            pOT->northWestF = new OctTree(Point(pOT->dimMin.x,midY,midZ),Point(midX,pOT->dimMax.y,pOT->dimMax.z));
            pOT->southWestF = new OctTree(Point(pOT->dimMin.x,pOT->dimMin.y,midZ),Point(midX,midY,pOT->dimMax.z));
            pOT->northEastF = new OctTree(Point(midX,midY,midZ),pOT->dimMax);
            pOT->southEastF = new OctTree(Point(midX,pOT->dimMin.y,midZ),Point(pOT->dimMax.x,midY,pOT->dimMax.z));

            pOT->northWestB = new OctTree(Point(pOT->dimMin.x,midY,pOT->dimMin.z),Point(midX,pOT->dimMax.y,midZ));
            pOT->southWestB = new OctTree(pOT->dimMin,Point(midX,midY,midZ));
            pOT->northEastB = new OctTree(Point(midX,midY,pOT->dimMin.z),Point(pOT->dimMax.x,pOT->dimMax.y,midZ));
            pOT->southEastB = new OctTree(Point(midX,pOT->dimMin.y,pOT->dimMin.z),Point(pOT->dimMax.x,midY,midZ));


            for(int i = 0;i<int(pOT->data.size());i++){
                (pOT->FindOctan(pOT->data[i]))->insert(pOT->data[i]);
                //(FindOctan(data[i])->data).push_back(data[i]);
            }
            (pOT->FindOctan(pt))->insert(pt);

            (pOT->data).clear();

        }else{
            (pOT->data).push_back(pt);
        }


        return true;
    }

    void printQ(){
        qDebug() << data.size() << " tamanio";
    }
     void PriP(Point *pt){
        cout<<"punto : ("<<pt->x<<", "<<pt->y<<", "<<pt->z<<" )"<<endl;
    } 
    
     void delete(Point *pt) {
        Octree *pOT = nullptr;
        if(find(pt,&pOT)) {
          //PriP(pt);
          //pt = nullptr;
          /*
            auto it = pt->data.begin();
            while(*it != point) ++it;
                pt->data.erase(it);*/  
        }else{
          cout<<"no lo encontro para eliminar"<<endl;
        }
    }

    ~OctTree(){
        for(int i = 0;i<data.size();i++)
            delete data[i];
        if(northWestF){
            delete northWestF;
            delete southWestF;
            delete northEastF;
            delete southEastF;
            delete northWestB;
            delete southWestB;
            delete northEastB;
            delete southEastB;
        }
    }
};

#endif // OCTREE_H
