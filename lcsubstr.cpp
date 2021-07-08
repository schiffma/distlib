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
	
 
    // Variable to store length of longest
    // common substring.
    int result = 0;
 
    // Variable to store ending point of
    // longest common substring in X.
    int end;
 
    // Matrix to store result of two
    // consecutive rows at a time.
    //int len[2][n];
	int dim = max(n,m);
	vector<vector<int>> len(2, vector<int>(dim));
 
    // Variable to represent which row of
    // matrix is current row.
    int currRow = 0;
 
    // For a particular value of i and j,
    // len[currRow][j] stores length of longest
    // common substring in string X[0..i] and Y[0..j].
    for (int i = 0; i <= m; i++) {
        for (int j = 0; j <= n; j++) {
            if (i == 0 || j == 0) {
                len[currRow][j] = 0;
            }
            else if (X_[i - 1] == Y_[j - 1]) {
                len[currRow][j] = len[1 - currRow][j - 1] + 1;
                if (len[currRow][j] > result) {
                    result = len[currRow][j];
                    end = i - 1;
                }
            }
            else {
                len[currRow][j] = 0;
            }
        }
 
        // Make current row as previous row and
        // previous row as new current row.
        currRow = 1 - currRow;
    }
 
    // If there is no common substring, print empty string.
    if (result == 0) {
        return "";
    }
 
    // Longest common substring is from index
    // end - result + 1 to index end in X.
    //return X.substr(end - result + 1, result);
	//return to_string(end - result + 1) + "/" + to_string(end);
	return vect2str(slice(X_,end - result + 1, end));
}
 
