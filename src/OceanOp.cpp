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
		)
{
    /*
	m_positionVarParameter = new StringParameter(
		"positionVar",
		"The variable name to use as per-point position.",
		"P" );

	m_velocityVarParameter = new StringParameter(
		"velocityVar",
		"The variable name to use as per-point velocity.",
		"v" );

	m_sampleLengthParameter = new FloatParameter(
		"sampleLength",
		"The sample time across which to displace P.",
		1.0 );

	m_sampleLengthVarParameter = new StringParameter(
		"sampleLengthVar",
		"The variable name to use as per-point sample length.",
		"" );

	parameters()->addParameter( m_positionVarParameter );
	parameters()->addParameter( m_velocityVarParameter );
	parameters()->addParameter( m_sampleLengthParameter );
	parameters()->addParameter( m_sampleLengthVarParameter );
    */
}

OceanOp::~OceanOp()
{
}

void OceanOp::modify( Object *input, const CompoundObject *operands )
{
	// get input and parameters
	Primitive *pt = static_cast<Primitive *>( input );

    /*
	std::string position_var = operands->member<StringData>( "positionVar" )->readable();
	std::string velocity_var = operands->member<StringData>( "velocityVar" )->readable();
	float sample_length = operands->member<FloatData>( "sampleLength" )->readable();
	std::string sample_length_var = operands->member<StringData>( "sampleLengthVar" )->readable();

	// should we look for pp sample length?
	bool use_pp_sample_length = ( sample_length_var!="" );

	// check for variables
	if ( pt->variables.count(position_var)==0 )
	{
		throw Exception( "Could not find position variable on primitive!" );
	}
	if ( pt->variables.count(velocity_var)==0 )
	{
		throw Exception( "Could not find velocity variable on primitive!" );
	}

	// access our P & V information
	V3fVectorData *p = pt->variableData<V3fVectorData>(position_var);
	V3fVectorData *v = pt->variableData<V3fVectorData>(velocity_var);
	if ( !p || !v )
	{
		throw Exception("Could not get position and velocity data from primitive!");
	}

	// check P and V are the same length
	std::vector<V3f> &p_data = p->writable();
	const std::vector<V3f> &v_data = v->readable();
	if ( p_data.size()!=v_data.size() )
	{
		throw Exception("Position and velocity variables must be the same length!");
	}

	// update p using v
	if ( !use_pp_sample_length )
	{
		std::vector<V3f>::iterator p_it = p_data.begin();
		std::vector<V3f>::const_iterator v_it = v_data.begin();
		for ( p_it=p_data.begin(), v_it=v_data.begin();
				p_it!=p_data.end(); ++p_it, ++v_it )
		{
			(*p_it) += (*v_it) * sample_length;
		}
	}
	else
	{
		// check for pp sample length variable
		if( pt->variables.count(sample_length_var)==0 )
		{
			throw Exception( "Could not find sample length variable on primitive!" );
		}

		// get data from parameter
		FloatVectorData *s = pt->variableData<FloatVectorData>(sample_length_var);
		if ( !s )
		{
			throw Exception("Could not get scale length data from primitive!");
		}

		// check size against p_data
		const std::vector<float> &s_data = s->readable();
		if ( s_data.size()!=p_data.size() )
		{
			throw Exception("Position and scale length variables must be the same length!");
		}

		std::vector<V3f>::iterator p_it = p_data.begin();
		std::vector<V3f>::const_iterator v_it = v_data.begin();
		std::vector<float>::const_iterator s_it = s_data.begin();
		for ( p_it=p_data.begin(), v_it=v_data.begin(), s_it=s_data.begin();
				p_it!=p_data.end(); ++p_it, ++v_it, ++s_it )
		{
			(*p_it) += (*v_it) * (*s_it) * sample_length;
		}
	}
    */
}
