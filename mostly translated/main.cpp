// ConsoleApplication3.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include "V.cpp"
#include "list_r.cpp"
#include <iostream>

// ConsoleApplication2.cpp : This file contains the 'main' function. Program execution begins and ends there.

#include <fstream>
#include <string>
#include <map>
#include <vector>
#include <cstring>
#include <stdlib.h>
using namespace std;

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
string getStateMachine(vector<unit> lines, int i);
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

	string IO = "parameter Wait = 0, ";
	vector <V> nodes;
	list_r scheduler;

	// Creates all the vertices
	scheduler.addVertices(lines, i);

	// Adds all dependencies
	scheduler.addDependencies(lines, i);

	// Performs alap scheduling
	scheduler.alap(latency);

	// Performs List R scheduling
	scheduler.listR_Scheduling();

	nodes = scheduler.getVertices();
	int last_time = nodes.at(nodes.size() - 2).getFinalTime();
	
	/* Writes all the states to the output string */
	for (int i = 1; i <= last_time; i++) {
		IO += "time_" + i;
		IO += " = " + i;
		IO += ", ";
	}

	IO += "Final = " + (last_time + 1);
	IO += ";\n";
	IO += "reg [1:0] State, StateNext;";
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
//returns parameter part of output file
/*
string getParam(vector<unit> inputs) {
	string parameters = "";
	map <string, string> param;
	for (int i = 0; i < inputs.size(); i++) {
		if (param.find(inputs[i].parameter) == param.end()) {
			param[inputs[i].parameter] = 'a';
			parameters += "\tparameter " + inputs[i].parameter + " = " + to_string(inputs[i].getParam()) + ";\n";
		}
	}
	return parameters;
}*/
//returns parameter part of output file
/*
string getParam(vector<unit> inputs) {
	string parameters = "";
	map <string, string> param;
	for (int i = 0; i < inputs.size(); i++) {
		if (param.find(inputs[i].parameter) == param.end()) {
			param[inputs[i].parameter] = 'a';
			parameters += "\tparameter " + inputs[i].parameter + " = " + to_string(inputs[i].getParam()) + ";\n";
		}
	}
	return parameters;
}*/
// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
