/*
 * Cortex Ocean - a port of the Houdini Ocean Toolkit
 * Copyright (C) 2011 Dan Bethell
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

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

BOOST_PYTHON_MODULE( CortexOcean )
{
    CortexOcean::bindOceanOp();
}
