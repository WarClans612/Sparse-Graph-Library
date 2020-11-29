// Developer: Wilbert (wilbert.phen@gmail.com)

#ifndef SPARSEMATRIX_H
#define SPARSEMATRIX_H

#include <iostream>
#include <vector>

template<typename fT>
class SparseMatrix {

public:

    SparseMatrix();
    ~SparseMatrix() = default;

    fT   operator() (size_t row, size_t col) const;
    fT & operator() (size_t row, size_t col);
    fT   operator() (size_t idx) const;
    fT & operator() (size_t idx);

private:
    
    size_t m_nrow;
    size_t m_ncol;

    std::vector<size_t> m_index;
    std::vector<size_t> m_indices;
    std::vector<fT> m_data;

};

#endif
