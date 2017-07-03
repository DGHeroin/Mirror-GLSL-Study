/*
-----------------------------------------------------------------------------
This source file is part of OGRE
(Object-oriented Graphics Rendering Engine)
For the latest info, see http://www.ogre3d.org

Copyright (c) 2000-2012 Torus Knot Software Ltd
Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
-----------------------------------------------------------------------------
*/
#ifndef _ShaderGLSLESProgramProcessor_
#define _ShaderGLSLESProgramProcessor_

#include "OgreShaderPrerequisites.h"
#include "OgreShaderProgramProcessor.h"
#include "OgreStringVector.h"

namespace Ogre {
namespace RTShader {

/** \addtogroup Core
*  @{
*/
/** \addtogroup RTShader
*  @{
*/

/** GLSL ES Language program processor class.
*/
class GLSLESProgramProcessor : public ProgramProcessor
{

// Interface.
public:	

	/** Class constructor.
	@param type The type of this program.
	*/
	GLSLESProgramProcessor			();

	/** Class destructor */
	virtual ~GLSLESProgramProcessor ();

	/** Return the target language of this processor. */
	virtual const String&           getTargetLanguage	() const { return TargetLanguage; }

	/** 
	@see ProgramProcessor::preCreateGpuPrograms
    */
	virtual bool                    preCreateGpuPrograms			(ProgramSet* programSet);

	/** 
	@see ProgramProcessor::postCreateGpuPrograms
	*/
	virtual bool                    postCreateGpuPrograms			(ProgramSet* programSet);


private:

	/** Bind texture samplers. */
	void                            bindTextureSamplers				(Program* pCpuProgram, GpuProgramPtr pGpuProgram);

	static String                   TargetLanguage;

	StringVector                    mLibraryPrograms;		// The list of created library shaders.
};


/** @} */
/** @} */

}
}

#endif

