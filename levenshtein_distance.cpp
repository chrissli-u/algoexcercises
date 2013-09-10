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


namespace stlext 
{
    template <class T>
    class matrix
    {
    private:
        class vector_wrapper
        {   
        public:
            typedef std::vector<T> row_storage_type;

            vector_wrapper(size_t size, const T & initializer_value):
                _storage(size, initializer_value)
            {}

            const T & operator[](size_t index) const { return _storage[index]; }
            T & operator[](size_t index) { return _storage[index]; }
            
            const T & at(size_t index) const { return _storage.at(index); }
            T & at(size_t index) { return _storage.at(index); }
        
        private:
            row_storage_type _storage;
        };

        typedef vector_wrapper row_storage_type;
        typedef std::vector<row_storage_type> matrix_storage_type;
    
    public:
        typedef row_storage_type matrix_row_type;

    public:
        matrix(size_t rows, size_t columns):
            _matrixStorage(rows, row_storage_type(columns, T()) ),
            _rows(rows),
            _columns(columns)
        {}

        explicit matrix(size_t dimension):
            _matrixStorage(dimension, row_storage_type(dimension, T()) ),
            _rows(dimension),
            _columns(dimension)
        {}
    
    public:
        const T & at(size_t row, size_t column) const { return _matrixStorage.at(row).at(column); }
        T & at(size_t row, size_t column) { return _matrixStorage.at(row).at(column); }

        const matrix_row_type & operator[](size_t index) const { return _matrixStorage[index]; }
        matrix_row_type & operator[](size_t index) { return _matrixStorage[index]; }

        size_t columns() const { return _columns; }
        size_t rows() const { return _rows; }

    private:
        matrix_storage_type     _matrixStorage;
        size_t                  _rows;
        size_t                  _columns;

    private:
        // This may look like not cannonical way for overloading stream output operator, but 
        // there are pitfalls in case of template paramter. The problem is that it requires predeclaration and 
        // then using strange syntax in friend declaration. 
        // For details, please see (http://www.parashift.com/c++-faq-lite/template-friends.html)
        friend std::ostream & operator << (std::ostream & streamInstance, const stlext::matrix<T> & matrixObject)
        {
            for (size_t i = 0; i < matrixObject.rows(); ++i)
            {
                for (size_t j = 0; j < matrixObject.columns(); ++j)
                {
                   streamInstance << matrixObject[i][j] << " ";
                }
                
                streamInstance << "\n";
            }

            return streamInstance;

        }
    };


    // TODO: extend it using Variadic Templates
    template <typename T>
    const T & minimum(const T & _1, const T & _2, const T & _3)
    {
        return std::min(std::min(_1, _2), _3);
    }
}


class LevenshteinDistanceCalculator
{
public:
    LevenshteinDistanceCalculator(const string & sourceString, const string & destinationString):
        _sourceString(sourceString),
        _destinationString(destinationString),
        _distanceMatrix(sourceString.size() + 1, destinationString.size() + 1)
    {}

    void CalculateMatrix();

private:
    typedef stlext::matrix<int> MatrixStorage;

private:
    const MatrixStorage & GetDistanceMatrix() const { return _distanceMatrix; }
    MatrixStorage & GetDistanceMatrix() { return _distanceMatrix; }

private:
    string          _sourceString;
    string          _destinationString;
    MatrixStorage   _distanceMatrix;
};


void LevenshteinDistanceCalculator::CalculateMatrix()
{
    // initializing prefixes matrix

    size_t sourceLength = _sourceString.size();
    size_t destinationLength = _destinationString.size();

    auto & distancesMatrix = GetDistanceMatrix();

    // Source prefixes can be transformed to empty string by dropping all characters
    for (size_t i = 0; i <= sourceLength; ++i)
    {
       distancesMatrix.at(i, 0) = i;
    }

    // Target prefixed can be inserted from empty source prefix by inseting every characters
    for (size_t j = 0; j <= destinationLength; ++j)
    {
        distancesMatrix.at(0, j) = j;
    }

    for (size_t i = 1; i <= sourceLength; ++i)
    {
        for (size_t j = 1; j <= destinationLength; ++j)
        {
            if (_sourceString.at(i-1) == _destinationString.at(j-1))
            {
                distancesMatrix[i][j] = distancesMatrix[i-1][j-1];
            }
            else
            {
                auto deletion = distancesMatrix.at(i - 1, j) + 1;
                auto insertion = distancesMatrix.at(i, j - 1) + 1;
                auto modification = distancesMatrix.at(i - 1, j - 1) + 1;
                
                distancesMatrix.at(i, j) = stlext::minimum(deletion, insertion, modification);
            }
        }
    }

    #ifndef NDEBUG
        std::cout << distancesMatrix << std::endl;
    #endif 

    std::cout << "Solution is: " << distancesMatrix.at(sourceLength, destinationLength) << std::endl;
}


////////////////////////////////////////////////////////////////////////////////

int main(int argc, char const *argv[])
{
   string source, target;

   std::cin >> source;
   std::cin >> target;

   LevenshteinDistanceCalculator stringsDifferenceCalculator(source, target);

   stringsDifferenceCalculator.CalculateMatrix();

   //std::cout << "Solution is: " << solution << std::endl;
}

