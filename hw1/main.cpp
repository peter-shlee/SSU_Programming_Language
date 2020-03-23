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
	if(takenStones.size() % 2)
		isMaxPlayer = true;
	else
		isMaxPlayer = false;

	cout 
	<< evaluate(untakenStones, takenStones[takenStones.size() - 1], isMaxPlayer) 
	<< endl;
}

float evaluate(const vector<int> &untakenStones
		, int lastTakenStone
		, bool isMaxPlayer){
	cout << "------------------------------" << endl;
	bool isAvailableStoneLeft = false;
	float result;

	if(isMaxPlayer) result = -1.0;
	else result = 1.0;

	cout << "size : " << untakenStones.size() << endl;
	if(!untakenStones.size()){
		cout << "isMaxPlayer : " << isMaxPlayer << endl;
		if(isMaxPlayer)
			return 1.0;
		else
			return -1.0;
	}
	for(int i = 0; i < untakenStones.size(); ++i){
		if(checkFactorOrMultiple(untakenStones[i], lastTakenStone)){
			isAvailableStoneLeft = true;
			cout << untakenStones[i] << endl;
			vector<int> *nextUntakenStones = 
				new vector<int>(untakenStones);
			vector<int>::iterator itr = nextUntakenStones->begin();
			while(itr != nextUntakenStones->end()){
				if(*itr == untakenStones[i]){
					nextUntakenStones->erase(itr);
					break;
				}
				++itr;
			}

			cout << "next size : " << nextUntakenStones->size() << endl;
			if(isMaxPlayer)
				result = max(result, evaluate(*nextUntakenStones, untakenStones[i], !isMaxPlayer));
			else
				result = min(result, evaluate(*nextUntakenStones, untakenStones[i], !isMaxPlayer));

			delete nextUntakenStones;
		}
	}

	if(!isAvailableStoneLeft){
		cout << "terminal node" << endl;
		if(isMaxPlayer)
			result = 1.0;
		else
			result = -1.0;
	}

	cout << result << endl;
	cout << "------------------------------" << endl;
	return result;
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
