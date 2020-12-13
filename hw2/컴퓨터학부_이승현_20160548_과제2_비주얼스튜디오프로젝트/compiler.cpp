#include "compiler.h"

const char* FUN[] = { "MINUS", "IF", "UNDEFINED" };

bool compile(char* terms) {
    termNode* head;

    removeRightWhitespace(terms);
    terms = removeLeftWhitespace(terms);
    char wrongChar;
    if (checkTermsIncludingWrongCharacter(terms, &wrongChar)) {
        // 에러 (알파벳과 "-"를 제외한 각종 기호가 입력되면 에러 처리) -> 실수가 들어가면 따로 에러처리 해야하는데 여기서 먼저 처리돼버림
        cout << '\"' << wrongChar << '\"' << "는 사용할 수 없는 문자입니다.";
        return false;
    }

    if (!checkParenthesis(terms)) {
        return false;
    }

    head = termToNode(&terms);

    if (head == NULL) {
        // 에러
        return false;
    }
    saveIntermediateCode(head);
    freeAllNode(head);
    return true;
}

bool isWrongCharacter(char c) { // 해당 문자가 이 언어에서 사용되지 않는 문자인지 검사
    if (('0' <= c && c <= '9') || ('a' <= c && c <= 'z') || ('A' <= c && c <= 'Z') || (c == '(') || (c == ')') || (c == '-')) {
        return false;
    } else {
        if (isspace(c)) return false;
        return true;
    }
}

char *removeLeftWhitespace(char *terms) { // term 문자열에서 공백이 아닌 첫번째 문자의 위치 리턴
    char* newStartPoint = terms;
    for (unsigned int i = 0; i < strlen(terms); ++i) {
        if (isspace(terms[i])) ++newStartPoint;
        else break;
    }

    return newStartPoint;
}

void removeRightWhitespace(char* terms) { // term 오른쪽의 공백 문자들을 제거하는 함수
    if (strlen(terms) == 0) return;
    char* newStartPoint = terms;
    for (unsigned int i = strlen(terms) - 1; i > 0; --i) {
        if (isspace(terms[i])) terms[i] = '\0';
        else break;
    }
}

termNode* termToNode(char **terms) { // term을 tree구조로 옮기는 함수
    if (!strcmp(*terms, "")) {
        cout << "terminal이 부족합니다.";
        return NULL;
    }
    termNode *newNode = new termNode;

    removeRightWhitespace(*terms);
    *terms = removeLeftWhitespace(*terms);
    char* term;

    if ((*terms)[0] == '(') { // 함수인 경우
        bool undefinedFunctionFlag = false;
        term = strtok(*terms, " \t");
        char* funcName = term + 1;
        *terms += (strlen(term) + 1);

        if (!strcmp(funcName, FUN[MINUS])) {
            newNode->type = FUN[MINUS];
        } else if (!strcmp(funcName, FUN[IF])) {
            newNode->type = FUN[IF];
        } else {
            newNode->type = FUN[UNDEFINED];
            undefinedFunctionFlag = true;
        }

        if (!undefinedFunctionFlag) {
            newNode->param1 = termToNode(terms);
            // terms의 위치를 다음 위치로 옮겨야함
            newNode->param2 = termToNode(terms);

            if (newNode->param1 == NULL || newNode->param2 == NULL) {
                // 자식노드들이 에러인 상황
                freeAllNode(newNode);
                return NULL;
            }

            //*terms = strtok(*terms, " \t");
            if ((*terms)[0] != ')') {
                //에러
                cout << "\")\" 의 위치가 잘못되었습니다.";
                freeAllNode(newNode);
                return NULL;
            } else {
                ++(*terms);
            }
        }
        

    } else {
        term = strtok(*terms, " \t"); 
        *terms += (strlen(term) + 1);
        char* positionOfRightParenthesis = NULL;
        for (int i = 0; i < strlen(term); ++i) {
            if (term[i] == ')') {
                positionOfRightParenthesis = &(term[i]);
                *positionOfRightParenthesis = '\0';
                *(*terms - 1) = ' ';
                *terms = &(term[i]);
                break;
            }
        }

        if (checkInvalidNumber(term)) {
            cout << "숫자가 입력되어야 할 자리에 \"-\"가 여러번 연속으로 입력되었습니다";
            freeAllNode(newNode);
            return NULL;
        }

        if (isInteger(term)) { // 정수인 경우
            int value = atoi(term);
            newNode->type = INTEGER;
            newNode->param1 = NULL;
            newNode->param2 = NULL;
            newNode->value = value;
        } else { // 변수인 경우
            newNode->type = VARIABLE;
            newNode->param1 = NULL;
            newNode->param2 = NULL;
        }

        for (int i = 0; term[i] != '\0'; ++i) {
            term[i] = '\0';
        }

        if (positionOfRightParenthesis != NULL) {
            *positionOfRightParenthesis = ')';
        }
    }

    return newNode;
}

