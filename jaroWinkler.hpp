// https://github.com/TriviaMarketing/Jaro-Winkler

#ifndef JAROWINKLER_HPP
#define JAROWINKLER_HPP

#include <string>

const double JARO_WEIGHT_STRING_A(1.0/3.0);
const double JARO_WEIGHT_STRING_B(1.0/3.0);
const double JARO_WEIGHT_TRANSPOSITIONS(1.0/3.0);

const unsigned long int JARO_WINKLER_PREFIX_SIZE(4);
const double JARO_WINKLER_SCALING_FACTOR(0.1);
const double JARO_WINKLER_BOOST_THRESHOLD(0.7);



double jaroDistance(const std::string& a, const std::string& b);
double jaroWinklerDistance(const std::string&a, const std::string& b);



#endif // JAROWINKLER_HPP

