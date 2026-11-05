#include "DepartmentTree.h"
#include <iostream>
#include <algorithm>

using namespace std;

DepartmentTree::DepartmentTree() : root(nullptr) {}
// Call the class func
DepartmentTree::~DepartmentTree() {
    clear(root);
}

void DepartmentTree::clear(DepartmentNode* node) {
    if (!node) return;
    for (auto child : node->children) {
        clear(child);
    }
    delete node;
}

bool DepartmentTree::isEmpty() const {
    return root == nullptr;
}

bool DepartmentTree::setRoot(const string& name) {
    if (root != nullptr) return false;
    root = new DepartmentNode(name, nullptr);
    return true;
}

DepartmentNode* DepartmentTree::searchHelper(DepartmentNode* node, const string& name) const {
    if (!node) return nullptr;
    
    //Case insensitive comparison check
    string nodeUpper = node->name;
    string targetUpper = name;
    transform(nodeUpper.begin(), nodeUpper.end(), nodeUpper.begin(), ::toupper);
    transform(targetUpper.begin(), targetUpper.end(), targetUpper.begin(), ::toupper);

    if (nodeUpper == targetUpper) return node;

    for (auto child : node->children) {
        DepartmentNode* found = searchHelper(child, name);
        if (found) return found;
    }
    return nullptr;
}

DepartmentNode* DepartmentTree::search(const string& name) const {
    return searchHelper(root, name);
}

bool DepartmentTree::addDepartment(const string& name, const string& parentName) {
    if (isEmpty()) {
        return setRoot(name);
    }

    //Check if name already exists
    if (search(name) != nullptr) return false;

    DepartmentNode* parentNode = search(parentName);
    if (!parentNode) return false;

    DepartmentNode* newNode = new DepartmentNode(name, parentNode);
    parentNode->children.push_back(newNode);
    return true;
}

void DepartmentTree::displayHierarchyHelper(DepartmentNode* node, int depth) const {
    if (!node) return;
    for (int i = 0; i < depth * 4; ++i) {
        cout << ' ';
    }
    cout << node->name << "\n";
    for (auto child : node->children) {
        displayHierarchyHelper(child, depth + 1);
    }
}

void DepartmentTree::displayHierarchy() const {
    if (isEmpty()) {
        cout << "Department tree is empty.\n";
        return;
    }
    displayHierarchyHelper(root, 0);
}

void DepartmentTree::preorderHelper(DepartmentNode* node, vector<string>& result) const {
    if (!node) return;
    result.push_back(node->name);
    for (auto child : node->children) {
        preorderHelper(child, result);
    }
}

void DepartmentTree::preorderTraversal() const {
    if (isEmpty()) {
        cout << "Department tree is empty.\n";
        return;
    }
    vector<string> result;
    preorderHelper(root, result);
    for (size_t i = 0; i < result.size(); ++i) {
        cout << result[i] << (i + 1 < result.size() ? " " : "");
    }
    cout << "\n";
}
