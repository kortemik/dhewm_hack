/*
===========================================================================

Doom 3 BFG Edition GPL Source Code
Copyright (C) 1993-2012 id Software LLC, a ZeniMax Media company. 

This file is part of the Doom 3 BFG Edition GPL Source Code ("Doom 3 BFG Edition Source Code").  

Doom 3 BFG Edition Source Code is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

Doom 3 BFG Edition Source Code is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Doom 3 BFG Edition Source Code.  If not, see <http://www.gnu.org/licenses/>.

In addition, the Doom 3 BFG Edition Source Code is also subject to certain additional terms. You should have received a copy of these additional terms immediately following the terms and conditions of the GNU General Public License which accompanied the Doom 3 BFG Edition Source Code.  If not, please request a copy in writing from id Software at the address below.

If you have questions concerning this license or the applicable additional terms, you may contact in writing id Software LLC, c/o ZeniMax Media Inc., Suite 120, Rockville, Maryland 20850 USA.

===========================================================================
*/
#ifndef __RENDERPROGS_H__
#define __RENDERPROGS_H__

#include "idlib/containers/StaticList.h"

static const int PC_ATTRIB_INDEX_VERTEX		= 0;
static const int PC_ATTRIB_INDEX_COLOR		= 3;
static const int PC_ATTRIB_INDEX_ST			= 8;
static const int PC_ATTRIB_INDEX_TANGENT	= 9;
static const int PC_ATTRIB_INDEX_BITANGENT	= 10;
static const int PC_ATTRIB_INDEX_NORMAL		= 11;

// This enum list corresponds to the global constant register indecies as defined in global.inc for all
// shaders.  We used a shared pool to keeps things simple.  If something changes here then it also
// needs to change in global.inc and vice versa
enum renderParm_t {
	// For backwards compatibility, do not change the order of the first 17 items
	RENDERPARM_SCREENCORRECTIONFACTOR = 0,
	RENDERPARM_WINDOWCOORD,
	RENDERPARM_DIFFUSEMODIFIER,
	RENDERPARM_SPECULARMODIFIER,

	RENDERPARM_LOCALLIGHTORIGIN,
	RENDERPARM_LOCALVIEWORIGIN,

	RENDERPARM_LIGHTPROJECTION_S,
	RENDERPARM_LIGHTPROJECTION_T,
	RENDERPARM_LIGHTPROJECTION_Q,
	RENDERPARM_LIGHTFALLOFF_S,

	RENDERPARM_BUMPMATRIX_S,
	RENDERPARM_BUMPMATRIX_T,

	RENDERPARM_DIFFUSEMATRIX_S,
	RENDERPARM_DIFFUSEMATRIX_T,

	RENDERPARM_SPECULARMATRIX_S,
	RENDERPARM_SPECULARMATRIX_T,

	RENDERPARM_VERTEXCOLOR_MODULATE,
	RENDERPARM_VERTEXCOLOR_ADD,

	// The following are new and can be in any order
	
	RENDERPARM_COLOR,
	RENDERPARM_VIEWORIGIN,
	RENDERPARM_GLOBALEYEPOS,

	RENDERPARM_MVPMATRIX_X,
	RENDERPARM_MVPMATRIX_Y,
	RENDERPARM_MVPMATRIX_Z,
	RENDERPARM_MVPMATRIX_W,

	RENDERPARM_MODELMATRIX_X,
	RENDERPARM_MODELMATRIX_Y,
	RENDERPARM_MODELMATRIX_Z,
	RENDERPARM_MODELMATRIX_W,

	RENDERPARM_PROJMATRIX_X,
	RENDERPARM_PROJMATRIX_Y,
	RENDERPARM_PROJMATRIX_Z,
	RENDERPARM_PROJMATRIX_W,

	RENDERPARM_MODELVIEWMATRIX_X,
	RENDERPARM_MODELVIEWMATRIX_Y,
	RENDERPARM_MODELVIEWMATRIX_Z,
	RENDERPARM_MODELVIEWMATRIX_W,

