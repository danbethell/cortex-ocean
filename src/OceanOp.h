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

#ifndef OCEANOP_H__
#define OCEANOP_H__

#include <IECore/ModifyOp.h>
#include <IECore/NumericParameter.h>
#include <IECore/CompoundParameter.h>
#include "Ocean.h"
using namespace IECore;

namespace CortexOcean
{
    class OceanOp : public ModifyOp
    {
    public:
        IE_CORE_DECLARERUNTIMETYPEDEXTENSION( OceanOp, 666001, ModifyOp );
        OceanOp();
        virtual ~OceanOp();

    protected:
        virtual void modify( Object *input, const CompoundObject *operands );

	private :

        // utility method for getting our parameter values & caching them
        template <class T>
        bool getParameter( const CompoundObject *operands, const std::string &name, T &cache_val )
        {
                bool value_changed = false;
                T val = operands->member<IECore::TypedData<T> >( name )->readable();
                if ( cache_val!=val || m_initcache )
                {
                    cache_val=val;
                    value_changed = true;
                }
                return value_changed;
        }


        // standard HOT parameters
        IntParameterPtr m_resolution, m_seed;
        FloatParameterPtr m_size, m_windspeed, m_winddir, m_shortwave;
        FloatParameterPtr m_waveheight, m_choppiness;
        FloatParameterPtr m_damp, m_windalign, m_depth, m_time;
        BoolParameterPtr m_chop, m_catmull, m_normals, m_jacobian;

        // few special parameters
        StringParameterPtr m_positionVarParameter;
        V2iParameterPtr m_positionIndexParameter;
		StringParameterPtr m_displaceVarParameter;

        // HOT stuff
        drw::Ocean *_ocean;
        drw::OceanContext *_ocean_context;
        float   _ocean_scale;
        bool _ocean_needs_rebuild;

        // member data used for caching our parameters
        bool m_initcache; // indicates we need to initialise our cache vars
        int _m_res, _m_seed;
        bool _m_chop, _m_jacobian, _m_normals, _m_catmull;
        float _m_size, _m_choppiness, _m_windspeed, _m_waveheight, _m_time;
        float _m_damp, _m_windalign, _m_depth, _m_winddir, _m_shortwave;
    };

    IE_CORE_DECLAREPTR( OceanOp );
}

#endif
