#include <boost/python.hpp>
#include "OceanOp.h"
#include "CortexOceanBinding.h"
#include <IECorePython/RunTimeTypedBinding.h>

using namespace boost::python;

namespace CortexOcean
{
    void bindOceanOp()
    {
        IECorePython::RunTimeTypedClass<OceanOp>()
            .def( init< >() )
            ;
    }
}

BOOST_PYTHON_MODULE( _CortexOcean )
{
    CortexOcean::bindOceanOp();
}
