# Developer: Wilbert (wilbert.phen@gmail.com)

import numpy as np
import math
import time
import pytest

from _sparse import SparseMatrix

def printm(m):
    for i in range(m.nrow):
        for j in range(m.ncol):
            print(m[i, j], end='\t')
        print('')

def make_matrices(size, sparse=True):
    mat1 = SparseMatrix(size, size)
    mat2 = SparseMatrix(size, size)
    mat3 = SparseMatrix(size, size)

    for it in range(size):
        for jt in range(size):
            mat1[it, jt] = it * size + jt + 1
            mat2[it, jt] = it * size + jt + 1
            mat3[it, jt] = 0

    return mat1, mat2, mat3


def test_basic():
    size = 100
    mat1, mat2, mat3, *_ = make_matrices(size)

    assert size == mat1.nrow
    assert size == mat1.ncol
    assert size == mat2.nrow
    assert size == mat2.ncol
    assert size == mat3.nrow
    assert size == mat3.ncol

    assert 2 == mat1[0, 1]
    assert size+2 == mat1[1, 1]
    assert size*2 == mat1[1, size-1]
    assert size*size == mat1[size-1, size-1]

    for i in range(mat1.nrow):
        for j in range(mat1.ncol):
            assert 0 != mat1[i, j]
            assert mat1[i, j] == mat2[i, j]
            assert 0 == mat3[i, j]

    assert mat1 == mat2
    assert mat1 is not mat2

def test_match():
    size = 10
    mat1, mat2, *_ = make_matrices(size)
    mat3, mat4 = np.arange(1, size*size+1).reshape(size, size), \
                 np.arange(1, size*size+1).reshape(size, size)

    ret_sparse = mat1 * mat2
    ret_numpy = np.dot(mat3, mat4)

    assert size == ret_sparse.nrow
    assert size == ret_sparse.ncol
    assert size == ret_numpy.shape[0]
    assert size == ret_numpy.shape[1]

    for i in range(ret_sparse.nrow):
        for j in range(ret_sparse.ncol):
            assert mat1[i, j] != ret_sparse[i, j]
            assert ret_sparse[i, j] == ret_numpy[i, j]
