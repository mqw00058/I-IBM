/* File Name:   Point.cpp
 * Description: Implementation of Point class.
 * Author:      Bo Gao
 * Date:        September 27, 2007
 */

#include "Point.h"

namespace MarchingCubes
{


Vector3f Point::getPosition(){
    return position;
}

Vector3f Point::getNormal(){
    return normal;
}

void Point::setNormal(const Vector3f n){
    normal = n;
}

void Point::setEigenNormal(const Vector3f n){
    eigenNormal = n;
}

Vector3f Point::getEigenNormal(){
    return eigenNormal;
}

void Point::setCenter(const Vector3f n){
    center = n;
}

Vector3f Point::getCenter(){
    return center;
}

void Point::setKnghb(const int *nghb){
    for (int i = 0; i < K; i ++){
        knghb[i] = nghb[i];
    }
}

int* Point::getKnghb(){
    return knghb;
}

void Point::setNormalConsistent(){
    normalConsistent = true;
}

bool Point::getNormalConsistent(){
    return normalConsistent;
}

void Point::setGrid(int i, int j, int k){
    grid[0] = i;
    grid[1] = j;
    grid[2] = k;
}

int* Point::getGrid(){
    return grid;
}

}