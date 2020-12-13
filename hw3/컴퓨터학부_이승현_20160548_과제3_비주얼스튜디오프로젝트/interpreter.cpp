#include "interpreter.h"
#include "compiler.h"

void interpreter(char* terms) { // ������������ ���ڴ� ������������� ��ȯ�� ��ɾ�
    int operandA; // ù��° �ǿ�����
    int operandB; // �ι�° �ǿ�����
    int result; // ���� ���
    char* term;
    stack<int> operands; // ��������� ����� ���� �ǿ����ڵ� ��Ƴ��� ����

    if (strstr(terms, FUN[UNDEFINED]) != NULL) { // ��ɾ UNDEFINED��� �ܾ ����ִٸ�
        cout << "undefined"; // undefined ���
        return; // ����
    }

    term = strtok(terms, " "); // ù��° ��ū�� �����´�
    while (true) {
        if (term == NULL) break; // ���̻� ���� ��ū�� ���ٸ� �ݺ� �׸�

        if (isInteger(term)) { // ������ ��ū�� ���ڶ��
            int num = atoi(term); // ���ڿ��� ������ ��ȯ
            operands.push(num); // ���ÿ� push
        } else { // ���ڰ� �ƴ϶��
            // ���ÿ��� �ǿ����� �ΰ��� ������
            operandB = operands.top();
            operands.pop();
            operandA = operands.top();
            operands.pop();

            if (!strcmp(term, FUN[MINUS])) { // MINUS �Լ��� ���
                result = operandA - operandB;
            } else if (!strcmp(term, FUN[IF])) { // IF �Լ��� ���
                if (operandA > 0) result = operandB;
                else result = 0;
            } else {

            }

            operands.push(result); // ���� ��� stack�� ����
        }

        term = strtok(NULL, " "); // ���� ��ū ����
    }

    result = operands.top();
    cout << result;
}

