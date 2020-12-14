// Developer: Wilbert (wilbert.phen@gmail.com)

#include <cmath>
#include <vector>
#include <string>
#include <iterator>
#include <algorithm>

#include "sparse.hpp"

/**
 * Default Constructor
 * Setting the data into identity matrx with specified dimension
**/
template<typename fT>
SparseMatrix<fT>::SparseMatrix(size_t nrow, size_t ncol, bool identity)
    : m_nrow(nrow), m_ncol(ncol)
{
    reset(identity);
}

/**
 * Copy Contructor
 * Setting the data into copied object
**/
template<typename fT>
SparseMatrix<fT>::SparseMatrix(SparseMatrix<fT> const & other)
    : m_nrow(other.m_nrow), m_ncol(other.m_ncol),
      m_index(other.m_index.size()),
      m_indices(other.m_indices.size()),
      m_data(other.m_data.size())
{
    m_index = other.m_index;
    m_indices = other.m_indices;
    m_data = other.m_data;
}

/**
 * Move Constructor
 * Swapping data content of two matrix
**/
template<typename fT>
SparseMatrix<fT>::SparseMatrix(SparseMatrix<fT> && other)
    : m_nrow(other.m_nrow), m_ncol(other.m_ncol),
      m_index(other.m_index.size()),
      m_indices(other.m_indices.size()),
      m_data(other.m_data.size())
{
    other.m_index.swap(m_index);
    other.m_indices.swap(m_indices);
    other.m_data.swap(m_data);
}

/**
 * Copy Constuctor
 * Init Sparse Matrix with vector of vector
**/
template<typename fT>
SparseMatrix<fT>::SparseMatrix(std::vector<std::vector<fT>> const & other, size_t nrow, size_t ncol)
    : m_nrow(nrow), m_ncol(ncol)
{
    // Create the matrix with zero value at all location
    reset(false);

    for(size_t i=0; i<m_nrow; ++i)
    {
        for(size_t j=0; j<m_ncol; ++j)
        {
            // Set the value using the () operator
            (*this)(i, j, other[i][j]);
        }
    }
}

/*
 * Reset the content of matrix to initial state
*/
template<typename fT>
void SparseMatrix<fT>::reset(bool identity)
{
    m_index.clear();
    m_indices.clear();
    m_data.clear();

    m_index.reserve(m_nrow+1);
    m_indices.reserve(m_nrow);
    m_data.reserve(m_nrow);

    if(identity)
    {
        for(size_t i=0; i<m_nrow; ++i)
        {
            m_index.push_back(i);
            m_indices.push_back(i);
            m_data.push_back(static_cast<fT>(1.));
        }
        m_index.push_back(m_nrow);
    }
    else
        for(size_t i=0; i<=m_nrow; ++i)
            m_index.push_back(static_cast<size_t>(0));
}

/*
 * Accessor for matrix elements
 * @param nrow matrix row
 * @param ncol matrix column
 * @return value at the specified matrix row and column
*/
template<typename fT>
fT SparseMatrix<fT>::operator() (size_t nrow, size_t ncol) const
{
    const size_t j = findIndex(nrow, ncol);
    if (j < m_index.at(nrow+1))
        return m_data.at(j);
    else
        return static_cast<fT>(0.);
}

/*
 * Mutator for matrix elements
 * @param nrow matrix row
 * @param ncol matrix column
 * @return reference to the value at the specified matrix row and column
*/
template<typename fT>
void SparseMatrix<fT>::operator() (size_t nrow, size_t ncol, fT value)
{
    const size_t j = findIndex(nrow, ncol);
    // If the location is found to be non-zero
    if(j < m_index.at(nrow+1))
    {
        // If the data value is non-zero
        if (fabs(value) > eps_)
        {
            m_data.at(j) = value;
        }
        // If the data value is zero, then remove the existing entry
        else
        {
            for(size_t i=nrow+1; i<=this->nrow(); ++i) --m_index.at(i);
            m_indices.erase(m_indices.begin() + j);
            m_data.erase(m_data.begin() + j);
        }
    }
    // If the location has zero value
    else
    {
        if (fabs(value) > eps_)
        {
            for(size_t i=nrow+1; i<=this->nrow(); ++i) ++m_index.at(i);
            m_indices.insert(m_indices.begin() + m_index.at(nrow + 1) - 1, ncol);
            m_data.insert(m_data.begin() + m_index.at(nrow + 1) - 1, value);
        }
    }
}

/*
 * Validate Multiplication
 * Calculate if the multiplication can be done
*/
template<typename fT>
void validate_multiplication(const SparseMatrix<fT> &mat1, const SparseMatrix<fT> &mat2)
{
    if (mat1.m_ncol != mat2.m_nrow)
    {
        throw std::out_of_range(
            "the number of first matrix column "
            "differs from that of second matrix row");
    }
}

/*
 * Check matrix size similarity
 * Calculate if the matrix size is the same
*/
template<typename fT>
void same_size(const SparseMatrix<fT> &mat1, const SparseMatrix<fT> &mat2)
{
    if (mat1.m_nrow != mat2.m_nrow || mat1.m_ncol != mat2.m_ncol)
    {
        throw std::out_of_range(
            "the dimension of first matrix "
            "differs from that of second matrix");
    }
}

