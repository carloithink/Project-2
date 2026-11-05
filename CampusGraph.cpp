#include "CampusGraph.h"
#include <iostream>
#include <queue>
#include <algorithm>
#include <set>

using namespace std;

bool CampusGraph::addBuilding(const string& name) {
    // Check if building already exists
    string upperName = name;
    for (const auto& b : buildings) {
        string existingUpper = b;
        transform(existingUpper.begin(), existingUpper.end(), existingUpper.begin(), ::toupper);
        string targetUpper = upperName;
        transform(targetUpper.begin(), targetUpper.end(), targetUpper.begin(), ::toupper);
        if (existingUpper == targetUpper) {
            return false;
        }
    }
    buildings.push_back(name);
    adjList[name] = vector<string>();
    return true;
}

bool CampusGraph::addConnection(const string& b1, const string& b2) {
    if (!hasBuilding(b1) || !hasBuilding(b2) || b1 == b2) {
        return false;
    }
    // Check if connection already exists
    auto& neighbors1 = adjList[b1];
    if (find(neighbors1.begin(), neighbors1.end(), b2) != neighbors1.end()) {
        return false; // Duplicate connection
    }

    adjList[b1].push_back(b2);
    adjList[b2].push_back(b1);
    return true;
}

bool CampusGraph::hasBuilding(const string& name) const {
    return adjList.find(name) != adjList.end();
}

size_t CampusGraph::getBuildingCount() const {
    return buildings.size();
}

size_t CampusGraph::getConnectionCount() const {
    size_t totalEdges = 0;
    for (const auto& pair : adjList) {
        totalEdges += pair.second.size();
    }
    return totalEdges / 2; // Undirected graph counted 1 time
}

int CampusGraph::getDegree(const string& building) const {
    auto it = adjList.find(building);
    if (it != adjList.end()) {
        return static_cast<int>(it->second.size());
    }
    return 0;
}

void CampusGraph::displayGraph() const {
    if (buildings.empty()) {
        cout << "No buildings available.\n";
        return;
    }
    for (const auto& b : buildings) {
        cout << b << ":";
        auto it = adjList.find(b);
        if (it != adjList.end() && !it->second.empty()) {
            for (const auto& neighbor : it->second) {
                cout << " " << neighbor;
            }
        } else {
            cout << " (no connections)";[cite: 11]
        }
        cout << "\n";
    }
}

void CampusGraph::displayCountsAndDegrees() const {
    cout << "Total buildings: " << getBuildingCount() << "\n";
    cout << "Total connections: " << getConnectionCount() << "\n";
    for (const auto& b : buildings) {
        cout << b << " degree: " << getDegree(b) << "\n";
    }
}

void CampusGraph::findReachable(const string& startBuilding) const {
    if (!hasBuilding(startBuilding)) {
        cout << "Error: Unknown building '" << startBuilding << "'.\n";
        return;
    }

    queue<string> q;
    set<string> visited;
    map<string, int> distances;

    q.push(startBuilding);
    visited.insert(startBuilding); // Mark visited when enqueued
    distances[startBuilding] = 0;

    vector<string> visitOrder;

    while (!q.empty()) {
        string curr = q.front();
        q.pop();
        visitOrder.push_back(curr);

        auto it = adjList.find(curr);
        if (it != adjList.end()) {
            for (const auto& neighbor : it->second) { // Follow neighbor insertion order
                if (visited.find(neighbor) == visited.end()) {
                    visited.insert(neighbor);
                    distances[neighbor] = distances[curr] + 1;
                    q.push(neighbor);
                }
            }
        }
    }

    for (const auto& b : visitOrder) {
        cout << b << " (distance " << distances[b] << ")\n";
    }
    cout << "Reachable: " << visitOrder.size() << " of " << buildings.size() << " buildings\n";
}
