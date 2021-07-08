#include <string>
#include <vector>

#include "utf8_unicode.hpp"

using std::string;
using std::vector;

// https://github.com/Meteorix/pylcs

vector<string> utf8_split(const string &str){
    vector<string> split;
    int len = str.length();
    int left = 0;
    int right = 1;

    for (int i = 0; i < len; i++){
        if (right >= len || ((str[right] & 0xc0) != 0x80)){
            string s = str.substr(left, right - left);
            split.push_back(s);
            // printf("%s %d %d\n", s.c_str(), left, right);
            left = right;
        }
        right ++;
    }
    return split;
}

// L. Schiffmann, July 2021
// This function returns the true size of a string in any utf8 case
int utf8_length(const string &str){
    return utf8_split(str).size();
}

// L. Schiffmann, July 2021
string vect2str(const vector<string> &v) {
  string s="";
   for (auto e: v){
     s+=e;
   }
   return s;
}

// L. Schiffmann, July 2021
// https://www.techiedelight.com/get-slice-sub-vector-from-vector-cpp/
vector<string> slice(vector<string> const &v, int m, int n)
{
    auto first = v.cbegin() + m;
    auto last = v.cbegin() + n + 1;    
    std::vector<string> vec(first, last);
    return vec;
}

