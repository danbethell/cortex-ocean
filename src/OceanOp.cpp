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

#include "OceanOp.h"

#include <IECore/PointsPrimitive.h>
#include <IECore/TypedPrimitiveParameter.h>

using namespace IECore;
using namespace CortexOcean;

IE_CORE_DEFINERUNTIMETYPED( OceanOp );

OceanOp::OceanOp()
	:	ModifyOp(
		"Deforms points using algorithms from the Houdini Ocean Toolkit.",
		new PrimitiveParameter(
			"result",
			"The updated Primitive with deformed points.",
			new PointsPrimitive()
			),
		new PrimitiveParameter(
			"input",
			"The input Primitive with points to deform.",
			new PointsPrimitive()
			)
		),
		m_initcache(true)
{
    // initialise our HOT member data
    _ocean = 0;
    _ocean_context = 0;
    _ocean_scale = 1.0f;
    _ocean_needs_rebuild = true;

    // standard HOT parameters
    m_resolution = new IntParameter( "res", "Ocean Resolution", 8 );
    m_size = new FloatParameter( "size", "Ocean Size", 200.f);
    m_windspeed = new FloatParameter( "windspeed", "Windspeed", 30.f);
    m_winddir = new FloatParameter( "winddir", "Wind Direction", 0.f);
    m_shortwave = new FloatParameter( "shortwave", "Shortest Wavelength", 1.f );
    m_waveheight = new FloatParameter( "waveheight", "Approx. Waveheight", 1.f );
    m_seed = new IntParameter( "seed", "Seed", 0 );
    m_chop = new BoolParameter( "chop", "Chop", false );
    m_choppiness = new FloatParameter( "choppiness", "Choppiness", 1.f );
    m_damp = new FloatParameter( "damp", "Damp Reflections", 1.f );
    m_windalign = new FloatParameter( "windalign", "Wind Alignment", 2.f );
    m_depth = new FloatParameter( "depth", "Ocean Depth", 200.f );
    m_time = new FloatParameter( "time", "Time", 0.0 );
    m_catmull = new BoolParameter( "catmull", "Catmull-Rom Interpolation", true );
    m_normals = new BoolParameter( "normals", "Normals", false );
    m_jacobian = new BoolParameter( "jacobian", "Jacobian", false );
    parameters()->addParameter( m_resolution );
    parameters()->addParameter( m_size );
    parameters()->addParameter( m_windspeed );
    parameters()->addParameter( m_winddir );
    parameters()->addParameter( m_shortwave );
    parameters()->addParameter( m_waveheight );
    parameters()->addParameter( m_seed );
    parameters()->addParameter( m_chop );
    parameters()->addParameter( m_choppiness );
    parameters()->addParameter( m_damp );
    parameters()->addParameter( m_windalign );
    parameters()->addParameter( m_depth );
    parameters()->addParameter( m_time );
    parameters()->addParameter( m_catmull );
    parameters()->addParameter( m_normals );
    parameters()->addParameter( m_jacobian );

    // few special ones
    m_positionVarParameter = new StringParameter(
		"positionVar",
		"The variable to use as per-sample position.",
		"P" );
    m_positionIndexParameter = new V2iParameter(
            "positionIndex",
            "The indices of the position vector to use.",
            Imath::V2i(0,2) );
    m_displaceVarParameter = new StringParameter(
		"displaceVar",
		"The variable that will be displaced.",
		"P" );
    parameters()->addParameter( m_positionVarParameter );
	parameters()->addParameter( m_positionIndexParameter );
	parameters()->addParameter( m_displaceVarParameter );
}

OceanOp::~OceanOp()
{
    if (_ocean)
    {
        delete _ocean;
    }
    if (_ocean_context)
    {
        delete _ocean_context;
    }
}

