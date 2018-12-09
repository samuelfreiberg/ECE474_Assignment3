#include "Lib.h"
#include "State.h"
using namespace std;


	//three hash table maps: map1: namne and size. map2: name and unsigned or signed. map3: name and type of input or output
	//create a set of variables being declared, if not declare then check the hash table for all the maps and put all of it down. 
	//Or just have object

string getStr(string var, char sign, int size, int max) {
	string temp = var;
	if (sign == 'U') {
		if (max > size)
			temp = "{'d0," + var + "}";
		else if (max < size)
			temp = var + "[" + std::to_string(max-1) + ":0]";
	}
	else {
		if (max > size) {
			if (size > 0)
				temp = "{" + var + "[" + std::to_string(size-1) + "],";
			else
				temp = "{0";
			for (int i = 0; i < (max - size - 1); i++)
				temp = temp + "0";
			if (size > 0)
				temp = temp + ", " + var + "[" + std::to_string(size - 2) + ":0]}";
			else
				temp = temp + ", " + var + "[0]}";
		}
		else if (max < size)
			temp = var + "[" + std::to_string(max-1) + ":0]";
	}

	return temp;
}


string printParm(int size) {
	string temp;
	int i = 0, j = 0;
	while (size + 2 > pow(2, i)) 
		i++;
	temp = "\nparameter init = 0, final = 1";
	for (j = 0; j < size; j++)
		temp = temp + ", s" + std::to_string(j) + " = " + std::to_string(j + 2);
	temp = temp + ";\n";
	temp = temp + "reg [" + std::to_string(i) + ":0] state, stateNext;\n";
	return temp;
}



