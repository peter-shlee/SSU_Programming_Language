#include <iostream>
#include <vector>
#include <algorithm>
#include <cstring>
#include <ctime>
using namespace std;

#define MINUS_INFINITY -1234567890.0
#define PLUS_INFINITY 1234567890.0

void play(int argc, char *argv[]);
int parseInt(const char *numStr);
vector<int> *getUntakenStones(const vector<int> &takenStones, int numOfStones);
bool checkPrimeNumber(int num);
bool checkFactorOrMultiple(int numA, int numB);
float evaluate(const vector<int> &untakenStones, int lastTakenStone, bool isMaxPlayer);
void test(vector<int> untakenStones, int numOfStones);
double calculateResultValue(const vector<int> &untakenStones, int lastTakenStone, bool isMaxPlayer);
float alphaBetaPruning(const vector<int> &untakenStones, int lastTakenStone, bool isMaxPlayer, int maxDepth, int currentDepth, float alpha, float beta, int &selectedNode, int &numOfVisitedNodes);
vector<int> *getFirstPickStones(int numOfStones);

int main(int argc, char *argv[]){
	play(argc, argv);

	return 0;
}

void play(int argc, char *argv[]){
	int numOfStones = parseInt(argv[1]);
	int numOfTakenStones = parseInt(argv[2]);
	int i;
	int selectedNode;
	int numOfVisitedNodes;
	int lastTakenStone;
	double result;
	clock_t start, end, interval;
	bool isMaxPlayer;

	vector<int> takenStones = vector<int>(numOfTakenStones);
	for(int i = 0; i < numOfTakenStones; ++i){
		takenStones[i] = parseInt(argv[i + 3]);
	}
	vector<int> &untakenStones = *getUntakenStones(takenStones, numOfStones);

	if(takenStones.size() % 2 == 0)
		isMaxPlayer = true;
	else
		isMaxPlayer = false;

	if(takenStones.size() == 0)
		lastTakenStone = 0;
	else
		lastTakenStone = takenStones[takenStones.size() - 1];

	i = 1;
	selectedNode = 0;
	start = clock();
	while(1){
		numOfVisitedNodes = 0;
		result = alphaBetaPruning(untakenStones, lastTakenStone , isMaxPlayer, i, 0, MINUS_INFINITY, PLUS_INFINITY, selectedNode, numOfVisitedNodes);
		end = clock();
		interval = (end - start) / (CLOCKS_PER_SEC / 1000);
		//cout << interval << endl;
		if(interval >= 500 || result == 1 || result == -1) break;
		++i;
	}
	cout << "Best Move : " << selectedNode << endl;
	cout << "Calculated Value : " << result << endl;
	cout << "Number of Visited Nodes : " << numOfVisitedNodes << endl;
	cout << "Max Depth : " << i << endl;
}

