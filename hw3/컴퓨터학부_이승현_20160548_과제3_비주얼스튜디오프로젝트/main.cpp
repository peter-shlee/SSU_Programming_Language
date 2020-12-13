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
bool checkTerm(const char* term);
bool checkSameNameFunc(const char* term);
void appendNewTerm(const char* term);
void printAndLoadDefun();

int main() {
    /*convertDescription("(ADD 12321 ted)", "(x y) (MINUS x (MINUS 0 y))");
    convertDescription("(ADD ang angang)", "(x y) (MINUS x (MINUS 0 y))");*/
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
        char* newTerm; // 명령어 문자열

        printMenu(); // 메뉴 출력
        inputNum = inputNumber(); // 사용자에게 메뉴 번호 입력받음

        switch (inputNum) {
        case 1: // Define DEFUN 선택 시, DEFUN을 사용하여 정의한 명령어를 defun.txt파일에 저장한다. (append)
            // 명령어 입력받아 defun.txt에 추가
            // 1. defun으로 함수를 정의할 때 매개변수가 하나도 없을 시 에러 처리
            // 2. defun으로 함수를 정의할 때 이미 정의되어 있는 경우 에러 처리

            // 1단계 사용자로부터 명령어를 입력받는다
            newTerm = getTerm();

            // 2단계 함수에 매개변수가 하나 이상 있는지 확인한다
            if (!checkTerm(newTerm)) {
                printf("매개변수가 하나 이상 있어야 합니다.\n");
                break;
            }

            // 3단계 기존의 defun.txt에 동일한 이름의 함수가 이미 정의되어있는지 확인한다.
            if (!checkSameNameFunc(newTerm)) {
                printf("이미 정의된 함수입니다.\n");
                break;
            }
            
            // 4단계 defun.txt에 새로운 함수를 추가한다.
            appendNewTerm(newTerm);
            break;
        case 2: // Print DEFUN 선택 시, defun.txt파일에 정의되어 있는 DEFUN 명령어를 화면에 출력한다. 파일로 불러온 DEFUN값은 끝날 때까지 유지된다.
            // 화면에 DEFUN 내용 출력
            // DEFUN에 저장되어있던 내용들 프로세스에 로드한다. (로드한 내용 컴파일러에서 이용) -> map 이용하여 함수 저장 first:함수이름, second:함수정의
            printAndLoadDefun();
            break;
        case 3: // Interpreter 선택 시, 같은 폴더 내에 존재하는 프로그램 파일명을 입력받아 내용을 출력 한 후 인터프리터를 실행한다. 인터프리터 실행이 끝나면 겨로가 값을 출력한다
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

            //printFileContents(fp); // 파일 내용 출력
            executeFromFile(fp); // 파일에 저장되어 있던 명령어들 실행
            fclose(fp);
            break;
        case 4: // Exit 메뉴 선택 시
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

void printAndLoadDefun() {
    FILE* fp;
    char buf[MAX_LENGTH];
    char funcName[MAX_LENGTH];
    char funcDef[MAX_LENGTH];
    // 파일 오픈
    if ((fp = fopen("defun.txt", "r")) == NULL) {
        fprintf(stderr, "fopen error for %s\n", "defun.txt");
        return;
    }
    // 기존의 함수 저장되어있던 map 초기화
    DEFUN.clear();

    // 파일 한줄씩 확인하며 반복
    while (fgets(buf, MAX_LENGTH, fp) != NULL) {
        if (buf[strlen(buf) - 1] == '\n') buf[strlen(buf) - 1] = '\0';
        // 화면에 DEFUN 내용 출력
        printf("%s\n", buf);
        // 읽어들인 명령어를 map에 추가한다. (로드한 내용 컴파일러에서 이용) -> map 이용하여 함수 저장 first:함수이름, second:함수정의
        strtok(buf, " ");
        strcpy(funcName, buf);
        int i = 0;
        while (buf[i] != '(')++i;
        strcpy(funcDef, buf + i);

        pair<char*, char*> *nextFunc = new pair<char*, char*>();
        nextFunc->first = (char*)malloc(strlen(funcName) + 1);
        if (nextFunc->first == NULL) return;
        nextFunc->second = (char*)malloc(strlen(funcDef) + 1);
        if (nextFunc->second == NULL) return;
        strcpy(nextFunc->first, funcName);
        strcpy(nextFunc->second, funcDef);

        DEFUN.insert(make_pair(nextFunc->first, nextFunc->second));
    }

}

void appendNewTerm(const char* term) {
    // defun.txt에 새로운 함수를 추가한다.
    FILE* fp;
    const char* realTerm;
    char buf[MAX_LENGTH];
    strcpy(buf, term);

    strtok(buf, " ");
    realTerm = strtok(NULL, " ");
    realTerm = term + (realTerm - buf);

    if ((fp = fopen("defun.txt", "a")) == NULL) {
        fprintf(stderr, "fopen error for %s\n", "defun.txt");
        return;
    }

    fprintf(fp, "%s\n", realTerm);
    fclose(fp);
    return;
}

bool checkSameNameFunc(const char* term) {
    char* funcName;
    char buf[MAX_LENGTH];
    strcpy(buf, term);

    funcName = strtok(buf, " ");
    funcName = strtok(buf, " ");

    if (DEFUN.find(buf) == DEFUN.end()) return false;

    return true;
}

bool checkTerm(const char* term) {
    char* curterm;
    char buf[MAX_LENGTH];
    strcpy(buf, term);

    curterm = strtok(buf, " ");
    if (strcmp(curterm, "DEFUN")) return false;

    int i = 0;
    while (term[i] != '(' && i < strlen(term)) ++i;
    while (term[++i] != ')' && i < strlen(term)) {
        if (term[i] != ' ') {
            return true;
        }
    }

    return false;
}

void printMenu() { // 메뉴를 출력하는 함수
    cout << "===================================" << endl;
    cout << "1. Define DEFUN" << endl;
    cout << "2. Print DEFUN" << endl;
    cout << "3. Interpreter" << endl;
    cout << "4. Exit" << endl;
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
        if (input >= 1 && input <= 4) break;
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
    /*cout << "문장을 입력하세요." << endl;
    cout << "-----------------------------------" << endl;*/
    while (getchar() != '\n');
    fgets(term, MAX_LENGTH, stdin);
    if (term[strlen(term) - 1] == '\n') term[strlen(term) - 1] = '\0';
    /*cout << endl;
    cout << "-----------------------------------" << endl;*/

    return term;
}

bool executeFromFile(FILE *fp) { // 파일에서 명령어를 읽어오는 함수
    bool isCompiledSucceeded;
    char buf[MAX_LENGTH];

    rewind(fp);
    fgets(buf, sizeof(buf), fp); // 파일에서 명령어 한줄 읽어온다
    while (1) {
        if (buf[strlen(buf) - 1] == '\n') buf[strlen(buf) - 1] = '\0';
        else break;
        cout << "**********" << endl;
        cout << buf << endl << endl;
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
    char* term = loadTermsFromFile(); // 중간코드 파일에서 컴파일된 명령어를 읽어온다
    cout << "Prefix To Postfix : " << term << endl;
    cout << "Result : ";
    interpreter(term); // 컴파일 된 명령어를 실행
    delete[] term;
    cout << endl;
}