//
//  List_r.cpp
//  ECE474HW2
//
//  Created by Mansur Olaitan on 12/6/18.
//  Copyright © 2018 Mansur Olaitan. All rights reserved.

//#include "main.cpp"
#include <stdio.h>
#include <iostream>
#include <unordered_set>
#include <vector>
#include <string>
#include <algorithm>
//#include <hashset>
#include <iterator>
#include "V.cpp"


using namespace std;

vector<string> tokenize(string s, string tokenizer) {
	int pos;
	vector <string> result;

	while ((pos = s.find(tokenizer)) != std::string::npos) {
		result.push_back(s.substr(0, pos));
		s.erase(0, pos + tokenizer.length());
	}
	if (s != " ") {
		result.push_back(s);
	}
	return result;
}

class list_r {
    private:
    vector<V> vertices;
    int v_id;
    
    public:
    list_r() {
        vector<V> vertices;
        
        vertices.push_back(*new V(0, "", "") );    // Top Sink node
        vertices.push_back(*new V(-1, "", "") );    // Bottom Sink node
        
        v_id = 1;
    }
    
    // Adds all vertices for specified section.
    // This gets called after all the declarations have been instantiated in the verilog file.
    // Variable "x" is the first line of statements in the input file'
     void addVertices(vector<string> lines, int x) {
        
        // Adds new vertex for each statement
        for(int i = x; i < lines.size(); i++) {
            vector<string> line = tokenize(lines.at(i), " ");
            //ArrayList<string> line = lines.at(i);
            // CHANGE BACK TO LINE
            string op = line.at(line.size() - 2);
            vertices.push_back(*new V(v_id, op, line.at(0)));    // Adds new vertex
            v_id++;
        }
    }

    // Searches for dependencies within the statements
     void addDependencies(vector<string> lines, int x) {
        
         unordered_set<string> set1; // = new HashSet<string>();        // Set to hold variables already worked on (dependencies)
        
        for(int i = x; i < lines.size(); i++ ) {
            vector<string> line = tokenize(lines.at(i), " ");
            //ArrayList<string> line = lines.at(i);
            
            // HAVE NOT ACCOUNTED FOR IF STATEMENTS YET
            // Determine if case: o = sel ? i1 : i0... I have not accounted for this case yet
            if(line.at(line.size()-2) != ":") {
                string var0 = line[0];
                string var1 = line[1];
                string var2 = line[line.size()-1];

				unordered_set<string>::iterator check0 = set1.find (var0);
				unordered_set<string>::iterator check1 = set1.find(var1);
				unordered_set<string>::iterator check2 = set1.find(var2);

                
                // If no dependency, add edge from sink node
                if(check1 == set1.end() && check2 == set1.end()) {
                    vertices.at(0).addSuccessor(findV(var0));            // Adds successor to sink node
                    findV(var0).addPredecessors(vertices.at(0));        // Adds predecessor
                    set1.insert(var0);
                }
                /* Initial Time is time from CDFG. Needed to compute slack for List_R scheduling */
                else {
                    if(check1 != set1.end() && check2 != set1.end()) {
                        findV(var1).addSuccessor(findV(var0));        // Adds successor
                        findV(var2).addSuccessor(findV(var0));        // Adds successor
                        findV(var0).addPredecessors(findV(var1));    // Adds predecessor
                        findV(var0).addPredecessors(findV(var2));    // Adds predecessor
                        findV(var0).setInitialTime(max(findV(var1).getInitialTime() + findV(var1).getDelay(), findV(var2).getInitialTime() + findV(var2).getDelay()));
                    }
                    else if(check1 != set1.end()) {
                        findV(var1).addSuccessor(findV(var0));        // Adds successor
                        findV(var0).addPredecessors(findV(var1));    // Adds predecessor
                        findV(var0).setInitialTime(findV(var1).getInitialTime() + findV(var1).getDelay());
                    }
                    else if(check2 != set1.end()) {
                        findV(var2).addSuccessor(findV(var0));        // Adds successor
                        findV(var0).addPredecessors(findV(var2));    // Adds predecessor
                        findV(var0).setInitialTime(findV(var2).getInitialTime() + findV(var2).getDelay());
                    }
                    set1.insert(var0);
                    vertices.at(0).addSuccessor(findV(var0));            // Adds successor to sink node
                }
            }
        }
        
        // Adds dependencies from leaf to bottom Sink node
        for(int j = 0; j < vertices.size(); j++) {
            if(vertices.at(j).getSuccessorSize() == 0) {    // If leaf, add successor to bottom sink node
                vertices.at(j).addSuccessor(vertices.at(1));
            }
            vertices.at(1).addPredecessors(vertices.at(j));    // Adds all vertices as predecessor to bottom sink node
        }
    }
    
