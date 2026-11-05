#include "Facility.h"
#include "HashTable.h"
#include "CampusGraph.h"
#include "DepartmentTree.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <algorithm>

using namespace std;
// Helper trim function for file parsing
string trim(const string& str) {
    size_t first = str.find_first_not_of(" \t\r\n");
    if (first == string::npos) return "";
    size_t last = str.find_last_not_of(" \t\r\n");
    return str.substr(first, (last - first + 1));
}

void loadFiles(const string& dataDir, CampusGraph& graph, HashTable& hashTable, DepartmentTree& deptTree) {
    //Load buildings.txt
    {
        string filename = dataDir + "/buildings.txt";
        ifstream file(filename);
        int accepted = 0, rejected = 0, lineNum = 0;
        if (!file.is_open()) {
            cout << "Error: Cannot open file " << filename << "\n";
        } else {
            string line;
            while (getline(file, line)) {
                lineNum++;
                string cleaned = trim(line);
                if (cleaned.empty() || cleaned[0] == '#') continue;
                
                if (graph.addBuilding(cleaned)) {
                    accepted++;
                } else {
                    rejected++;
                    cout << "Warning: " << filename << ", line " << lineNum << ": duplicate or invalid building.\n";
                }
            }
        }
        cout << "buildings.txt: " << accepted << " accepted, " << rejected << " rejected\n";
    }

    // Load connections.txt
    {
        string filename = dataDir + "/connections.txt";
        ifstream file(filename);
        int accepted = 0, rejected = 0, lineNum = 0;
        if (!file.is_open()) {
            cout << "Error: Cannot open file " << filename << "\n";
        } else {
            string line;
            while (getline(file, line)) {
                lineNum++;
                string cleaned = trim(line);
                if (cleaned.empty() || cleaned[0] == '#') continue;

                stringstream ss(cleaned);
                string b1, b2;
                if (getline(ss, b1, '|') && getline(ss, b2)) {
                    b1 = trim(b1);
                    b2 = trim(b2);
                    if (graph.addConnection(b1, b2)) {
                        accepted++;
                    } else {
                        rejected++;
                        cout << "Warning: " << filename << ", line " << lineNum << ": invalid connection reference or duplicate.\n";
                    }
                } else {
                    rejected++;
                    cout << "Warning: " << filename << ", line " << lineNum << ": malformed record.\n";
                }
            }
        }
        cout << "connections.txt: " << accepted << " accepted, " << rejected << " rejected\n";
    }

    //Load facilities.txt
    {
        string filename = dataDir + "/facilities.txt";
        ifstream file(filename);
        int accepted = 0, rejected = 0, lineNum = 0;
        if (!file.is_open()) {
            cout << "Error: Cannot open file " << filename << "\n";
        } else {
            string line;
            while (getline(file, line)) {
                lineNum++;
                string cleaned = trim(line);
                if (cleaned.empty() || cleaned[0] == '#') continue;

                stringstream ss(cleaned);
                string id, name, bldg, cat;
                if (getline(ss, id, '|') && getline(ss, name, '|') &&
                    getline(ss, bldg, '|') && getline(ss, cat)) {
                    id = trim(id);
                    name = trim(name);
                    bldg = trim(bldg);
                    cat = trim(cat);

                    // Validate ID format: f followed by 3 digits
                    if (id.length() != 4 || toupper(id[0]) != 'F' ||
                        !isdigit(id[1]) || !isdigit(id[2]) || !isdigit(id[3])) {
                        rejected++;
                        cout << "Warning: " << filename << ", line " << lineNum << ": invalid ID format.\n";
                        continue;
                    }

                    //Normalize ID to uppercase
                    string upperId = "";
                    for (char c : id) upperId += static_cast<char>(toupper(c));

                    if (!graph.hasBuilding(bldg)) {
                        rejected++;
                        cout << "Warning: " << filename << ", line " << lineNum << ": unknown building reference.\n";
                        continue;
                    }

                    Facility fac(upperId, name, bldg, cat);
                    if (hashTable.insert(fac)) {
                        accepted++;
                    } else {
                        rejected++;
                        cout << "Warning: " << filename << ", line " << lineNum << ": duplicate facility ID.\n";
                    }
                } else {
                    rejected++;
                    cout << "Warning: " << filename << ", line " << lineNum << ": malformed record.\n";
                }
            }
        }
        cout << "facilities.txt: " << accepted << " accepted, " << rejected << " rejected (hash capacity " 
                  << hashTable.getCapacity() << ", entries " << hashTable.getEntries() << ", collisions " << hashTable.getCollisions() << ")\n";
    }

    //Load departments.txt
    {
        string filename = dataDir + "/departments.txt";
        ifstream file(filename);
        int accepted = 0, rejected = 0, lineNum = 0;
        if (!file.is_open()) {
            cout << "Error: Cannot open file " << filename << "\n";
        } else {
            string line;
            while (getline(file, line)) {
                lineNum++;
                string cleaned = trim(line);
                if (cleaned.empty() || cleaned[0] == '#') continue;

                stringstream ss(cleaned);
                string dept, parent;
                if (getline(ss, dept, '|') && getline(ss, parent)) {
                    dept = trim(dept);
                    parent = trim(parent);

                    if (deptTree.isEmpty()) {
                        if (parent == "NONE") {
                            deptTree.setRoot(dept);
                            accepted++;
                        } else {
                            rejected++;
                            cout << "Warning: " << filename << ", line " << lineNum << ": root must use NONE as parent.\n";
                        }
                    } else {
                        if (deptTree.addDepartment(dept, parent)) {
                            accepted++;
                        } else {
                            rejected++;
                            cout << "Warning: " << filename << ", line " << lineNum << ": unknown parent or duplicate department.\n";
                        }
                    }
                } else {
                    rejected++;
                    cout << "Warning: " << filename << ", line " << lineNum << ": malformed record.\n";
                }
            }
        }
        cout << "departments.txt: " << accepted << " accepted, " << rejected << " rejected\n";
    }
}

