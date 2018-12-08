#include "pch.h"
#include <iostream>
// ConsoleApplication3.cpp : This file contains the 'main' function. Program execution begins and ends there.


#include <fstream>
#include <string>
#include <map>
#include <vector>
#include <cstring>
#include <algorithm>
#include <unordered_set>
#include <iterator>
#include <stdlib.h>
#include <stdio.h>
using namespace std;

/* Start of V.cpp */
/* Start of V.cpp */
/* Start of V.cpp */
/* Start of V.cpp */
/* Start of V.cpp */
/* Start of V.cpp */
/* Start of V.cpp */
/* Start of V.cpp */
/* Start of V.cpp */
/* Start of V.cpp */
/* Start of V.cpp */
/* Start of V.cpp */
/* Start of V.cpp */

class V {
private:
	int ID;                     // Only used for sink nodes. Top sink node = 0, Bottom sink node = -1
	string op;            // Operation of the vertex (+, *, /, etc.)
	string varName;                // The variable being operated on. ie: h = a * b... h is the varName
	vector<V> successors; // Array of successors
	vector<V> predecessors;
	int delay;                    // Cycle delay. Multipliers = 2, divider/modulo = 3, all others = 1
	bool isScheduled;        // True if scheduled, false otherwise
	int timing;                    // Timing for ALAP
	int initialTime;            // Initial time from CDFG

	// Constructor
public:
	int final_time;                // Timing for list_r
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
	
	int pleaseWork() const {
		return this->ID;
	}

	bool operator ==(const V & obj) const {
		if (ID == obj.ID)
			return true;
		else
			return false;
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
		return this->op;
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
		if (op == "*") {
			return 2;
		}
		else if (op == "/" || op == "%") {
			return 3;
		}
		else {
			return 1;
		}
	}
};

class unit {
public:
	string varName;    //a,b,c,etc. Was "uname"
	string type; 			//i,e, input,output, wire. Was "utype"
	string dataType; 	//int8,16,etc. Was "parameter"

	string getDetails() {
		return this->varName + " " + this->dataType + "  " + this->type;
	}

	int getParam() {
		map <string, int> temp;
		temp["Int1"] = 1;
		temp["Int2"] = 2;
		temp["Int8"] = 8;
		temp["Int16"] = 16;
		temp["Int32"] = 32;
		temp["Int64"] = 64;
		temp["UInt1"] = 1;
		temp["UInt2"] = 2;
		temp["UInt8"] = 8;
		temp["UInt16"] = 16;
		temp["UInt32"] = 32;
		temp["UInt64"] = 64;
		return temp[this->dataType];
	}
};

map <string, string> sizes;		// <Variable Name, Data Type>
string toVerilog(vector<string> data, string filename, int latency);
int getDeclarations(vector <string> lines, vector <unit> &declarations);
string setDeclarations(vector<unit>inputs);
string getParam(vector<unit> inputs);
vector<string> tokenize(string s, string tokenizer);
string removeSpaces(string s);
void outFileFun(string outfilename, string output);
string getStateMachine(vector<string> lines, int i, int latency);
//void addVertices(vector<unit> lines, int i);
//void addDependencies(vector<unit> lines, int i);
//void alap(int latency);

int main(int argc, char ** argv) {

	// Ensures correct command-line usage
	if (argc != 5) {
		cout << "USAGE: hlsyn cFile latency verilogFile";
		return -1;
	}

	string inputFile = argv[2];				// Input C File to be read from
	int latency = atoi(argv[3]);			// Latency contraint
	cout << latency << "\n";
	string outputFile = argv[4];			// Output file

	vector <string> lines;		// Array to hold each line from input file

	ifstream infile;
	infile.open("test1.txt");
	string temp = "";

	// Reads input file
	if (infile.is_open()) {
		while (getline(infile, temp)) {
			if (temp.length() > 1 && temp.substr(0, 2) != "//") {		// Ensures no empty lines or comments
				// Removes all spaces from line
				temp = removeSpaces(temp);
				lines.push_back(temp);
			}
		}

		infile.close();

		string output = toVerilog(lines, outputFile, latency);

		outFileFun(outputFile, output);
	}
	else {
		cout << "File missing/Incorrect path\n";
	}
}


