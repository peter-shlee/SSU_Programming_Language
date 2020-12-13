#include "compiler.h"

map<char*, char*, cmpByFuncName> DEFUN;

const char* FUN[] = { "MINUS", "IF", "UNDEFINED" };

bool compile(char* terms) {
    termNode* head;

    removeRightWhitespace(terms);
    terms = removeLeftWhitespace(terms);
    char wrongChar;
    if (checkTermsIncludingWrongCharacter(terms, &wrongChar)) {
        // 에러 (알파벳과 "-", "/"를 제외한 각종 기호가 입력되면 에러 처리) -> 실수가 들어가면 따로 에러처리 해야하는데 여기서 먼저 처리돼버림
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
    if (('0' <= c && c <= '9') || ('a' <= c && c <= 'z') || ('A' <= c && c <= 'Z') || (c == '(') || (c == ')') || (c == '-') || (c == '/')) {
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

termNode* termToNode(char** terms) { // term을 tree구조로 옮기는 함수
    if (!strcmp(*terms, "")) {
        cout << "terminal이 부족합니다.";
        return NULL;
    }
    termNode* newNode = new termNode;

    removeRightWhitespace(*terms);
    *terms = removeLeftWhitespace(*terms);
    char* term;

    if ((*terms)[0] == '(') { // 함수인 경우
        bool undefinedFunctionFlag = false;
        int termLen = strlen(*terms);
        term = strtok(*terms, " \t");
        char* funcName = term + 1;
        *terms += (strlen(term) + 1);

        if (!strcmp(funcName, FUN[MINUS])) {
            newNode->type = FUN[MINUS];
        } else if (!strcmp(funcName, FUN[IF])) {
            newNode->type = FUN[IF];
        } else { // IF, MINUS 이외의 function
            bool defFuncFlag = false;
            // 정의된 함수인지 확인
            auto defunItr = DEFUN.begin();
            if ((defunItr = DEFUN.find(funcName)) != DEFUN.end()) defFuncFlag = true;
            // 정의된 함수일 때
            if (defFuncFlag) { // defunItr 이용
                char curFun[BUFFER_SIZE];
                memset(curFun, 0, BUFFER_SIZE);
                // 사용자 정의 함수 위치를 함수 정의부로 바꿔치기
                *terms -= (strlen(term) + 1);
                for (int i = 0; i < termLen; ++i) {
                    if ((*terms)[i] == 0) (*terms)[i] = ' ';
                }
                int i = 0;
                int cnt = 0;
                while (1) {
                    if ((*terms)[i] == '(') ++cnt;
                    else if ((*terms)[i] == ')') --cnt;

                    if (cnt == 0) break;
                    ++i;
                }
                strncpy(curFun, *terms, i + 1);
                curFun[i + 1] = 0;
                string convertedTerm = convertDescription(curFun, (*defunItr).second); 
                if (convertedTerm == string("")) return NULL;
                
                // term - 1 에서부터 길이 i + 1만큼 convertedTerm으로 치환한다
                char after[BUFFER_SIZE];
                strcpy(after, *terms + i + 1);
                sprintf(*terms, "%s%s", convertedTerm.c_str(), after);

                return termToNode(terms);
            } else { // 정의되지 않은 함수일 때
                newNode->type = FUN[UNDEFINED];
                undefinedFunctionFlag = true;
            }

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
        } else {
            printf("정의되지 않은 함수입니다.");
            return NULL;
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

string convertDescription(const char *term, const char* description) {
    char usrParamBuf[BUFFER_SIZE];
    char desParamBuf[BUFFER_SIZE];
    //char desBuf[BUFFER_SIZE];
    char resultBuf[BUFFER_SIZE];
    char* params;
    char paramsBuf[BUFFER_SIZE];
    char* nextUsrParam;
    char* nextDesParam = NULL;
    char* des;
    string desStr;
    strcpy(usrParamBuf, term);
    memset(paramsBuf, 0, BUFFER_SIZE);
    int i = -1;
    int pCnt = 0;
    while (1) {
        ++i;
        if (usrParamBuf[i] == '(') ++pCnt;
        else if (usrParamBuf[i] == ')') --pCnt;
        else continue;

        if (pCnt == 0) break;
    }
    strncpy(paramsBuf, usrParamBuf, i);
    params = paramsBuf;
    i = 0;
    int desParamLen;
    while (params[i] != ' ') ++i;
    while (params[i] == ' ') ++i;
    params += i;

    strcpy(desParamBuf, description + 1);
    strtok(desParamBuf, ")");
    desParamLen = strlen(desParamBuf);
    des = desParamBuf + strlen(desParamBuf) + 2;
    desStr = des;

    if (params[0] == '(') {
        char curParamBuf[BUFFER_SIZE];
        char* lastCharPtr = params + strlen(params);
        nextDesParam = strtok(desParamBuf, " ");
        while (params < lastCharPtr) {
            i = -1;
            pCnt = 0;
            while (1) {
                ++i;
                if (params[i] == '(') ++pCnt;
                else if (params[i] == ')') --pCnt;
                else continue;

                if (pCnt == 0) break;
            }
            strncpy(curParamBuf, params, i + 1);
            curParamBuf[i + 1] = 0;
            params += (i + 1);
            i = 0;
            while (params[i] != ' ') ++i;
            while (params[i] == ' ') ++i;
            params += i;

            if (nextDesParam == NULL) break;
            string paramstr = string(nextDesParam);
            int paramLen = strlen(nextDesParam);
            size_t position;

            while ((position = desStr.find(paramstr, 0)) != string::npos) {
                desStr.replace(position, paramLen, curParamBuf);
            }

            paramstr = "(" + string(nextDesParam);
            while ((position = desStr.find(paramstr, 0)) != string::npos) {
                desStr.replace(position, paramLen + 1, "(" + string(curParamBuf));
            }

            paramstr = string(nextDesParam) + ")";
            while ((position = desStr.find(paramstr, 0)) != string::npos) {
                desStr.replace(position, paramLen + 1, string(curParamBuf) + ")");
            }

            nextDesParam = strtok(NULL, " ");
        }

    } else {
        nextDesParam = strtok(desParamBuf, " ");
        while (nextDesParam != NULL && nextDesParam < desParamBuf + desParamLen) {
            string paramstr = string(nextDesParam);
            int paramLen = strlen(nextDesParam);
            size_t position;

            params = strtok(params, " ");
            if (params == NULL) return string("");
            string usrparamstr = params;
            params += strlen(params) + 1;

            while ((position = desStr.find(paramstr, 0)) != string::npos) {
                desStr.replace(position, paramLen, usrparamstr);
            }

            paramstr = "(" + string(nextDesParam);
            while ((position = desStr.find(paramstr, 0)) != string::npos) {
                desStr.replace(position, paramLen + 1, "(" + usrparamstr);
            }

            paramstr = string(nextDesParam) + ")";
            while ((position = desStr.find(paramstr, 0)) != string::npos) {
                desStr.replace(position, paramLen + 1, usrparamstr + ")");
            }

            nextDesParam += strlen(nextDesParam) + 1;
            nextDesParam = strtok(nextDesParam, " ");
        }
    }

    



    //cout << desStr << endl;

    return desStr;
}