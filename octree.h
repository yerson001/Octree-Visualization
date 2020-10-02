#ifndef OCTREE_H
#define OCTREE_H

#include "point.h"
#include <vector>
#include <QtDebug>
using namespace std;
class OctTree
{
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

    OctTree *northWestF;
    OctTree *southWestF;
    OctTree *northEastF;
    OctTree *southEastF;
    OctTree *northWestB;
    OctTree *southWestB;
    OctTree *northEastB;
    OctTree *southEastB;

    bool find(Point *pt, OctTree **pQT ){
        if(leaf){
            *pQT = this;
            for(int i=0;i<data.size();i++){

                if(data[i]->x == pt->x && data[i]->y == pt->y && data[i]->z == pt->z){
                    return true;
                }
            }
            return false;
        }
        return ubic(pt)->find(pt,pQT);
    }

    OctTree* ubic(Point *p){
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
        OctTree *pQT = nullptr;

        if(find(pt,&pQT)){
            return false;
        }
        if(pQT==nullptr) return false;
        if( pQT->stopCond() ) {
            pQT->leaf = false;

            float midX = (pQT->dimMax.x + pQT->dimMin.x)/2;
            float midY = (pQT->dimMax.y + pQT->dimMin.y)/2;
            float midZ = (pQT->dimMax.z + pQT->dimMin.z)/2;

            pQT->northWestF = new OctTree(Point(pQT->dimMin.x,midY,midZ),Point(midX,pQT->dimMax.y,pQT->dimMax.z));
            pQT->southWestF = new OctTree(Point(pQT->dimMin.x,pQT->dimMin.y,midZ),Point(midX,midY,pQT->dimMax.z));
            pQT->northEastF = new OctTree(Point(midX,midY,midZ),pQT->dimMax);
            pQT->southEastF = new OctTree(Point(midX,pQT->dimMin.y,midZ),Point(pQT->dimMax.x,midY,pQT->dimMax.z));

            pQT->northWestB = new OctTree(Point(pQT->dimMin.x,midY,pQT->dimMin.z),Point(midX,pQT->dimMax.y,midZ));
            pQT->southWestB = new OctTree(pQT->dimMin,Point(midX,midY,midZ));
            pQT->northEastB = new OctTree(Point(midX,midY,pQT->dimMin.z),Point(pQT->dimMax.x,pQT->dimMax.y,midZ));
            pQT->southEastB = new OctTree(Point(midX,pQT->dimMin.y,pQT->dimMin.z),Point(pQT->dimMax.x,midY,midZ));


            for(int i = 0;i<int(pQT->data.size());i++){
                (pQT->ubic(pQT->data[i]))->insert(pQT->data[i]);
                //(ubic(data[i])->data).push_back(data[i]);
            }
            (pQT->ubic(pt))->insert(pt);

            (pQT->data).clear();

        }else{
            (pQT->data).push_back(pt);
        }


        return true;
    }

    void printQ(){
        qDebug() << data.size() << " tamanio";
    }

    bool leaf;
    Point dimMax;
    Point dimMin;
    vector<Point*> data;

private:

    bool stopCond(){
        if(data.size() >= 100){
            return true;
        }
        return false;

    }
};

#endif // OCTREE_H
