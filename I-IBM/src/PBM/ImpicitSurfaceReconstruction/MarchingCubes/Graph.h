/* File Name:   Graph.h
 * Description: Header file defining Graph class.
 * Author:      Bo Gao
 * Date:        October 6, 2007
 */

#ifndef _GRAPH_H_
#define _GRAPH_H_

#include <iostream>
#include <vector>
#include "Node.h"
#include "Edge.h"

using namespace std;

class Graph{
    protected:
        vector<Node> nodes;
        vector<Edge> edges;

    public:
        Graph(){};
        virtual ~Graph();
        int addNode(const Node n);
        int addEdge(const Edge e);
        Node getNode(int) const;
        Edge getEdge(int) const;
        vector<Node> getNodes();
        vector<Edge> getEdges();
        int getNodeSize();
        int getEdgeSize();
        void qsort(int, int);
        int partition(int, int);
        inline void swap(int, int);
};

#endif
