#include "material.h"
#include "material.cpp"
#include <boost/python.hpp>
#include <boost/python/suite/indexing/vector_indexing_suite.hpp>

BOOST_PYTHON_MODULE(librel)
{
    using namespace boost::python;
    class_<std::vector<double> >("double_vector")
        .def(vector_indexing_suite<std::vector<double> >())
    ;
    class_<Material>("Material", init<unsigned int,double, std::string, optional<bool> >())
      .def("state", &Material::state)
      .def("set_ExpId", &Material::set_ExpId)
    ;
}
