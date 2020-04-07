#include <iostream>
#include <vector>
#include <algorithm>
#include <cstring>
using namespace std;

#define MINUS_INFINITY -1234567890.0
#define PLUS_INFINITY 1234567890.0
#define MAX_VISIT_COUNT 100

void play(int argc, char *argv[]);
int parseInt(const char *numStr);
vector<int> *getUntakenStones(const vector<int> &takenStones, int numOfStones);
bool checkPrimeNumber(int num);
bool checkFactorOrMultiple(int numA, int numB);
float evaluate(const vector<int> &untakenStones, int lastTakenStone, bool isMaxPlayer);
void test(vector<int> untakenStones, int numOfStones);
double calculateResultValue(const vector<int> &untakenStones, int lastTakenStone, bool isMaxPlayer);
float alphaBetaPruning(const vector<int> &untakenStones, int lastTakenStone, bool isMaxPlayer, int maxDepth, int currentDepth, float alpha, float beta, int *visitedNodeCnt);
vector<int> *getFirstPickStones(int numOfStones);

int main(int argc, char *argv[]){
	play(argc, argv);

	return 0;
}

void play(int argc, char *argv[]){
	int numOfStones = parseInt(argv[1]);
	int numOfTakenStones = parseInt(argv[2]);
	//cout << numOfStones << ',' << numOfTakenStones << endl;
	vector<int> takenStones = vector<int>(numOfTakenStones);
	for(int i = 0; i < numOfTakenStones; ++i){
		takenStones[i] = parseInt(argv[i + 3]);
	}
//	for(int i = 0; i < numOfTakenStones; ++i){
//		cout << takenStones[i] << endl;
//	}

	test(takenStones, numOfStones);
}

void test(vector<int> takenStones, int numOfStones){
	// test code for parseInt();
	cout << endl << "parseInt() test" << endl;
	cout << parseInt("1") << endl;
	cout << parseInt("12") << endl;
	cout << parseInt("123") << endl;
	cout << parseInt("1234") << endl;
	cout << parseInt("12345") << endl;
	cout << parseInt("123456") << endl;



	// test code for getUntakenStones();
	cout << endl << "getUntakenStones() test" << endl;
	vector<int> &untakenStones = 
		*getUntakenStones(takenStones, numOfStones);
	cout << "size : " << untakenStones.size() << endl;
	for(int i = 0; i < untakenStones.size(); ++i){
		cout << untakenStones[i] << endl;
	}

	// test code for checkFactorOrMultiple();
	cout << endl << "checkFactorOrMultiple() test" << endl;
	cout << "result must be 1 : " << checkFactorOrMultiple(2, 4) << endl;
	cout << "result must be 0 : " << checkFactorOrMultiple(2, 3) << endl;
	cout << "result must be 0 : " << checkFactorOrMultiple(3, 2) << endl;
	cout << "result must be 1 : " << checkFactorOrMultiple(1, 7) << endl;
	cout << "result must be 1 : " << checkFactorOrMultiple(7, 1) << endl;
	cout << "result must be 1 : " << checkFactorOrMultiple(2, 8) << endl;
	cout << "result must be 1 : " << checkFactorOrMultiple(6, 2) << endl;
	cout << "result must be 1 : " << checkFactorOrMultiple(85, 17) << endl;
	cout << "result must be 0 : " << checkFactorOrMultiple(19, 5) << endl;
	cout << "result must be 0 : " << checkFactorOrMultiple(3, 2) << endl;
	cout << "result must be 0 : " << checkFactorOrMultiple(14, 5) << endl;

	// test code for checkPrimeNumber();
	cout << endl << "checkPrimeNumber() test" << endl;
	cout << "result must be 0 : "  << checkPrimeNumber(1) << endl;
	cout << "result must be 0 : "  << checkPrimeNumber(4) << endl;
	cout << "result must be 0 : "  << checkPrimeNumber(14) << endl;
	cout << "result must be 0 : "  << checkPrimeNumber(512) << endl;
	cout << "result must be 1 : "  << checkPrimeNumber(2) << endl;
	cout << "result must be 1 : "  << checkPrimeNumber(5) << endl;
	cout << "result must be 1 : "  << checkPrimeNumber(13) << endl;
	cout << "result must be 1 : "  << checkPrimeNumber(73) << endl;
	cout << "result must be 1 : "  << checkPrimeNumber(97) << endl;

	// test code for evaluate();
	cout << endl << "evaluate() test" << endl;
	bool isMaxPlayer;
	if(takenStones.size() % 2 == 0)
		isMaxPlayer = true;
	else
		isMaxPlayer = false;

	int lastTakenStone;
	if(takenStones.size() == 0) /////////////////////////////////////////////////////////////// 첫번째 선택인 경우 따로 처리해야함
		lastTakenStone = 0;
	else
		lastTakenStone = takenStones[takenStones.size() - 1];

	cout << "taken stones count : " << takenStones.size() << endl;
	cout << "last taken stone : " << lastTakenStone << endl; 
	cout << "isMaxPlayer : " << isMaxPlayer << endl;
//	cout 
//	<< evaluate(untakenStones, lastTakenStone , isMaxPlayer) 
//	<< endl;
	
	// test code for calculateResultValue();
	cout << endl << "calculateResultValue() test" << endl;
	cout << calculateResultValue(untakenStones, lastTakenStone, isMaxPlayer) << endl;

	// test code for alphaBetaPruning
	cout << endl << "alphaBetaPruning() test" << endl;
	int visitedNodeCnt = 0;
	cout << alphaBetaPruning(untakenStones, lastTakenStone , isMaxPlayer, 5, 0, MINUS_INFINITY, PLUS_INFINITY, &visitedNodeCnt) << endl;
}

