#include <iostream>
#include <vector>
#include <queue>
#include <stack>
#include <unordered_set>
#include <set>
#include <map>
#include <unordered_map>
#include <string>
#include <ctime>
//#include <chrono>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
using namespace std;
//using namespace std::chrono;

void helperMsg(){
    cout<<"input format: solver <input_file>"<<endl;
}

void formatIllegal(){
	cout<<"file format illegal"<<endl;
}

void showMatrix(vector<vector<int>>& matrix){
	for (int i = 0; i < matrix.size(); i++)
	{
		for (int j = 0; j < matrix[0].size(); j++)
		{
			cout<<matrix[i][j]<<" ";
		}
		cout<<endl;
	}
}

/**
 * return true if at current state, the clauses is true or undecidable
 *
 **/
bool isValid(vector<vector<int>>& clauses, vector<int>& model){
	for (int i = 0; i < clauses.size(); i++)
	{
		bool isClouseValid = false;
		for (int j = 1; j < clauses[0].size(); j++)
		{
			int oper1 = clauses[i][j];
			int oper2 = model[j];
			// one element in the clause returns true
			if(oper1 * oper2 == 1){
			    isClouseValid = true;
				break;
			}
			// one element in the clause is undecidable, return true
			else if(oper1 != 0 && oper2 == 0){
				isClouseValid = true;
				break;
			}
		}
		// current clouse is not satisfiable
		if(isClouseValid == false){
			return false;
		}
	}
	return true;
}

bool findAModel(vector<vector<int>>& clauses, vector<int>& model, int pos){
	// base condition
	int options[] = {-1, 1}; // -1 means false, 1 means true
	if(pos == model.size()){
		return true;
	}
	for (int i = 0; i < 2; i++)
	{
		model[pos] = options[i];
		if(isValid(clauses, model)){
			bool isFound = findAModel(clauses, model, pos + 1);
			if(isFound == true){
				return true;
			}
		}
	}
	model[pos] = 0; // mark as undecided
	return false;
}


struct SetCmp
{
	bool operator()(const pair<int, int>&a, const pair<int, int>&b){
		if(a.second > b.second){
			return true;
		}else if(a.second < b.second){
			return false;
		}else{
			if(a.first < b.first){
				return true;
			}else{
				return false;
			}
		}
	}
};

class VariableCounter
{
public:
	VariableCounter();
	VariableCounter(vector<int> counterdata);
	VariableCounter(const VariableCounter & a);
	~VariableCounter();
	int next();
	bool hasNext();
	void reset();
	void print();
private:
	// first - index
	// second - count
	set<pair<int, int>, SetCmp> counter;
	set<pair<int, int>, SetCmp>::iterator it;
};
VariableCounter::VariableCounter()
{
	
}

VariableCounter::VariableCounter(vector<int> counterdata)
{
	for(int i = 2; i < counterdata.size(); i++){
		this->counter.insert(make_pair(i, counterdata[i]));	
	}
	// reset iterator
	reset();
}
void VariableCounter::print(){
	for (auto it = counter.begin(); it != counter.end(); it++)
	{
		cout<<it->first<<" "<<it->second<<endl; 
	}
}
VariableCounter::~VariableCounter()
{

}
bool VariableCounter::hasNext(){
	return this->it != this->counter.end();
}
int VariableCounter::next(){
	if(!hasNext()){
		return 0;
	}
	int v = this->it->first;
	this->it++;
	return v;
}
void VariableCounter::reset(){
	this->it = this->counter.begin();
}

class Chaff
{
public:
	/**
	 * 0 1 2 -3 4
	 * 0 2 3  
	 * 0 -1 4
	 * column 0 is always set to 0, reserved
	 * 
	 **/
	vector<map<int, int>> clausesRow;
	vector<map<int, int>> clausesCol;

	int numOfClauses;
	int numOfVariables;
	bool isConflict;
	/**
	 * count and sort variables in each polarity in clauses by frequency in descending order
	 * 
	 **/
	VariableCounter* vc;
	
	/**
	 * decision stack, 
	 * second - true means tried both sides
	 * second - false means tried one side
	 * first - sign indicates polarity
	 **/
	stack<pair<int, bool>> decisionStack;
	
