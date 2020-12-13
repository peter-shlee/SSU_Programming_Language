#include "main.h"
#include "compiler.h"
#include "interpreter.h"

#define MAX_LENGTH 1024

void run();
void printMenu(); // �޴��� ����ϴ� �Լ�
int inputNumber(); // ���ڸ� �Է¹޴� �Լ�
char *getFileName(); // ���ϸ��� �Է¹޴� �Լ�
void printFileContents(FILE* fp); // ������ ������ ����ϴ� �Լ�
char* getTerm(); // ��ɾ �Է¹޴� �Լ�
char* loadTermsFromFile(); // ���Ͽ��� ��ɾ �о���� �Լ�
bool executeFromFile(FILE* fp); // ������ ��ɾ���� �����Ű�� �Լ�
void executeInterpreter(); // ���������͸� �����Ű�� �Լ�
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

    while (1) { // ����ڰ� ���� �޴� ������ �� ���� �ݺ�
        FILE* fp;
        int inputNum; // ����ڰ� ������ �޴� ��ȣ
        bool endFlag = false; // ����ڰ� ���Ḧ �����ߴٴ� ���� ��Ÿ���� ���� �÷���
        bool fopenSucceedFlag = false; // ���� ������ �����ߴٴ� ���� ��Ÿ���� ���� �÷���
        bool isCompiledSucceeded; // ��ɾ� �������� �����ߴٴ� ���� ��Ÿ���� ���� �÷���
        char* fileName; // ���ϸ� ���ڿ�
        char* newTerm; // ��ɾ� ���ڿ�

        printMenu(); // �޴� ���
        inputNum = inputNumber(); // ����ڿ��� �޴� ��ȣ �Է¹���

        switch (inputNum) {
        case 1: // Define DEFUN ���� ��, DEFUN�� ����Ͽ� ������ ��ɾ defun.txt���Ͽ� �����Ѵ�. (append)
            // ��ɾ� �Է¹޾� defun.txt�� �߰�
            // 1. defun���� �Լ��� ������ �� �Ű������� �ϳ��� ���� �� ���� ó��
            // 2. defun���� �Լ��� ������ �� �̹� ���ǵǾ� �ִ� ��� ���� ó��

            // 1�ܰ� ����ڷκ��� ��ɾ �Է¹޴´�
            newTerm = getTerm();

            // 2�ܰ� �Լ��� �Ű������� �ϳ� �̻� �ִ��� Ȯ���Ѵ�
            if (!checkTerm(newTerm)) {
                printf("�Ű������� �ϳ� �̻� �־�� �մϴ�.\n");
                break;
            }

            // 3�ܰ� ������ defun.txt�� ������ �̸��� �Լ��� �̹� ���ǵǾ��ִ��� Ȯ���Ѵ�.
            if (!checkSameNameFunc(newTerm)) {
                printf("�̹� ���ǵ� �Լ��Դϴ�.\n");
                break;
            }
            
            // 4�ܰ� defun.txt�� ���ο� �Լ��� �߰��Ѵ�.
            appendNewTerm(newTerm);
            break;
        case 2: // Print DEFUN ���� ��, defun.txt���Ͽ� ���ǵǾ� �ִ� DEFUN ��ɾ ȭ�鿡 ����Ѵ�. ���Ϸ� �ҷ��� DEFUN���� ���� ������ �����ȴ�.
            // ȭ�鿡 DEFUN ���� ���
            // DEFUN�� ����Ǿ��ִ� ����� ���μ����� �ε��Ѵ�. (�ε��� ���� �����Ϸ����� �̿�) -> map �̿��Ͽ� �Լ� ���� first:�Լ��̸�, second:�Լ�����
            printAndLoadDefun();
            break;
        case 3: // Interpreter ���� ��, ���� ���� ���� �����ϴ� ���α׷� ���ϸ��� �Է¹޾� ������ ��� �� �� ���������͸� �����Ѵ�. ���������� ������ ������ �ܷΰ� ���� ����Ѵ�
            while (true) {
                fileName = getFileName(); // ���ϸ� �Է¹���

                if ((fp = fopen(fileName, "r")) == NULL) { // ���� ����
                    // �Է��� ���ϸ��� ���� ���� �������� �ʴ� ��� ���� �޼����� ��� �� �ٽ� �Է¹޴´�.
                    cout << "������ �������� �ʽ��ϴ�." << endl << endl;
                    fopenSucceedFlag = false;
                } else {
                    fopenSucceedFlag = true;
                }
                if (fopenSucceedFlag) break;
                else delete[] fileName;
            }
            delete[] fileName;

            //printFileContents(fp); // ���� ���� ���
            executeFromFile(fp); // ���Ͽ� ����Ǿ� �ִ� ��ɾ�� ����
            fclose(fp);
            break;
        case 4: // Exit �޴� ���� ��
            endFlag = true;
            break;
        default:
            break;
        }

        if (endFlag) {
            cout << "���α׷��� �����մϴ�." << endl;
            break;
        }
    }

}

