// Developer: Wilbert (wilbert.phen@gmail.com)

#include <vector>
#include <string>

#include "graph.hpp"

/**
 * Default Constructor
**/
template<typename fT>
SparseGraph<fT>::SparseGraph(size_t dim, bool identity)
    : m_adj_mat(dim, dim, identity)
{

}

/**
 * Copy Contructor
 * Setting the data into copied object
**/
template<typename fT>
SparseGraph<fT>::SparseGraph(SparseGraph<fT> const & other)
    : m_adj_mat(other.m_adj_mat)
{

}

/**
 * Move Constructor
 * Swapping data content of two graph
**/
template<typename fT>
SparseGraph<fT>::SparseGraph(SparseGraph<fT> && other)
    : m_adj_mat(other.m_adj_mat)
{

}

/**
 * Copy Constuctor
 * Init Sparse Graph with vector of vector
**/
template<typename fT>
SparseGraph<fT>::SparseGraph(std::vector<std::vector<fT>> const & other, size_t dim)
    : m_adj_mat(other, dim, dim)
{
    
}

/*
 * Load sparse matrix from text file
*/
template<typename fT>
void SparseGraph<fT>::load(std::string filename)
{
    m_adj_mat.load(filename);
}

/*
 * Save sparse matrix from text file
*/
template<typename fT>
void SparseGraph<fT>::save(std::string filename)
{
    m_adj_mat.save(filename);
}

/*
 * Reset the content of matrix to initial state
*/
template<typename fT>
void SparseGraph<fT>::reset(bool identity)
{
    m_adj_mat.reset(identity);
}

/*
 * Accessor for graph elements
 * @param nrow graph row
 * @param ncol graph column
 * @return value at the specified graph row and column
*/
template<typename fT>
fT SparseGraph<fT>::operator() (size_t nrow, size_t ncol) const
{
    return m_adj_mat(nrow, ncol);
}

/*
 * Mutator for graph elements
 * @param nrow graph row
 * @param ncol graph column
 * @return reference to the value at the specified graph row and column
*/
template<typename fT>
void SparseGraph<fT>::operator() (size_t nrow, size_t ncol, fT value)
{
    m_adj_mat(nrow, ncol, value);
}

/*
 * Equality Comparison
*/
template<typename fT>
bool SparseGraph<fT>::operator== (SparseGraph<fT> const & other)
{
    if (m_adj_mat == other.m_adj_mat) return true;
    return false;
}

/*
 * Increase the node count of the graph
*/
template<typename fT>
void SparseGraph<fT>::add_node()
{
    m_adj_mat.expand_row();
    m_adj_mat.expand_col();
}

/*
 * Decrease the node count of the graph
*/
template<typename fT>
void SparseGraph<fT>::remove_node()
{
    m_adj_mat.shrink_row();
    m_adj_mat.shrink_col();
}

/*
 * Return reference to internal adjacency matrix
*/
template<typename fT>
const SparseMatrix<fT> & SparseGraph<fT>::to_sparse_matrix()
{
    return m_adj_mat;
}

template class SparseGraph<double>;
