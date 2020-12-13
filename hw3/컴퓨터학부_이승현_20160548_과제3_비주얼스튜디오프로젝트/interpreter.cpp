#include "interpreter.h"
#include "compiler.h"

void interpreter(char* terms) { // 인터프리터의 인자는 후위연산식으로 변환된 명령어
    int operandA; // 첫번째 피연산자
    int operandB; // 두번째 피연산자
    int result; // 연산 결과
    char* term;
    stack<int> operands; // 후위연산식 계산을 위해 피연산자들 담아놓는 스택

    if (strstr(terms, FUN[UNDEFINED]) != NULL) { // 명령어에 UNDEFINED라는 단어가 들어있다면
        cout << "undefined"; // undefined 출력
        return; // 종료
    }

    term = strtok(terms, " "); // 첫번째 토큰을 가져온다
    while (true) {
        if (term == NULL) break; // 더이상 남은 토큰이 없다면 반복 그만

        if (isInteger(term)) { // 가져온 토큰이 숫자라면
            int num = atoi(term); // 문자열을 정수로 변환
            operands.push(num); // 스택에 push
        } else { // 숫자가 아니라면
            // 스택에서 피연산자 두개를 꺼낸다
            operandB = operands.top();
            operands.pop();
            operandA = operands.top();
            operands.pop();

            if (!strcmp(term, FUN[MINUS])) { // MINUS 함수인 경우
                result = operandA - operandB;
            } else if (!strcmp(term, FUN[IF])) { // IF 함수인 경우
                if (operandA > 0) result = operandB;
                else result = 0;
            } else {

            }

            operands.push(result); // 연산 결과 stack에 넣음
        }

        term = strtok(NULL, " "); // 다음 토큰 꺼냄
    }

    result = operands.top();
    cout << result;
}