	/**
	 * decisionTable is used for quick access by index
	 * it is consistent to decisionStack in information
	 * column 0 is reserved.
	 * 1 - true
	 * -1 - false
	 * 0 - unassigned (may be stored in implicationTable)
	 **/
	vector<int> decisionTable;

	/**
	 * implication table
	 * vector index indicate the variable index
	 * first - 1 means true, 0 means unassigned, -1 means false
	 * second - indicates the stack height when the variable is implied
	 **/
	vector<pair<int, int>> implicationTable;

	/**
	 * index of two watched variables in each clause
	 * watchVariable[i][j] , 0 <= i < numOfClouses, j = 0, 1
	 **/
	vector<vector<int>> watchVariable;
	
	
	/**
	 * try to assign an value to an unassigned variable based on the algorithm elaborated in section 3, p532
	 * return value:
	 * true - a variable is assigned successfully
	 * false - no unassigned variables
	 **/
	bool decide();

	/**
	 * try to find an implication and assign it to model
	 * return value:
	 * true - no implication can be found
	 * false - conflict happens
	 **/
	bool bcp();

	/**
	 * backtracking algorithm, flip the last assigned not tried both ways variables
	 * return value:
	 * true - resolved a conflict
	 * false - can not resolve conflict
	 **/
	bool resolveConflict();
	
	/**
	 * initialize 
	 * 
	 **/
	void init(vector<vector<int>>& clauses);

	/**
	 * populate model from decision table and implication table
	 **/
	void genModel();
public:
	
	/**
	 * 0 1 -1 1 1 -1 -1
	 * column 0 is always set to 0, reserved
	 * 0 means unassigned
	 * 1 means true
	 * -1 means false
	 **/
	vector<int> model;
	void printModel();
	void printVectorSet(vector<map<int, int>>& mp);
	Chaff(vector<vector<int>>& clauses);
	~Chaff();
	bool solve();
	vector<int> getModel();
};
void Chaff::printVectorSet(vector<map<int, int>>& mp){
	for(int i = 0; i < mp.size(); i++){
		for(auto it = mp[i].begin(); it != mp[i].end(); it++){
			cout<<"idx = "<<i<<", key = "<<it->first<<", value = "<<it->second<<endl;
		}
	}
}
Chaff::Chaff(vector<vector<int>>& clauses)
{
	this->numOfClauses = clauses.size();
	if(this->numOfClauses == 0){
		throw "clauses can not be empty.";
	}
	this->numOfVariables = clauses[0].size() - 1;
	if(this->numOfVariables <= 0){
		throw "number of variables in a clause should be equal or more than 1";
	}
	init(clauses);
}

void Chaff::init(vector<vector<int>>& clauses){
	isConflict = false;
	// init clausesRow, clausesCol
	this->clausesRow = vector<map<int, int>>(numOfClauses, map<int, int>());
	this->clausesCol = vector<map<int, int>>(numOfVariables + 1, map<int, int>());

	for (int i = 0; i < numOfClauses; ++i)
	{
		for (int j = 1; j < numOfVariables + 1; ++j)
		{
			if(clauses[i][j] == 0){
				continue;
			}
			this->clausesRow[i][j] = clauses[i][j];
			this->clausesCol[j][i] = clauses[i][j];
		}
	}
	// init decisionTable and implicationTable
	this->decisionTable = vector<int>(numOfVariables + 1, 0);
	this->implicationTable = vector<pair<int, int>>(numOfVariables + 1, make_pair(0, 0));
	// init watchVariables
	this->watchVariable = vector<vector<int>>(numOfClauses, vector<int>(2, 0));
	for (int i = 0; i < watchVariable.size(); i++)
	{
		if(this->clausesRow[i].size() < 1){
			throw "clause input illegal";
		}
		auto it = this->clausesRow[i].begin();
		this->watchVariable[i][0] = it->first;
		it++;
		// there may be only one literal in the clause, so watchVariable[1] may be 0
		if(it != this->clausesRow[i].end()){
			this->watchVariable[i][1] = it->first;
		}else{
			// only one literal in clauses[i]
			// push the variable in implication table, current stack height is 0
			int oldLiteral = this->implicationTable[this->clausesRow[i].begin()->first].first;
			if(oldLiteral != 0 && oldLiteral != this->clausesRow[i].begin()->second){
				// conflict, return unsatisfiable
				isConflict = true;
			}
			this->implicationTable[this->clausesRow[i].begin()->first] = make_pair(this->clausesRow[i].begin()->second, 0);
			// TODO: do we need to call bcp() here?
			this->watchVariable[i][0] = 0;// do not watch variables in this clause anymore
		}
	}
	// init model
	this->model = vector<int>(numOfVariables + 1, 0);

	// initialize variable counter
	// j * 2 stores the number occurence of variable negate j
	// j * 2 + 1 stores the number of occurnece of variable j
	vector<int> counter = vector<int>(numOfVariables * 2 + 2, 0);
	for (int i = 0; i < numOfClauses; i++)
	{
		for (int j = 1; j < numOfVariables + 1; j++)
		{
			// TODO: traverse clausesRow rather than clauses
			// variable j is true
			if(clauses[i][j] == 1){
				counter[j*2+1]++;
			}else if(clauses[i][j] == -1){
				counter[j*2]++;
			}
		}
	}
	this->vc = new VariableCounter(counter);
	//vc->print();

}

