#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include "src/UserInterface.h"

namespace py = pybind11;

class PyBase : public Base_Interface
{
public:
    using Base_Interface::Base_Interface;
    using Base_Interface::v_num_;
    using Base_Interface::f_num_;
    using Base_Interface::dim_;
    using Base_Interface::data_;

    void ConvertToData() override { PYBIND11_OVERRIDE_PURE(void, Base_Interface, ConvertToData, ); }
    void FindAllFace() override { PYBIND11_OVERRIDE_PURE(void, Base_Interface, FindAllFace, ); }
    void Output() override { PYBIND11_OVERRIDE_PURE(void, Base_Interface, Output, ); }
};

PYBIND11_MODULE(face_enumeration, m)
{
    py::class_<Base_Interface, PyBase>(m, "Base_Interface")
        .def(py::init<std::vector<std::vector<size_t>> &>())
        .def(py::init<std::vector<std::vector<size_t>> &, int, int, int>())
        .def_readwrite("data_", &PyBase::data_)
        .def_readwrite("v_num_", &PyBase::v_num_)
        .def_readwrite("f_num_", &PyBase::f_num_)
        .def_readwrite("dim_", &PyBase::dim_)
        .def("ConvertToData", &Base_Interface::ConvertToData)
        .def("FindAllFace", &Base_Interface::FindAllFace)
        .def("Output", &Base_Interface::Output);

    py::class_<Interface_KP, Base_Interface>(m, "Interface_KP")
        .def(py::init<std::vector<std::vector<size_t>> &>())
        .def_readwrite("start_v_storage", &Interface_KP::start_f_storage)
        .def_readwrite("start_f_storage", &Interface_KP::start_v_storage)
        .def_readwrite("V_set", &Interface_KP::V_set)
        .def_readwrite("L", &Interface_KP::L)
        .def_readwrite("Q", &Interface_KP::Q)
        .def_readwrite("dimersation_store", &Interface_KP::dimersation_store)
        .def("ConvertToData", &Interface_KP::ConvertToData)
        .def("FindAllFace", &Interface_KP::FindAllFace)
        .def("Output", &Interface_KP::Output);

    py::class_<Vertex_set>(m, "Vertex_set")
        .def(py::init<std::list<size_t>>())
        .def(py::init<const Vertex_set&>())
        .def_readonly("vertices", &Vertex_set::vertices);

    py::class_<H_Diag_Node>(m, "H_Diag_Node")
        .def(py::init<Vertex_set, Vertex_set>())
        .def_readonly("Vert_adrG", &H_Diag_Node::Vert_adrG)
        .def_readonly("Vert_adrH", &H_Diag_Node::Vert_adrH);
}