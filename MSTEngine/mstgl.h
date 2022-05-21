#pragma once

#ifdef _WIN64
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#endif

#include <gl/GL.h>

#if defined PLATFORM_WEB || defined __EMSCRIPTEN__

#include <EGL/egl.h>
#include <GLES2/gl2.h>
#define GL_GLEXT_PROTOTYPES
#include <GLES2/gl2ext.h>

#ifndef glBindVertexArray
#define glBindVertexArray    glBindVertexArrayOES
#endif
#ifndef glGenVertexArrays
#define glGenVertexArrays    glGenVertexArraysOES
#endif
#ifndef glDeleteVertexArrays
#define glDeleteVertexArrays glDeleteVertexArraysOES
#endif

#endif

bool LoadGLExtensions();

#define GL_ARRAY_BUFFER          0x8892		  
#define GL_STATIC_DRAW           0x88E4		  
#define GL_DYNAMIC_DRAW          0x88E8		  
#define GL_FRAGMENT_SHADER       0x8B30	  
#define GL_VERTEX_SHADER         0x8B31		  
#define GL_COMPILE_STATUS        0x8B81		  
#define GL_LINK_STATUS           0x8B82		  
#define GL_INFO_LOG_LENGTH       0x8B84	  
#define GL_TEXTURE0              0x84C0			  
#define GL_ELEMENT_ARRAY_BUFFER  0x8893 		  
#define GL_CLAMP_TO_EDGE         0x812F

#ifdef _WIN64
typedef void   (APIENTRY* PFNGLATTACHSHADERPROC) (GLuint program, GLuint shader);
extern  PFNGLATTACHSHADERPROC glAttachShader;

typedef void   (APIENTRY* PFNGLBINDBUFFERPROC) (GLenum target, GLuint buffer);
extern  PFNGLBINDBUFFERPROC glBindBuffer;

typedef void   (APIENTRY* PFNGLBINDVERTEXARRAYPROC) (GLuint array);
extern  PFNGLBINDVERTEXARRAYPROC glBindVertexArray;

typedef void   (APIENTRY* PFNGLBUFFERDATAPROC) (GLenum target, ptrdiff_t size, const GLvoid* data, GLenum usage);
extern  PFNGLBUFFERDATAPROC glBufferData;

typedef void   (APIENTRY* PFNGLCOMPILESHADERPROC) (GLuint shader);
extern  PFNGLCOMPILESHADERPROC glCompileShader;

typedef GLuint(APIENTRY* PFNGLCREATEPROGRAMPROC) (void);
extern  PFNGLCREATEPROGRAMPROC glCreateProgram;

typedef GLuint(APIENTRY* PFNGLCREATESHADERPROC) (GLenum type);
extern  PFNGLCREATESHADERPROC glCreateShader;

typedef void   (APIENTRY* PFNGLDELETEBUFFERSPROC) (GLsizei n, const GLuint* buffers);
extern  PFNGLDELETEBUFFERSPROC glDeleteBuffers;

typedef void   (APIENTRY* PFNGLDELETEPROGRAMPROC) (GLuint program);
extern  PFNGLDELETEPROGRAMPROC glDeleteProgram;

typedef void   (APIENTRY* PFNGLDELETESHADERPROC) (GLuint shader);
extern  PFNGLDELETESHADERPROC glDeleteShader;

typedef void   (APIENTRY* PFNGLDELETEVERTEXARRAYSPROC) (GLsizei n, const GLuint* arrays);
extern  PFNGLDELETEVERTEXARRAYSPROC glDeleteVertexArrays;

typedef void   (APIENTRY* PFNGLDETACHSHADERPROC) (GLuint program, GLuint shader);
extern  PFNGLDETACHSHADERPROC glDetachShader;

typedef void   (APIENTRY* PFNGLENABLEVERTEXATTRIBARRAYPROC) (GLuint index);
extern  PFNGLENABLEVERTEXATTRIBARRAYPROC glEnableVertexAttribArray;

typedef void   (APIENTRY* PFNGLGENBUFFERSPROC) (GLsizei n, GLuint* buffers);
extern  PFNGLGENBUFFERSPROC glGenBuffers;

