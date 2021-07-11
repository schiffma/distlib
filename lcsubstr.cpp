// Lorenz Schiffmann, July 2021

// Space optimized CPP implementation to print
// longest common substring.
#include <string>
#include <vector>
#include "utf8_unicode.hpp"
#include "pylcs.hpp"

using std::string;
using std::vector;
using std::max;

// Function to find longest common subsequence.
string lcstr(const string &str1, const string &str2)
{
    if (str1 == "" || str2 == "")
        return "";	
    structRet ret;
	ret = lcs2_length_(str1, str2);
    vector<string> str1v = utf8_split(str1);
	return vect2str(slice(str1v, ret.start, ret.start + ret.max - 1));
}
 

// modified version from 
//https://www.geeksforgeeks.org/printing-longest-common-subsequence/
 
// Function to find longest common subsequence
/* Returns length of LCS for X[0..m-1], Y[0..n-1] */
string lcseq( const string &X, const string &Y)
{
   if (X == "" || X == "")
        return "";	
   int m = 0;
   int n = 0;	
   vector<string> X_= utf8_split(X);	
   vector<string> Y_= utf8_split(Y);
   m = X_.size();
   n = Y_.size();
   //int L[m+1][n+1];
   vector<vector<int>> L(m + 1, vector<int>(n + 1));
  
   /* Following steps build L[m+1][n+1] in bottom up fashion. Note
      that L[i][j] contains length of LCS of X[0..i-1] and Y[0..j-1] */
   for (int i=0; i<=m; i++)
   {
     for (int j=0; j<=n; j++)
     {
       if (i == 0 || j == 0)
         L[i][j] = 0;
       else if (X_[i-1] == Y_[j-1])
         L[i][j] = L[i-1][j-1] + 1;
       else
         L[i][j] = max(L[i-1][j], L[i][j-1]);
     }
   }
  
   // Following code is used to print LCS
   int index = L[m][n];
  
   // Create a character array to store the lcs string
   //string lcs[index+1];
   vector<string> lcs(index);
   //lcs[index] = '\0'; // Set the terminating character
  
   // Start from the right-most-bottom-most corner and
   // one by one store characters in lcs[]
   int i = m, j = n;
   while (i > 0 && j > 0)
   {
      // If current character in X[] and Y are same, then
      // current character is part of LCS
      if (X_[i-1] == Y_[j-1])
      {
          lcs[index-1] = X_[i-1]; // Put current character in result
          i--; j--; index--;     // reduce values of i, j and index
      }
  
      // If not same, then find the larger of two and
      // go in the direction of larger value
      else if (L[i-1][j] > L[i][j-1])
         i--;
      else
         j--;
   }
  
   // Print the lcs
   //cout << "LCS of " << X << " and " << Y << " is " << lcs << endl;
   return vect2str(lcs);
}


