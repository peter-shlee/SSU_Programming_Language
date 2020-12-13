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

int main() {

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
        char* term; // ��ɾ� ���ڿ�

        printMenu(); // �޴� ���
        inputNum = inputNumber(); // ����ڿ��� �޴� ��ȣ �Է¹���

        switch (inputNum) {
        case 1: // File Load �޴� ���� ��
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

            printFileContents(fp); // ���� ���� ���
            executeFromFile(fp); // ���Ͽ� ����Ǿ� �ִ� ��ɾ�� ����
            fclose(fp);
            break;
        case 2: // Interactive Mode ���� ��
            term = getTerm(); // ��ɾ ���� �Է¹޴´�
            isCompiledSucceeded = compile(term); // ��ɾ �������Ѵ�.
            delete[] term;

            if (isCompiledSucceeded) executeInterpreter(); // ������ �����ߴٸ� ���������� ����
            else cout << " - ������ ����" << endl;
            cout << "-----------------------------------" << endl;
            break;
        case 3: // Exit �޴� ���� ��
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

void printMenu() { // �޴��� ����ϴ� �Լ�
    cout << "===================================" << endl;
    cout << "1. File Load" << endl;
    cout << "2. Interactive Mode" << endl;
    cout << "3. Exit" << endl;
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
        if (input >= 1 && input <= 3) break;
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
    cout << "������ �Է��ϼ���." << endl;
    cout << "-----------------------------------" << endl;
    while (getchar() != '\n');
    fgets(term, MAX_LENGTH, stdin);
    if (term[strlen(term) - 1] == '\n') term[strlen(term) - 1] = '\0';
    cout << endl;
    cout << "-----------------------------------" << endl;

    return term;
}

bool executeFromFile(FILE *fp) { // ���Ͽ��� ��ɾ �о���� �Լ�
    bool isCompiledSucceeded;
    char buf[MAX_LENGTH];

    rewind(fp);
    fgets(buf, sizeof(buf), fp); // ���Ͽ��� ��ɾ� ���� �о�´�
    while (1) {
        if (buf[strlen(buf) - 1] == '\n') buf[strlen(buf) - 1] = '\0';
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
    cout << "��� : ";
    char* term = loadTermsFromFile(); // �߰��ڵ� ���Ͽ��� �����ϵ� ��ɾ �о�´�
    interpreter(term); // ������ �� ��ɾ ����
    delete[] term;
    cout << endl;
}