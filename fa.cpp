// Nicklaus McClendon
// nhm150030@utdallas.edu
// CS 4386.001
// Assignment 1

#include <iostream>
#include <string>
#include <set>
#include <vector>
#include <map>

class State {
private: 
  std::vector<std::set<State*>*> inTransition;
  std::vector<std::set<State*>*> outTransition;
  std::map<char,int>* symbolMap;
  int symbolCount;
  char epsilon;
  
public:
  int id;
  
  State(std::map<char,int>* symbolMapIn, int symbolCountIn, char epsilonIn, int idIn) {
	symbolMap = symbolMapIn;
	symbolCount = symbolCountIn;
	epsilon = epsilonIn;
	id = idIn;
	for (int i = 0; i < symbolCount; i++) {
	  inTransition.push_back(new std::set<State*>);
	  outTransition.push_back(new std::set<State*>);
	}
  }

  ~State() {
	for (int i = 0; i < symbolCount; i++) {
	  delete inTransition[i];
	  delete outTransition[i];
	}
  }

  std::set<State*>* getTransitions(char symbol) {
	return outTransition[(*symbolMap)[symbol]];
  }

  void addTransition(char symbol, State* toState) {
	int location = (*symbolMap)[symbol];
	outTransition[location]->insert(toState);
	toState->addBackTransition(symbol, this);
  }

  void addTransition(int location, State* toState) {
	outTransition[location]->insert(toState);
	toState->addBackTransition(location, this);
  }

  void addBackTransition(char symbol, State* fromState) {
	int location = (*symbolMap)[symbol];
	inTransition[location]->insert(fromState);
  }

  void addBackTransition(int location, State* toState) {
	inTransition[location]->insert(toState);
  }

  void delTransition(char symbol, State* toState) {
	int location = (*symbolMap)[symbol];
	outTransition[location]->erase(toState);
	toState->delBackTransition(symbol, this);
  }

  void delBackTransition(char symbol, State* fromState) {
	int location = (*symbolMap)[symbol];
	inTransition[location]->erase(fromState);
  }

  void clearSymbol(char symbol) {
	int location = (*symbolMap)[symbol];
	inTransition[location]->clear();
  }

  void epsilonExtend() {
	  std::set<State*>::iterator iter;
	  int i = (*symbolMap)[epsilon];
	  for (iter = (*outTransition[i]).begin(); iter != (*outTransition[i]).end(); ++iter) {
		std::set<State*>::iterator innerIter;
		std::set<State*>* innerSet = (*iter)->getTransitions(epsilon);
		for (innerIter = (*innerSet).begin(); innerIter != (*innerSet).end(); ++innerIter) {
		  this->addTransition(epsilon, (*innerIter));
		}
	  }
  }

  void epsilonClose() {
	std::set<State*>::iterator iter;
	int i = (*symbolMap)[epsilon];
	for (iter = (*inTransition[i]).begin(); iter != (*inTransition[i]).end(); ++iter) {
	  for (int j = 0; j < i; j++) {
		std::set<State*>::iterator innerIter;
		for (innerIter = (*outTransition[j]).begin(); innerIter != (*outTransition[j]).end(); ++innerIter) {
		  (*iter)->addTransition(j, (*innerIter));
		}
	  }
	}
	for (int j = 0; j < i; j++) {
	  for (iter = (*inTransition[j]).begin(); iter != (*inTransition[j]).end(); ++iter) {
		  std::set<State*>::iterator innerIter;
		  for (innerIter = (*outTransition[i]).begin(); innerIter != (*outTransition[i]).end(); ++innerIter) {
			(*iter)->addTransition(j, (*innerIter));
		  }
	  }
	}
  }

  void print() {
	for (int i = 0; i < symbolCount - 1; i++) {
	  std::cout << '{';
	  std::set<State*>::iterator iter;
	  for (iter = (*outTransition[i]).begin(); iter != (*outTransition[i]).end(); ++iter) {
		std::cout << (*iter)->id << ',';
	  }
  	  std::cout << "} ";
	}
	std::cout << std::endl;
  }

};

int main() {
  // Load basic attributes
  int stateCount, symbolCount;
  char epsilon;
  std::map<char, int> symbolMap;
  std::vector<State*> states;
  std::cin >> stateCount;
  std::cin >> symbolCount;
  std::cin >> epsilon;
  char symbolTable[symbolCount];
  
  // Build Symbol Map
  for (int i = 0; i < symbolCount; i++) {
	char symbol;
	std::cin >> symbol;
	symbolMap[symbol] = i;
	symbolTable[i] = symbol;
  }
  // Initialize States
  for (int i = 0; i < stateCount; i++) {
	states.push_back(new State(&symbolMap, symbolCount, epsilon, i));
  }
  // Build State Transitions
  for (int i = 0; i < stateCount; i++) {
	for (int j = 0; j < symbolCount; j++) {
	  // Read number of transitions under current symbol
	  int count;
	  std::cin >> count;
 	  for (int k = 0; k < count; k++) {
		int transitionTo;
		std::cin >> transitionTo;
		states[i]->addTransition(symbolTable[j], states[transitionTo]);
	  }
	}
  }

  // Make sure all epsilon transitions are enumerated
  for (int i = 0; i < stateCount; i++) {
	for (int j = 0; j < stateCount; j++) {
	  states[j]->epsilonExtend();
	}
  }

  // Epsilon Closure
  for (int i = 0; i < stateCount; i++) {
	states[i]->epsilonClose();
  }

  for (int i = 0; i < symbolCount; i++) {
	states[i]->clearSymbol(epsilon);
  }

  for (int i = 0; i < symbolCount - 1; i++) {
	std::cout << symbolTable[i] << " ";
  }
  std::cout << std::endl;
  for (int i = 0; i < stateCount; i++) {
	std::cout << i << " ";
   	states[i]->print();
  }
}