Chaff::~Chaff()
{
	delete this->vc;
}
bool Chaff::decide(){
	while((*vc).hasNext()){
		int v = (*vc).next();
		int idx = v / 2;
		int polarity = v % 2 == 1? 1: -1;
		if(decisionTable[idx] != 0 || implicationTable[idx].first != 0){
			continue;
		}
		// new unassinged varibles found
		decisionTable[idx] = polarity;//if polarity is true, set true; else set false. In order to make the literal true
		
		// try the first side of variable
		decisionStack.push(make_pair(idx * polarity, false));
		return true;
	}
	// no unassigned variables
	return false;
}
bool Chaff::bcp(){
	// last assigned variable
	int idx = abs(decisionStack.top().first);
	int polarity = decisionStack.top().first > 0? 1: -1;
	queue<pair<int, int>> impliedVariables;// first - index, second - polarity
	impliedVariables.push(make_pair(idx, polarity));
	
	//BFS search
	while(impliedVariables.empty() == false){
		int idx = impliedVariables.front().first;
		int polarity = impliedVariables.front().second;
		impliedVariables.pop();
		
		for(auto it = clausesCol[idx].begin(); it != clausesCol[idx].end(); it++)
		{
			int row = it->first;
			int clausePolarity = it->second;
			if(watchVariable[row][0] == idx || watchVariable[row][1] == idx){
				int updateWatchidx, lastWatchidx;
				if(watchVariable[row][0] == idx){
					updateWatchidx = 0;
					lastWatchidx = 1;
				}else{
					updateWatchidx = 1;
					lastWatchidx = 0;
				}
				//watched variable is set to 0
				if(clausePolarity * polarity == -1){
					bool updateWatchVariable = false;
					for (auto itc = clausesRow[row].begin(); itc != clausesRow[row].end(); itc++)
					{
						int col = itc->first;
						// the other watched variable
						if(col == watchVariable[row][1] || col == watchVariable[row][0]){
							continue;
						}
						int variable = decisionTable[col] != 0? decisionTable[col]: implicationTable[col].first;
					
						// find a variable that is unassigned or literal is 1
						if(variable == 0 || variable * clausesRow[row][col] == 1){
							watchVariable[row][updateWatchidx] = col;
							updateWatchVariable = true;
							break;
						}
					}
					// watchVariable can be implied
					if(updateWatchVariable == false){
						int lastLiteralIdx = watchVariable[row][lastWatchidx];
						int lastLiteralPolarity = decisionTable[lastLiteralIdx] != 0? decisionTable[lastLiteralIdx]: implicationTable[lastLiteralIdx].first;
						if(lastLiteralPolarity == 0){
							// unassinged, imply it!
							implicationTable[lastLiteralIdx] = make_pair(clausesRow[row][lastLiteralIdx], decisionStack.size());
							impliedVariables.push(make_pair(lastLiteralIdx, implicationTable[lastLiteralIdx].first));
						}else{
							// assgined, check it!
							if(lastLiteralPolarity * clausesRow[row][lastLiteralIdx] != 1){
								// conflict
								return false;
							}
						}
					}
				}
			}
		}
	}
	return true;
}

