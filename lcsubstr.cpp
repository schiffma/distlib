// modified uf8-supported version of 
// https://www.geeksforgeeks.org/print-longest-common-substring/

// Space optimized CPP implementation to print
// longest common substring.
#include <string>
#include <vector>
#include "utf8_unicode.hpp"
#include "pylcs.hpp"

using std::string;
using std::vector;
using std::to_string;
using std::max;

 
// Function to find longest common substring.
string lcstr(const string &str1, const string &str2)
{
    if (str1 == "" || str2 == "")
        return "";	
    structRet ret;
	ret = lcs2_length_(str1, str2);
    vector<string> str1v = utf8_split(str1);
	return vect2str(slice(str1v, ret.start, ret.start + ret.max - 1));
}
 