	RENDERPARM_TEXTUREMATRIX_S,
	RENDERPARM_TEXTUREMATRIX_T,

	RENDERPARM_TEXGEN_0_S,
	RENDERPARM_TEXGEN_0_T,
	RENDERPARM_TEXGEN_0_Q,
	RENDERPARM_TEXGEN_0_ENABLED,

	RENDERPARM_TEXGEN_1_S,
	RENDERPARM_TEXGEN_1_T,
	RENDERPARM_TEXGEN_1_Q,
	RENDERPARM_TEXGEN_1_ENABLED,

	RENDERPARM_WOBBLESKY_X,
	RENDERPARM_WOBBLESKY_Y,
	RENDERPARM_WOBBLESKY_Z,

	RENDERPARM_OVERBRIGHT,
	RENDERPARM_ENABLE_SKINNING,
	RENDERPARM_ALPHA_TEST,

	RENDERPARM_TOTAL,
	RENDERPARM_USER = 128,
};


struct glslUniformLocation_t {
	int		parmIndex;
	GLint	uniformIndex;
};



/*
================================================================================================
idRenderProgManager
================================================================================================
*/
class idRenderProgManager {
public:
	idRenderProgManager();
	virtual ~idRenderProgManager();

	void	Init();
	void	Shutdown();

	void	SetRenderParm( renderParm_t rp, const float * value );
	void	SetRenderParms( renderParm_t rp, const float * values, int numValues );

	int		FindVertexShader( const char * name );
	int		FindFragmentShader( const char * name );

	void	BindShader( int vIndex, int fIndex );

	void	BindShader_GUI( ) { BindShader_Builtin( BUILTIN_GUI ); }
	void	BindShader_Color( ) { BindShader_Builtin( BUILTIN_COLOR ); }
	void	BindShader_Texture( ) { BindShader_Builtin( BUILTIN_TEXTURED ); }
	void	BindShader_TextureVertexColor() { BindShader_Builtin( BUILTIN_TEXTURE_VERTEXCOLOR ); };
	void	BindShader_TextureTexGenVertexColor() { BindShader_Builtin( BUILTIN_TEXTURE_TEXGEN_VERTEXCOLOR ); };
	void	BindShader_Interaction()  { BindShader_Builtin( BUILTIN_INTERACTION ); }
	void	BindShader_InteractionAmbient()  { BindShader_Builtin( BUILTIN_INTERACTION_AMBIENT ); }
	void	BindShader_SimpleShade() { BindShader_Builtin( BUILTIN_SIMPLESHADE ); }
	void	BindShader_Environment() { BindShader_Builtin( BUILTIN_ENVIRONMENT ); }
	void	BindShader_BumpyEnvironment() { BindShader_Builtin( BUILTIN_BUMPY_ENVIRONMENT ); }

	void	BindShader_Depth() { BindShader_Builtin( BUILTIN_DEPTH ); }
	void	BindShader_Shadow() { BindShader( builtinShaders[BUILTIN_SHADOW], -1 ); }
	void	BindShader_ShadowDebug() { BindShader_Builtin( BUILTIN_SHADOW_DEBUG ); }

	void	BindShader_BlendLight() { BindShader_Builtin( BUILTIN_BLENDLIGHT ); }
	void	BindShader_Fog() { BindShader_Builtin( BUILTIN_FOG ); }
	void	BindShader_SkyBox() { BindShader_Builtin( BUILTIN_SKYBOX ); }
	void	BindShader_WobbleSky() { BindShader_Builtin( BUILTIN_WOBBLESKY ); }
	void	BindShader_StereoDeGhost() { BindShader_Builtin( BUILTIN_STEREO_DEGHOST ); }
	void	BindShader_StereoWarp() { BindShader_Builtin( BUILTIN_STEREO_WARP ); }
	void	BindShader_StereoInterlace() { BindShader_Builtin( BUILTIN_STEREO_INTERLACE ); }
	void	BindShader_PostProcess() { BindShader_Builtin( BUILTIN_POSTPROCESS ); }
	void	BindShader_MotionBlur() { BindShader_Builtin( BUILTIN_MOTION_BLUR); }

