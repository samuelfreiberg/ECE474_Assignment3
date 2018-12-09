#ifndef STATE_H
#define STATE_H
#include "Lib.h"

using namespace std;
class State
{

public:
	State(int size); //constructor needs to pass in size of object vector
	State(string name); //constructor used to create own name, mainly just init and done state
	void addString(string temp);
	bool printState(ofstream &outp);
	void setNext(string t);

private:
	vector <string> s;
	string next;

};


#endif