    // Performs ALAP scheduling
     void alap(int latency) {
        
        // Schedules the bottom sink node for upper latency bound + 1 and sets isScheduled to true
        vertices.at(1).setTiming(latency + 1);
        vertices.at(1).setScheduled(true);
        
        // while all vertices are not scheduled, run the algorithm
        while(!allVerticesScheduled()) {
            // Iterate through vertices, find a vertex where all successors are scheduled
            for(int i = 0; i < vertices.size(); i++) {
                
                V currentV = vertices.at(i);
                bool valid = true;        // boolean to check if all successors are scheduled
                int lowestTime = 100;        // For multiple successors, holds minimum time
                
                if(currentV.getIsScheduled() == false) {            // If vertex is not scheduled, check all it's successors to determine if their scheduled
                    vector<V> successors = currentV.getSuccessors();
                    for(int j = 0; j < successors.size(); j++) {
                        if(successors.at(j).getIsScheduled() == false) {        // If all successors are not scheduled, break
                            valid = false;
                            break;
                        }
                        else {
                            if(successors.at(j).getTiming() < lowestTime) {
                                lowestTime = successors.at(j).getTiming();
                            }
                        }
                    }
                    if(valid == true) {
                        if(lowestTime-vertices.at(i).getDelay() < 1 && i != 0) {
                            cout << "Latency not possible!" << endl;
                            return;
                        }
                        vertices.at(i).setTiming(lowestTime - vertices.at(i).getDelay());
                        vertices.at(i).setScheduled(true);
                    }
                }
            }
        }
        // Top sink node is always time 0
        vertices.at(0).setTiming(0);
    }
    
	

    /* Runs list_r scheduling */
    void listR_Scheduling() {
        
		int resources[3];// = new int[3];
        resources[0] = 1;    // Resources requirement for ALU
        resources[1] = 1;    // Resources requirement for Multiplier
        resources[2] = 1;    // Resources requirement for Divider/Modulo
        
        // Unschedules all vertices besides top sink node
        for(int x = 1; x < vertices.size(); x++) {
            vertices.at(x).setScheduled(false);
        }
        vertices.at(0).setScheduled(true);        // Sets top sink node to scheduled
        vertices.at(1).setScheduled(true);        // Sets top sink node to scheduled
        
        int time_step = 1;
        unordered_set<V> unscheduled;
        
        while(!allVerticesScheduled()) {
            //System.out.print("Unscheduled: " );
            for(int i = 2; i < vertices.size(); i++) {
                if(vertices.at(i).getInitialTime() == time_step && !vertices.at(i).getIsScheduled()) {
                    unscheduled.insert(vertices.at(i));
                    //System.out.print(vertices.at(i).getName() + ", ");
                }
            }
            
            //System.out.println();
            
            /* Holds current # of resources at this time step */
            int mult_count = 0;
            int div_modulo_count = 0;
            int alu_count = 0;
            
			vector<V>::iterator it;// = unscheduled.iterator();
            while(it != it.end()) {
                V curr = (V) it.next();
                
                // If slack = 0, we must schedule now
                if(findV(curr.getName()).getTiming() - time_step == 0 && allPredecessorsScheduled(findV(curr.getName()))) {
                    findV(curr.getName()).setFinalTime(time_step);
                    findV(curr.getName()).setScheduled(true);
                    it.remove();
                    unscheduled.erase(findV(curr.getName()));
                    
                    /* Updates resources */
                    if(findV(curr.getName()).getOperation() == "*" ) {
                        mult_count++;
                        if(mult_count > resources[1]) {
                            resources[1]++;
                        }
                    }
                    else if(findV(curr.getName()).getOperation() == "/" || findV(curr.getName()).getOperation() == "%") {
                        div_modulo_count++;
                        if(div_modulo_count > resources[2]) {
                            resources[2]++;
                        }
                    }
                    else {
                        alu_count++;
                        if(alu_count > resources[0]) {
                            resources[0]++;
                        }
                    }
                }
            }
            printScheduled();
            
            /* Schedule operations requiring no additional resources */
            while(mult_count < resources[1]) {
				vector<V>::iterator it1;
                
                bool found = false;    // Variable to determine if any var is found
                
                while(it1.hasNext()) {
                    V curr = (V) it1.next();
                    
                    if(findV(curr.getName()).getDelay() == 2 ) {
                        findV(curr.getName()).setFinalTime(time_step);
                        findV(curr.getName()).setScheduled(true);
                        it1.remove();
                        unscheduled.erase(findV(curr.getName()));
                        mult_count++;
                        found = true;
                        break;
                    }
                }
                if(!found) {
                    break;
                }
            }
            
            while(div_modulo_count < resources[2]) {
				vector<V>::iterator it1;
				bool found = false;
                while(it1.hasNext()) {
                    V curr = (V) it1.next();
                    
                    if(findV(curr.getName()).getDelay() == 3 ) {
                        findV(curr.getName()).setFinalTime(time_step);
                        findV(curr.getName()).setScheduled(true);
                        it1.remove();
                        unscheduled.remove(findV(curr.getName()));
                        div_modulo_count++;
                        found = true;
                        break;
                    }
                }
                if(!found) {
                    break;
                }
            }
            
            while(alu_count < resources[0]) {
				vector<V>::iterator it1;
				bool found = false;
                while(it1.hasNext()) {
                    V curr = (V) it1.next();
                    
                    if(findV(curr.getName()).getDelay() == 1 ) {
                        findV(curr.getName()).setFinalTime(time_step);
                        findV(curr.getName()).setScheduled(true);
                        it1.remove();
                        unscheduled.erase(findV(curr.getName()));
                        alu_count++;
                        found = true;
                        break;
                    }
                }
                if(!found) {
                    break;
                }
            }
            
            //printScheduled();
            time_step++;
        }
    }
    
    
    // Finds vertex by name
     V findV(string varName) {
        for(int i = 0; i < vertices.size(); i++) {
            if(vertices.at(i).getID() != 0 && vertices.at(i).getID() != -1 && vertices.at(i).getName() == varName) {
                return vertices.at(i);
            }
        }
        return;
    }
    
