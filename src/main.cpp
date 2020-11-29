// Developer: Wilbert (wilbert.phen@gmail.com)

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/operators.h>
#include <pybind11/numpy.h>

#include <string>
#include "sparse.hpp"

namespace py = pybind11;

PYBIND11_MODULE(_sparse, m) {
    py::class_<SparseMatrix<double>>(m, "SparseMatrix")
        .def(py::init<>());
}