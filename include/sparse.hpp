// Developer: Wilbert (wilbert.phen@gmail.com)

#ifndef SPARSEMATRIX_H
#define SPARSEMATRIX_H

#define eps_ 1.0e-16

#include <iostream>
#include <vector>
#include <string>

template<typename fT>
class SparseMatrix {

public:

    SparseMatrix(size_t nrow=1, size_t ncol=1, bool identity=false);
    SparseMatrix(SparseMatrix<fT> const & other);
    SparseMatrix(SparseMatrix<fT> && other);
    SparseMatrix(std::vector<std::vector<fT>> const & other, size_t nrow, size_t ncol);
    ~SparseMatrix() = default;
    
    void load(std::string filename);
    void save(std::string filename);
    void reset(bool identity=false);

    fT   operator() (size_t nrow, size_t ncol) const;
    void operator() (size_t nrow, size_t ncol, fT value);

    template<typename tfT>
    friend void validate_multiplication(const SparseMatrix<tfT> &mat1, const SparseMatrix<tfT> &mat2);
    template<typename tfT>
    friend void same_size(const SparseMatrix<tfT> &mat1, const SparseMatrix<tfT> &mat2);

    bool operator== (SparseMatrix<fT> const &);
    bool operator!= (SparseMatrix<fT> const &);

    SparseMatrix &  operator= (const SparseMatrix<fT>& other);
    SparseMatrix &  operator+=(const SparseMatrix<fT>& other);
    SparseMatrix    operator+ (const SparseMatrix<fT>& other) const;
    SparseMatrix &  operator-=(const SparseMatrix<fT>& other);
    SparseMatrix    operator- (const SparseMatrix<fT>& other) const;
    SparseMatrix &  operator*=(fT alpha);
    SparseMatrix    operator* (fT alpha) const;
    SparseMatrix    operator* (const SparseMatrix<fT>& other) const;
    std::vector<fT> operator* (const std::vector<fT> other) const;
    SparseMatrix &  operator/=(fT alpha);
    SparseMatrix    operator/ (fT alpha) const;

    size_t findIndex(size_t nrow, size_t ncol) const;
    size_t nrow() const { return m_nrow; }
    size_t ncol() const { return m_ncol; }


private:
    
    size_t m_nrow;
    size_t m_ncol;

    std::vector<size_t> m_index;
    std::vector<size_t> m_indices;
    std::vector<fT> m_data;

};

template<typename fT>
void validate_multiplication(const SparseMatrix<fT> &mat1, const SparseMatrix<fT> &mat2);
template<typename fT>
void same_size(const SparseMatrix<fT> &mat1, const SparseMatrix<fT> &mat2);

#endif
