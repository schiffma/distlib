// L. Schiffmann, July 2021

#ifndef PYLCS_HPP
#define PYLCS_HPP

#include <string>

// L. Schiffmann, July 2021
// struct for max length and start position

struct structRet {
   int max;
   int start;
};

int lcs_length_(const std::string &str1, const std::string &str2);
structRet lcs2_length_(const std::string &str1, const std::string &str2);

int levenshtein_distance(const std::string &str1, const std::string &str2);

#endif