#ifndef CAMPUSGRAPH_H
#define CAMPUSGRAPH_H

#include <string>
#include <vector>
#include <unordered_map>
#include <map>

using namespace std;

class CampusGraph {
private:
    //Adjacency list preserving insertion order
    vector<string> buildings;
    map<string, vector<string>> adjList;

public:
    CampusGraph() = default;

    bool addBuilding(const string& name);
    bool addConnection(const string& b1, const string& b2);
    bool hasBuilding(const string& name) const;

    size_t getBuildingCount() const;
    size_t getConnectionCount() const;
    int getDegree(const string& building) const;

    void displayGraph() const;
    void displayCountsAndDegrees() const;
    void findReachable(const string& startBuilding) const; //bfs
};
#endif
