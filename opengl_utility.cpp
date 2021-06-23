#include "win32_asteroids.h"

static void
OpenGLRenderText(font_buffer *fontBuffer, char *text, glm::vec2 pos, float scale)
{
    glActiveTexture(GL_TEXTURE0);
    int messageLength = strlen(text);
    glyph character = {};
    
    for(int messageIndex = 0; messageIndex < messageLength; ++messageIndex)
    {
        character.textureID = fontBuffer->textureID[text[messageIndex]];
        character.size = fontBuffer->size[text[messageIndex]];
        character.bearing = fontBuffer->bearing[text[messageIndex]];
        character.advance = fontBuffer->advance[text[messageIndex]];
        
        float xpos = pos.x + character.bearing.x * scale;
        float ypos = pos.y - (character.size.y - character.bearing.y) * scale;
        
        float w = character.size.x * scale;
        float h = character.size.y * scale;
        
        // TODO: It wouldn't be a big deal to cache all of the vertex data for every glyph.
        
        float vertices[24] = {
            xpos,     ypos + h,   0.0f, 0.0f,            
            xpos,     ypos,       0.0f, 1.0f,
            xpos + w, ypos,       1.0f, 1.0f,
            
            xpos,     ypos + h,   0.0f, 0.0f,
            xpos + w, ypos,       1.0f, 1.0f,
            xpos + w, ypos + h,   1.0f, 0.0f 
        };
        
        glBindTexture(GL_TEXTURE_2D, character.textureID);
        glBindBuffer(GL_ARRAY_BUFFER, fontBuffer->vertexBuffer);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glDisableVertexAttribArray(0);
        glBindTexture(GL_TEXTURE_2D, 0);
        
        pos.x += (character.advance >> 6) * scale; // bitshift by 6 to get value in pixels (2^6 = 64)
    }
}

static void
OpenGLDraw(GLuint vertexAttribute, GLenum drawEnum, int start, int vertexCount)
{
    glEnableVertexAttribArray(0);
    glBindVertexArray(vertexAttribute);
    glDrawArrays(drawEnum, start, vertexCount);
    glDisableVertexAttribArray(0);
}

static void
OpenGLInitializeModelBuffer(GLuint *vertexBuffer, GLuint *vertexAttributeObject, 
                            float *vertexData,
                            int vertexSize,
                            int bufferSize,
                            GLenum usage)
{
    glGenVertexArrays(1, vertexAttributeObject);
    glGenBuffers(1, vertexBuffer);
    glBindVertexArray(*vertexAttributeObject); 
    glBindBuffer(GL_ARRAY_BUFFER, *vertexBuffer);
    if(vertexData)
    {
        glBufferData(GL_ARRAY_BUFFER, bufferSize, vertexData, usage); 
    }
    else
    {
        glBufferData(GL_ARRAY_BUFFER, bufferSize, NULL, usage); 
    }
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, vertexSize, GL_FLOAT, GL_FALSE,
                          vertexSize * sizeof(float), (void*)0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

static void
OpenGLUpdateAspectRatio(int width, int height)
{
    // Aspect ratio
    // https://gist.github.com/ForeverZer0/6614babfabf2f4be83267d8f609970b2
    
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0f, width, height, 0.0f, 1.0f, -1.0f);
    
    float ratioX = width / (float)WINDOW_WIDTH;
    float ratioY = height / (float)WINDOW_HEIGHT;
    float ratio = ratioX < ratioY ? ratioX : ratioY;
    
    int viewWidth = WINDOW_WIDTH * ratio;
    int viewHeight = WINDOW_HEIGHT * ratio;
    
    int viewX = (width - (WINDOW_WIDTH * ratio)) / 2;
    int viewY = (height - (WINDOW_HEIGHT * ratio)) / 2;
    
    glViewport(viewX, viewY, viewWidth, viewHeight);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

static GLuint
OpenGLCreateProgram(unsigned int *shaderList, int shaderCount)
{
    bool shadersValid = true;
    for(int shader = 0; shader < shaderCount; ++shader)
    {
        if(shaderList[shader] == 0)
        {
            shadersValid = false;
        }
    }
    
    if(!shadersValid)
    {
        return 0;
    }
    
    GLuint program = glCreateProgram();
    
    for (int shader = 0; shader < shaderCount; ++shader)
    {
        glAttachShader(program, shaderList[shader]);
    }
    
    glLinkProgram(program);
    
    GLint status;
    glGetProgramiv(program, GL_LINK_STATUS, &status);
    if(status == GL_FALSE)
    {
        GLint infoLength;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &infoLength);
        
        GLchar *info = (GLchar *)malloc(infoLength + 1);
        glGetProgramInfoLog(program, infoLength, NULL, info);
        // TODO: Logging
        fprintf(stderr, "\nA shader failed to link: %s\n", info);
        free(info);
    }
    
    for (int shader = 0; shader < shaderCount; ++shader)
    {
        glDetachShader(program, shaderList[shader]);
    }
    
    return program;
}

static GLuint
OpenGLCreateShader(GLenum shaderType, char* shaderBuffer)
{
    char *strShaderType = 0;
    switch(shaderType)
    {
        case GL_VERTEX_SHADER: strShaderType = "vertex"; break;
        case GL_GEOMETRY_SHADER: strShaderType = "geometry"; break;
        case GL_FRAGMENT_SHADER: strShaderType = "fragment"; break;
    }
    
    const char *shaderText = shaderBuffer;
    
    GLuint shader = glCreateShader(shaderType);
    glShaderSource(shader, 1, &shaderText, NULL);
    glCompileShader(shader);
    
    GLint status;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
    if(status == GL_FALSE)
    {
        GLint infoLogLength;
        glGetShaderiv(shader,GL_INFO_LOG_LENGTH, &infoLogLength);
        
        GLchar *strInfoLog = (GLchar *)malloc(infoLogLength);
        glGetShaderInfoLog(shader, infoLogLength, NULL, strInfoLog);
        
        // TODO: Logging
        fprintf(stderr, "\nAn error occured when compiling the %s shader\n", strShaderType);
        fprintf(stderr, "%s\n", strInfoLog);
        fprintf(stderr, "%s\n", shaderBuffer);
        free(strInfoLog);
    }
    
    return shader;
}

