﻿#pragma once

#include "GL/glext.h"
#include <GL/gl.h>
#include <GL/glu.h>
#include <windows.h>

#include <string>

extern PFNGLDELETEOBJECTARBPROC glDeleteObjectARB;
extern PFNGLGETHANDLEARBPROC glGetHandleARB;
extern PFNGLDETACHOBJECTARBPROC glDetachObjectARB;
extern PFNGLCREATESHADEROBJECTARBPROC glCreateShaderObjectARB;
extern PFNGLSHADERSOURCEARBPROC glShaderSourceARB;
extern PFNGLCOMPILESHADERARBPROC glCompileShaderARB;
extern PFNGLCREATEPROGRAMOBJECTARBPROC glCreateProgramObjectARB;
extern PFNGLATTACHOBJECTARBPROC glAttachObjectARB;
extern PFNGLLINKPROGRAMARBPROC glLinkProgramARB;
extern PFNGLVALIDATEPROGRAMARBPROC glValidateProgramARB;
extern PFNGLUSEPROGRAMOBJECTARBPROC glUseProgramObjectARB;
extern PFNGLGETOBJECTPARAMETERIVARBPROC glGetObjectParameterivARB;

extern PFNGLGETINFOLOGARBPROC glGetInfoLogARB;

extern PFNGLUNIFORM1FARBPROC glUniform1fARB;
extern PFNGLUNIFORM2FARBPROC glUniform2fARB;
extern PFNGLUNIFORM3FARBPROC glUniform3fARB;
extern PFNGLUNIFORM4FARBPROC glUniform4fARB;

extern PFNGLUNIFORM1FVARBPROC glUniform1fvARB;
extern PFNGLUNIFORM2FVARBPROC glUniform2fvARB;
extern PFNGLUNIFORM3FVARBPROC glUniform3fvARB;
extern PFNGLUNIFORM4FVARBPROC glUniform4fvARB;

extern PFNGLUNIFORM1IARBPROC glUniform1iARB;

extern PFNGLGETUNIFORMLOCATIONARBPROC glGetUniformLocationARB;

extern PFNGLACTIVETEXTUREPROC glActiveTexture;

void initShadersFunctions();
int loadShader(const char* filename, char** shaderSrc, int* programLength);

// Вызываем эту функцию перед рендером для настройки шейдеров
void initShadersFunctions();

class Shader
{
  protected:
  public:
    GLhandleARB program;
    GLhandleARB vertex;
    GLhandleARB fragment;

    std::string VshaderFileName;
    std::string FshaderFileName;

    Shader()
    {
    }

    ~Shader()
    {
    }

    void LoadShaderFromFile();

    void Compile();

    void UseShader();

    static void DontUseShaders();
};

// Загружает код шейдера из файла
//************************************
// Parameter: char * filename      Имя файла
// Parameter: char * * shaderSrc   Указатель на массив символов, в который загрузится код шейдера. После того, как
//   станет ненужным, надо освободить память!
// Parameter: int * programLength  Указатель на длину массива символов.
//************************************
int loadShader(const char* filename, char** shaderSrc, int* programLength);
