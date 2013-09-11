#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <set>
#include <list>
#include <sstream>
#include <cassert>


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
    const T & minimum(const T & _1, const T & _2, const T & _3, unsigned * p_argNumber=nullptr)
    {
        #define return_statement(number, val) {if ((p_argNumber) != nullptr) *(p_argNumber) = (number); return (val); }

        if (_1 < _2) 
        {
            if (_1 < _3) 
            {
                return_statement(1, _1);
            }
        }
        else
        {
            if (_2 < _3)
            {
                return_statement(2, _2);
            }
        }

        return_statement(3, _3);
        
        #undef return_statement
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

    if (sourceLength == destinationLength && 
        _sourceString == _destinationString)
    {
        // 0
        return;
    }

    if (sourceLength == 0)
    {
        // destinationLength
        return;
    }

    if (destinationLength == 0)
    {
        // sourceLength
        return;
    }

    vector<vector<int> > v;
    v.push_back(vector<int>(destinationLength + 1, 0));
    v.push_back(vector<int>(destinationLength + 1, 0));


    for (size_t i = 0; i <= destinationLength; ++i)
    {
        v[0].at(i) = i;
    }

    size_t idx = 0;
    vector<int> * p_currentV1 = &v[idx];
    vector<int> * p_currentV2 = &v[idx+1];

    for (size_t i = 0; i < sourceLength; ++i)
    {
        p_currentV2->at(0) = i + 1;
        
        for (size_t j = 0; j < destinationLength; ++j)
        {

            if (_sourceString.at(i) == _destinationString.at(j))
            {
                p_currentV2->at(j+1) = p_currentV1->at(j);
            }
            else
            {
                auto insertion      = p_currentV2->at(j) + 1;
                auto deletion       = p_currentV1->at(j+1) + 1;            
                auto modification   = p_currentV1->at(j) + 1;
     
                unsigned minArgument;
                p_currentV2->at(j+1) = stlext::minimum(insertion, deletion, modification, &minArgument);

                switch(minArgument)
                {
                    case 1:
                    {
                        std::cout << "insertion" << std::endl;
                        break;
                    }
                    case 2:
                    {
                        std::cout << "deletion" << std::endl;
                        break;
                    }
                    case 3:
                    {
                        std::cout << "modification" << std::endl;
                        break;
                    }
                }                

            }
            
            
            // if ((p_currentV1->at(j) != p_currentV2->at(j+1)))
            // {
            // }
        }

        assert(p_currentV1->size() == p_currentV2->size());
        std::copy(p_currentV2->begin(), p_currentV2->end(), p_currentV1->begin());
    }

    std::cout << "Solution is: " << p_currentV2->at(destinationLength) << std::endl;
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

