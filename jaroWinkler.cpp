// modified uf8-supported version of 
// https://github.com/TriviaMarketing/Jaro-Winkler

#include <string>
#include <vector>
#include <algorithm>

#include "jaroWinkler.hpp"
#include "utf8_unicode.hpp"

using std::string;
using std::vector;
using std::min;
using std::max;

double jaroDistance(const string& a, const string& b)
{
    // Register strings length.
    //int aLength(a.size());
    //int bLength(b.size());
	// Register strings length utf8 compatible
    vector<string> a_ = utf8_split(a);
    vector<string> b_ = utf8_split(b);
    int aLength = a_.size();
    int bLength = b_.size();
	
    
    // If one string has null length, we return 0.
    if (aLength == 0 || bLength == 0)
    {
        return 0.0;
    }
    
    // Calculate max length range.
    int maxRange(max(0, max(aLength, bLength) / 2 - 1));
    
    // Creates 2 vectors of integers.
    vector<bool> aMatch(aLength, false);
    vector<bool> bMatch(bLength, false);
    
    // Calculate matching characters.
    int matchingCharacters(0);
    for (int aIndex(0); aIndex < aLength; ++aIndex)
    {
        // Calculate window test limits (limit inferior to 0 and superior to bLength).
        int minIndex(max(aIndex - maxRange, 0));
        int maxIndex(min(aIndex + maxRange + 1, bLength));
        
        if (minIndex >= maxIndex)
        {
            // No more common character because we don't have characters in b to test with characters in a.
            break;
        }
        
        for (int bIndex(minIndex); bIndex < maxIndex; ++bIndex)
        {
            if (!bMatch.at(bIndex) && a_.at(aIndex) == b_.at(bIndex))
            {
                // Found some new match.
                aMatch[aIndex] = true;
                bMatch[bIndex] = true;
                ++matchingCharacters;
                break;
            }
        }
    }
    
    // If no matching characters, we return 0.
    if (matchingCharacters == 0)
    {
        return 0.0;
    }
    
    // Calculate character transpositions.
    vector<int> aPosition(matchingCharacters, 0);
    vector<int> bPosition(matchingCharacters, 0);
    
    for (int aIndex(0), positionIndex(0); aIndex < aLength; ++aIndex)
    {
        if (aMatch.at(aIndex))
        {
            aPosition[positionIndex] = aIndex;
            ++positionIndex;
        }
    }
    
    for (int bIndex(0), positionIndex(0); bIndex < bLength; ++bIndex)
    {
        if (bMatch.at(bIndex))
        {
            bPosition[positionIndex] = bIndex;
            ++positionIndex;
        }
    }
    
    // Counting half-transpositions.
    int transpositions(0);
    for (int index(0); index < matchingCharacters; ++index)
    {
        if (a_.at(aPosition.at(index)) != b_.at(bPosition.at(index)))
        {
            ++transpositions;
        }
    }
    
	// Clear the vectors
	//aMatch.clear();
	//bMatch.clear();	
	
    // Calculate Jaro distance.
    return (
        JARO_WEIGHT_STRING_A * matchingCharacters / aLength +
        JARO_WEIGHT_STRING_B * matchingCharacters / bLength +
        JARO_WEIGHT_TRANSPOSITIONS * (matchingCharacters - transpositions / 2) / matchingCharacters
    );
}





double jaroWinklerDistance(const string&a, const string& b)
{
    // Calculate Jaro distance.
    double distance(jaroDistance(a, b));

    vector<string> a_ = utf8_split(a);
    vector<string> b_ = utf8_split(b);
    
    if (distance > JARO_WINKLER_BOOST_THRESHOLD)
    {
        // Calculate common string prefix.
        int commonPrefix(0);
        for (int index(0), indexEnd(min(min(size_t(a_.size()),size_t(b_.size())), size_t(JARO_WINKLER_PREFIX_SIZE))); index < indexEnd; ++index)			
        {
            if (a_.at(index) == b_.at(index))
            {
                ++commonPrefix;
            }
            else
            {
                break;
            }
        }
        
        // Calculate Jaro-Winkler distance.
        distance += JARO_WINKLER_SCALING_FACTOR * commonPrefix * (1.0 - distance);
    }
    
    return distance;
}


