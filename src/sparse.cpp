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
SparseMatrix<fT>::SparseMatrix(size_t nrow, size_t ncol)
    : m_nrow(nrow), m_ncol(ncol)
{
    m_index.reserve(nrow+1);
    m_indices.reserve(nrow);
    m_data.reserve(nrow);

    for(size_t i=0; i<nrow; ++i)
    {
        m_index.push_back(i);
        m_indices.push_back(i);
        m_data.push_back(static_cast<fT>(1.));
    }
    m_index.push_back(nrow);
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

/*
 * Accessor for matrix elements
 * @param nrow matrix row
 * @param ncol matrix column
 * @return value at the specified matrix row and column
*/
template<typename fT>
fT SparseMatrix<fT>::operator() (size_t nrow, size_t ncol) const
{
    if(nrow == ncol)
        return m_data.at(m_index.at(nrow));

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
    if(nrow == ncol)
        m_data.at(m_index.at(nrow)) = value;
    else
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
