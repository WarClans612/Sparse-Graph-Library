# Developer: Wilbert (wilbert.phen@gmail.com)

import numpy as np
import math
import time
import pytest

from _sparse import SparseMatrix

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

def test_load_save():
    size = 10
    mat1, mat2, mat3, *_ = make_matrices(size)

    mat1.save('mat1.txt')
    mat2.save('mat2.txt')
    mat3.save('mat3.txt')

    mat1_load = SparseMatrix()
    mat1_load.load('mat1.txt')
    mat2_load = SparseMatrix()
    mat2_load.load('mat2.txt')
    mat3_load = SparseMatrix()
    mat3_load.load('mat3.txt')

    for i in range(mat1_load.nrow):
        for j in range(mat1_load.ncol):
            assert 0 != mat1_load[i, j]
            assert mat1_load[i, j] == mat2_load[i, j]
            assert 0 == mat3_load[i, j]

def test_expand_shrink_row():
    size = 10
    mat1, mat2, mat3, *_ = make_matrices(size)

    assert size*size == mat1[size-1, size-1]
    assert size*size == mat2[size-1, size-1]
    assert 0 == mat3[size-1, size-1]

    mat1.shrink_row()
    mat2.shrink_row()
    mat3.shrink_row()

    for i in range(mat1.nrow):
        for j in range(mat1.ncol):
            assert 0 != mat1[i, j]
            assert mat1[i, j] == mat2[i, j]
            assert 0 == mat3[i, j]

    mat1.expand_row()
    mat2.expand_row()
    mat3.expand_row()

    assert 0 == mat1[size-1, size-1]
    assert 0 == mat2[size-1, size-1]
    assert 0 == mat3[size-1, size-1]

def test_expand_shrink_col():
    size = 10
    mat1, mat2, mat3, *_ = make_matrices(size)

    assert size*size == mat1[size-1, size-1]
    assert size*size == mat2[size-1, size-1]
    assert 0 == mat3[size-1, size-1]

    mat1.shrink_col()
    mat2.shrink_col()
    mat3.shrink_col()

    for i in range(mat1.nrow):
        for j in range(mat1.ncol):
            assert 0 != mat1[i, j]
            assert mat1[i, j] == mat2[i, j]
            assert 0 == mat3[i, j]

    mat1.expand_col()
    mat2.expand_col()
    mat3.expand_col()

    assert 0 == mat1[size-1, size-1]
    assert 0 == mat2[size-1, size-1]
    assert 0 == mat3[size-1, size-1]