// Converts to HLSM in verilog
string toVerilog(vector<string> data, string filename, int latency) {
	cout << "Hi\n";
	string ofile = "`timescale 1ns / 1ps\n\nmodule HLSM (Clk, Rst, Start, Done";
	string wires = "";

	vector <unit> declarations;

	// Updates vector "declarations" with the inputs, outputs, and variable declarations
	// int i is the location of the first statement
	int i = getDeclarations(data, declarations);

	// Initializes the parameters for verilog function
	for (int x = 0; x < declarations.size(); x++) {
		if (declarations[x].type != "variable") {
			ofile = ofile + ", " + declarations[x].varName;
		}
	}

	ofile = ofile + ");\n";

	// Parameters are done. Move to local variables

	ofile += setDeclarations(declarations) + "\n";
	ofile += getStateMachine(data, i, latency);
	//ofile += getParam(declarations)+"\n";
	//ofile += operations(data, i) + "\n";

	return ofile;
}

/* Calls list_r, creates valid HLSM */
string getStateMachine(vector<string> lines, int i, int latency) {

	//string IO = "parameter Wait = 0, ";
	//vector <V> nodes;
	//list_r scheduler;

	//// Creates all the vertices
	//scheduler.addVertices(lines, i);

	//// Adds all dependencies
	//scheduler.addDependencies(lines, i);

	//// Performs alap scheduling
	//scheduler.alap(latency);

	//// Performs List R scheduling
	//scheduler.listR_Scheduling();

	//nodes = scheduler.getVertices();
	//int last_time = nodes.at(nodes.size() - 2).getFinalTime();
	//
	///* Writes all the states to the output string */
	//for (int i = 1; i <= last_time; i++) {
	//	IO += "time_" + i;
	//	IO += " = " + i;
	//	IO += ", ";
	//}

	//IO += "Final = " + (last_time + 1);
	//IO += ";\n";
	//IO += "reg [1:0] State, StateNext;";
}



/* INPUTS: 		vector <string> lines = array of input lines
*
* FUNCTIONALITY: Configures the inputs, outputs, and variables
*
*/
int getDeclarations(vector <string> lines, vector <unit> &declarations) {

	int i = 0;

	// Iterates through input lines
	while (i < lines.size()) {
		string line = lines[i];

		// Breakes input line up into array of words
		vector <string> tokens = tokenize(line, " ");

		// Once we get to statements, we stop.
		if (tokens[0] != "input" &&  tokens[0] != "output"&& tokens[0] != "variable") {
			return i;
			break;
		}

		string type = tokens[0];				// input, output, or variable
		string dataType = tokens[1];		// holds the data type (Int1, Int2, etc...)

		unit temp;
		temp.type = type;
		temp.dataType = dataType;

		int j = 2;

		// Iterate through rest of line (These will be the variable names)
		while (j < tokens.size()) {
			temp.varName = tokens[j].substr(0, tokens[j].length() - 1);	// Gets rid of comma
			j++;
			declarations.push_back(temp);

			// Adds to global map of all variables
			sizes[temp.varName] = temp.dataType;
		}

		declarations.pop_back();
		temp.varName = tokens[j - 1];
		sizes[temp.varName] = temp.dataType;
		declarations.push_back(temp);
		i++;
	}
	return 0;
}

// Types input, output declarations in outputfile
string setDeclarations(vector<unit>inputs) {
	string IO = "\tinput Clk, Rst, Start;\n";
	IO += "\toutput reg Done;\n";

	if (inputs[0].getParam() == 1) {
		IO += ";\n\t" + inputs[0].type + " " + inputs[0].varName;
	}
	else {
		IO += "\t" + inputs[0].type + " " + "[" + to_string(inputs[0].getParam() - 1) + ":0] " + inputs[0].varName;
	}

	for (int i = 1; i < inputs.size(); i++) {
		// Check if input or output/reg
		if (inputs[i].type == "input") {
			if (inputs[i].dataType == inputs[i - 1].dataType  && inputs[i].type == inputs[i - 1].type) {
				IO += ", " + inputs[i].varName;
			}
			else {
				if (inputs[i].getParam() == 1) {
					IO += ";\n\t" + inputs[i].type + " " + inputs[i].varName;
				}
				else {
					IO += ";\n\t" + inputs[i].type + " " + "[" + to_string(inputs[i].getParam() - 1) + ":0] " + inputs[i].varName;
				}
			}
		}
		else {
			if (inputs[i].dataType == inputs[i - 1].dataType  && inputs[i].type == inputs[i - 1].type) {
				IO += ", " + inputs[i].varName;
			}
			else {
				if (inputs[i].getParam() == 1) {
					IO += ";\n\t" + inputs[i].type + " reg " + inputs[i].varName;
				}
				else {
					IO += ";\n\t" + inputs[i].type + " reg " + "[" + to_string(inputs[i].getParam() - 1) + ":0] " + inputs[i].varName;
				}
			}
		}
	}

	IO += ";";
	return IO;
}

