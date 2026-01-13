/* File Name:   Graph.cpp
 * Description: Implementation of Graph class.
 * Author:      Bo Gao
 * Date:        October 6, 2007
 */
#include "Graph.h"

Graph::~Graph(){
    nodes.clear();
    edges.clear();
}

int Graph::addNode(const Node n){
    nodes.push_back(n);
    return nodes.size();
}

int Graph::addEdge(const Edge e){
    edges.push_back(e);
    return edges.size();
}

void Graph::qsort(int left, int right){
    int i;
    if (left >= right)
        return;
    i = partition(left, right);
    qsort(left, i - 1);
    qsort(i, right);
}

int Graph::partition(int left, int right){
    float pivot = edges[(int)(left + right) / 2].getWeight();
    while (left <= right){
        while (edges[left].getWeight() < pivot)
            left ++;
        while (edges[right].getWeight() > pivot)
            right --;
        if (left <= right){
            swap(left, right);
            left ++;
            right --;
        }
    }
    return left;
}

inline void Graph::swap(int left, int right){
    Edge tmp;
    tmp = edges[left];
    edges[left] = edges[right];
    edges[right] = tmp;
}

Node Graph::getNode(int idx) const{
    return nodes[idx];
}

Edge Graph::getEdge(int idx) const{
    return edges[idx];
}

vector<Node> Graph::getNodes(){
    return nodes;
}

vector<Edge> Graph::getEdges(){
    return edges;
}


int Graph::getNodeSize(){
    return nodes.size();
}

int Graph::getEdgeSize(){
    return edges.size();
}