typedef void   (APIENTRY* PFNGLGENVERTEXARRAYSPROC) (GLsizei n, GLuint* arrays);
extern  PFNGLGENVERTEXARRAYSPROC glGenVertexArrays;

typedef GLint(APIENTRY* PFNGLGETATTRIBLOCATIONPROC) (GLuint program, const char* name);
extern  PFNGLGETATTRIBLOCATIONPROC glGetAttribLocation;

typedef void   (APIENTRY* PFNGLGETPROGRAMINFOLOGPROC) (GLuint program, GLsizei bufSize, GLsizei* length, char* infoLog);
extern  PFNGLGETPROGRAMINFOLOGPROC glGetProgramInfoLog;

typedef void   (APIENTRY* PFNGLGETPROGRAMIVPROC) (GLuint program, GLenum pname, GLint* params);
extern  PFNGLGETPROGRAMIVPROC glGetProgramiv;

typedef void   (APIENTRY* PFNGLGETSHADERINFOLOGPROC) (GLuint shader, GLsizei bufSize, GLsizei* length, char* infoLog);
extern  PFNGLGETSHADERINFOLOGPROC glGetShaderInfoLog;

typedef void   (APIENTRY* PFNGLGETSHADERIVPROC) (GLuint shader, GLenum pname, GLint* params);
extern  PFNGLGETSHADERIVPROC glGetShaderiv;

typedef void   (APIENTRY* PFNGLLINKPROGRAMPROC) (GLuint program);
extern  PFNGLLINKPROGRAMPROC glLinkProgram;

typedef void   (APIENTRY* PFNGLSHADERSOURCEPROC) (GLuint shader, GLsizei count, const char** string, const GLint* length);
extern  PFNGLSHADERSOURCEPROC glShaderSource;

typedef void   (APIENTRY* PFNGLUSEPROGRAMPROC) (GLuint program);
extern  PFNGLUSEPROGRAMPROC glUseProgram;

typedef void   (APIENTRY* PFNGLVERTEXATTRIBPOINTERPROC) (GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const GLvoid* pointer);
extern  PFNGLVERTEXATTRIBPOINTERPROC glVertexAttribPointer;

typedef void   (APIENTRY* PFNGLBINDATTRIBLOCATIONPROC) (GLuint program, GLuint index, const char* name);
extern  PFNGLBINDATTRIBLOCATIONPROC glBindAttribLocation;

typedef GLint(APIENTRY* PFNGLGETUNIFORMLOCATIONPROC) (GLuint program, const char* name);
extern  PFNGLGETUNIFORMLOCATIONPROC glGetUniformLocation;

typedef void   (APIENTRY* PFNGLUNIFORMMATRIX4FVPROC) (GLint location, GLsizei count, GLboolean transpose, const GLfloat* value);
extern  PFNGLUNIFORMMATRIX4FVPROC glUniformMatrix4fv;

typedef void   (APIENTRY* PFNGLACTIVETEXTUREPROC) (GLenum texture);
extern  PFNGLACTIVETEXTUREPROC glActiveTexture;

typedef void   (APIENTRY* PFNGLUNIFORM1IPROC) (GLint location, GLint v0);
extern  PFNGLUNIFORM1IPROC glUniform1i;

typedef void   (APIENTRY* PFNGLUNIFORM1FPROC) (GLint location, GLfloat v0);
extern  PFNGLUNIFORM1FPROC glUniform1f;

typedef void   (APIENTRY* PFNGLGENERATEMIPMAPPROC) (GLenum target);
extern  PFNGLGENERATEMIPMAPPROC glGenerateMipmap;

typedef void   (APIENTRY* PFNGLDISABLEVERTEXATTRIBARRAYPROC) (GLuint index);
extern  PFNGLDISABLEVERTEXATTRIBARRAYPROC glDisableVertexAttribArray;

typedef void   (APIENTRY* PFNGLUNIFORM2FVPROC) (GLint location, GLsizei count, const GLfloat* value);
extern  PFNGLUNIFORM2FVPROC glUniform2fv;

