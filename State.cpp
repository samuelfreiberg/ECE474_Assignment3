#include "State.h"
using namespace std;

State::State(int size)
{
	s.push_back("\ts" + std::to_string(size) + ": begin \n");
	next = "";
}
State::State(string name)//used for init and done state
{
	s.push_back("\t"+name + ": begin \n");
	next = "";
}


void State::addString(string temp)
{
	s.push_back("\t\t"+temp+"\n");
}
bool State::printState(ofstream &outp) {
	if (!outp.good())
		return false;
	for (int i = 0; i < s.size(); i++) 
		outp << s.at(i);
	outp << next;
	return true;
}
void State::setNext(string t) {
	next = "\t\t" + t + "\n\tend\n";
}