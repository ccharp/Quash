#ifndef UTILITES_H
#define UTILITIES_H

#include <string>
#include <vector>

using namespace std;

string cleanSpecialChars(const string str); 

vector<string> tokenize(const string str, const char delimeter);

void printStrVector(vector<string> words, string message);

char **argify(const vector<string> tokens, char **args); 

#endif
