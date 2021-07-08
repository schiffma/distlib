#ifndef UTF8_UNICODE_HPP
#define UTF8_UNICODE_HPP

#include <string>
#include <vector>

std::vector<std::string> utf8_split(const std::string &str);
int utf8_length(const std::string &str);
std::string vect2str(const std::vector<std::string> &v);
std::vector<std::string> slice(std::vector<std::string> const &v, int m, int n);

#endif