bool Chaff::resolveConflict(){
	while(!decisionStack.empty()){
		auto decision = decisionStack.top();
		decisionStack.pop();
		int idx = abs(decision.first);
		int polarity = decision.first > 0? 1: -1;
		// delete assignment in decision table
		decisionTable[idx] = 0;
		if(decision.second == true){
			// tried both sides, go to next iteration
			continue;
		}
		
		// tried both ways
		decision.second = true;
		
		// flip the decision
		polarity *= -1;
		decision.first = idx * polarity;

		// push it back
		decisionStack.push(decision);
		// update decision table
		decisionTable[idx] = polarity;
		
		// undo all invalid implications
		int height = decisionStack.size();
		 
		for(int i = 1; i < implicationTable.size(); i++){
			if(implicationTable[i].second >= height){
				implicationTable[i].first = 0;
				implicationTable[i].second = 0;
			}
		}
		
		// seek variable pointer to the top of assignment stack
		(*vc).reset();
		return true;
	}

	// can not found "not tried both sides variables"
	return false;
}

void Chaff::genModel(){
	for(int i = 1; i < numOfVariables + 1; i++){
		int value = decisionTable[i] != 0? decisionTable[i]: implicationTable[i].first;
		model[i] = value;
	}
}

bool Chaff::solve(){
	if(isConflict){
		// conflict in single literal clauses
		return false;
	}
	while(true){
		if(!decide()){
			// satisfiable
			genModel();
			return true;
		}
		while(!bcp()){
			if(!resolveConflict()){
				return false;
			}
		}
	}
}

void Chaff::printModel(){
	for (int i = 0; i < model.size(); i++)
	{
		cout<<model[i]<<" ";
	}
	cout<<endl;
}

int main(int argc, char const *argv[])
{
	int numVariables, numClauses;
	char p[1000], cnf[100];
	if(argc != 2){
		helperMsg();
		return -1;
	}
	string spath = argv[0];
	string sinput = argv[1];
	FILE * fp = fopen(sinput.c_str(), "r");
	if(fp == nullptr){
		cout<<"can not open input file"<<endl;
		return -1;
	}
	if(!fscanf(fp,"%s %s %d %d",p, cnf, &numVariables, &numClauses)){
		cout<<"1"<<endl;
		formatIllegal();
		return -1;
	}
	if(strcmp(p,"p") || strcmp(cnf, "cnf")){
		cout<<p<<endl;
		cout<<cnf<<endl;
		formatIllegal();
		return -1;
	}
	if(numVariables <= 0 || numClauses <= 0){
		cout<<"3"<<endl;
		formatIllegal();
		return -1;
	}
	/**
	 * Input clauses
	 * -1 - negate
	 * column 0 is reserved, its value is always 0
	 **/
	vector<vector<int>> clauses = vector<vector<int>>(numClauses, vector<int>(numVariables + 1, 0));
	
	/**
	 * model 
	 * 0 means undecided
	 * 1 means true
	 * -1 means false
	 **/
	vector<int> model = vector<int>(numVariables + 1, 0);
	
	for (int i = 0; i < numClauses; i++)
	{
		int num;
		fscanf(fp, "%d", &num);
		while(num != 0){
			int col = abs(num);
			clauses[i][col] = num < 0? -1: 1;
			fscanf(fp, "%d", &num);
		}
	}
	fclose(fp);
	Chaff chaff(clauses);
//	milliseconds ms_start = duration_cast<milliseconds>(system_clock::now().time_since_epoch());
//	cout<<"start solving"<<endl;
	bool isFound = chaff.solve();
	if(isFound == true && isValid(clauses, chaff.model)){
		cout<<"s SATISFIABLE"<<endl;
		cout<<"v";
		for (int j = 1; j < numVariables + 1; j++)
		{
			int ele = chaff.model[j] * j;
			cout<<" "<<ele;
		}
		cout<<endl;
	}
	else{
		cout<<"s UNSATISFIABLE"<<endl;
	}
//	milliseconds ms_end = duration_cast<milliseconds>(system_clock::now().time_since_epoch());
//	cout<<"time elapse: "<<((double)ms_end.count() - ms_start.count()) / 1000<<"s"<<endl;
//	getchar();
	return 0;
}