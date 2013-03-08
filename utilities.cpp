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
	stringstream ss(str);

	string newStr = cleanSpecialChars(str);

	string word;
	while(ss) {
		ss >> word; 
		words.push_back(word);		
	}

	return words;
}


#endif