bool checkInvalidNumber(char *term) {
    for (int i = 0; i < strlen(term); ++i) {
        if ((term[i] >= '0' && term[i] <= '9') || term[i] == '-') continue;
        else return false;
    }

    if (term[0] == '-' && term[1] == '-') return true;
    else return false;
}

bool checkTermsIncludingWrongCharacter(char* terms, char* wrongChar) { // term에 이상한 문자가 섞여있지 않는지 검사하는 함수
    for (int i = 0; i < strlen(terms); ++i) {
        if (isWrongCharacter(terms[i])) {
            *wrongChar = terms[i];
            return true;
        }
    }
    *wrongChar = '\0';
    return false;
}

bool isInteger(char *term) { // 정수인지 아닌지 판별할때 사용하는 함수
    if (term[0] == '-' || ('0' <= term[0] && term[0] <= '9')) {
        for (int i = 1; i < strlen(term); ++i) {
            if ('0' <= term[i] && term[i] <= '9') {
                continue;
            } else {
                return false;
            }
        }
    } else {
        return false;
    }

    return true;
}

void printAllTerms(termNode* node, FILE *fp) { // 트리의 에 저장된 term들을 후위연산식으로 출력하는 함수
    if (!strcmp(node->type, INTEGER) || !strcmp(node->type, VARIABLE) || !strcmp(node->type, FUN[UNDEFINED])) {
        if (!strcmp(node->type, INTEGER)) fprintf(fp, "%d ", node->value); //cout << node->value << ' ';
        else fprintf(fp, "%s ", FUN[UNDEFINED]); //cout << "undefined ";
    } else {
        printAllTerms(node->param1, fp);
        printAllTerms(node->param2, fp);
        fprintf(fp, "%s ", node->type);
    }

    return;
}

bool saveIntermediateCode(termNode* node) {
    FILE* fp;

    if ((fp = fopen(INTERMEDIATE_CODE_FILENAME, "w")) == NULL) {
        fprintf(stderr, "fopen error for %s\n", INTERMEDIATE_CODE_FILENAME);
        return false;
    }

    printAllTerms(node, fp);

    fclose(fp);
    return true;
}

void freeAllNode(termNode* node) {
    if (node->param1 != NULL) freeAllNode(node->param1);
    if (node->param2 != NULL) freeAllNode(node->param2);

    node->param1 = NULL;
    node->param2 = NULL;
    node->type = NULL;
    delete node;
}

bool checkParenthesis(char *terms) {
    stack<char> parenStack;
    for (int i = 0; i < strlen(terms); ++i) {
        if (terms[i] == '(') {
            parenStack.push('(');
        } else if (terms[i] == ')') {
            if (parenStack.empty()) {
                cout << "\")\" 의 위치가 잘못되었습니다.";
                return false;
            }
            parenStack.pop();
        }
    }

    if (!parenStack.empty()) {
        cout << "\"(\" 의 위치가 잘못되었습니다.";
        return false;
    }

    return true;
}