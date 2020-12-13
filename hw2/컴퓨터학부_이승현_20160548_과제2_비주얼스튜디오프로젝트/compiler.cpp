#include "compiler.h"

const char* FUN[] = { "MINUS", "IF", "UNDEFINED" };

bool compile(char* terms) {
    termNode* head;

    removeRightWhitespace(terms);
    terms = removeLeftWhitespace(terms);
    char wrongChar;
    if (checkTermsIncludingWrongCharacter(terms, &wrongChar)) {
        // ���� (���ĺ��� "-"�� ������ ���� ��ȣ�� �ԷµǸ� ���� ó��) -> �Ǽ��� ���� ���� ����ó�� �ؾ��ϴµ� ���⼭ ���� ó���Ź���
        cout << '\"' << wrongChar << '\"' << "�� ����� �� ���� �����Դϴ�.";
        return false;
    }

    if (!checkParenthesis(terms)) {
        return false;
    }

    head = termToNode(&terms);

    if (head == NULL) {
        // ����
        return false;
    }
    saveIntermediateCode(head);
    freeAllNode(head);
    return true;
}

bool isWrongCharacter(char c) { // �ش� ���ڰ� �� ���� ������ �ʴ� �������� �˻�
    if (('0' <= c && c <= '9') || ('a' <= c && c <= 'z') || ('A' <= c && c <= 'Z') || (c == '(') || (c == ')') || (c == '-')) {
        return false;
    } else {
        if (isspace(c)) return false;
        return true;
    }
}

char *removeLeftWhitespace(char *terms) { // term ���ڿ����� ������ �ƴ� ù��° ������ ��ġ ����
    char* newStartPoint = terms;
    for (unsigned int i = 0; i < strlen(terms); ++i) {
        if (isspace(terms[i])) ++newStartPoint;
        else break;
    }

    return newStartPoint;
}

void removeRightWhitespace(char* terms) { // term �������� ���� ���ڵ��� �����ϴ� �Լ�
    if (strlen(terms) == 0) return;
    char* newStartPoint = terms;
    for (unsigned int i = strlen(terms) - 1; i > 0; --i) {
        if (isspace(terms[i])) terms[i] = '\0';
        else break;
    }
}

termNode* termToNode(char **terms) { // term�� tree������ �ű�� �Լ�
    if (!strcmp(*terms, "")) {
        cout << "terminal�� �����մϴ�.";
        return NULL;
    }
    termNode *newNode = new termNode;

    removeRightWhitespace(*terms);
    *terms = removeLeftWhitespace(*terms);
    char* term;

    if ((*terms)[0] == '(') { // �Լ��� ���
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
            // terms�� ��ġ�� ���� ��ġ�� �Űܾ���
            newNode->param2 = termToNode(terms);

            if (newNode->param1 == NULL || newNode->param2 == NULL) {
                // �ڽĳ����� ������ ��Ȳ
                freeAllNode(newNode);
                return NULL;
            }

            //*terms = strtok(*terms, " \t");
            if ((*terms)[0] != ')') {
                //����
                cout << "\")\" �� ��ġ�� �߸��Ǿ����ϴ�.";
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
            cout << "���ڰ� �ԷµǾ�� �� �ڸ��� \"-\"�� ������ �������� �ԷµǾ����ϴ�";
            freeAllNode(newNode);
            return NULL;
        }

        if (isInteger(term)) { // ������ ���
            int value = atoi(term);
            newNode->type = INTEGER;
            newNode->param1 = NULL;
            newNode->param2 = NULL;
            newNode->value = value;
        } else { // ������ ���
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

bool checkTermsIncludingWrongCharacter(char* terms, char* wrongChar) { // term�� �̻��� ���ڰ� �������� �ʴ��� �˻��ϴ� �Լ�
    for (int i = 0; i < strlen(terms); ++i) {
        if (isWrongCharacter(terms[i])) {
            *wrongChar = terms[i];
            return true;
        }
    }
    *wrongChar = '\0';
    return false;
}

bool isInteger(char *term) { // �������� �ƴ��� �Ǻ��Ҷ� ����ϴ� �Լ�
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

void printAllTerms(termNode* node, FILE *fp) { // Ʈ���� �� ����� term���� ������������� ����ϴ� �Լ�
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
                cout << "\")\" �� ��ġ�� �߸��Ǿ����ϴ�.";
                return false;
            }
            parenStack.pop();
        }
    }

    if (!parenStack.empty()) {
        cout << "\"(\" �� ��ġ�� �߸��Ǿ����ϴ�.";
        return false;
    }

    return true;
}