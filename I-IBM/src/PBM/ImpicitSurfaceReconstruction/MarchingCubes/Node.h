/* File Name:   Node.h
 * Description: Header file defining Node class.
 * Author:      Bo Gao
 * Date:        October 4, 2007
 */

#ifndef _NODE_H_
#define _NODE_H_

#include <vector>
#include "Edge.h"

using namespace std;

class Node{
    private:
        int id;
        int set;
        int first;
        int next;
        vector< Edge > edges;

    public:
        Node(){};
        Node(int i):id(i){};
        int getId() const;
        void setSet(int i);
        int getSet() const;
        int getNext() const;
        void setNext(int i);
        void setFirst(int i);
        int getFirst() const;
        void addEdge(const Edge e);
        bool hasEdge(const Edge e);
        void clearEdges();
        vector<Edge> getEdges() const;
};

#endif
