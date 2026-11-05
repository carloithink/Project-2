Project 2: Smart Campus Navigation and Facility Analytics System (Milestone 1)

1. Purpose

This C++ program loads campus infrastructure data from text files and allows users 
to query facilities using a custom hash table, explore campus buildings and walkways via an 
undirected graph with BFS reachability, and manage academic departments through a general tree structure

2. Build and Run Instructions (Tested on UNT CSE CELL machines)

- To compile the project, run:
  make
  (or manually: g++ -std=c++17 src/*.cpp -I include -o smart_campus)

- To run the program with default data (using the data/ directory):
  ./smart_campus

- To run with a custom test data directory (NFR-10)
  ./smart_campus testdata/case1

3. Directory Layout (8.1 Project Layout)[cite: 16]

Project2/
|-- include/
|   |-- Facility.h
|   |-- HashTable.h
|   |-- CampusGraph.h
|   |-- DepartmentTree.h
|-- src/
|   |-- Facility.cpp
|   |-- HashTable.cpp
|   |-- CampusGraph.cpp
|   |-- DepartmentTree.cpp
|   |-- main.cpp
|-- data/
|-- testdata/
|-- README.txt
|-- CONTRIBUTIONS.txt
|-- design_document.txt
|-- test_results.txt
|-- .gitignore
