/* File Name:   Node.cpp
 * Description: Implementation of Node class.
 * Author:      Bo Gao
 * Date:        October 4, 2007
 */

#include "Node.h"

int Node::getId() const{
    return id;
}

void Node::setSet(int i){
    set = i;
}

int Node::getSet() const{
    return set;
}

void Node::setNext(int i){
    next = i;
};

int Node::getNext() const{
    return next;
}

void Node::setFirst(int i){
    first = i;
}

int Node::getFirst() const{
    return first;
}

void Node::addEdge(const Edge e){
    edges.push_back(e);
}

bool Node::hasEdge(const Edge e){
    for (int i = 0; i < (int) edges.size(); i ++)
        if (edges[i] == e)
            return true;
    return false;
}

void Node::clearEdges(){
    edges.clear();
}

vector<Edge> Node::getEdges() const{
    return edges;
}
