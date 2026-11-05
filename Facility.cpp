#include "Facility.h"
#include <iostream>

using namespace std;

Facility::Facility() : id(""), name(""), building(""), category("") {}   //call to class

Facility::Facility(const string& id, const string& name, const string& building, const string& category)
    : id(id), name(name), building(building), category(category) {}

string Facility::getId() const { return id; }
string Facility::getName() const { return name; }
string Facility::getBuilding() const { return building; }
string Facility::getCategory() const { return category; }

void Facility::display() const {
    cout << id << " | " << name << " | " << building << " | " << category << "\n";
}