//returns parameter part of output file
string getParam(vector<unit> inputs) {
	string parameters = "";
	map <string, string> param;

	for (int i = 0; i < inputs.size(); i++) {
		if (param.find(inputs[i].dataType) == param.end()) {
			param[inputs[i].dataType] = 'a';
			parameters += "\tparameter " + inputs[i].dataType + " = " + to_string(inputs[i].getParam()) + ";\n";
		}
	}
	return parameters;
}

// Breaks up statement into array of words
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

//writes string to file
void outFileFun(string outfilename, string output) {
	ofstream outfile;
	outfile.open(outfilename.c_str());
	outfile << output;
	outfile.close();
}

//Removes spaces and inline coments
string removeSpaces(string s) {
	int count = 0;
	for (int i = 0; i < s.length(); i++) {
		if (s[i] == ' ' || s[i] == '\t') {
			count++;
		}
		else {
			count = 0;
		}
		if (count > 1) {
			i--;
			s.erase(s.begin() + i);
		}
		if (s[i] == '/') {
			s = s.substr(0, i);
		}
	}
	if (s[0] == ' ' || s[0] == '\t') {
		s.erase(s.begin());
	}
	if (s[s.length() - 1] == ' ' || s[s.length() - 1] == '\t') {
		s.erase(s.begin() + s.length() - 1);
	}
	return s;
}

template<>
struct hash<V>
{
	size_t
		operator()(const V & obj) const
	{
		return hash<int>()(obj.pleaseWork());
	}
};

class list_r {
private:
	vector<V> vertices;
	int v_id;

public:
	list_r() {
		vector<V> vertices;

		vertices.push_back(*new V(0, "", ""));    // Top Sink node
		vertices.push_back(*new V(-1, "", ""));    // Bottom Sink node

		v_id = 1;
	}



