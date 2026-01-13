/* File Name:   Edge.h
 * Description: Header file defining Edge class.
 * Author:      Bo Gao
 * Date:        October 6, 2007
 */

#ifndef _EDGE_H_
#define _EDGE_H_

#include <qDebug>

class Edge{
    private:
        int startNode, endNode;
        float weight;
        int set;
    public:
        Edge(){};
        Edge(int &sn, int &en, float &w):
            startNode(sn), endNode(en), weight(w){};
        int operator==(const Edge &e);
        int getStartNode() const;
        int getEndNode() const;
        float getWeight() const;
        int hasNode(int &n) const;
        void setSet(int i);
        int getSet() const;
};

#endif