void OceanOp::modify( Object *input, const CompoundObject *operands )
{
	// get input and parameters
	Primitive *pt = static_cast<Primitive *>( input );

	// get our standard parameters
    _ocean_needs_rebuild |= getParameter<int>( operands, "res", _m_res );
    _ocean_needs_rebuild |= getParameter<float>( operands, "size", _m_size );
    _ocean_needs_rebuild |= getParameter<bool>( operands, "chop", _m_chop );
    getParameter<float>( operands, "choppiness", _m_choppiness );
    _ocean_needs_rebuild |= getParameter<bool>( operands, "jacobian", _m_jacobian );
    _ocean_needs_rebuild |= getParameter<bool>( operands, "normals", _m_normals );
    _ocean_needs_rebuild |= getParameter<float>( operands, "windspeed", _m_windspeed );
    _ocean_needs_rebuild |= getParameter<float>( operands, "waveheight", _m_waveheight );
    _ocean_needs_rebuild |= getParameter<float>( operands, "damp", _m_damp );
    _ocean_needs_rebuild |= getParameter<float>( operands, "windalign", _m_windalign );
    _ocean_needs_rebuild |= getParameter<float>( operands, "depth", _m_depth );
    _ocean_needs_rebuild |= getParameter<float>( operands, "winddir", _m_winddir );
    _ocean_needs_rebuild |= getParameter<float>( operands, "shortwave", _m_shortwave );
    _ocean_needs_rebuild |= getParameter<int>( operands, "seed", _m_seed );
    getParameter<float>( operands, "time", _m_time );
    getParameter<bool>( operands, "catmull", _m_catmull );

    // get some others parameters
    std::string position_var = operands->member<StringData>( "positionVar" )->readable();
    Imath::V2i positionIndex = operands->member<V2iData>( "positionIndex" )->readable();
    std::string displace_var = operands->member<StringData>( "displaceVar" )->readable();

    m_initcache=false;

    // rebuild our ocean if necessary
    if (!_ocean || _ocean_needs_rebuild)
    {
        if (_ocean)
        {
            delete _ocean;
        }
        if (_ocean_context)
        {
            delete _ocean_context;
        }

        int gridres  = 1 << _m_res;
        float stepsize = 200 / _m_size;
        _ocean = new drw::Ocean(gridres,gridres,stepsize,stepsize,
                                _m_windspeed,_m_shortwave,1.0,_m_winddir,1-_m_damp,_m_windalign,
                                _m_depth,_m_seed);
        _ocean_scale   = _ocean->get_height_normalize_factor();
        _ocean_context = _ocean->new_context(true,_m_chop,_m_normals && !_m_chop,_m_jacobian);
        _ocean_needs_rebuild = false;
    }

    // sum up the waves at this timestep
    _ocean->update(_m_time,*_ocean_context,true,_m_chop,_m_normals && !_m_chop,_m_jacobian,
                   _ocean_scale * _m_waveheight,_m_choppiness);

    // get our data to modify
    V3fVectorData *p = pt->variableData<V3fVectorData>(position_var);
    V3fVectorData *d = pt->variableData<V3fVectorData>(displace_var);
    if ( !p || !d )
    {
        throw Exception("Could not get position and displace data from primitive!");
    }

    // perform our displacement
    const std::vector<Imath::V3f> &pos_data = p->readable();
    std::vector<Imath::V3f> &p_data = p->writable();
    std::vector<Imath::V3f>::const_iterator pos_it = pos_data.begin();
    std::vector<Imath::V3f>::iterator p_it = p_data.begin();
    for ( pos_it=pos_data.begin(), p_it=p_data.begin();
            pos_it!=pos_data.end(); ++pos_it, ++p_it )
    {
        const Imath::V3f &pos = *(pos_it);
        Imath::V3f &p = *(p_it);
        if (!_m_catmull)
        {
            _ocean_context->eval_xz(p[positionIndex[0]],p[positionIndex[1]]);
        }
        else
        {
            _ocean_context->eval2_xz(p[positionIndex[0]],p[positionIndex[1]]);
        }

        if (_m_chop)
        {
            p += Imath::V3f( _ocean_context->disp[0], _ocean_context->disp[1], _ocean_context->disp[2] );
        }
        else
        {
            p[1] += _ocean_context->disp[1];
        }

        if (_m_normals && !_m_chop)
        {
            // TBA
            /*
          UT_Vector3* normal = (UT_Vector3*) ppt->castAttribData<UT_Vector3>(normal_index);
            normal->assign(_ocean_context->normal[0],
                           _ocean_context->normal[1],
                           _ocean_context->normal[2]);
            normal->normalize();
            */
        }

        if (_m_jacobian)
        {
            // TBA
            /*
            float *js = (float*)ppt->castAttribData<float>(jminus_index);
            *js = _ocean_context->Jminus;
            UT_Vector3* eminus = (UT_Vector3*)ppt->castAttribData<UT_Vector3>(eminus_index);
            eminus->assign(_ocean_context->Eminus[0],0,_ocean_context->Eminus[1]);
            */
        }
    }
}