	// unbind the currently bound render program
	void	Unbind();

	// this should only be called via the reload shader console command
	void	LoadAllShaders();
	void	KillAllShaders();

	static const int	MAX_GLSL_USER_PARMS = 8;
	const char*	GetGLSLParmName( int rp ) const;
	int			GetGLSLCurrentProgram() const { return currentRenderProgram; }
	void		SetUniformValue( const renderParm_t rp, const float * value );
	void		CommitUniforms();
	int			FindGLSLProgram( const char* name, int vIndex, int fIndex );
	void		ZeroUniforms();

protected:
	void	LoadVertexShader( int index );
	void	LoadFragmentShader( int index );

	enum {
		BUILTIN_GUI,
		BUILTIN_COLOR,
		BUILTIN_SIMPLESHADE,
		BUILTIN_TEXTURED,
		BUILTIN_TEXTURE_VERTEXCOLOR,
		BUILTIN_TEXTURE_TEXGEN_VERTEXCOLOR,
		BUILTIN_INTERACTION,
		BUILTIN_INTERACTION_AMBIENT,
		BUILTIN_ENVIRONMENT,
		BUILTIN_BUMPY_ENVIRONMENT,

		BUILTIN_DEPTH,
		BUILTIN_SHADOW,
		BUILTIN_SHADOW_DEBUG,

		BUILTIN_BLENDLIGHT,
		BUILTIN_FOG,
		BUILTIN_SKYBOX,
		BUILTIN_WOBBLESKY,
		BUILTIN_POSTPROCESS,
		BUILTIN_STEREO_DEGHOST,
		BUILTIN_STEREO_WARP,
		BUILTIN_STEREO_INTERLACE,
		BUILTIN_MOTION_BLUR,

		MAX_BUILTINS
	};
	int builtinShaders[MAX_BUILTINS];
	void BindShader_Builtin( int i ) { BindShader( builtinShaders[i], builtinShaders[i] ); }

	bool	CompileGLSL( GLenum target, const char * name );
	GLuint	LoadGLSLShader( GLenum target, const char * name, idList<int> & uniforms );
	void	LoadGLSLProgram( const int programIndex, const int vertexShaderIndex, const int fragmentShaderIndex );

	static const GLuint INVALID_PROGID = 0xFFFFFFFF;

	struct vertexShader_t {
					vertexShader_t() : progId( INVALID_PROGID ) {}
		idStr		name;
		GLuint		progId;
		idList<int>	uniforms;
	};
	struct fragmentShader_t {
					fragmentShader_t() : progId( INVALID_PROGID ) {}
		idStr		name;
		GLuint		progId;
		idList<int>	uniforms;
	};

	struct glslProgram_t {
		glslProgram_t() :	progId( INVALID_PROGID ),
							vertexShaderIndex( -1 ),
							fragmentShaderIndex( -1 ),
							vertexUniformArray( -1 ),
							fragmentUniformArray( -1 ) {}
		idStr		name;
		GLuint		progId;
		int			vertexShaderIndex;
		int			fragmentShaderIndex;
		GLint		vertexUniformArray;
		GLint		fragmentUniformArray;
		idList<glslUniformLocation_t> uniformLocations;
	};
	int	currentRenderProgram;
	idList<glslProgram_t> glslPrograms;
	idStaticList<idVec4, RENDERPARM_USER + MAX_GLSL_USER_PARMS> glslUniforms;


	int				currentVertexShader;
	int				currentFragmentShader;
	idList<vertexShader_t> vertexShaders;
	idList<fragmentShader_t> fragmentShaders;
};

extern idRenderProgManager renderProgManager;

#endif
