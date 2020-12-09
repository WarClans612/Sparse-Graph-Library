// Developer: Wilbert (wilbert.phen@gmail.com)

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/operators.h>
#include <pybind11/numpy.h>

#include <string>
#include "sparse.hpp"

namespace py = pybind11;

PYBIND11_MODULE(_sparse, m) {
    using Matrix = SparseMatrix<double>;
    py::class_<Matrix>(m, "SparseMatrix", py::buffer_protocol())
        .def(py::init<size_t, size_t>())
        .def_property("nrow", &Matrix::nrow, nullptr)
        .def_property("ncol", &Matrix::ncol, nullptr)
        .def("__eq__", &Matrix::operator==)
        .def("__setitem__", [](Matrix &mat, std::pair<size_t, size_t> i, double v) {
            mat(i.first, i.second, v);
        })
        .def("__getitem__", [](Matrix &mat, std::pair<size_t, size_t> i) {
            return mat(i.first, i.second);
        });
}