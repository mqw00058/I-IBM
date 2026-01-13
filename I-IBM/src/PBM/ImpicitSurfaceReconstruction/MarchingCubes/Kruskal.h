/* File Name:   Kruskal.h
 * Description: Header file defining Kruskal class.
 * Author:      Bo Gao
 * Date:        October 6, 2007
 */

#ifndef _KRUSKAL_H_
#define _KRUSKAL_H_

#include <vector>
#include "Graph.h"

class Kruskal : public Graph{
    private:
        vector<Edge> selEdges;
    public:
        Kruskal():Graph(){};
        int addEdge(const Edge e);
        void mtsKruskal();
        vector<int> getConnectNodes(int);
};

#endif
