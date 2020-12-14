# Developer: Wilbert (wilbert.phen@gmail.com)

import numpy as np
import math
import time
import pytest

from _sparse import SparseGraph

def make_graphs(size, sparse=True):
    gra1 = SparseGraph(size, size)
    gra2 = SparseGraph(size, size)
    gra3 = SparseGraph(size, size)

    for it in range(size):
        for jt in range(size):
            gra1[it, jt] = it * size + jt + 1
            gra2[it, jt] = it * size + jt + 1
            gra3[it, jt] = 0

    return gra1, gra2, gra3

def test_basic():
    size = 100
    gra1, gra2, gra3, *_ = make_graphs(size)

    assert size == gra1.dim
    assert size == gra2.dim
    assert size == gra3.dim

    assert 2 == gra1[0, 1]
    assert size+2 == gra1[1, 1]
    assert size*2 == gra1[1, size-1]
    assert size*size == gra1[size-1, size-1]

    for i in range(gra1.dim):
        for j in range(gra1.dim):
            assert 0 != gra1[i, j]
            assert gra1[i, j] == gra2[i, j]
            assert 0 == gra3[i, j]

    assert gra1 == gra2
    assert gra1 is not gra2
