#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <set>
#include <list>
#include <sstream>


using std::string;
using std::vector;
using std::map;
using std::set;
using std::list;


class LevenshteinDistanceCalculator
{
public:
    LevenshteinDistanceCalculator(const string & sourceString, const string & destinationString):
        _sourceString(sourceString),
        _destinationString(destinationString)
    {}

    //typedef unsigned DistanceVectorType;

    unsigned computeDistance() const;

private:
    string  _sourceString;
    string  _destinationString;
};

template <typename T>
const T & minimum(const T & _1, const T & _2, const T & _3)
{
    return std::min(std::min(_1, _2), _3);
}


typedef vector<int> introw;
typedef vector<introw> intmatrix;


string to_string(const intmatrix & matrix)
{
    std::stringstream aux_ss;

    for (size_t i = 0; i < matrix.size(); ++i)
    {
        for (size_t j = 0; j < matrix.begin()->size(); ++j)
        {
           aux_ss << matrix[i][j] << " ";
        }
        
        aux_ss << "\n";
}

    return aux_ss.str();
}


unsigned LevenshteinDistanceCalculator::computeDistance() const
{
    // initializing prefixes matrix

    unsigned sourceLength = _sourceString.size();
    unsigned destinationLength = _destinationString.size();

    typedef vector<int> introw;
    typedef vector<introw> intmatrix;
    intmatrix prefixesDistances(sourceLength + 1, introw(destinationLength + 1, 0));

    // Source prefixes can be transformed to empty string by dropping all characters
    for (unsigned i = 0; i <= sourceLength; ++i)
    {
       prefixesDistances.at(i).at(0) = i;
    }

    // Target prefixed can be inserted from empty source prefix by inseting every characters
    for (unsigned j = 0; j <= destinationLength; ++j)
    {
        prefixesDistances.at(0).at(j) = j;
    }

    unsigned i, j;

    try 
    {
        for (i = 1; i <= sourceLength; ++i)
        {
            for (j = 1; j <= destinationLength; ++j)
            {
                if (_sourceString.at(i-1) == _destinationString.at(j-1))
                {
                    prefixesDistances[i][j] = prefixesDistances[i-1][j-1];
                }
                else
                {
                    auto deletion = prefixesDistances.at(i-1).at(j) + 1;
                    auto insertion = prefixesDistances.at(i).at(j-1) + 1;
                    auto modification = prefixesDistances.at(i-1).at(j-1) + 1;
                    
                    prefixesDistances.at(i).at(j) = minimum(deletion, insertion, modification);
                }
            }
        }
    }
    catch(...)
    {
        std::cout << "FAULT: i=" << i << ", j=" << j << std::endl;

        throw;
    }

    #ifndef NDEBUG
        std::cout << to_string(prefixesDistances);
    #endif

    return prefixesDistances.at(sourceLength).at(destinationLength);
}


////////////////////////////////////////////////////////////////////////////////

int main(int argc, char const *argv[])
{
    string source, target;

    std::cin >> source;
    std::cin >> target;

    LevenshteinDistanceCalculator stringsDifferenceCalculator(source, target);

    auto solution = stringsDifferenceCalculator.computeDistance();

    std::cout << "Solution is: " << solution << std::endl;
}

