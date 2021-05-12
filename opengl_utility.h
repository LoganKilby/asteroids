/* date = March 28th 2021 2:59 am */

#ifndef OPENGL_UTILITY_H
#define OPENGL_UTILITY_H

static void
OpenGLInitializeModelBuffer(GLuint *, GLuint *, 
                            float *,
                            int,
                            int,
                            GLenum);
static void
OpenGLUpdateAspectRatio(int, int);

static GLuint
OpenGLCreateProgram(unsigned int*, int);

static GLuint
OpenGLCreateShader(GLenum, char*);

struct open_gl_viewport
{
    GLint x;
    GLint y;
    GLsizei width;
    GLsizei height;
};
#endif //OPENGL_UTILITY_H