float alphaBetaPruning(const vector<int> &untakenStones, int lastTakenStone, bool isMaxPlayer, int maxDepth, int currentDepth, float alpha, float beta, int &selectedNode, int &numOfVisitedNodes){
	int selectNode;
	++numOfVisitedNodes;
	
	//  아직 고른 돌이 없는 경우
	if (lastTakenStone == 0) {
		vector<int> nextUntakenStones = vector<int>(untakenStones);
		vector<int> *firstPickStones = getFirstPickStones(nextUntakenStones.size());
		float value = MINUS_INFINITY;
		float tmpValue;
		for(int i = 0; i < firstPickStones->size(); ++i) {
			vector<int>::iterator itr = nextUntakenStones.begin();
			while(itr != firstPickStones->end()){
				if(*itr == (*firstPickStones)[i]){
					nextUntakenStones.erase(itr);
					break;
				}
				++itr;
			}
			sort(nextUntakenStones.begin(), nextUntakenStones.end());

			tmpValue = value;
			value = max(value, alphaBetaPruning(nextUntakenStones, (*firstPickStones)[i], !isMaxPlayer, maxDepth, currentDepth + 1, alpha, beta, selectedNode, numOfVisitedNodes)); // 다음 단계 진행
			if(tmpValue != value) selectNode = (*firstPickStones)[i]; 
			alpha = max(alpha, value);
			if(alpha >= beta) {
				break;
			}

			nextUntakenStones.push_back((*firstPickStones)[i]);
		}
		delete firstPickStones;

		selectedNode = selectNode;

		return value;
	}

	/////////////////////////////////////////////////////////////////////////////////////////
	// 단말 노드인지 확인
	bool isTerminalNode = true;
	for(int i = 0; i < untakenStones.size(); ++i){
		if(checkFactorOrMultiple(untakenStones[i], lastTakenStone)){
			isTerminalNode = false;
			break;
		}
	}

	if(maxDepth == currentDepth || isTerminalNode) {
		return calculateResultValue(untakenStones, lastTakenStone, isMaxPlayer);
	}


	float value;
	float tmpValue;
	if(isMaxPlayer) {
		value = MINUS_INFINITY; //

		//
		for(int i = 0; i < untakenStones.size(); ++i){ // 남아있는 돌들 검사
			if(checkFactorOrMultiple(untakenStones[i], lastTakenStone)){ // 고를 수 있는 돌이라면

				vector<int> *nextUntakenStones = new vector<int>(untakenStones); // 해당 돌을 고른 후 남아있는 돌들의 vector
				vector<int>::iterator itr = nextUntakenStones->begin();
				while(itr != nextUntakenStones->end()){
					if(*itr == untakenStones[i]){
						nextUntakenStones->erase(itr);
						break;
					}
					++itr;
				} // vector에서 돌 하나 빼서 다음 단계에서 사용할 남아있는 돌들 vector 만드는 작업
				sort(nextUntakenStones->begin(), nextUntakenStones->end());

				tmpValue = value;
				value = max(value, alphaBetaPruning(*nextUntakenStones, untakenStones[i], !isMaxPlayer, maxDepth, currentDepth + 1, alpha, beta, selectedNode, numOfVisitedNodes));
				if(tmpValue != value) selectNode = untakenStones[i]; 
				alpha = max(alpha, value);
				delete nextUntakenStones;
				if(alpha >= beta) break;
			}
		}
		//

	} else {
		value = PLUS_INFINITY; // 


		//
		for(int i = 0; i < untakenStones.size(); ++i){ // 남아있는 돌들 검사
			if(checkFactorOrMultiple(untakenStones[i], lastTakenStone)){ // 고를 수 있는 돌이라면

				vector<int> *nextUntakenStones = new vector<int>(untakenStones); // 해당 돌을 고른 후 남아있는 돌들의 vector
				vector<int>::iterator itr = nextUntakenStones->begin();
				while(itr != nextUntakenStones->end()){
					if(*itr == untakenStones[i]){
						nextUntakenStones->erase(itr);
						break;
					}
					++itr;
				} // vector에서 돌 하나 빼서 다음 단계에서 사용할 남아있는 돌들 vector 만드는 작업
				sort(nextUntakenStones->begin(), nextUntakenStones->end());

				tmpValue = value;
				value = min(value, alphaBetaPruning(*nextUntakenStones, untakenStones[i], !isMaxPlayer, maxDepth, currentDepth + 1, alpha, beta, selectedNode, numOfVisitedNodes));//
				if(tmpValue != value) selectNode = untakenStones[i]; 
				beta = min(beta, value); //
				delete nextUntakenStones;
				if(alpha >= beta) break;
			}
		}
		//

	}

	if(currentDepth == 0){
		selectedNode = selectNode;
	}

	return value;
}

vector<int> *getFirstPickStones(int numOfStones) {
	vector<int> stones = vector<int>();

	for(int i = 1; i <= numOfStones / 2; ++i) {
		if(i % 2 == 1) stones.push_back(i);
	}

	return new vector<int>(stones);
}

