// Developer: Wilbert (wilbert.phen@gmail.com)

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/operators.h>
#include <pybind11/numpy.h>

#include <vector>
#include <string>
#include "sparse.hpp"
#include "graph.hpp"

namespace py = pybind11;

PYBIND11_MODULE(_sparse, m) {
    using Matrix = SparseMatrix<double>;
    py::class_<Matrix>(m, "SparseMatrix", py::buffer_protocol())
        .def(py::init<size_t, size_t, bool>(),
            py::arg("nrow")=1, py::arg("ncol")=1, py::arg("identity")=false
        )
        .def(py::init<Matrix&>())
        .def(py::init<std::vector<std::vector<double>>&, size_t, size_t>())
        .def("load", &Matrix::load)
        .def("save", &Matrix::save)
        .def("reset", &Matrix::reset)
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
        })
        .def("expand_row", &Matrix::expand_row)
        .def("expand_col", &Matrix::expand_col)
        .def("shrink_row", &Matrix::shrink_row)
        .def("shrink_col", &Matrix::shrink_col);

    using Graph = SparseGraph<double>;
    py::class_<Graph>(m, "SparseGraph", py::buffer_protocol())
        .def(py::init<size_t, bool>(),
            py::arg("dim")=1, py::arg("identity")=false
        )
        .def(py::init<Graph&>())
        .def(py::init<std::vector<std::vector<double>>&, size_t>())
        .def("load", &Graph::load)
        .def("save", &Graph::save)
        .def("reset", &Graph::reset)
        .def("__eq__", &Graph::operator==)
        .def("add_node", &Graph::add_node)
        .def("remove_node", &Graph::remove_node)
        .def("__setitem__", [](Graph &gra, std::pair<size_t, size_t> i, double v) {
            gra(i.first, i.second, v);
        })
        .def("__getitem__", [](Graph &gra, std::pair<size_t, size_t> i) {
            return gra(i.first, i.second);
        })
        .def("to_sparse_matrix", &Graph::to_sparse_matrix)
        .def_property("dim", &Graph::dim, nullptr);
}