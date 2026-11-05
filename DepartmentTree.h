#ifndef DEPARTMENTTREE_H
#define DEPARTMENTTREE_H

#include <string>
#include <vector>

using namespace std;

struct DepartmentNode {
    string name;
    DepartmentNode* parent;
    vector<DepartmentNode*> children; //General tree w/ arbitrary children

    DepartmentNode(const string& deptName, DepartmentNode* p = nullptr)
        : name(deptName), parent(p) {}
};

class DepartmentTree {
private:
    DepartmentNode* root;

    void clear(DepartmentNode* node);
    void preorderHelper(DepartmentNode* node, vector<string>& result) const;
    void displayHierarchyHelper(DepartmentNode* node, int depth) const;
    DepartmentNode* searchHelper(DepartmentNode* node, const string& name) const;

public:
    DepartmentTree();
    ~DepartmentTree();

    bool isEmpty() const;
    bool setRoot(const string& name);
    bool addDepartment(const string& name, const string& parentName);
    
    DepartmentNode* search(const string& name) const; //node
    void displayHierarchy() const;
    void preorderTraversal() const;
};
#endif