float evaluate(const vector<int> &untakenStones
		, int lastTakenStone
		, bool isMaxPlayer){
	//cout << "------------------------------" << endl;
	bool isAvailableStoneLeft = false;
	float result;

	if(isMaxPlayer) result = -1.0; // max player 차례라면 result를 -1.0으로 놓고 시작
	else result = 1.0; // min player 차례라면 result를 1.0으로 놓고 시작

	//cout << "size : " << untakenStones.size() << endl; // 아직 안가져간 돌의 개수 출력
	if(!untakenStones.size()){ // 돌이 하나도 남아있지 않다면
		// cout << "isMaxPlayer : " << isMaxPlayer << endl; 
		if(isMaxPlayer)
			return -1.0; // max player차례라면 -1.0 리턴
		else
			return 1.0; // min player 차례라면 1.0 리턴
	}
	for(int i = 0; i < untakenStones.size(); ++i){ // 남아있는 돌들 검사
		if(checkFactorOrMultiple(untakenStones[i], lastTakenStone)){ // 고를 수 있는 돌이라면
			isAvailableStoneLeft = true; // 아직 고를 수 있는 돌이 남아있다는 사실을 기록해둔다
			//cout << untakenStones[i] << endl;
			vector<int> *nextUntakenStones = 
				new vector<int>(untakenStones); // 해당 돌을 고른 후 남아있는 돌들의 vector
			vector<int>::iterator itr = nextUntakenStones->begin();
			while(itr != nextUntakenStones->end()){
				if(*itr == untakenStones[i]){
					nextUntakenStones->erase(itr);
					break;
				}
				++itr;
			} // vector에서 돌 하나 빼서 다음 단계에서 사용할 남아있는 돌들 vector 만드는 작업

			//cout << "next size : " << nextUntakenStones->size() << endl;
			if(isMaxPlayer)
				result = max(result, evaluate(*nextUntakenStones, untakenStones[i], !isMaxPlayer)); // 다음 단계 진행
			else
				result = min(result, evaluate(*nextUntakenStones, untakenStones[i], !isMaxPlayer));

			delete nextUntakenStones;
		}
	}

	if(!isAvailableStoneLeft){ // 더이상 고를 수 있는 돌이 없었다면
		//cout << "terminal node" << endl;
		if(isMaxPlayer) // max player 차례라면
			result = -1.0;
		else
			result = 1.0;
	}

	//cout << "ismaxPlayer : " << isMaxPlayer << endl;
	//cout << result << endl;
	//cout << "------------------------------" << endl;
	return result;
}

