// Developer: Wilbert (wilbert.phen@gmail.com)

#ifndef SPARSEGRAPH_H
#define SPARSEGRAPH_H

#include "sparse.hpp"

template<typename fT>
class SparseGraph {

public:

    SparseGraph(size_t dim=1, bool identity=false);
    SparseGraph(SparseGraph<fT> const & other);
    SparseGraph(SparseGraph<fT> && other);
    SparseGraph(std::vector<std::vector<fT>> const & other, size_t dim);
    ~SparseGraph() = default;

    void load(std::string filename);
    void save(std::string filename);
    void reset(bool identity=false);

    fT   operator() (size_t nrow, size_t ncol) const;
    void operator() (size_t nrow, size_t ncol, fT value);
    
    bool operator== (SparseGraph<fT> const &);

    void add_node();
    void remove_node();

    const SparseMatrix<fT> & to_sparse_matrix();
    size_t dim() const { return m_adj_mat.nrow(); }

private:

    SparseMatrix<fT> m_adj_mat;

};

#endif
