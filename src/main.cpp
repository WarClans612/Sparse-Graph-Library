// Developer: Wilbert (wilbert.phen@gmail.com)

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/operators.h>
#include <pybind11/numpy.h>

#include <vector>
#include <string>
#include "sparse.hpp"

namespace py = pybind11;

PYBIND11_MODULE(_sparse, m) {
    using Matrix = SparseMatrix<double>;
    py::class_<Matrix>(m, "SparseMatrix", py::buffer_protocol())
        .def(py::init<size_t, size_t, bool>(),
            py::arg("nrow")=1, py::arg("ncol")=1, py::arg("identity")=false
        )
        .def_property("nrow", &Matrix::nrow, nullptr)
        .def_property("ncol", &Matrix::ncol, nullptr)
        .def("__eq__", &Matrix::operator==)
        .def("__ne__", &Matrix::operator!=)
        .def("assign", &Matrix::operator=)
        .def(py::self += py::self)
        .def(py::self + py::self)
        .def(py::self -= py::self)
        .def(py::self - py::self)
        .def(py::self *= double())
        .def(py::self * double())
        .def(py::self * py::self)
        .def(py::self * std::vector<double>())
        .def(py::self /= double())
        .def(py::self / double())
        .def("__setitem__", [](Matrix &mat, std::pair<size_t, size_t> i, double v) {
            mat(i.first, i.second, v);
        })
        .def("__getitem__", [](Matrix &mat, std::pair<size_t, size_t> i) {
            return mat(i.first, i.second);
        });
}