double calculateResultValue(const vector<int> &untakenStones, int lastTakenStone, bool isMaxPlayer) {
	double resultValue;
	bool isStone1Left = false;
	int canGetStoneCount = 0;
	int primeNumberCount = 0;
	int multipleCount = 0;

	
	vector<int>::const_iterator itr = untakenStones.begin();
	while(itr != untakenStones.end()){
		if(*itr == 1) {
			++canGetStoneCount;
			isStone1Left = true;
			break;
		}
		if(checkFactorOrMultiple(*itr, lastTakenStone)){ // 고를 수 있는 돌 개수
			++canGetStoneCount;
		}
		if(checkPrimeNumber(*itr)) { // 남은 돌 중 소수 개수
			++primeNumberCount;
		}
		if(((*itr) % lastTakenStone) == 0) { // 마지막으로 고른 돌의 배수 개수
			++multipleCount;
		}

		++itr;
	}
	
	if(canGetStoneCount == 0) { // 고를 수 있는 돌이 없다면
		if(isMaxPlayer) return -1.0;
		else return 1.0;
	}

	if(isStone1Left) return 0.0;
	if(lastTakenStone == 1) {
		if(canGetStoneCount % 2)
			resultValue = 0.5;
		else
			resultValue = -0.5;
	} else if(checkPrimeNumber(lastTakenStone)) {
		if(multipleCount % 2)
			resultValue = 0.7;
		else
			resultValue = -0.7;
	} else {
		if(primeNumberCount % 2)
			resultValue = 0.6;
		else
			resultValue = -0.6;
	}

	if(!isMaxPlayer) resultValue *= -1.0;

//	cout << "canGetStoneCount : " << canGetStoneCount  << endl;
//	cout << "primeNumberCount : " << primeNumberCount  << endl;
//	cout << "multipleCount : " << multipleCount  << endl;

	return resultValue;
}

vector<int> *getUntakenStones(const vector<int> &takenStones, int numOfStones){
	int takenStonesIndex = 0;
	int untakenStonesIndex = 0;
	int untakenStoneNum = 1;

	vector<int> *sortedTakenStones = new vector<int>(takenStones);
	sort(sortedTakenStones->begin(), sortedTakenStones->end());
	//cout << "sort complete" << endl;

	vector<int> *untakenStones = 
		new vector<int>(numOfStones - takenStones.size());

	if(takenStones.size() == 0) {
		for(int i = 0; i < numOfStones; ++i) {
			(*untakenStones)[i] = i + 1;
		}
		return untakenStones;
	}


	while(untakenStoneNum <= numOfStones){
		if((*sortedTakenStones)[takenStonesIndex] == untakenStoneNum){
			++takenStonesIndex;
		} else {
			(*untakenStones)[untakenStonesIndex++] = 
				untakenStoneNum;
		}
		++untakenStoneNum;
	}

	delete sortedTakenStones;

	return untakenStones;
}

bool checkFactorOrMultiple(int numA, int numB){
	bool isFactorOrMultiple = false;
	if(numA > numB){
		if(!(numA % numB)){
			isFactorOrMultiple = true;
		}
	} else {
		if(!(numB % numA)){
			isFactorOrMultiple = true;
		}
	}

	return isFactorOrMultiple;
}

bool checkPrimeNumber(int num){
	if(num == 1) return false;

	bool isPrimeNumber = true;

	for(int n = 2; n <= num / 2; ++n){
		if(!(num % n)){
			isPrimeNumber = false;
			break;
		}
	}

	return isPrimeNumber;
}

int parseInt(const char *numStr){
	int parsedInt = 0;
	for(int i = 0; i < strlen(numStr); ++i){
		parsedInt *= 10;
		parsedInt += numStr[i] - '0';
	}

	return parsedInt;
}