typedef void   (APIENTRY* PFNGLUNIFORM3FVPROC) (GLint location, GLsizei count, const GLfloat* value);
extern  PFNGLUNIFORM3FVPROC glUniform3fv;

typedef void   (APIENTRY* PFNGLUNIFORM4FVPROC) (GLint location, GLsizei count, const GLfloat* value);
extern  PFNGLUNIFORM4FVPROC glUniform4fv;

typedef void   (APIENTRY* PFNGLBUFFERSUBDATAPROC)  (GLenum target, ptrdiff_t offset, ptrdiff_t size, const void* data);
extern  PFNGLBUFFERSUBDATAPROC glBufferSubData;

#endif // _WIN64

#if defined MST_GL_IMPLEMENTATION

#pragma region Windows Implementation
#if defined _WIN64

PFNGLATTACHSHADERPROC glAttachShader;
PFNGLBINDBUFFERPROC glBindBuffer;
PFNGLBINDVERTEXARRAYPROC glBindVertexArray;
PFNGLBUFFERDATAPROC glBufferData;
PFNGLCOMPILESHADERPROC glCompileShader;
PFNGLCREATEPROGRAMPROC glCreateProgram;
PFNGLCREATESHADERPROC glCreateShader;
PFNGLDELETEBUFFERSPROC glDeleteBuffers;
PFNGLDELETEPROGRAMPROC glDeleteProgram;
PFNGLDELETESHADERPROC glDeleteShader;
PFNGLDELETEVERTEXARRAYSPROC glDeleteVertexArrays;
PFNGLDETACHSHADERPROC glDetachShader;
PFNGLENABLEVERTEXATTRIBARRAYPROC glEnableVertexAttribArray;
PFNGLGENBUFFERSPROC glGenBuffers;
PFNGLGENVERTEXARRAYSPROC glGenVertexArrays;
PFNGLGETATTRIBLOCATIONPROC glGetAttribLocation;
PFNGLGETPROGRAMINFOLOGPROC glGetProgramInfoLog;
PFNGLGETPROGRAMIVPROC glGetProgramiv;
PFNGLGETSHADERINFOLOGPROC glGetShaderInfoLog;
PFNGLGETSHADERIVPROC glGetShaderiv;
PFNGLLINKPROGRAMPROC glLinkProgram;
PFNGLSHADERSOURCEPROC glShaderSource;
PFNGLUSEPROGRAMPROC glUseProgram;
PFNGLVERTEXATTRIBPOINTERPROC glVertexAttribPointer;
PFNGLBINDATTRIBLOCATIONPROC glBindAttribLocation;
PFNGLGETUNIFORMLOCATIONPROC glGetUniformLocation;
PFNGLUNIFORMMATRIX4FVPROC glUniformMatrix4fv;
PFNGLACTIVETEXTUREPROC glActiveTexture;
PFNGLUNIFORM1IPROC glUniform1i;
PFNGLUNIFORM1FPROC glUniform1f;
PFNGLGENERATEMIPMAPPROC glGenerateMipmap;
PFNGLDISABLEVERTEXATTRIBARRAYPROC glDisableVertexAttribArray;
PFNGLUNIFORM2FVPROC glUniform2fv;
PFNGLUNIFORM3FVPROC glUniform3fv;
PFNGLUNIFORM4FVPROC glUniform4fv;
PFNGLBUFFERSUBDATAPROC glBufferSubData;

