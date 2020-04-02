#include <iostream>
#include <vector>
#include <algorithm>
#include <cstring>
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

	cout << "taken stones count : " << takenStones.size() << endl;
	cout << "last taken stone : " << takenStones[takenStones.size() - 1] << endl; 
	cout << "isMaxPlayer : " << isMaxPlayer << endl;
	cout 
	<< evaluate(untakenStones, takenStones[takenStones.size() - 1], isMaxPlayer) 
	<< endl;
	
	// test code for calculateResultValue();
	cout << endl << "calculateResultValue() test" << endl;
	cout << calculateResultValue(untakenStones, takenStones[takenStones.size() - 1] , isMaxPlayer) << endl;
}

float evaluate(const vector<int> &untakenStones
		, int lastTakenStone
		, bool isMaxPlayer){
	//cout << "------------------------------" << endl;
	bool isAvailableStoneLeft = false;
	float result;

	if(isMaxPlayer) result = -1.0; // max player 차례라면 result를 -1.0으로 놓고 시작
	else result = 1.0; // min player 차례라면 result를 1.0으로 놓고 시작

	// cout << "size : " << untakenStones.size() << endl; // 아직 안가져간 돌의 개수 출력
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
			// cout << untakenStones[i] << endl;
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

			// cout << "next size : " << nextUntakenStones->size() << endl;
			if(isMaxPlayer)
				result = max(result, evaluate(*nextUntakenStones, untakenStones[i], !isMaxPlayer)); // 다음 단계 진행
			else
				result = min(result, evaluate(*nextUntakenStones, untakenStones[i], !isMaxPlayer));

			delete nextUntakenStones;
		}
	}

	if(!isAvailableStoneLeft){ // 더이상 고를 수 있는 돌이 없었다면
		// cout << "terminal node" << endl;
		if(isMaxPlayer) // max player 차례라면
			result = -1.0;
		else
			result = 1.0;
	}

	//cout << result << endl;
	//cout << "------------------------------" << endl;
	return result;
}

double calculateResultValue(const vector<int> &untakenStones
			, int lastTakenStone
			, bool isMaxPlayer) {
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

	cout << "canGetStoneCount : " << canGetStoneCount  << endl;
	cout << "primeNumberCount : " << primeNumberCount  << endl;
	cout << "multipleCount : " << multipleCount  << endl;

	return resultValue;
}

vector<int> *getUntakenStones(const vector<int> &takenStones, int numOfStones){
	int takenStonesIndex = 0;
	int untakenStonesIndex = 0;
	int untakenStoneNum = 1;

	vector<int> *sortedTakenStones = new vector<int>(takenStones);
	sort(sortedTakenStones->begin(), sortedTakenStones->end());

	vector<int> *untakenStones = 
		new vector<int>(numOfStones - takenStones.size());

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