bool openFiles(char* inputFile, char* lat, char* outputFile) {

	ifstream inp;
	ofstream outp;
	string var1, var2, var3, var4, var5, var6, var7, var8;

	inp.open(inputFile);
	if (!inp.good()) return false;

	int i = 0, tempSize = 0, max = 0, pos = 0, stackHolder = 0;
	int ptrA = 0, ptrM = 0, ptrD = 0, ptrL = 0; //pointers for adder, multi, div/mod, and logical
	char tempSign;
	string line, word, temp;
	istringstream buffer;
	vector <string> print;
	map <string, int> size; //32 bit, 16 bit
	map <string, char> sign; // U for unsigned or S for signed
	State init = State("init"), done = State("final");//used for begginning state and final state
	vector <State> states;

	stack<int> ifStack;
	stack<int> nothingStack;
	stack<int> noElseStack;
	
	

	//creates beginning of file
	print.push_back("`timescale 1ns / 1ps\n\n\n");
	print.push_back("module HLSM (Clk, Rst, Start, Done");
	print.push_back("input Clk, Rst, Start;\n");
	print.push_back("output reg Done;\n\n");



	//reads in file line by line
	while (getline(inp, line)) {
		temp = "";
		/* skips comments and empty lines */
		if (line == "\n" || line == "")
			continue;
		else if (line.at(0) == '/' && line.at(1) == '/')
			continue;
		/*reads inputs and outputs*/
		if (line.find("input ") == 0){
			temp = "input ";
			buffer.str (line);
			while (buffer >> word) {
				if (word == "input")
					continue;
				else if (word.find("Int") == 0 || word.find("Int") == 1) {
					tempSign = word.at(0);
					if (tempSign == 'U') {
						tempSize = stoi(word.substr(4));
						temp = temp + "unsigned ";
					}
					else
						tempSize = stoi(word.substr(3));
					if (tempSize > 1)
						temp = temp + "[" + std::to_string(tempSize-1) + ":0]";
				}
				else{
					if (word.find(',') != string::npos)
						word = word.substr(0, word.size() - 1);
					size.insert(pair<string,int>(word, tempSize));
					sign.insert(pair<string, char>(word, tempSign));
					print.at(1) = print.at(1) + ", " + word;
					temp = temp + " " + word + ",";
				}
			}
			temp.at(temp.size() - 1) = ';';
			print.push_back(temp + "\n");
			buffer.clear();
			continue;
		}
		else if (line.find("output ") == 0){
			temp = "output ";
			buffer.str(line);
			while (buffer >> word) {
				if (word == "output")
					continue;
				else if (word.find("Int") == 0 || word.find("Int") == 1) {
					tempSign = word.at(0);
					if (tempSign == 'U') {
						tempSize = stoi(word.substr(4));
						temp = temp + "unsigned ";
					}
					else
						tempSize = stoi(word.substr(3));
					if (tempSize > 1)
						temp = temp + "[" + std::to_string(tempSize - 1) + ":0]";
				}
				else {
					if (word.find(',') != string::npos)
						word = word.substr(0, word.size() - 1);
					size.insert(pair<string, int>(word, tempSize));
					sign.insert(pair<string, char>(word, tempSign));
					print.at(1) = print.at(1) + ", " + word;
					temp = temp + " " + word + ",";
				}
			}
			temp.at(temp.size() - 1) = ';';
			print.push_back(temp+"\n");
			buffer.clear();
			continue;
		}
		else if (line.find("variable ") == 0){
			temp = "wire ";
			buffer.str(line);
			while (buffer >> word) {
				if (word == "variable")
					continue;
				else if (word.find("Int") == 0 || word.find("Int") == 1) {
					tempSign = word.at(0);
					if (tempSign == 'U') {
						tempSize = stoi(word.substr(4));
						temp = temp + "unsigned ";
					}
					else
						tempSize = stoi(word.substr(3));
					if (tempSize > 1)
						temp = temp + "[" + std::to_string(tempSize - 1) + ":0]";
				}
				else {
					if (word.find(',') != string::npos)
						word = word.substr(0, word.size() - 1);
					size.insert(pair<string, int>(word, tempSize));
					sign.insert(pair<string, char>(word, tempSign));
					print.at(1) = print.at(1) + ", " + word;
					temp = temp + " " + word + ",";
				}
			}
			temp.at(temp.size() - 1) = ';';
			print.push_back(temp + "\n");
			buffer.clear();
			continue; 
		}



		if (line.find("if ") != string::npos || line.find("if(") != string::npos) {//stack
			ifStack.push(states.size()-1); // size is s6, adding 6 to the stack
			states.at(states.size() - 1).addString(line.substr(line.find("if"), line.rfind(')')-line.find("if") + 1));
			states.at(states.size() - 1).addString("\tstateNext <= s" + std::to_string(states.size()) + ";\n\t\telse\n");
			ptrA = states.size(); ptrL = states.size(); ptrM = states.size();
			states.push_back(states.size());


		}
		else if (line.find("else") != string::npos) {
			stackHolder = ifStack.top();
			ifStack.pop();
			states.at(stackHolder).addString("\t\t\tstateNext <= s" + std::to_string(states.size()) + ";\n");
			ptrA = states.size(); ptrL = states.size(); ptrM = states.size();
			states.push_back(states.size());
		}
		else if (line.find("}") != string::npos) {
			ptrA = states.size(); ptrL = states.size(); ptrM = states.size();
			if(!nothingStack.empty())
				if (nothingStack.top() == states.size() - 1) {
					noElseStack.push(ifStack.top());
					ifStack.pop();
				}
				else
					nothingStack.push(states.size() - 1);
			else
				nothingStack.push(states.size() - 1);

		}
		
		buffer.str(line);
		if (line.find("+") != string::npos) { //complete adder
			buffer >> var1 >> var2 >> var3 >> var4 >> var5;
			if (size.find(var1) == size.end() || size.find(var3) == size.end() || size.find(var5) == size.end()) { cout << "Erorr, missing variable\n"; return true; }
			max = size[var1];
			var3 = getStr(var3, sign[var3], size[var3], max);
			var5 = getStr(var5, sign[var5], size[var5], max);

			if (states.size() == ptrA) {
				states.push_back(states.size());
				states.at(states.size() - 1).addString("ADD #(" + std::to_string(max) + ") component" + std::to_string(pos) + "(" + var3 + ", " + var5 + ", " + var1 + ");");
				if (states.size() > 1)
					states.at(states.size() - 2).setNext("stateNext <= s" + std::to_string(states.size() - 1) + ";");
			}
			else {
				states.at(ptrA).addString("ADD #(" + std::to_string(max) + ") component" + std::to_string(pos) + "(" + var3 + ", " + var5 + ", " + var1 + ");");
			}
			pos = pos++;
			ptrA = ptrA + 1;
		}
		else if (line.find("<<") != string::npos || line.find(">>") != string::npos) {
			buffer >> var1 >> var2 >> var3 >> var4 >> var5;
			switch (var4.at(0)) {
			case '<':
				if (size.find(var1) == size.end() || size.find(var3) == size.end() || size.find(var5) == size.end()) { cout << "Erorr, missing variable\n"; return true; }
				max = size[var1];
				var3 = getStr(var3, sign[var3], size[var3], max);
				var5 = getStr(var5, sign[var5], size[var5], max);
				if (states.size() == ptrL) {
					states.push_back(states.size());
					states.at(states.size() - 1).addString("SHL #(" + std::to_string(max) + ") component" + std::to_string(pos) + "(" + var3 + ", " + var5 + ", " + var1 + ");");
					if (states.size() > 1)
						states.at(states.size() - 2).setNext("stateNext <= s" + std::to_string(states.size() - 1) + ";");
				}
				else {
					states.at(ptrL).addString("SHL #(" + std::to_string(max) + ") component" + std::to_string(pos) + "(" + var3 + ", " + var5 + ", " + var1 + ");");
				}
				break;
			case '>':
				if (size.find(var1) == size.end() || size.find(var3) == size.end() || size.find(var5) == size.end()) { cout << "Erorr, missing variable\n"; return true; }
				max = size[var1];
				var3 = getStr(var3, sign[var3], size[var3], max);
				var5 = getStr(var5, sign[var5], size[var5], max);
				if (states.size() == ptrL) {
					states.push_back(states.size());
					states.at(states.size() - 1).addString("SHL #(" + std::to_string(max) + ") component" + std::to_string(pos) + "(" + var3 + ", " + var5 + ", " + var1 + ");");
					if (states.size() > 1)
						states.at(states.size() - 2).setNext("stateNext <= s" + std::to_string(states.size() - 1) + ";");
				}
				else {
					states.at(ptrL).addString("SHR #(" + std::to_string(max) + ") component" + std::to_string(pos) + "(" + var3 + ", " + var5 + ", " + var1 + ");");
				}
				break;
			default: cout << "Invalid" << endl;
			}
			ptrL = ptrL + 1;
			pos += 1;
		}
		else if (line.find("-") != string::npos) { //complete subtractor
			buffer >> var1 >> var2 >> var3 >> var4 >> var5;
			if (size.find(var1) == size.end() || size.find(var3) == size.end() || size.find(var5) == size.end()) { cout << "Erorr, missing variable\n"; return true; }
			max = size[var1];
			var3 = getStr(var3, sign[var3], size[var3], max);
			var5 = getStr(var5, sign[var5], size[var5], max);

			if (states.size() == ptrA) {
				states.push_back(states.size());
				states.at(states.size() - 1).addString("SUB #(" + std::to_string(max) + ") component" + std::to_string(pos) + "(" + var3 + ", " + var5 + ", " + var1 + ");");
				if (states.size() > 1)
					states.at(states.size() - 2).setNext("stateNext <= s" + std::to_string(states.size() - 1) + ";");
			}
			else {
				states.at(ptrA).addString("SUB #(" + std::to_string(max) + ") component" + std::to_string(pos) + "(" + var3 + ", " + var5 + ", " + var1 + ");");
			}
			pos = pos++;
			ptrA = ptrA + 1;
		}
		else if (line.find(">") != string::npos || line.find("<") != string::npos || line.find("==") != string::npos) {
			buffer >> var1 >> var2 >> var3 >> var4 >> var5; //COMP
			switch (var4.at(0)) {
			case '=':
				if (size.find(var1) == size.end() || size.find(var3) == size.end() || size.find(var5) == size.end()) { cout << "Erorr, missing variable\n"; return true; }
				var1 = var1 + "[0]";
				max = std::max(size[var3],size[var5]);
				var3 = getStr(var3, sign[var3], size[var3], max);
				var5 = getStr(var5, sign[var5], size[var5], max);
				if (states.size() == ptrL) {
					states.push_back(states.size());
					states.at(states.size() - 1).addString("COMP #(" + std::to_string(max) + ") component" + std::to_string(pos) + "(" + var3 + ", " + var5 + ", 0, 0, " + var1 + ");");
					if (states.size() > 1)
						states.at(states.size() - 2).setNext("stateNext <= s" + std::to_string(states.size() - 1) + ";");
				}
				else {
					states.at(ptrL).addString("COMP #(" + std::to_string(max) + ") component" + std::to_string(pos) + "(" + var3 + ", " + var5 + ", 0, 0, " + var1 + ");");
				}
				break;
			case '<':
				if (size.find(var1) == size.end() || size.find(var3) == size.end() || size.find(var5) == size.end()) { cout << "Erorr, missing variable\n"; return true; }
				var1 = var1 + "[0]";
				max = std::max(size[var3], size[var5]);
				var3 = getStr(var3, sign[var3], size[var3], max);
				var5 = getStr(var5, sign[var5], size[var5], max);
				if (states.size() == ptrL) {
					states.push_back(states.size());
					states.at(states.size() - 1).addString("COMP #(" + std::to_string(max) + ") component" + std::to_string(pos) + "(" + var3 + ", " + var5 + ", 0, " + var1 + ", 0);");
					if (states.size() > 1)
						states.at(states.size() - 2).setNext("stateNext <= s" + std::to_string(states.size() - 1) + ";");
				}
				else {
					states.at(ptrL).addString("COMP #(" + std::to_string(max) + ") component" + std::to_string(pos) + "(" + var3 + ", " + var5 + ", 0, " + var1 + ", 0);");
				}
				break;
			case '>':
				if (size.find(var1) == size.end() || size.find(var3) == size.end() || size.find(var5) == size.end()) { cout << "Erorr, missing variable\n"; return true; }
				var1 = var1 + "[0]";
				max = std::max(size[var3], size[var5]);
				var3 = getStr(var3, sign[var3], size[var3], max);
				var5 = getStr(var5, sign[var5], size[var5], max);
				if (states.size() == ptrL) {
					states.push_back(states.size());
					states.at(states.size() - 1).addString("COMP #(" + std::to_string(max) + ") component" + std::to_string(pos) + "(" + var3 + ", " + var5 + ", " + var1 + ", 0, 0);");
					if (states.size() > 1)
						states.at(states.size() - 2).setNext("stateNext <= s" + std::to_string(states.size() - 1) + ";");
				}
				else {
					states.at(ptrL).addString("COMP #(" + std::to_string(max) + ") component" + std::to_string(pos) + "(" + var3 + ", " + var5 + ", " + var1 + ", 0, 0);");
				}
				break;
			default: cout << "Invalid" << endl;
			}
			ptrL++;
			pos += 1;
		}
		else if (line.find("*") != string::npos) { //MUL
			buffer >> var1 >> var2 >> var3 >> var4 >> var5;
			if (size.find(var1) == size.end() || size.find(var3) == size.end() || size.find(var5) == size.end()) { cout << "Erorr, missing variable\n"; return true; }
			max = size[var1];
			var3 = getStr(var3, sign[var3], size[var3], max);
			var5 = getStr(var5, sign[var5], size[var5], max);

			if (states.size() > ptrM) {
				states.at(ptrM).addString("MULT #(" + std::to_string(max) + ") component" + std::to_string(pos) + "(" + var3 + ", " + var5 + ", " + var1 + ");");
				
			}
			else {
				while (states.size() != ptrM + 2) {
					states.push_back(states.size());
					//states.at(states.size() - 1).addString("SUB #(" + to_string(max) + ") component" + std::to_string(pos) + "(" + var3 + ", " + var5 + ", " + var1 + ");");
					if (states.size() > 1)
						states.at(states.size() - 2).setNext("stateNext <= s" + std::to_string(states.size() - 1) + ";");
				}
				states.at(ptrM).addString("MULT #(" + std::to_string(max) + ") component" + std::to_string(pos) + "(" + var3 + ", " + var5 + ", " + var1 + ");");
			}
			pos = pos++;
			ptrM = ptrM + 2;
		}
		else if (line.find("?") != string::npos) { //MUX
			buffer >> var1 >> var2 >> var3 >> var4 >> var5 >> var6 >> var7;
			if (size.find(var1) == size.end() || size.find(var3) == size.end() || size.find(var5) == size.end() || size.find(var7) == size.end()) { cout << "Erorr, missing variable\n"; return true; }

			max = size[var1];
			var7 = getStr(var7, sign[var7], size[var7], max);
			var5 = getStr(var5, sign[var5], size[var5], max);

			if (states.size() == ptrL) {
				states.push_back(states.size());
				states.at(states.size() - 1).addString("MUX2x1 #(" + std::to_string(max) + ") component" + std::to_string(pos) + "(" + var5 + ", " + var7 + ", " + var3 + "[0], " + var1 + ");");
				if (states.size() > 1)
					states.at(states.size() - 2).setNext("stateNext <= s" + std::to_string(states.size() - 1) + ";");
			}
			else {
				states.at(ptrL).addString("MUX2x1 #(" + std::to_string(max) + ") component" + std::to_string(pos) + "(" + var5 + ", " + var7 + ", " + var3 + "[0], " + var1 + ");");
			}

			ptrL++;
			pos += 1;
		}

		buffer.clear();
	}

	if (ifStack.empty() && (!nothingStack.empty() || !noElseStack.empty())) {
		max = nothingStack.top() + 1;
		if (max < states.size()) {
			while ((ifStack.empty() && (!nothingStack.empty() || !noElseStack.empty()))) {
				if (!nothingStack.empty()) {
					stackHolder = nothingStack.top();
					nothingStack.pop();
					states.at(stackHolder).addString("\tstateNext <= s" + std::to_string(max) + ";\n");
				}
				if (!noElseStack.empty()) {
					stackHolder = noElseStack.top();
					noElseStack.pop();
					states.at(stackHolder).addString("\tstateNext <= s" + std::to_string(max) + ";\n");
				}
			}
			states.at(states.size() - 1).setNext("stateNext <= final;");
		}
		else
			while ((ifStack.empty() && (!nothingStack.empty() || !noElseStack.empty()))) {
				if (!nothingStack.empty()) {
					stackHolder = nothingStack.top();
					nothingStack.pop();
					states.at(stackHolder).addString("\tstateNext <= final;\n");
				}
				if (!noElseStack.empty()) {
					stackHolder = noElseStack.top();
					noElseStack.pop();
					states.at(stackHolder).addString("\tstateNext <= final;\n");
				}
			}
	}
	else
		states.at(states.size() - 1).setNext("stateNext <= final;");

	if (!ifStack.empty()) {
		max = nothingStack.top() + 1;
		if (max < states.size()) {
			while (!ifStack.empty()) {		
				stackHolder = ifStack.top();
				ifStack.pop();
				states.at(stackHolder).addString("\tstateNext <= s" + std::to_string(max) + ";\n");
			}
			states.at(states.size() - 1).setNext("stateNext <= final;");
		}
		else
			while (!ifStack.empty()) {
				stackHolder = ifStack.top();
				ifStack.pop();
				states.at(stackHolder).addString("\tstateNext <= final;\n");
			}
	}

	if (states.size() > std::atoi(lat)) {
		cout << "Not enough latency";
		return true;
	}

	print.at(1) = print.at(1) + ");\n\n"; //finishes module declaration
	outp.open(outputFile); //no errors ran into previously, will begin final process to print out everything

	print.push_back(printParm(states.size()));

	print.push_back("\nalways @(*) begin\n\tcase(State) begin\n");
	
	//open and prints vectors into outp last after all checks were done
	
	for (i = 0; i < print.size(); i++)
		outp << print.at(i);
	//Creates init state
	init.addString("if(Start)");
	if (states.size() > 0)
		init.addString("\tstateNext <= s0;");
	else
		init.addString("\tstateNext <= final;");
	init.addString("else");
	init.setNext("\tstateNext <= init;");
	init.printState(outp);

	//ends all states and prints them out
	for (i = 0; i < states.size(); i++) {
		states.at(i).printState(outp);
	}

	//Creates done state
	done.addString("reg fin( 1, Clk, 0, Done)");
	done.setNext("stateNext <= init;");
	done.printState(outp);

	outp << "\tendcase\nend\n";//ends cases and states printing, ends
	outp << "\nalways @(posedge Clk) begin\n\tstate <= stateNext;\nend\nalways @(posedge Rst) begin\n\tstate <= S0;\n\treg init( 0, 1, 1, Done);\nend\n";
	outp << "\nendmodule";

	outp.close();
	inp.close();
	return true;
}



// prints main arguements 
void printArgs(int argc, char* argv[]) {

	for (int i = 0; i < argc; i++) {
		cout << i << "  " << argv[i] << endl;
	}
}

int main(int argc, char* argv[]) {

	if (argc != 4) {
		cout << "Failed due to incorrect arguments: " << endl;
		printArgs(argc, argv);
		return 0;    //CMAKE ERROR can't return failure since this is an int type function
	}

	if (!openFiles(argv[1], argv[2], argv[3])) {
		cout << "Failed to open input file: " << argv[1] << endl;
		printArgs(argc, argv);
		return 0;
	}

}