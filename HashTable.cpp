#include "HashTable.h"
#include <algorithm>
#include <iostream>

using namespace std;

HashTable::HashTable(size_t cap) : capacity(cap), numEntries(0), collisionCount(0) {
    table.resize(capacity);
}

//Custom string hash function using normalized key characters and table capacity
size_t HashTable::hashFunction(const string& key) const {
    unsigned long hashValue = 0;
    for (char c : key) {
        hashValue = (hashValue * 31) + static_cast<unsigned char>(c);
    }
    return hashValue % capacity;
}

bool HashTable::insert(const Facility& facility) {
    string key = facility.getId();
    size_t index = hashFunction(key);

    //Check for duplicate key (case-insensitively, since IDs are normalized to uppercase)
    for (const auto& node : table[index]) {
        if (node.key == key) {
            return false; // Duplicate
        }
    }

    //Check if bucket is already occupied
    if (!table[index].empty()) {
        collisionCount++;
    }

    table[index].push_back({key, facility});
    numEntries++;
    return true;
}

bool HashTable::search(const string& id, Facility& outFacility) const {
    size_t index = hashFunction(id);
    for (const auto& node : table[index]) {
        if (node.key == id) {
            outFacility = node.facility;
            return true;
        }
    }
    return false;
}

size_t HashTable::getCapacity() const { return capacity; }
size_t HashTable::getEntries() const { return numEntries; }
size_t HashTable::getCollisions() const { return collisionCount; }

void HashTable::displayAll() const {
    for (size_t i = 0; i < capacity; ++i) {
        for (const auto& node : table[i]) {
            node.facility.display();
        }
    }
}