int main(int argc, char* argv[]) {
    string dataDir = "data";
    if (argc > 1) {
        dataDir = argv[1]; //Support optional data directory command line
    }

    CampusGraph graph;
    HashTable hashTable(101);
    DepartmentTree deptTree;

    loadFiles(dataDir, graph, hashTable, deptTree);

    int choice = 0;
    do {
        cout << "\n--- Milestone 1 Menu ---\n"
                  << "1. View all facilities\n"
                  << "2. Search facility by ID\n"
                  << "3. View campus graph\n"
                  << "4. Display graph counts and degrees\n"
                  << "5. Find reachable buildings (BFS)\n"
                  << "6. Add department\n"
                  << "7. Search department\n"
                  << "8. Display department hierarchy\n"
                  << "9. Run preorder traversal\n"
                  << "10. Exit\n"
                  << "Enter choice: ";

        if (!(cin >> choice)) {
            cout << "Invalid input. Please enter a number.\n";
            cin.clear();
            cin.ignore(32767, '\n');
            continue;
        }
        cin.ignore(); //Clear newline

        switch (choice) {
            case 1:
                hashTable.displayAll();
                break;
            case 2: {
                string id;
                cout << "Enter facility ID (e.g., F001): ";
                getline(cin, id);
                id = trim(id);
                //Normalize to uppercase
                string upperId = "";
                for (char c : id) upperId += static_cast<char>(toupper(c));

                Facility fac;
                if (hashTable.search(upperId, fac)) {
                    cout << "Found: ";
                    fac.display();
                } else {
                    cout << "Not found: no facility with ID " << upperId << "\n";[cite: 15, 18]
                }
                break;
            }
            case 3:
                graph.displayGraph();
                break;
            case 4:
                graph.displayCountsAndDegrees();
                break;
            case 5: {
                string bldg;
                cout << "Enter starting building name: ";
                getline(cin, bldg);
                bldg = trim(bldg);
                graph.findReachable(bldg);
                break;
            }
            case 6: {
                string dept, parent;
                cout << "Enter new department name: ";
                getline(cin, dept);
                dept = trim(dept);
                if (dept.empty()) {
                    cout << "Error: Blank name entered.\n";
                    break;
                }
                if (deptTree.isEmpty()) {
                    deptTree.setRoot(dept);
                    cout << "Root department added successfully.\n";
                } else {
                    cout << "Enter parent department name: ";
                    getline(cin, parent);
                    parent = trim(parent);
                    if (deptTree.addDepartment(dept, parent)) {
                        cout << "Department added successfully.\n";
                    } else {
                        cout << "Error: Unknown parent or duplicate department name.\n";
                    }
                }
                break;
            }
            case 7: {
                string name;
                cout << "Enter department name to search: ";
                getline(cin, name);
                name = trim(name);
                DepartmentNode* node = deptTree.search(name);
                if (node) {
                    cout << "Found: " << node->name 
                              << " (Parent: " << (node->parent ? node->parent->name : "None") 
                              << ", Direct children: " << node->children.size() << ")\n";
                } else {
                    cout << "Department not found.\n";
                }
                break;
            }
            case 8:
                deptTree.displayHierarchy();
                break;
            case 9:
                deptTree.preorderTraversal();
                break;
            case 10:
                cout << "Exiting Milestone 1...\n";
                break;
            default:
                cout << "Invalid menu choice. Please select between 1 and 10.\n";
        }
    } while (choice != 10);

    return 0;
}
