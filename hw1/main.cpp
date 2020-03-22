#include <iostream>
#include <vector>
#include <algorithm>
#include <cstring>
using namespace std;

void play(int argc, char *argv[]);
int parseInt(const char *numStr);
vector<int> getTakenStones(const vector<int> &takenStones, int numOfStones);
bool checkPrimeNumber(int num);
bool checkFactorOrMultiple(int numA, int numB);

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


	// test code for parseInt();
	cout << endl << "parseInt() test" << endl;
	cout << parseInt("1") << endl;
	cout << parseInt("12") << endl;
	cout << parseInt("123") << endl;
	cout << parseInt("1234") << endl;
	cout << parseInt("12345") << endl;
	cout << parseInt("123456") << endl;



	// test code for getTakenStones();
	cout << endl << "getTakenStones() test" << endl;
	vector<int> untakenStones = getTakenStones(takenStones, numOfStones);
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
}

float evaluate(vector<int> takenStones, int lastTakenStone){
//	for(int i = 0; i < /*아직 선택되지 않은 돌들*/; ++i){
//		if(checkFactorOrMultiple(/*아직 선택되지 않은 돌들*/, lastTakenStone)){
//
//			evaluate(//////////////, takenStones[i]);
//		}
//
//	}

}

vector<int> getTakenStones(const vector<int> &takenStones, int numOfStones){
	int takenStonesIndex = 0;
	int untakenStonesIndex = 0;
	int untakenStoneNum = 1;

	vector<int> *sortedTakenStones = new vector<int>(takenStones);
	sort(sortedTakenStones->begin(), sortedTakenStones->end());

	vector<int> untakenStones = 
		vector<int>(numOfStones - takenStones.size());

	while(untakenStoneNum <= numOfStones){
		if((*sortedTakenStones)[takenStonesIndex] == untakenStoneNum){
			++takenStonesIndex;
		} else {
			untakenStones[untakenStonesIndex++] = untakenStoneNum;
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
