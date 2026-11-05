#ifndef FACILITY_H
#define FACILITY_H

#include <string>
using namespace std;

class Facility {
private:
    string id;          // Normalized to uppercase
    string name;        // Original spelling
    string building;    // Spelling from buildings.txt
    string category;    // Original spelling

public:
    Facility();
    Facility(const string& id, const string& name, const string& building, const string& category);

    string getId() const;
    string getName() const;
    string getBuilding() const;
    string getCategory() const;

    void display() const;
};
#endif