bool LoadGLExtensions()
{
#define LOAD_GL_EXTENSION(type, fn) fn = (type)wglGetProcAddress(#fn); if (fn == nullptr) {return false;} 
	{
		LOAD_GL_EXTENSION(PFNGLATTACHSHADERPROC, glAttachShader);
		LOAD_GL_EXTENSION(PFNGLBINDBUFFERPROC, glBindBuffer);
		LOAD_GL_EXTENSION(PFNGLBINDVERTEXARRAYPROC, glBindVertexArray);
		LOAD_GL_EXTENSION(PFNGLBUFFERDATAPROC, glBufferData);
		LOAD_GL_EXTENSION(PFNGLCOMPILESHADERPROC, glCompileShader);
		LOAD_GL_EXTENSION(PFNGLCREATEPROGRAMPROC, glCreateProgram);
		LOAD_GL_EXTENSION(PFNGLCREATESHADERPROC, glCreateShader);
		LOAD_GL_EXTENSION(PFNGLDELETEBUFFERSPROC, glDeleteBuffers);
		LOAD_GL_EXTENSION(PFNGLDELETEPROGRAMPROC, glDeleteProgram);
		LOAD_GL_EXTENSION(PFNGLDELETESHADERPROC, glDeleteShader);
		LOAD_GL_EXTENSION(PFNGLDELETEVERTEXARRAYSPROC, glDeleteVertexArrays);
		LOAD_GL_EXTENSION(PFNGLDETACHSHADERPROC, glDetachShader);
		LOAD_GL_EXTENSION(PFNGLENABLEVERTEXATTRIBARRAYPROC, glEnableVertexAttribArray);
		LOAD_GL_EXTENSION(PFNGLGENBUFFERSPROC, glGenBuffers);
		LOAD_GL_EXTENSION(PFNGLGENVERTEXARRAYSPROC, glGenVertexArrays);
		LOAD_GL_EXTENSION(PFNGLGETATTRIBLOCATIONPROC, glGetAttribLocation);
		LOAD_GL_EXTENSION(PFNGLGETPROGRAMINFOLOGPROC, glGetProgramInfoLog);
		LOAD_GL_EXTENSION(PFNGLGETPROGRAMIVPROC, glGetProgramiv);
		LOAD_GL_EXTENSION(PFNGLGETSHADERINFOLOGPROC, glGetShaderInfoLog);
		LOAD_GL_EXTENSION(PFNGLGETSHADERIVPROC, glGetShaderiv);
		LOAD_GL_EXTENSION(PFNGLLINKPROGRAMPROC, glLinkProgram);
		LOAD_GL_EXTENSION(PFNGLSHADERSOURCEPROC, glShaderSource);
		LOAD_GL_EXTENSION(PFNGLUSEPROGRAMPROC, glUseProgram);
		LOAD_GL_EXTENSION(PFNGLVERTEXATTRIBPOINTERPROC, glVertexAttribPointer);
		LOAD_GL_EXTENSION(PFNGLBINDATTRIBLOCATIONPROC, glBindAttribLocation);
		LOAD_GL_EXTENSION(PFNGLGETUNIFORMLOCATIONPROC, glGetUniformLocation);
		LOAD_GL_EXTENSION(PFNGLUNIFORMMATRIX4FVPROC, glUniformMatrix4fv);
		LOAD_GL_EXTENSION(PFNGLACTIVETEXTUREPROC, glActiveTexture);
		LOAD_GL_EXTENSION(PFNGLUNIFORM1IPROC, glUniform1i);
		LOAD_GL_EXTENSION(PFNGLUNIFORM1FPROC, glUniform1f);
		LOAD_GL_EXTENSION(PFNGLGENERATEMIPMAPPROC, glGenerateMipmap);
		LOAD_GL_EXTENSION(PFNGLDISABLEVERTEXATTRIBARRAYPROC, glDisableVertexAttribArray);
		LOAD_GL_EXTENSION(PFNGLUNIFORM2FVPROC, glUniform2fv);
		LOAD_GL_EXTENSION(PFNGLUNIFORM3FVPROC, glUniform3fv);
		LOAD_GL_EXTENSION(PFNGLUNIFORM4FVPROC, glUniform4fv);
		LOAD_GL_EXTENSION(PFNGLBUFFERSUBDATAPROC, glBufferSubData);
	}
#undef LOAD_GL_EXTENSION

	return true;
}
#endif   //WIN 64

#if defined PLATFORM_WEB
bool LoadGLExtensions()
{
	// I think that exmscritpen technically handles this for me
	return true;
}
#endif

#pragma endregion // WINDOWS IMPLEMENTAION
#endif //MST_GL_IMPLEMENTATION
