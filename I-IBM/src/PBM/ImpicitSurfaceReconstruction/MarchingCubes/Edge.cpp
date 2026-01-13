/* File Name:   Edge.cpp
 * Description: Implementation of Edge class.
 * Author:      Bo Gao
 * Date:        October 6, 2007
 */

#include "Edge.h"

int Edge::operator==(const Edge &e){
    return ((startNode == e.startNode) && (endNode == e.endNode)) ||
           ((endNode == e.startNode) && (startNode == e.endNode));
}

int Edge::getStartNode() const{
    return startNode;
}

int Edge::getEndNode() const{
    return endNode;
}

float Edge::getWeight() const{
    return weight;
}

int Edge::hasNode(int &n) const{
    if (startNode == n)
        return endNode;
    if (endNode == n)
        return startNode;
    return -1;
}

void Edge::setSet(int i){
    set = i;
}

int Edge::getSet() const{
    return set;
}
