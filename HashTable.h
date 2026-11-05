#ifndef HASHTABLE_H
#define HASHTABLE_H

#include "Facility.h"
#include <string>
#include <vector>
#include <list>

using namespace std;

class HashTable {
private:
    struct HashNode {
        string key; //Facility ID
        Facility facility;
    };

    vector<list<HashNode>> table; //Separate chaining
    size_t capacity;
    size_t numEntries;
    size_t collisionCount; //diagnostic

    size_t hashFunction(const string& key) const; //Custom hash function

public:
    explicit HashTable(size_t cap = 101);

    bool insert(const Facility& facility);
    bool search(const string& id, Facility& outFacility) const;
    
    size_t getCapacity() const;
    size_t getEntries() const;
    size_t getCollisions() const;
    
    void displayAll() const;
};
#endif