/*
 * Equality Comparison
*/
template<typename fT>
bool SparseMatrix<fT>::operator== (SparseMatrix<fT> const & other)
{
    if (this == &other) return true;
    if (m_nrow != other.m_nrow || m_ncol != other.m_ncol) return false;
    if (m_index != other.m_index) return false;
    if (m_indices != other.m_indices) return false;
    if (m_data != other.m_data) return false;
    return true;
}
template<typename fT>
bool SparseMatrix<fT>::operator!= (SparseMatrix<fT> const & other)
{
    return !(*this == other);
}

/*
 * Assignment Operator
 * Creating deep copy object of the original
*/
template<typename fT>
SparseMatrix<fT>& SparseMatrix<fT>::operator=(const SparseMatrix<fT>& other)
{
    if (this != &other)
    {
        m_nrow = other.m_nrow;
        m_ncol = other.m_ncol;
        m_index = other.m_index;
        m_indices = other.m_indices;
        m_data = other.m_data;
    }
    return *this;
}

/*
 * Addition Operator
 * Return results of matrix addition
*/
template<typename fT>
SparseMatrix<fT>& SparseMatrix<fT>::operator+=(const SparseMatrix<fT>& other)
{
    // Check dimension
    same_size(*this, other);

    // Add new value where other matrix is not zero
    for(size_t i=0; i<other.m_nrow; ++i)
        for (size_t j=m_index.at(i); j<m_index.at(i+1); ++j)
        {
            // Use mutator function to update added value
            if( fabs(m_data.at(j)) > eps_)
                (*this)( i, j, (*this)(i, j) + other(i, j));
        }
    return *this;
}
template<typename fT>
SparseMatrix<fT> SparseMatrix<fT>::operator+(const SparseMatrix<fT>& other) const
{
    // New matrix to be returned
    // Initialized with called class
    SparseMatrix<fT> ret(*this);
    ret += other;
    return ret;
}

/*
 * Substraction Operator
 * Return results of matrix substraction
*/
template<typename fT>
SparseMatrix<fT>& SparseMatrix<fT>::operator-=(const SparseMatrix<fT>& other)
{
    // Check dimension
    same_size(*this, other);

    // Add new value where other matrix is not zero
    for(size_t i=0; i<other.m_nrow; ++i)
        for (size_t j=m_index.at(i); j<m_index.at(i+1); ++j)
        {
            // Use mutator function to update substracted value
            if( fabs(m_data.at(j)) > eps_)
                (*this)( i, j, (*this)(i, j) - other(i, j));
        }
    return *this;
}
template<typename fT>
SparseMatrix<fT> SparseMatrix<fT>::operator-(const SparseMatrix<fT>& other) const
{
    // New matrix to be returned
    // Initialized with called class
    SparseMatrix<fT> ret(*this);
    ret -= other;
    return ret;
}

/*
 * Multiplication Operator
 * Return results of matrix multiplication
*/
template<typename fT>
SparseMatrix<fT>& SparseMatrix<fT>::operator*=(fT alpha) 
{
    // Multiply element array by alpha
    for (size_t i=0; i < m_data.size(); ++i) m_data.at(i) *= alpha;
    return *this;
}
template<typename fT>
SparseMatrix<fT> SparseMatrix<fT>::operator*(fT alpha) const
{
    // New matrix to be returned
    // Initialized with called class
    SparseMatrix<fT> ret(*this);
    ret *= alpha;
    return ret;
}
template<typename fT>
SparseMatrix<fT> SparseMatrix<fT>::operator* (const SparseMatrix<fT>& other) const
{
    // New matrix to be returned
    // Initialized with correct dimension
    SparseMatrix<fT> ret(m_nrow, other.m_ncol);

    for(size_t i=0; i<ret.m_nrow; ++i)
        for(size_t k=0; k<ret.m_ncol; ++k)
            for(size_t j=0; j<m_ncol; ++j)
                ret(i, k, ret(i, k) + ((*this)(i,j) * other(j,k)));
    return ret;
}
template<typename fT>
std::vector<fT> SparseMatrix<fT>::operator* (const std::vector<fT> other) const
{
    // New vector to be returned
    std::vector<fT> ret;

    if(m_ncol == other.size())
        for(size_t i = 0; i < m_nrow; ++i)
        {
            fT sum = static_cast<fT>(0.);
            for(size_t j = m_index.at(i); j < m_index.at(i+1); ++j)
                sum += other.at(m_indices.at(j)) * m_data.at(j);
        }
    else
    {
        throw std::out_of_range(
            "the dimension of first matrix column"
            "differs from that of vector size");
    }

    return ret;
}

/*
 * Division Operator
 * Return results of matrix division
*/
template<typename fT>
SparseMatrix<fT>& SparseMatrix<fT>::operator/=(fT alpha) 
{
    // Divide element array by alpha
    for (size_t i=0; i < m_data.size(); ++i) m_data.at(i) /= alpha;
    return *this;
}
template<typename fT>
SparseMatrix<fT> SparseMatrix<fT>::operator/(fT alpha) const
{
    // New matrix to be returned
    // Initialized with called class
    SparseMatrix<fT> ret(*this);
    ret /= alpha;
    return ret;
}


template<typename fT>
size_t SparseMatrix<fT>::findIndex(size_t nrow, size_t ncol) const
{
    typename std::vector<size_t>::const_iterator indices_it = \
            std::find(
                m_indices.begin() + m_index.at(nrow),
                m_indices.begin() + m_index.at(nrow+1),
                ncol);

    return static_cast<size_t>(std::distance(m_indices.begin(), indices_it));
}

template class SparseMatrix<double>;

