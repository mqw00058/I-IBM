/* File Name:   Kruskal.cpp
 * Description: Implementation of Kruskal class.
 * Author:      Bo Gao
 * Date:        October 6, 2007
 * Description: Implementation of Kruskal algorithm. Inheritance of Graph
 *              class.
 */

#include "Kruskal.h"

int Kruskal::addEdge(const Edge e){
    if (nodes[e.getStartNode()].hasEdge(e))
        return edges.size();
    if (e.getEndNode() < nodes.size() && nodes[e.getEndNode()].hasEdge(e))
        return edges.size();

    edges.push_back(e);
    nodes[e.getStartNode()].addEdge(e);
    if (e.getEndNode() < nodes.size())
        nodes[e.getEndNode()].addEdge(e);
    return edges.size();
}

void Kruskal::mtsKruskal(){
    int treeId = 0;
    qsort(0, (int)edges.size() - 1);
    for (int i = 0; i < (int)nodes.size(); i ++){
        nodes[i].setSet(i);
        nodes[i].setFirst(i);
        nodes[i].setNext(-1);
        nodes[i].clearEdges();
    }
    for (int i = 0; i < (int)edges.size(); i ++){
        edges[i].setSet(0);
    }
    qDebug() << endl << "Kruskal start: finding Minimum Spanning Tree..." << endl;
    qDebug() << "graph edges: " << (int)edges.size() << endl;
    float percent = 0.0;
    for (int i = 0; i < (int)edges.size(); i ++){
        Node start = nodes[edges[i].getStartNode()];
        Node end = nodes[edges[i].getEndNode()];
        if (start.getSet() == end.getSet())
            continue;
        edges[i].setSet(1);
        selEdges.push_back(edges[i]);
        nodes[edges[i].getStartNode()].addEdge(edges[i]);
        nodes[edges[i].getEndNode()].addEdge(edges[i]);
        int j;
        for (j = edges[i].getStartNode(); nodes[j].getNext() >= 0;
                j = nodes[j].getNext());
        nodes[j].setNext(nodes[edges[i].getEndNode()].getFirst());
        int f = nodes[edges[i].getStartNode()].getFirst();
        int set = nodes[edges[i].getStartNode()].getSet();
        for (j = nodes[edges[i].getEndNode()].getFirst(); j >= 0;
                j = nodes[j].getNext()){
            nodes[j].setFirst(f);
            nodes[j].setSet(set);
        }
        if ((float) i / (float)edges.size() > percent){
            percent += 0.01;
          //  qDebug().width(4);
            qDebug() << (int)(percent * 100.0) << "% \r" << flush;
        }
    }
    qDebug() << endl << "MTS edges: " << selEdges.size() << endl;
    qDebug() << "kruskal end" << endl;
}

vector<int> Kruskal::getConnectNodes(int n){
    vector<int> connectNodes;
    for (int i = 0; i < (int)nodes[n].getEdges().size(); i ++)
        if (nodes[n].getEdges()[i].hasNode(n) != -1)
            connectNodes.push_back(nodes[n].getEdges()[i].hasNode(n));
    return connectNodes;
} 
