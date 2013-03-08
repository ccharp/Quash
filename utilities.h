#include <string>
#include <vector>

using namespace std;

string cleanSpecialChars(const string str); 

vector<string> tokenize(const string str, const char delimeter);

char **argify(const vector<string> tokens, char **args); 
