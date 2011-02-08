#ifndef OCEANOP_H__
#define OCEANOP_H__

#include <IECore/ModifyOp.h>
using namespace IECore;

namespace CortexOcean
{
    class OceanOp : public ModifyOp
    {
    public:
        IE_CORE_DECLARERUNTIMETYPEDEXTENSION( OceanOp, 666001, ModifyOp );
        OceanOp();
        virtual ~OceanOp();

        /*
		StringParameter * positionVarParameter();
		const StringParameter * positionVarParameter() const;
		StringParameter * velocityVarParameter();
		const StringParameter * velocityVarParameter() const;
		FloatParameter * sampleLengthParameter();
		const FloatParameter * sampleLengthParameter() const;
		StringParameter * sampleLengthVarParameter();
		const StringParameter * sampleLengthVarParameter() const;
        */

    protected:
        virtual void modify( Object *input, const CompoundObject *operands );

	private :
        /*
		StringParameterPtr m_positionVarParameter;
		StringParameterPtr m_velocityVarParameter;
		FloatParameterPtr m_sampleLengthParameter;
		StringParameterPtr m_sampleLengthVarParameter;
        */
    };

    IE_CORE_DECLAREPTR( OceanOp );
}

#endif
