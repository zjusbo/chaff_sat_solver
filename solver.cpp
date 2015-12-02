#include <iostream>
#include <vector>
#include <queue>
#include <unordered_set>
#include <set>
#include <unordered_map>
#include <string>

using namespace std;

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
int main(int argc, char const *argv[])
{
	int numVariables, numClauses;
	char p[10], cnf[10];
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
		formatIllegal();
		return -1;
	}
	if(strcmp(p,"p") || strcmp(cnf, "cnf")){
		formatIllegal();
		return -1;
	}
	if(numVariables <= 0 || numClauses <= 0){
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
	bool isFound = findAModel(clauses, model, 1); // start from position one
	if(isFound == true){
		cout<<"s SATISFIABLE"<<endl;
		cout<<"v";
		for (int j = 1; j < numVariables + 1; j++)
		{
			int ele = model[j] == 1? j: -j;
			cout<<" "<<ele;
		}
		cout<<endl;
	}
	else{
		cout<<"s UNSATISFIABLE"<<endl;
	}
	showMatrix(clauses);
	fclose(fp);
	getchar();
	
	return 0;

}