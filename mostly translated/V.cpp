//
//  V.cpp
//  ECE474HW2
//
//  Created by Mansur Olaitan on 12/6/18.
//  Copyright © 2018 Mansur Olaitan. All rights reserved.
//

#include <stdio.h>
#include <iostream>
#include <vector>
using namespace std;

class V {
    
    int ID;                     // Only used for sink nodes. Top sink node = 0, Bottom sink node = -1
    string op;            // Operation of the vertex (+, *, /, etc.)
    string varName;                // The variable being operated on. ie: h = a * b... h is the varName
    vector<V> successors; // Array of successors
    vector<V> predecessors;
    int delay;                    // Cycle delay. Multipliers = 2, divider/modulo = 3, all others = 1
    bool isScheduled;        // True if scheduled, false otherwise
    int timing;                    // Timing for ALAP
    int initialTime;            // Initial time from CDFG
    int final_time;                // Timing for list_r
    
    // Constructor
    public:
        V(int id, string op, string varName) {
            this->ID = id;
            this->op = op;
            this->varName = varName;
            vector<V> successors;
            vector<V> predecessors;
            delay = calculateDelay(op);
            isScheduled = false;
            timing = -1;
            initialTime = 1;
            final_time = -1;
        }
    
    // Sets the delay
     void setDelay(int delay) {
        this->delay = delay;
    }
    
     void setFinalTime(int time) {
        this->final_time = time;
    }
    
     int getFinalTime() {
        return this->final_time;
    }
    
     string getOperation() {
        return this->operation;
    }
    
    // Gets delay
     int getDelay() {
        return this->delay;
    }
    
    // Adds a successor
     void addSuccessor(V vertex) {
        this->successors.push_back(vertex);
    }
    
    // Returns successor array
     vector<V> getSuccessors() {
        return this->successors;
    }
    
    // Adds a successor
     void addPredecessors(V vertex) {
        this->predecessors.push_back(vertex);
    }
    
    // Returns successor array
     vector<V> getPredecessors() {
        return this->predecessors;
    }
    
     void setScheduled(bool set) {
        this->isScheduled = set;
    }
    
    // Returns size of successor array
     int getSuccessorSize() {
        return this->successors.size();
    }
    
     void setTiming(int time) {
        this->timing = time;
    }
    
     int getTiming() {
        return this->timing;
    }
    
     int getID() {
        return this->ID;
    }
    
     string getName() {
        return this->varName;
    }
    
     bool getIsScheduled() {
        return this->isScheduled;
    }
    
     void setInitialTime(int time) {
        this->initialTime = time;
    }
    
     int getInitialTime() {
        return this->initialTime;
    }
    /*
     * Sets the delay for specified operator
     * Multipliers = 2
     * Divider/modulo = 3
     * All others = 1
     */
     int calculateDelay(string op) {
        if(op == "*") {
            return 2;
        }
        else if(op == "/" || op == "%") {
            return 3;
        }
        else {
            return 1;
        }
    }
};