//float alphaBetaPruning(const vector<int> &untakenStones, int lastTakenStone, bool isMaxPlayer, int maxDepth, int currentDepth, float alpha, float beta){
//	//cout << "------------------------------" << endl;
//	cout << endl << lastTakenStone << endl << endl;
//	bool isAvailableStoneLeft = false;
//	float result;
//	int selectedStone = untakenStones.size() + 1;
//	float bestResult;
//
//	if(isMaxPlayer) bestResult = alpha;
//	else bestResult = beta;
//
//	if (lastTakenStone == 0) {
//		vector<int> nextUntakenStones = vector<int>(untakenStones);
//		vector<int> *firstPickStones = getFirstPickStones(nextUntakenStones.size());
//		for(int i = 0; i < firstPickStones->size(); ++i) {
//			vector<int>::iterator itr = nextUntakenStones.begin();
//			while(itr != firstPickStones->end()){
//				if(*itr == (*firstPickStones)[i]){
//					nextUntakenStones.erase(itr);
//					break;
//				}
//				++itr;
//			}
//
//			alpha = max(alpha, alphaBetaPruning(nextUntakenStones, (*firstPickStones)[i], !isMaxPlayer, maxDepth, currentDepth + 1, alpha, beta)); // 다음 단계 진행
//			if(alpha >= bestResult && selectedStone > untakenStones[i]) {
//				bestResult = alpha;
//				selectedStone = untakenStones[i];
//			}
//
//			nextUntakenStones.push_back((*firstPickStones)[i]);
//			sort(nextUntakenStones.begin(), nextUntakenStones.end());
//		}
//		delete firstPickStones;
//
//		cout << "000 bestResult : " << bestResult << ", alphaOrBeta : " << result << ", lastTakenStone : " << lastTakenStone << ", selectedStone : " << selectedStone << endl;
//		return alpha;
//	}
//
//	result = calculateResultValue(untakenStones, lastTakenStone, isMaxPlayer);
//	if(maxDepth == currentDepth) return result;
//
//	for(int i = 0; i < untakenStones.size(); ++i){ // 남아있는 돌들 검사
//		if(checkFactorOrMultiple(untakenStones[i], lastTakenStone)){ // 고를 수 있는 돌이라면
//			cout << "lastTaken : " << lastTakenStone << ", cur : " << untakenStones[i] << endl;
//			isAvailableStoneLeft = true; // 아직 고를 수 있는 돌이 남아있다는 사실을 기록해둔다
//			// cout << untakenStones[i] << endl;
//			vector<int> *nextUntakenStones = new vector<int>(untakenStones); // 해당 돌을 고른 후 남아있는 돌들의 vector
//			vector<int>::iterator itr = nextUntakenStones->begin();
//			while(itr != nextUntakenStones->end()){
//				if(*itr == untakenStones[i]){
//					nextUntakenStones->erase(itr);
//					break;
//				}
//				++itr;
//			} // vector에서 돌 하나 빼서 다음 단계에서 사용할 남아있는 돌들 vector 만드는 작업
//			sort(nextUntakenStones->begin(), nextUntakenStones->end());
//
//			// cout << "next size : " << nextUntakenStones->size() << endl;
//			if(isMaxPlayer)
//				alpha = max(alpha, alphaBetaPruning(*nextUntakenStones, untakenStones[i], !isMaxPlayer, maxDepth, currentDepth + 1, alpha, beta)); // 다음 단계 진행
//				if(alpha > bestResult && selectedStone > untakenStones[i]) {
//					bestResult = alpha;
//					selectedStone = untakenStones[i];
//				}
//			else
//				beta = min(beta, alphaBetaPruning(*nextUntakenStones, untakenStones[i], !isMaxPlayer, maxDepth, currentDepth + 1, alpha, beta));
//				if(beta < bestResult && selectedStone > untakenStones[i]) {
//					bestResult = beta;
//					selectedStone = untakenStones[i];
//				}
//
//			if(alpha >= beta) {
//				cout << "a>b bestResult : " << bestResult << ", alphaOrBeta : " << result << ", lastTakenStone : " << lastTakenStone << ", selectedStone : " << selectedStone << endl;
//				if(isMaxPlayer) return alpha;
//				else return beta;
//			}
//
//			delete nextUntakenStones;
//		}
//	}
//
//	if(!isAvailableStoneLeft){ // 더이상 고를 수 있는 돌이 없었다면
//		cout << "terminal :" << lastTakenStone << endl;
//		return result;
//	}
//
//
//	//cout << result << endl;
//	//cout << "------------------------------" << endl;
//	
//	if(isMaxPlayer) result = alpha;
//	else result = beta;
//
//	cout << "fff bestResult : " << bestResult << ", alphaOrBeta : " << result << ", lastTakenStone : " << lastTakenStone << ", selectedStone : " << selectedStone << endl;
//	return result;
//}

float alphaBetaPruning(const vector<int> &untakenStones, int lastTakenStone, bool isMaxPlayer, int maxDepth, int currentDepth, float alpha, float beta, int *visitedNodeCnt){
	++(*visitedNodeCnt);
	cout << "vcnt : " << *visitedNodeCnt << endl;
	int selectedNode;
	// 
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
			value = max(value, alphaBetaPruning(nextUntakenStones, (*firstPickStones)[i], !isMaxPlayer, maxDepth, currentDepth + 1, alpha, beta, visitedNodeCnt)); // 다음 단계 진행
			if(tmpValue != value) selectedNode = (*firstPickStones)[i]; 
			alpha = max(alpha, value);
			if(alpha >= beta) {
				break;
			}

			nextUntakenStones.push_back((*firstPickStones)[i]);
		}
		delete firstPickStones;

		cout << selectedNode << endl;

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

	if(maxDepth == currentDepth || isTerminalNode || *visitedNodeCnt >= MAX_VISIT_COUNT) {
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
				value = max(value, alphaBetaPruning(*nextUntakenStones, untakenStones[i], !isMaxPlayer, maxDepth, currentDepth + 1, alpha, beta, visitedNodeCnt));
				if(tmpValue != value) selectedNode = untakenStones[i]; 
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
				value = min(value, alphaBetaPruning(*nextUntakenStones, untakenStones[i], !isMaxPlayer, maxDepth, currentDepth + 1, alpha, beta, visitedNodeCnt));//
				if(tmpValue != value) selectedNode = untakenStones[i]; 
				beta = min(beta, value); //
				delete nextUntakenStones;
				if(alpha >= beta) break;
			}
		}
		//

	}

	if(currentDepth == 0) cout << selectedNode << endl;

	return value;
}

vector<int> *getFirstPickStones(int numOfStones) {
	vector<int> stones = vector<int>();

	for(int i = 1; i < (numOfStones / 2); ++i) {
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
	cout << "sort complete" << endl;

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