	// Adds all vertices for specified section.
	// This gets called after all the declarations have been instantiated in the verilog file.
	// Variable "x" is the first line of statements in the input file'
	void addVertices(vector<string> lines, int x) {

		// Adds new vertex for each statement
		for (int i = x; i < lines.size(); i++) {
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

		std::unordered_set<std::string> set1; // = new HashSet<string>();        // Set to hold variables already worked on (dependencies)

		for (int i = x; i < lines.size(); i++) {
			vector<string> line = tokenize(lines.at(i), " ");
			//ArrayList<string> line = lines.at(i);

			// HAVE NOT ACCOUNTED FOR IF STATEMENTS YET
			// Determine if case: o = sel ? i1 : i0... I have not accounted for this case yet
			if (line.at(line.size() - 2) != ":") {
				string var0 = line[0];
				string var1 = line[1];
				string var2 = line[line.size() - 1];

				std::unordered_set<std::string>::iterator check1 = set1.find(var1);
				std::unordered_set<std::string>::iterator check2 = set1.find(var2);


				// If no dependency, add edge from sink node
				if (check1 == set1.end() && check2 == set1.end()) {
					vertices.at(0).addSuccessor(findV(var0));            // Adds successor to sink node
					findV(var0).addPredecessors(vertices.at(0));        // Adds predecessor
					set1.insert(var0);
				}
				/* Initial Time is time from CDFG. Needed to compute slack for List_R scheduling */
				else {
					if (check1 != set1.end() && check2 != set1.end()) {
						findV(var1).addSuccessor(findV(var0));        // Adds successor
						findV(var2).addSuccessor(findV(var0));        // Adds successor
						findV(var0).addPredecessors(findV(var1));    // Adds predecessor
						findV(var0).addPredecessors(findV(var2));    // Adds predecessor
						findV(var0).setInitialTime(max(findV(var1).getInitialTime() + findV(var1).getDelay(), findV(var2).getInitialTime() + findV(var2).getDelay()));
					}
					else if (check1 != set1.end()) {
						findV(var1).addSuccessor(findV(var0));        // Adds successor
						findV(var0).addPredecessors(findV(var1));    // Adds predecessor
						findV(var0).setInitialTime(findV(var1).getInitialTime() + findV(var1).getDelay());
					}
					else if (check2 != set1.end()) {
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
		for (int j = 0; j < vertices.size(); j++) {
			if (vertices.at(j).getSuccessorSize() == 0) {    // If leaf, add successor to bottom sink node
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
		while (!allVerticesScheduled()) {
			// Iterate through vertices, find a vertex where all successors are scheduled
			for (int i = 0; i < vertices.size(); i++) {

				V currentV = vertices.at(i);
				bool valid = true;        // boolean to check if all successors are scheduled
				int lowestTime = 100;        // For multiple successors, holds minimum time

				if (currentV.getIsScheduled() == false) {            // If vertex is not scheduled, check all it's successors to determine if their scheduled
					vector<V> successors = currentV.getSuccessors();
					for (int j = 0; j < successors.size(); j++) {
						if (successors.at(j).getIsScheduled() == false) {        // If all successors are not scheduled, break
							valid = false;
							break;
						}
						else {
							if (successors.at(j).getTiming() < lowestTime) {
								lowestTime = successors.at(j).getTiming();
							}
						}
					}
					if (valid == true) {
						if (lowestTime - vertices.at(i).getDelay() < 1 && i != 0) {
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
		for (int x = 1; x < vertices.size(); x++) {
			vertices.at(x).setScheduled(false);
		}
		vertices.at(0).setScheduled(true);        // Sets top sink node to scheduled
		vertices.at(1).setScheduled(true);        // Sets top sink node to scheduled

		int time_step = 1;
		std::unordered_set<V> unscheduled;

		while (!allVerticesScheduled()) {
			//System.out.print("Unscheduled: " );
			for (int i = 2; i < vertices.size(); i++) {
				if (vertices.at(i).getInitialTime() == time_step && !vertices.at(i).getIsScheduled()) {
					unscheduled.insert(vertices.at(i));
					//System.out.print(vertices.at(i).getName() + ", ");
				}
			}

			//System.out.println();

			/* Holds current # of resources at this time step */
			int mult_count = 0;
			int div_modulo_count = 0;
			int alu_count = 0;

			//vector<V>::iterator it;// = unscheduled.iterator();
			for (unordered_set<V>::iterator it = unscheduled.begin(); it != unscheduled.end(); ++it) {
				V curr = *it;
				// If slack = 0, we must schedule now
				if (curr.getTiming() - time_step == 0 && allPredecessorsScheduled(curr)) {
					curr.setFinalTime(time_step);
					curr.setScheduled(true);

					unscheduled.erase(findV(curr.getName()));

					/* Updates resources */
					if (findV(curr.getName()).getOperation() == "*") {
						mult_count++;
						if (mult_count > resources[1]) {
							resources[1]++;
						}
					}
					else if (findV(curr.getName()).getOperation() == "/" || findV(curr.getName()).getOperation() == "%") {
						div_modulo_count++;
						if (div_modulo_count > resources[2]) {
							resources[2]++;
						}
					}
					else {
						alu_count++;
						if (alu_count > resources[0]) {
							resources[0]++;
						}
					}
				}
			}
			printScheduled();

			/* Schedule operations requiring no additional resources */
			while (mult_count < resources[1]) {
				vector<V>::iterator it1;

				bool found = false;    // Variable to determine if any var is found

				for (unordered_set<V>::iterator it = unscheduled.begin(); it != unscheduled.end(); ++it) {
					V curr1 = *it1;

					if (curr1.getDelay() == 2) {
						curr1.setFinalTime(time_step);
						curr1.setScheduled(true);
						unscheduled.erase(curr1);
						mult_count++;
						found = true;
						break;
					}
				}
				if (!found) {
					break;
				}
			}

			while (div_modulo_count < resources[2]) {
				bool found = false;
				for (unordered_set<V>::iterator it2 = unscheduled.begin(); it2 != unscheduled.end(); ++it2) {
					V curr2 = *it2;

					if (curr2.getDelay() == 3) {
						curr2.setFinalTime(time_step);
						curr2.setScheduled(true);
						// it2.remove();
						unscheduled.erase(curr2);
						div_modulo_count++;
						found = true;
						break;
					}
				}
				if (!found) {
					break;
				}
			}

			while (alu_count < resources[0]) {
				vector<V>::iterator it1;
				bool found = false;
				for (unordered_set<V>::iterator it3 = unscheduled.begin(); it3 != unscheduled.end(); ++it3) {
					V curr3 = *it3;

					if (curr3.getDelay() == 1) {
						curr3.setFinalTime(time_step);
						curr3.setScheduled(true);
						//    it3.remove();
						unscheduled.erase(curr3);
						alu_count++;
						found = true;
						break;
					}
				}
				if (!found) {
					break;
				}
			}

			//printScheduled();
			time_step++;
		}
	}


	// Finds vertex by name
	V findV(string varName) {
		for (int i = 0; i < vertices.size(); i++) {
			if (vertices.at(i).getID() != 0 && vertices.at(i).getID() != -1 && vertices.at(i).getName() == varName) {
				return vertices.at(i);
			}
		}
	}

	void printFinalTime() {
		for (int i = 0; i < vertices.size(); i++) {
			cout << vertices.at(i).getName() + " is scheduled in time " + to_string(vertices.at(i).getFinalTime()) << endl;
		}
	}


	// Returns true if all vertices scheduled, false otherwise
	bool allVerticesScheduled() {
		for (int i = 0; i < vertices.size(); i++) {
			if (vertices.at(i).getIsScheduled() == false) {
				return false;
			}
		}
		return true;
	}

	// True if all predecessors scheduled, false otherwise
	bool allPredecessorsScheduled(V curr) {
		for (int i = 0; i < curr.getPredecessors().size(); i++) {
			if (!curr.getPredecessors().at(i).getIsScheduled()) {
				return false;
			}
		}
		return true;
	}

	void printV() {
		for (int i = 0; i < vertices.size(); i++) {
			cout << "Vertex #" + to_string(i) + ": " + vertices.at(i).getName() + ", " + vertices.at(i).getOperation() << endl;
		}
	}

	void printDependencies() {
		for (int i = 0; i < vertices.size(); i++) {
			cout << "Vertex " + vertices.at(i).getName() + " has the following dependencies: " << endl;

			for (int j = 0; j < vertices.at(i).getSuccessorSize(); j++) {
				cout << vertices.at(i).getSuccessors().at(j).getName() + ", ";
			}
			cout << endl;
		}
	}

	void printPredecessors() {
		for (int i = 0; i < vertices.size(); i++) {
			cout << "Vertex " + vertices.at(i).getName() + " has the following predecessors: ";

			for (int j = 0; j < vertices.at(i).getPredecessors().size(); j++) {
				cout << vertices.at(i).getPredecessors().at(j).getName() + ", ";
			}
			cout << endl;
		}
	}

	/* Returns vertices */
	vector<V> getVertices() {
		return this->vertices;
	}

	struct less_than_key
	{
		inline bool operator() (const V& struct1, const V& struct2)
		{
			return (struct1.final_time < struct2.final_time);
		}
	};

	void print_alap() {
		for (int i = 0; i < vertices.size(); i++) {
			cout << "Vertex " + vertices.at(i).getName() + " is scheduled in time " + to_string(vertices.at(i).getTiming()) << endl;
		}
	}
	void printDelays() {
		for (int i = 0; i < vertices.size(); i++) {
			cout << vertices.at(i).getDelay() << endl;
		}
	}

	void printScheduled() {
		cout << "Scheduled: ";
		for (int i = 0; i < vertices.size(); i++) {
			if (vertices.at(i).getIsScheduled()) {
				cout << vertices.at(i).getName() + ", ";
			}
		}
		cout << endl;
	}

	void printInitialTimes() {
		for (int i = 2; i < vertices.size(); i++) {
			cout << vertices.at(i).getInitialTime() << endl;
		}
	}
};