void printAndLoadDefun() {
    FILE* fp;
    char buf[MAX_LENGTH];
    char funcName[MAX_LENGTH];
    char funcDef[MAX_LENGTH];
    // ���� ����
    if ((fp = fopen("defun.txt", "r")) == NULL) {
        fprintf(stderr, "fopen error for %s\n", "defun.txt");
        return;
    }
    // ������ �Լ� ����Ǿ��ִ� map �ʱ�ȭ
    DEFUN.clear();

    // ���� ���پ� Ȯ���ϸ� �ݺ�
    while (fgets(buf, MAX_LENGTH, fp) != NULL) {
        if (buf[strlen(buf) - 1] == '\n') buf[strlen(buf) - 1] = '\0';
        // ȭ�鿡 DEFUN ���� ���
        printf("%s\n", buf);
        // �о���� ��ɾ map�� �߰��Ѵ�. (�ε��� ���� �����Ϸ����� �̿�) -> map �̿��Ͽ� �Լ� ���� first:�Լ��̸�, second:�Լ�����
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
    // defun.txt�� ���ο� �Լ��� �߰��Ѵ�.
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

void printMenu() { // �޴��� ����ϴ� �Լ�
    cout << "===================================" << endl;
    cout << "1. Define DEFUN" << endl;
    cout << "2. Print DEFUN" << endl;
    cout << "3. Interpreter" << endl;
    cout << "4. Exit" << endl;
    cout << "===================================" << endl;

    return;
}

int inputNumber() { // ���ڸ� �Է¹޴� �Լ�
    int input;
    while (true) {
        cout << "�޴��� �����ϼ��� >> ";
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

char *getFileName() { // ���ϸ��� �Է¹޴� �Լ�
    char *fileName = new char[MAX_LENGTH];
    cout << "���ϸ��� �Է��ϼ��� >> ";
    cin >> fileName;
    cout << endl;

    return fileName;
}

void printFileContents(FILE *fp) { // ������ ������ ����ϴ� �Լ�
    char buf[MAX_LENGTH];

    cout << "���� ������" << endl;
    cout << "-----------------------------------" << endl;
    fgets(buf, sizeof(buf), fp);
    while (1) {
        if (buf[strlen(buf) - 1] == '\n') buf[strlen(buf) - 1] = '\0';
        cout << buf << endl;

        if (feof(fp)) break;
        fgets(buf, sizeof(buf), fp);
    }
    cout << "-----------------------------------" << endl;
    cout << "�Դϴ�." << endl;
    cout << endl;

    return;
}

char* getTerm() { // ��ɾ �Է¹޴� �Լ�
    char* term = new char[MAX_LENGTH];
    /*cout << "������ �Է��ϼ���." << endl;
    cout << "-----------------------------------" << endl;*/
    while (getchar() != '\n');
    fgets(term, MAX_LENGTH, stdin);
    if (term[strlen(term) - 1] == '\n') term[strlen(term) - 1] = '\0';
    /*cout << endl;
    cout << "-----------------------------------" << endl;*/

    return term;
}

bool executeFromFile(FILE *fp) { // ���Ͽ��� ��ɾ �о���� �Լ�
    bool isCompiledSucceeded;
    char buf[MAX_LENGTH];

    rewind(fp);
    fgets(buf, sizeof(buf), fp); // ���Ͽ��� ��ɾ� ���� �о�´�
    while (1) {
        if (buf[strlen(buf) - 1] == '\n') buf[strlen(buf) - 1] = '\0';
        else break;
        cout << "**********" << endl;
        cout << buf << endl << endl;
        isCompiledSucceeded = compile(buf); // ������ �ϰ�, ������ ����� ����
        if(isCompiledSucceeded) executeInterpreter(); // ������ �����ߴٸ� ���������� ����
        else cout << " - ������ ����" << endl; // ������ �����ߴٸ� �����޼��� ���

        if (feof(fp)) break;
        fgets(buf, sizeof(buf), fp);
    }
    cout << endl;

    return true;
}

char* loadTermsFromFile() { // �߰��ڵ� ���Ͽ��� ��ɾ �о���� �Լ�
    char* buf = new char[MAX_LENGTH];
    FILE* fp;

    if ((fp = fopen(INTERMEDIATE_CODE_FILENAME, "r")) == NULL) { // �߰��ڵ� ������ open�Ѵ�
        fprintf(stderr, "fopen error for %s\n", INTERMEDIATE_CODE_FILENAME);
        return NULL;
    }

    fgets(buf, MAX_LENGTH, fp); // �߰��ڵ� ���Ͽ��� �����ϵ� ��ɾ �о�´�
    if (buf[strlen(buf) - 1] == '\n') buf[strlen(buf) - 1] = '\0';

    fclose(fp);
    return buf; // �о�� ��ɾ �����Ѵ�
}

void executeInterpreter() { // ���������͸� �����Ű�� �Լ�
    char* term = loadTermsFromFile(); // �߰��ڵ� ���Ͽ��� �����ϵ� ��ɾ �о�´�
    cout << "Prefix To Postfix : " << term << endl;
    cout << "Result : ";
    interpreter(term); // ������ �� ��ɾ ����
    delete[] term;
    cout << endl;
}