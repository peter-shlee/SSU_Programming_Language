#ifndef __COMPILER_H__
#define __COMPILER_H__
#include "main.h"
#include <string>
#include <iostream>

#define BUFFER_SIZE 1024

bool compile(char* terms);
bool isWrongCharacter(char c);
char* removeLeftWhitespace(char* terms);
void removeRightWhitespace(char* terms);
termNode* termToNode(char** terms);
bool checkTermsIncludingWrongCharacter(char* terms, char *wrongChar);
bool isInteger(char* term);
void printAllTerms(termNode* node, FILE* fp);
bool saveIntermediateCode(termNode* node);
void freeAllNode(termNode* node);
bool checkInvalidNumber(char* term);
bool checkParenthesis(char* terms);
string convertDescription(const char* term, const char* description);

#endif
