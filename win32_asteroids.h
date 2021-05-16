/* date = March 24th 2021 8:47 pm */
#include "libs\glm\glm\glm.hpp"
#include "utility.h"

#ifndef WIN32_ASTEROIDS_H
#define WIN32_ASTEROIDS_H

#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 720

struct font_buffer
{
    GLuint vertexAttribute;
    GLuint vertexBuffer;
    unsigned int textureID[128];
    glm::ivec2 size[128];
    glm::ivec2 bearing[128];
    unsigned int advance[128];
};

struct glyph
{
    unsigned int textureID;
    glm::ivec2 size;
    glm::ivec2 bearing;
    unsigned int advance;
};

struct window_dimensions
{
    int width;
    int height;
};

struct open_gl_state
{
    double secondsElapsed;
    double frameTime;
    file_buffer vertexShader;
    file_buffer fragmentShader;
    GLuint shaderProgram;
    GLuint fontShaderProgram;
    
    GLint rMatrixUniform;
    GLint projectionUniform;
    GLint fragmentShaderUniform;
    GLint fontProjectionUniform;
    GLint fontTextColorUniform;
    glm::mat4 projectionMatrix;
    glm::mat4 fontProjectionMatrix;
    int IsInitialized;
};

// TODO: decide whether it's better to have projectiles[50] instead of a pointer to an array of unknown
// size. If I want to store the game memory in one place, I think I'll have to know that there is room
// for n projectiles, asteroids, etc
struct projectile
{
    unsigned int isActive;
    float angle;
    glm::vec2 position;
    glm::vec2 wrappedPositions[6];
};

struct projectile_data
{
    GLuint vertexAttribute;
    GLuint vertexBuffer;
    unsigned int fireProjectile;
    int lastProjTimeStamp;
    int currentCount;
    int maxCount;
    float speed;
    glm::vec2 size; 
    projectile *projectiles;
    
};

struct asteroid
{
    // TODO: make isActive and isSmall bit flags
    int isActive;
    int isSmall;
    int bufferIndex;
    
    float movementAngle;
    float movementSpeed;
    float rotationAngle;
    float rotationSpeed;
    
    glm::vec2 size;
    glm::vec2 position;
    glm::vec2 wrappedPositions[6];
};

struct asteroid_data
{
    GLuint vertexAttributes[3];
    GLuint vertexBuffers[3];
    int bufferSelected; //0, 1, 2
    int bufferVertexCount[3];
    
    GLuint collisionVertexAttributes[3];
    GLuint collisionVertexBuffers[3];
    int collisionVertexCount[3];
    
    int maxCount;
    int currentCount;
    asteroid *asteroids;
};

struct ship_data
{
    GLuint collisionVertexAttribute;
    GLuint collisionVertexBuffer;
    
    GLuint vertexAttribute;
    GLuint vertexBuffer;
    int vertexCount;
    
    GLuint thrusterAttribute;
    GLuint thrusterBuffer;
    
    glm::vec2 position;
    glm::vec2 wrappedPositions[6];
    glm::vec2 size;
    
    // TODO: bit flags
    int isVisible;
    int isAnimating;
    int isThrusterActive;
    int collisionTimeout;
    
    float collisionMaxTimeout;
    float animationFrequency;
    float animationCounter;
    
    float angle;
    float movementAngle;
    float currentSpeed;
    float turnSpeed;
};

struct input_state
{
    int key;
    int wasDown;
    int isDown;
    int keyBuffer;
    
    int isRepeating;
    int repeatingKey;
};

struct game_state
{
    int stage;
    int maxLives;
    int livesLeft;
    int score;
    file_buffer highScores;
    int topTenScores[10];
};

#endif //WIN32_ASTEROIDS_H
