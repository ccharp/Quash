#ifndef UTILS_CPP
#define UTILS_CPP

#include <string>
#include <iostream>
#include <vector>
#include <sstream>

using namespace std;

// Pads each special character with white space 
// for tokenizing.
string cleanSpecialChars(const string str) {
	string newStr;

	for(char c : str) {
		if('|' == c || '>' == c || '<' == c || '&' == c) {
			newStr.push_back(' ');
			newStr.push_back(c);
			newStr.push_back(' ');
		} else {
			newStr.push_back(c);	
		}	
	}

	return newStr;
}

vector<string> tokenize(const string str, const char delimeter) {

	vector<string> words;

	string cleanedStr = cleanSpecialChars(str);
	stringstream ss(cleanedStr);

	string word;
	while(ss) {
		ss >> word; 
		words.push_back(word);		
	}

	return words;
}

// Converts tokens in a C style argument array, argv. 
char **argify(const vector<string> tokens, char **args) {
	int argc = tokens.size(); 
	args = new char*[argc + 1];

	for(unsigned int i = 0; i < argc; i++) {
		args[i] = new char[tokens[i].length() + 1];
		strcpy(args[i], words[i].c_str());
		args[i][words[i].length()] = '\0';	
	}

	return args;
}


#endif
