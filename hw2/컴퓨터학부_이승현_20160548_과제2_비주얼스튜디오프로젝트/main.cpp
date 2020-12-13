#include "main.h"
#include "compiler.h"
#include "interpreter.h"

#define MAX_LENGTH 1024

void run();
void printMenu(); // 메뉴를 출력하는 함수
int inputNumber(); // 숫자를 입력받는 함수
char *getFileName(); // 파일명을 입력받는 함수
void printFileContents(FILE* fp); // 파일의 내용을 출력하는 함수
char* getTerm(); // 명령어를 입력받는 함수
char* loadTermsFromFile(); // 파일에서 명령어를 읽어오는 함수
bool executeFromFile(FILE* fp); // 파일의 명령어들을 실행시키는 함수
void executeInterpreter(); // 인터프리터를 실행시키는 함수

int main() {

    run();

    return 0;
}

void run() {

    while (1) { // 사용자가 종료 메뉴 선택할 때 까지 반복
        FILE* fp;
        int inputNum; // 사용자가 선택한 메뉴 번호
        bool endFlag = false; // 사용자가 종료를 선택했다는 것을 나타내기 위한 플래그
        bool fopenSucceedFlag = false; // 파일 오픈이 성공했다는 것을 나타내기 위한 플래그
        bool isCompiledSucceeded; // 명령어 컴파일이 성공했다는 것을 나타내기 위한 플래그
        char* fileName; // 파일명 문자열
        char* term; // 명령어 문자열

        printMenu(); // 메뉴 출력
        inputNum = inputNumber(); // 사용자에게 메뉴 번호 입력받음

        switch (inputNum) {
        case 1: // File Load 메뉴 선택 시
            while (true) {
                fileName = getFileName(); // 파일명 입력받음

                if ((fp = fopen(fileName, "r")) == NULL) { // 파일 오픈
                    // 입력한 파일명이 폴더 내에 존재하지 않는 경우 에러 메세지를 출력 후 다시 입력받는다.
                    cout << "파일이 존재하지 않습니다." << endl << endl;
                    fopenSucceedFlag = false;
                } else {
                    fopenSucceedFlag = true;
                }
                if (fopenSucceedFlag) break;
                else delete[] fileName;
            }
            delete[] fileName;

            printFileContents(fp); // 파일 내용 출력
            executeFromFile(fp); // 파일에 저장되어 있던 명령어들 실행
            fclose(fp);
            break;
        case 2: // Interactive Mode 선택 시
            term = getTerm(); // 명령어를 직접 입력받는다
            isCompiledSucceeded = compile(term); // 명령어를 컴파일한다.
            delete[] term;

            if (isCompiledSucceeded) executeInterpreter(); // 컴파일 성공했다면 인터프리터 실행
            else cout << " - 컴파일 에러" << endl;
            cout << "-----------------------------------" << endl;
            break;
        case 3: // Exit 메뉴 선택 시
            endFlag = true;
            break;
        default:
            break;
        }

        if (endFlag) {
            cout << "프로그램을 종료합니다." << endl;
            break;
        }
    }

}

void printMenu() { // 메뉴를 출력하는 함수
    cout << "===================================" << endl;
    cout << "1. File Load" << endl;
    cout << "2. Interactive Mode" << endl;
    cout << "3. Exit" << endl;
    cout << "===================================" << endl;

    return;
}

int inputNumber() { // 숫자를 입력받는 함수
    int input;
    while (true) {
        cout << "메뉴를 선택하세요 >> ";
        cin >> input;
        if (cin.fail()) {
            while (getchar() != '\n');
            cin.clear();
            continue;
        }
        if (input >= 1 && input <= 3) break;
    }

    cout << endl;
    return input;
}

char *getFileName() { // 파일명을 입력받는 함수
    char *fileName = new char[MAX_LENGTH];
    cout << "파일명을 입력하세요 >> ";
    cin >> fileName;
    cout << endl;

    return fileName;
}

void printFileContents(FILE *fp) { // 파일의 내용을 출력하는 함수
    char buf[MAX_LENGTH];

    cout << "파일 내용은" << endl;
    cout << "-----------------------------------" << endl;
    fgets(buf, sizeof(buf), fp);
    while (1) {
        if (buf[strlen(buf) - 1] == '\n') buf[strlen(buf) - 1] = '\0';
        cout << buf << endl;

        if (feof(fp)) break;
        fgets(buf, sizeof(buf), fp);
    }
    cout << "-----------------------------------" << endl;
    cout << "입니다." << endl;
    cout << endl;

    return;
}

char* getTerm() { // 명령어를 입력받는 함수
    char* term = new char[MAX_LENGTH];
    cout << "문장을 입력하세요." << endl;
    cout << "-----------------------------------" << endl;
    while (getchar() != '\n');
    fgets(term, MAX_LENGTH, stdin);
    if (term[strlen(term) - 1] == '\n') term[strlen(term) - 1] = '\0';
    cout << endl;
    cout << "-----------------------------------" << endl;

    return term;
}

bool executeFromFile(FILE *fp) { // 파일에서 명령어를 읽어오는 함수
    bool isCompiledSucceeded;
    char buf[MAX_LENGTH];

    rewind(fp);
    fgets(buf, sizeof(buf), fp); // 파일에서 명령어 한줄 읽어온다
    while (1) {
        if (buf[strlen(buf) - 1] == '\n') buf[strlen(buf) - 1] = '\0';
        isCompiledSucceeded = compile(buf); // 컴파일 하고, 컴파일 결과를 저장
        if(isCompiledSucceeded) executeInterpreter(); // 컴파일 성공했다면 인터프리터 실행
        else cout << " - 컴파일 에러" << endl; // 컴파일 실패했다면 에러메세지 출력

        if (feof(fp)) break;
        fgets(buf, sizeof(buf), fp);
    }
    cout << endl;

    return true;
}

char* loadTermsFromFile() { // 중간코드 파일에서 명령어를 읽어오는 함수
    char* buf = new char[MAX_LENGTH];
    FILE* fp;

    if ((fp = fopen(INTERMEDIATE_CODE_FILENAME, "r")) == NULL) { // 중간코드 파일을 open한다
        fprintf(stderr, "fopen error for %s\n", INTERMEDIATE_CODE_FILENAME);
        return NULL;
    }

    fgets(buf, MAX_LENGTH, fp); // 중간코드 파일에서 컴파일된 명령어를 읽어온다
    if (buf[strlen(buf) - 1] == '\n') buf[strlen(buf) - 1] = '\0';

    fclose(fp);
    return buf; // 읽어온 명령어를 리턴한다
}

void executeInterpreter() { // 인터프리터를 실행시키는 함수
    cout << "결과 : ";
    char* term = loadTermsFromFile(); // 중간코드 파일에서 컴파일된 명령어를 읽어온다
    interpreter(term); // 컴파일 된 명령어를 실행
    delete[] term;
    cout << endl;
}