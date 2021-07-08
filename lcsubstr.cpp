// modified uf8-supported version of 
// https://www.geeksforgeeks.org/print-longest-common-substring/

// Space optimized CPP implementation to print
// longest common substring.
#include <string>
#include <vector>
#include "utf8_unicode.hpp"

using std::string;
using std::vector;
using std::to_string;
using std::max;
 
// Function to find longest common substring.
string lcstr(const string &X, const string &Y)
{
    // If a string is empty then return empty string
    if (X == "" || Y == "") return "";
    // Find length of both the strings.
    //int m = X.length();
    //int n = Y.length();

	vector<string> X_ = utf8_split(X);
    vector<string> Y_ = utf8_split(Y);
    int m = X_.size();
    int n = Y_.size();		
	
    int end = 0;
	int result = 0;
 
    // Longest common substring is from index
    // end - result + 1 to index end in X.
    //return X.substr(end - result + 1, result);
	return to_string(end - result + 1) + "/" + to_string(end);
	//return vect2str(slice(X_,end - result + 1, end));
}
 