     void printFinalTime() {
        for(int i = 0; i < vertices.size(); i++) {
            cout << vertices.at(i).getName() + " is scheduled in time " + to_string(vertices.at(i).getFinalTime()) << endl;
        }
    }
    
    
    // Returns true if all vertices scheduled, false otherwise
     bool allVerticesScheduled() {
        for(int i = 0; i < vertices.size(); i++) {
            if(vertices.at(i).getIsScheduled() == false) {
                return false;
            }
        }
        return true;
    }
    // Breaks up statement into array of words
     vector<string> tokenize(string s, string tokenizer) {
        int pos;
		vector<string> result;  // = new ArrayList<string>();
        
        while ( (pos = s.indexOf(tokenizer) ) != s.length()) {
            result.add(s.substring(0, pos));
            string temp = s.substring(0, pos + tokenizer.length());
            s = s.replace(temp, "");
        }
        if (s != " ") {
            result.push_back(s);
        }
        return result;
    }
    
    // True if all predecessors scheduled, false otherwise
     bool allPredecessorsScheduled(V curr) {
        for(int i = 0; i < curr.getPredecessors().size(); i++) {
            if(!curr.getPredecessors().at(i).getIsScheduled()) {
                return false;
            }
        }
        return true;
    }
    
     void printV() {
        for(int i = 0; i < vertices.size(); i++) {
            cout << "Vertex #" + to_string(i) + ": " + vertices.at(i).getName() + ", " + vertices.at(i).getOperation() << endl;
        }
    }
    
     void printDependencies() {
        for(int i = 0; i < vertices.size(); i++) {
			cout << "Vertex " + vertices.at(i).getName() + " has the following dependencies: " << endl;
            
            for(int j = 0; j < vertices.at(i).getSuccessorSize(); j++) {
                cout << vertices.at(i).getSuccessors().at(j).getName() + ", ";
            }
            cout << endl;
        }
    }
    
     void printPredecessors() {
        for(int i = 0; i < vertices.size(); i++) {
            cout << "Vertex " + vertices.at(i).getName() + " has the following predecessors: ";
            
            for(int j = 0; j < vertices.at(i).getPredecessors().size(); j++) {
                cout << vertices.at(i).getPredecessors().at(j).getName() + ", ";
            }
			cout << endl;
        }
    }
    
     void print_alap() {
        for(int i = 0; i < vertices.size(); i++) {
            cout << "Vertex " + vertices.at(i).getName() + " is scheduled in time " + to_string(vertices.at(i).getTiming()) << endl;
        }
    }
     void printDelays() {
        for(int i = 0; i < vertices.size(); i++) {
			cout << vertices.at(i).getDelay() << endl;
        }
    }
    
     void printScheduled() {
        cout << "Scheduled: ";
        for(int i = 0; i < vertices.size(); i++) {
            if(vertices.at(i).getIsScheduled()) {
                cout << vertices.at(i).getName() + ", ";
            }
        }
		cout << endl;
    }
    
     void printInitialTimes() {
        for(int i = 2; i < vertices.size(); i++) {
            cout << vertices.at(i).getInitialTime() << endl;
        }
    }
};
