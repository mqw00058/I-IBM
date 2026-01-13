/* File Name:   Point.h
 * Description: Header file defining Point class.
 * Author:      Bo Gao
 * Date:        September 27, 2007
 */
#ifndef _POINT_
#define _POINT_

#include "Vector3f.h"
#include "nr.h"

const int K = 8;    // K nearst neighbor points

namespace MarchingCubes
{


class Point{
    private:
        int knghb[K];
        bool normalConsistent;
        int grid[3];
        Vector3f position;
	Vector3f normal;
        Vector3f center;
        Vector3f eigenNormal;
    public:
	Point(){};
	Point(const Vector3f p) : normalConsistent(false), position(p){};
	Vector3f getPosition();
	void setNormal(const Vector3f);
	Vector3f getNormal();
        void setEigenNormal(const Vector3f);
        Vector3f getEigenNormal();
        void setCenter(const Vector3f);
        Vector3f getCenter();
        void setKnghb(const int*);
        int *getKnghb();
        void setNormalConsistent();
        bool getNormalConsistent();
        void setGrid(int, int, int);
        int *getGrid();
};
}
#endif /* _POINT_ */
