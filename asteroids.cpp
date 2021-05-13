#include "vertex_data.h"
#include "win32_asteroids.h"
#include "math.h"
#include "opengl_utility.h"
#include "asteroids.h"
#include "time.h"
#include "collision.h"

#define W_KEY 87
#define A_KEY 65
#define S_KEY 83
#define D_KEY 68
#define F_KEY 70
#define SPACE_KEY 32

#define SHIP_ACCELERATION 400.0f

#define ArrayCount(Array) (sizeof(Array) / sizeof((Array)[0]))

#define GLT_ENABLED 0

static void
ReverseString(char *input)
{
    int length = strlen(input);
    int lastPos = length - 1;
    char tmp;
    for(int i = 0; i < length / 2; i++)
    {
        tmp = input[i];
        input[i] = input[lastPos - i];
        input[lastPos - i] = tmp;
    }
}

static int
IntArrayToInt(int *numbers, int size)
{
    int result = 0;
    int powSize = size - 1;
    for(int i = 0; i < size; ++i)
    {
        if(powSize)
        {
            result += numbers[i] * pow((double)10, (double)powSize);
            --powSize;
        }
    }
    
    return result;
}

static float
NormalizeAngle(float a)
{
    float result = 0;
    unsigned int fullRotations = fabs(a / 360);
    if(a < 0)
    {
        a += (360 * fullRotations) + 360;
    }
    else if(a > 0)
    {
        a -= 360 * fullRotations;
    }
    
    return a;
}

static void
SpawnAsteroid(asteroid_data *asteroidData, glm::vec2 position, glm::vec2 size, int isSmall)
{
    if(asteroidData->currentCount == asteroidData->maxCount)
    {
        return;
    }
    
    int bufferIndex = ((double)rand() / (RAND_MAX + 1)) * 3;
    float angle = ((double)rand() / (RAND_MAX + 1)) * 360.0f;
    float rotationAngle = ((double)rand() / (RAND_MAX + 1)) * 360.0f;
    if(rotationAngle < 180)
    {
        rotationAngle *= -1;
    }
    float rotationSpeed = ((double)rand() / (RAND_MAX + 1)) * 25.0f;
    
    for(int aIndex = 0; aIndex < asteroidData->maxCount; ++aIndex)
    {
        if(!asteroidData->asteroids[aIndex].isActive)
        {
            asteroidData->asteroids[aIndex].position = position;
            asteroidData->asteroids[aIndex].movementSpeed = 100.0f; // TODO: scale with difficulty
            asteroidData->asteroids[aIndex].movementAngle = angle; // TODO: randomize
            asteroidData->asteroids[aIndex].rotationAngle = rotationAngle; // TODO: randomize
            asteroidData->asteroids[aIndex].rotationSpeed = rotationSpeed; // TODO: ramdomize
            asteroidData->asteroids[aIndex].size = size;
            asteroidData->asteroids[aIndex].bufferIndex = bufferIndex;
            asteroidData->asteroids[aIndex].isActive = 1;
            asteroidData->asteroids[aIndex].isSmall = isSmall;
            asteroidData->currentCount++;
            break;
        }
    }
}

static void
DebugSpawnProjectiles(projectile_data *projectileData, glm::vec2 position)
{
    if(projectileData->currentCount == projectileData->maxCount)
    {
        return;
    }
    
    projectileData->projectiles[0].isActive = 1;
    projectileData->projectiles[0].angle = 0;
    projectileData->projectiles[0].position = position;
    
    projectileData->projectiles[1].isActive = 1;
    projectileData->projectiles[1].angle = 0;
    projectileData->projectiles[1].position.x = position.x + 100;
    projectileData->projectiles[1].position.y = position.y;
    
    projectileData->projectiles[2].isActive = 1;
    projectileData->projectiles[2].angle = 0;
    projectileData->projectiles[2].position.x = position.x + 200;
    projectileData->projectiles[2].position.y = position.y;
    
    projectileData->projectiles[3].isActive = 1;
    projectileData->projectiles[3].angle = 0;
    projectileData->projectiles[3].position.x = position.x + 300;
    projectileData->projectiles[3].position.y = position.y;
    
    projectileData->currentCount += 4;
}

static void
DebugSpawnAsteroid(asteroid_data *asteroidData, glm::vec2 position, glm::vec2 size, int index, int vertexBufferIndex)
{
    if(asteroidData->currentCount == asteroidData->maxCount)
    {
        return;
    }
    
    int bufferIndex = ((double)rand() / (RAND_MAX + 1)) * 3;
    float angle =  ((double)rand() / (RAND_MAX + 1)) * 360;
    
    asteroidData->asteroids[index].position = position;
    asteroidData->asteroids[index].movementSpeed = 0;
    asteroidData->asteroids[index].movementAngle = 0;
    asteroidData->asteroids[index].rotationAngle = 0;
    asteroidData->asteroids[index].rotationSpeed = 0;
    asteroidData->asteroids[index].size = size;
    asteroidData->asteroids[index].bufferIndex = vertexBufferIndex;
    asteroidData->asteroids[index].isActive = 1;
    asteroidData->asteroids[index].isSmall = false;
    asteroidData->currentCount++;
}



static void
DrawShip(ship_data shipData, glm::vec2 position, GLint matrixUniform, bool drawCollisionBox)
{
    glm::mat4 shipModel = glm::mat4(1.0f);
    shipModel = glm::translate(shipModel, glm::vec3(position, 0.0f));
    shipModel = glm::rotate(shipModel, 
                            glm::radians(shipData.angle), 
                            glm::vec3(0.0f, 0.0f, 1.0f));
    shipModel = glm::scale(shipModel, glm::vec3(shipData.size, 1.0f));
    
    glUniformMatrix4fv(matrixUniform, 1, GL_FALSE, glm::value_ptr(shipModel));
    
    glBindBuffer(GL_ARRAY_BUFFER, shipData.vertexBuffer);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glDisableVertexAttribArray(0);
    
    if(drawCollisionBox)
    {
        glBindBuffer(GL_ARRAY_BUFFER, shipData.collisionVertexBuffer);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);
        glDrawArrays(GL_LINE_STRIP, 0, 6);
        glDisableVertexAttribArray(0);
    }
    
    if(shipData.isThrusterActive)
    {
        glBindBuffer(GL_ARRAY_BUFFER, shipData.thrusterBuffer);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);
        glDrawArrays(GL_LINES, 0, 6);
        glDisableVertexAttribArray(0);
    }
}


static void
DrawProjectile(projectile projectile, glm::vec2 position, glm::vec2 size, 
               GLint matrixUniform, GLuint vertexBuffer)
{
    glm::mat4 projectileModel = glm::mat4(1.0f);
    projectileModel = glm::translate(projectileModel, 
                                     glm::vec3(position, 0.0f));
    
    projectileModel = glm::rotate(projectileModel,
                                  glm::radians(projectile.angle),
                                  glm::vec3(0.0f, 0.0f, 1.0f));
    
    projectileModel =  glm::scale(projectileModel, glm::vec3(size, 1.0f));
    glUniformMatrix4fv(matrixUniform, 1, GL_FALSE, glm::value_ptr(projectileModel));
    
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);
    glDrawArrays(GL_LINES, 0, 2);
    glDisableVertexAttribArray(0);
}

static void
DrawAsteroid(asteroid asteroid, glm::vec2 position, GLint matrixUniform, GLuint vertexAttributes[3],
             int bufferVertexCount[3], GLuint vertexBuffers[3])
{
    glm::mat4 asteroidModel = glm::mat4(1.0f);
    asteroidModel = glm::translate(asteroidModel, glm::vec3(position, 0.0f));
    asteroidModel = glm::rotate(asteroidModel, 
                                glm::radians(asteroid.rotationAngle), 
                                glm::vec3(0.0f, 0.0f, 1.0f));
    asteroidModel = glm::scale(asteroidModel, glm::vec3(asteroid.size, 1.0f));
    
    glUniformMatrix4fv(matrixUniform, 1, GL_FALSE, glm::value_ptr(asteroidModel));
    
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffers[asteroid.bufferIndex]);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);
    glDrawArrays(GL_LINE_STRIP, 0, bufferVertexCount[asteroid.bufferIndex]);
    glDisableVertexAttribArray(0);
}

static void
UpdatePosition(glm::vec2 *position, glm::vec2 wrappedPositions[6], 
               float frameTime, float speed, float angle, window_dimensions wDims)
{
    position->x += frameTime * speed *
        -sin(Radians(angle));
    position->y += frameTime * speed *
        cos(Radians(angle));
    
    wrappedPositions[0].x = position->x + wDims.width;
    wrappedPositions[0].y = position->y;
    wrappedPositions[1].x = position->x - wDims.width;
    wrappedPositions[1].y = position->y;
    wrappedPositions[2].x = position->x;
    wrappedPositions[2].y = position->y + wDims.height;
    wrappedPositions[3].x = position->x;
    wrappedPositions[3].y = position->y - wDims.height;
    wrappedPositions[4].x = position->x - wDims.width;
    wrappedPositions[4].y = position->y - wDims.height;
    wrappedPositions[5].x = position->x + wDims.width;
    wrappedPositions[5].y = position->y + wDims.height;
    
    if((position->x > wDims.width || position->x < 0) ||
       (position->y > wDims.height || position->y < 0))
    {
        for(int i = 0; i < 6; ++i)
        {
            if((wrappedPositions[i].x < wDims.width && 
                wrappedPositions[i].x > 0) &&
               (wrappedPositions[i].y < wDims.height &&
                wrappedPositions[i].y > 0))
            {
                *position = wrappedPositions[i];
                break;
            }
        }
    }
}

static void
UpdateGameAndRender(open_gl_state *openGLState,
                    game_state *gameState,
                    window_dimensions windowDimensions,
                    input_state input, ship_data *shipData,
                    projectile_data *projectileData,
                    asteroid_data *asteroidData,
                    font_buffer *fontBuffer)
{
    if(!openGLState->IsInitialized)
    {
        srand(time(0));
        glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
        glEnable(GL_CULL_FACE);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        
        FT_Library ft;
        if(InitializeFreeType(&ft))
        {
            FT_Face face;
            if(LoadFont(ft, &face, "fonts/Hyperspace.ttf"))
            {
                FT_Set_Pixel_Sizes(face, 0, 48);
                LoadGlyphs(face, fontBuffer);
                
                OpenGLInitializeModelBuffer(&fontBuffer->vertexBuffer,
                                            &fontBuffer->vertexAttribute,
                                            0, 4, sizeof(float) * 6 * 4,
                                            GL_DYNAMIC_DRAW);
            }
            else
            {
                // Logging 
            }
        }
        else
        {
            // Logging
        }
        FT_Done_FreeType(ft);
        
        unsigned int tempShaderList[2];
        file_buffer vertexShader;
        file_buffer fragmentShader;
        
        ReadEntireFile(&vertexShader, "shaders/object_vertex_shader");
        ReadEntireFile(&fragmentShader, "shaders/object_fragment_shader");
        tempShaderList[0] = OpenGLCreateShader(GL_VERTEX_SHADER, vertexShader.buffer);
        tempShaderList[1] = OpenGLCreateShader(GL_FRAGMENT_SHADER, fragmentShader.buffer);
        openGLState->shaderProgram = OpenGLCreateProgram(&tempShaderList[0], 2);
        glDeleteShader(tempShaderList[0]);
        glDeleteShader(tempShaderList[1]);
        vertexShader = {};
        fragmentShader = {};
        
        ReadEntireFile(&vertexShader, "shaders/font_vertex_shader");
        ReadEntireFile(&fragmentShader, "shaders/font_fragment_shader");
        tempShaderList[0] = OpenGLCreateShader(GL_VERTEX_SHADER, vertexShader.buffer);
        tempShaderList[1] = OpenGLCreateShader(GL_FRAGMENT_SHADER, fragmentShader.buffer);
        openGLState->fontShaderProgram = OpenGLCreateProgram(&tempShaderList[0], 2);
        glDeleteShader(tempShaderList[0]);
        glDeleteShader(tempShaderList[1]);
        
        openGLState->rMatrixUniform =
            glGetUniformLocation(openGLState->shaderProgram, "rMatrix");
        openGLState->projectionUniform =
            glGetUniformLocation(openGLState->shaderProgram, "projection");
        openGLState->fontProjectionUniform =
            glGetUniformLocation(openGLState->fontShaderProgram, "projection");
        openGLState->fontTextColorUniform = glGetUniformLocation(openGLState->fontShaderProgram, "textColor");
        
        openGLState->projectionMatrix = glm::ortho(0.0f, (float)windowDimensions.width,
                                                   0.0f, (float)windowDimensions.height,
                                                   -1.0f, 1.0f);
        openGLState->fontProjectionMatrix = glm::ortho(0.0f, (float)windowDimensions.width,
                                                       0.0f, (float)windowDimensions.height);
        
        OpenGLInitializeModelBuffer(&projectileData->vertexBuffer,
                                    &projectileData->vertexAttribute,
                                    projectileVertexData, 4, sizeof(projectileVertexData),
                                    GL_STATIC_DRAW);
        int maxProjectileCount = 20;
        unsigned int projectileStorageSize = sizeof(projectile) * maxProjectileCount;
        projectileData->projectiles = (projectile *)malloc(projectileStorageSize);
        memset(projectileData->projectiles, 0, projectileStorageSize);
        projectileData->maxCount = maxProjectileCount;
        projectileData->size = glm::vec2(15.0f, 15.0f);
        projectileData->speed = 450.0f;
        
        asteroidData->bufferVertexCount[0] = (sizeof(asteroidBig0) / sizeof(float)) / 4;
        asteroidData->bufferVertexCount[1] = (sizeof(asteroidBig1) / sizeof(float)) / 4;
        asteroidData->bufferVertexCount[2] = (sizeof(asteroidBig2) / sizeof(float)) / 4;
        
        OpenGLInitializeModelBuffer(&asteroidData->vertexBuffers[0], 
                                    &asteroidData->vertexAttributes[0],
                                    asteroidBig0, 4, sizeof(asteroidBig0),
                                    GL_STATIC_DRAW);
        
        OpenGLInitializeModelBuffer(&asteroidData->vertexBuffers[1], 
                                    &asteroidData->vertexAttributes[1],
                                    asteroidBig1, 4, sizeof(asteroidBig1),
                                    GL_STATIC_DRAW);
        
        OpenGLInitializeModelBuffer(&asteroidData->vertexBuffers[2], 
                                    &asteroidData->vertexAttributes[2],
                                    asteroidBig2, 4, sizeof(asteroidBig2),
                                    GL_STATIC_DRAW);
        
        asteroidData->maxCount = 15;
        asteroidData->asteroids = (asteroid *)malloc(sizeof(asteroid) * asteroidData->maxCount);
        memset(asteroidData->asteroids, 0, sizeof(asteroid) * asteroidData->maxCount);
        
        glm::vec2 spawnPosition;
        for(int stageIndex = 0; stageIndex < 4; ++stageIndex)
        {
            spawnPosition.x = ((double)rand() / (RAND_MAX + 1)) * windowDimensions.width;
            spawnPosition.y = ((double)rand() / (RAND_MAX + 1)) * windowDimensions.height;
            SpawnAsteroid(asteroidData, spawnPosition, glm::vec2(100.0f, 100.0f), 0);
        }
        
        shipData->vertexCount = (sizeof(triangle) / sizeof(float)) / 4;
        OpenGLInitializeModelBuffer(&shipData->vertexBuffer, &shipData->vertexAttribute,
                                    triangle, 4, sizeof(triangle), GL_STATIC_DRAW);
        OpenGLInitializeModelBuffer(&shipData->thrusterBuffer, &shipData->thrusterAttribute,
                                    thruster, 4, sizeof(thruster), GL_STATIC_DRAW);
        OpenGLInitializeModelBuffer(&shipData->collisionVertexBuffer,
                                    &shipData->collisionVertexAttribute,
                                    collisionVertexData, 4,
                                    sizeof(collisionVertexData),
                                    GL_STATIC_DRAW);
        
        shipData->position.x = (windowDimensions.width / 2) + 1;
        shipData->position.y = (windowDimensions.height / 2) + 1;
        shipData->size = glm::vec2(45.0f, 45.0f);
        shipData->currentSpeed = 0;
        shipData->angle = 0.0f;
        shipData->turnSpeed = 160.0f;
        shipData->isVisible = 1;
        shipData->animationFrequency = 0.25f;
        
        gameState->stage = 0;
        gameState->maxLives = 5;
        gameState->livesLeft = 5;
        
        ReadEntireFile(&gameState->highScores, "high_scores.txt");
        if(gameState->highScores.size > 0)
        {
            int digitBuffer[11] = {};
            int j = 0;
            int digitCount = 0;
            int topTenIndex = 0;
            for(int i = 0; i < gameState->highScores.size; ++i)
            {
                if(topTenIndex > 9)
                {
                    break;
                }
                
                while(gameState->highScores.buffer[i] > 47 &&
                      gameState->highScores.buffer[i] < 58)
                {
                    digitBuffer[j] = (int)gameState->highScores.buffer[i] - 48;
                    j++;
                    i++;
                    digitCount++;
                }
                
                if(digitBuffer[0] > 0)
                {
                    gameState->topTenScores[topTenIndex] = IntArrayToInt(digitBuffer, digitCount);
                    memset(digitBuffer, 0, 11);
                    topTenIndex++;
                    digitCount = 0;
                    i--;
                    j = 0;
                }
            }
            
            InsertionSort(gameState->topTenScores, sizeof(gameState->topTenScores) / sizeof(int));
        }
        else
        {
            memset(gameState->topTenScores, 0, sizeof(gameState->topTenScores));
        }
        
        openGLState->IsInitialized = 1;
    }
    
    open_gl_viewport viewport = {};
    glGetIntegerv(GL_VIEWPORT, (GLint *)&viewport);
    if(viewport.width != windowDimensions.width ||
       viewport.height != windowDimensions.height)
    {
        openGLState->projectionMatrix = glm::ortho(0.0f, (float)windowDimensions.width,
                                                   0.0f, (float)windowDimensions.height,
                                                   -1.0f, 1.0f);
        
        OpenGLUpdateAspectRatio(windowDimensions.width, windowDimensions.height);
    }
    
    if(input.isDown)
    {
        switch(input.key)
        {
            case W_KEY:
            {
                if(shipData->currentSpeed < 3000.0f)
                {
                    if(shipData->currentSpeed > 25 &&
                       shipData->movementAngle != shipData->angle) 
                    {
                        shipData->currentSpeed -= 2 * SHIP_ACCELERATION * openGLState->frameTime;
#if 0
                        // TODO: Simulate curved motion. Currently unsolved
                        shipData->movementAngle = NormalizeAngle(shipData->movementAngle);
                        shipData->angle = NormalizeAngle(shipData->angle);
                        if(shipData->movementAngle < shipData->angle)
                        {
                            shipData->movementAngle += 
                                (shipData->angle - shipData->movementAngle) * openGLState->frameTime;
                        }
                        else if(shipData->movementAngle > shipData->angle)
                        {
                            shipData->movementAngle -= 
                                (shipData->movementAngle - shipData->angle) * openGLState->frameTime;
                        }
#endif
                    }
                    else
                    {
                        shipData->currentSpeed += SHIP_ACCELERATION * openGLState->frameTime;
                        shipData->movementAngle = shipData->angle;
                    }
                }
            } break;
            
            case A_KEY:
            {
                shipData->angle += shipData->turnSpeed * openGLState->frameTime;
                if(input.keyBuffer == W_KEY)
                {
                    shipData->movementAngle = shipData->angle;
                }
                
            } break;
            
            case S_KEY:
            {
            } break;
            
            case D_KEY:
            {
                shipData->angle -= shipData->turnSpeed  * openGLState->frameTime;
                if(input.keyBuffer == W_KEY)
                {
                    shipData->movementAngle = shipData->angle;
                }
            } break;
            case F_KEY:
            {
                shipData->position.x = windowDimensions.width / 2;
                shipData->position.y = windowDimensions.height / 2;
                shipData->angle = 0;
                shipData->movementAngle = 0;
                shipData->currentSpeed = 0;
                memset(projectileData->projectiles, 0, sizeof(projectile) * projectileData->maxCount);
                projectileData->currentCount = 0;
            } break;
            
            case SPACE_KEY:
            {
                if(gameState->stage > 0)
                {
                    if(projectileData->currentCount < projectileData->maxCount)
                    {
                        float currentTiming = openGLState->secondsElapsed;
                        float timeSinceLastProj = currentTiming -
                            projectileData->lastProjTimeStamp;
                        
                        if(timeSinceLastProj > 1.0f)
                        {
                            projectileData->lastProjTimeStamp = currentTiming;
                            
                            int projectileIndex = 0;
                            for(int projIndex = 0; 
                                projIndex < projectileData->maxCount; 
                                ++projIndex)
                            {
                                if(!projectileData->projectiles[projIndex].isActive)
                                {
                                    projectileData->projectiles[projIndex].isActive = 1;
                                    projectileData->projectiles[projIndex].angle = shipData->angle;
                                    projectileData->projectiles[projIndex].position = shipData->position;
                                    projectileData->currentCount++;
                                    break;
                                }
                            }
                        }
                    }
                } 
                else
                {
                    memset(asteroidData->asteroids, 0, sizeof(asteroid) * asteroidData->maxCount);
                    memset(projectileData->projectiles, 0, sizeof(projectile) * projectileData->maxCount);
                    asteroidData->currentCount = 0;
                    projectileData->currentCount = 0;
                    
                    shipData->position.x = windowDimensions.width / 2;
                    shipData->position.y = windowDimensions.height / 2;
                    shipData->angle = 0;
                    shipData->movementAngle = 0;
                    shipData->currentSpeed = 0;
                    shipData->isVisible = 1;
                    
                    gameState->stage = 1;
                    gameState->score = 0;
                    gameState->livesLeft = 5;
                } 
            } break;
        }
        
        if((input.key == A_KEY || input.key == D_KEY) &&
           input.keyBuffer != W_KEY)
        {
            shipData->currentSpeed -= 0.20f * (openGLState->frameTime) * shipData->currentSpeed;
        }
        
        if(input.key == W_KEY || input.keyBuffer == W_KEY)
        {
            shipData->isThrusterActive = 1;
        }
        else
        {
            shipData->isThrusterActive = 0;
        }
    }
    else
    {
        shipData->isThrusterActive = 0;
        shipData->currentSpeed -= 0.50f * (openGLState->frameTime) * shipData->currentSpeed;
    }
    
    if(asteroidData->currentCount == 0)
    {
        glm::vec2 spawnPosition;
        for(int stageIndex = 0; stageIndex < gameState->stage; ++stageIndex)
        {
            spawnPosition.x = ((double)rand() / (RAND_MAX + 1)) * windowDimensions.width;
            spawnPosition.y = ((double)rand() / (RAND_MAX + 1)) * windowDimensions.height;
            SpawnAsteroid(asteroidData, spawnPosition, glm::vec2(100.0f, 100.0f), 0);
        }
        gameState->stage++;
    }
    
    if(gameState->stage > 0)
    {
        UpdatePosition(&shipData->position, shipData->wrappedPositions, openGLState->frameTime, 
                       shipData->currentSpeed, shipData->movementAngle, windowDimensions);
    }
    
    for(int projectileIndex = 0; projectileIndex < projectileData->maxCount; ++projectileIndex)
    {
        if(projectileData->projectiles[projectileIndex].isActive)
        {
            UpdatePosition(&projectileData->projectiles[projectileIndex].position,
                           projectileData->projectiles[projectileIndex].wrappedPositions,
                           openGLState->frameTime,
                           projectileData->speed,
                           projectileData->projectiles[projectileIndex].angle,
                           windowDimensions);
        }
    }
    
    for(int asteroidIndex = 0; asteroidIndex < asteroidData->maxCount; ++asteroidIndex)
    {
        if(asteroidData->asteroids[asteroidIndex].isActive)
        {
            asteroidData->asteroids[asteroidIndex].rotationAngle +=
                asteroidData->asteroids[asteroidIndex].rotationSpeed * openGLState->frameTime;
            
            UpdatePosition(&asteroidData->asteroids[asteroidIndex].position,
                           asteroidData->asteroids[asteroidIndex].wrappedPositions,
                           openGLState->frameTime,
                           asteroidData->asteroids[asteroidIndex].movementSpeed,
                           asteroidData->asteroids[asteroidIndex].movementAngle,
                           windowDimensions);
        }
    }
    
    if(shipData->isAnimating)
    {
        if(shipData->animationCounter > shipData->collisionMaxTimeout)
        {
            shipData->isVisible = 1;
            shipData->isAnimating = 0;
            shipData->collisionTimeout = 0;
        }
        else if(openGLState->secondsElapsed > shipData->animationCounter)
        {
            shipData->isVisible = shipData->isVisible ^ 1;
            shipData->animationCounter += shipData->animationFrequency;
        }
    }
    
    if(gameState->stage > 0)
    {
        collision_box asteroidCollisionBox = {};
        collision_box projectileCollisionBox = {};
        if(shipData->collisionTimeout)
        {
            for(int aIndex = 0; aIndex < asteroidData->maxCount; ++aIndex)
            {
                if(asteroidData->asteroids[aIndex].isActive)
                {
                    glm::mat4 asteroidCollisionMatrix = glm::mat4(1.0f);
                    asteroidCollisionMatrix = 
                        glm::translate(asteroidCollisionMatrix, 
                                       glm::vec3(asteroidData->asteroids[aIndex].position, 0.0f));
                    asteroidCollisionMatrix = glm::scale(asteroidCollisionMatrix, glm::vec3(asteroidData->asteroids[aIndex].size, 1.0f));
                    
                    switch(asteroidData->asteroids[aIndex].bufferIndex)
                    {
                        case 0:
                        {
                            asteroidCollisionBox = NewCollisionBox(asteroidCollisionVertex0);
                        } break;
                        case 1:
                        {
                            asteroidCollisionBox = NewCollisionBox(asteroidCollisionVertex1);
                        } break;
                        case 2:
                        {
                            asteroidCollisionBox = NewCollisionBox(asteroidCollisionVertex2);
                        } break;
                    }
                    
                    asteroidCollisionBox.topLeft = openGLState->projectionMatrix * asteroidCollisionMatrix * asteroidCollisionBox.topLeft;
                    asteroidCollisionBox.bottomRight = openGLState->projectionMatrix * asteroidCollisionMatrix * asteroidCollisionBox.bottomRight;
                    
                    for(int pIndex = 0; pIndex < projectileData->maxCount; ++pIndex)
                    {
                        if(projectileData->projectiles[pIndex].isActive)
                        {
                            glm::mat4 projectileCollisionMatrix = glm::mat4(1.0f);
                            projectileCollisionMatrix = glm::translate(projectileCollisionMatrix, glm::vec3(projectileData->projectiles[pIndex].position, 0.0f));
                            projectileCollisionMatrix = glm::scale(projectileCollisionMatrix, glm::vec3(projectileData->size, 1.0f));
                            
                            projectileCollisionBox = NewCollisionBox(projectileCollisionData);
                            projectileCollisionBox.topLeft = openGLState->projectionMatrix * projectileCollisionMatrix * projectileCollisionBox.topLeft;
                            projectileCollisionBox.bottomRight = openGLState->projectionMatrix * projectileCollisionMatrix * projectileCollisionBox.bottomRight;
                            
                            
                            bool collisionDetected = CheckCollision(projectileCollisionBox, asteroidCollisionBox);
                            if(collisionDetected)
                            {
                                // NOTE: Projectile & Asteroid collision
                                projectileData->projectiles[pIndex].isActive = 0;
                                projectileData->currentCount--;
                                asteroidData->asteroids[aIndex].isActive = 0;
                                asteroidData->currentCount--;
                                if(!asteroidData->asteroids[aIndex].isSmall)
                                {
                                    glm::vec2 spawnPosition = asteroidData->asteroids[aIndex].position;
                                    SpawnAsteroid(asteroidData, spawnPosition, glm::vec2(25.0f, 25.0f), true);
                                    SpawnAsteroid(asteroidData, spawnPosition, glm::vec2(25.0f, 25.0f), true);
                                    SpawnAsteroid(asteroidData, spawnPosition, glm::vec2(25.0f, 25.0f), true);
                                    gameState->score += 50;
                                }
                                else
                                {
                                    gameState->score += 100;
                                }
                            }
                        }
                    }
                }
            }
        }
        else
        {
            glm::mat4 shipCollisionMatrix = glm::mat4(1.0f);
            shipCollisionMatrix = glm::translate(shipCollisionMatrix, glm::vec3(shipData->position, 0.0f));
            shipCollisionMatrix = glm::scale(shipCollisionMatrix, glm::vec3(shipData->size, 1.0f));
            
            collision_box shipCollisionBox = NewCollisionBox(collisionVertexData);
            shipCollisionBox.topLeft = openGLState->projectionMatrix * shipCollisionMatrix * shipCollisionBox.topLeft;
            shipCollisionBox.bottomRight = openGLState->projectionMatrix * shipCollisionMatrix * shipCollisionBox.bottomRight;
            
            for(int aIndex = 0; aIndex < asteroidData->maxCount; ++aIndex)
            {
                if(asteroidData->asteroids[aIndex].isActive)
                {
                    glm::mat4 asteroidCollisionMatrix = glm::mat4(1.0f);
                    asteroidCollisionMatrix = glm::translate(asteroidCollisionMatrix, 
                                                             glm::vec3(asteroidData->asteroids[aIndex].position, 0.0f));
                    asteroidCollisionMatrix = glm::scale(asteroidCollisionMatrix, glm::vec3(asteroidData->asteroids[aIndex].size, 1.0f));
                    
                    switch(asteroidData->asteroids[aIndex].bufferIndex)
                    {
                        case 0:
                        {
                            asteroidCollisionBox = NewCollisionBox(asteroidCollisionVertex0);
                        } break;
                        case 1:
                        {
                            asteroidCollisionBox = NewCollisionBox(asteroidCollisionVertex1);
                        } break;
                        case 2:
                        {
                            asteroidCollisionBox = NewCollisionBox(asteroidCollisionVertex2);
                        } break;
                    }
                    
                    asteroidCollisionBox.topLeft = openGLState->projectionMatrix * asteroidCollisionMatrix * asteroidCollisionBox.topLeft;
                    asteroidCollisionBox.bottomRight = openGLState->projectionMatrix * asteroidCollisionMatrix * asteroidCollisionBox.bottomRight;
                    
                    bool collisionDetected = CheckCollision(shipCollisionBox, asteroidCollisionBox);
                    if(collisionDetected)
                    {
                        // NOTE: Ship & Asteroid collision
                        if(gameState->livesLeft > 1)
                        {
                            float collisionTimer = openGLState->secondsElapsed + 1.5f;
                            
                            gameState->livesLeft--;
                            shipData->isAnimating = 1;
                            shipData->collisionTimeout = 1;
                            shipData->collisionMaxTimeout = collisionTimer;
                            shipData->animationCounter = openGLState->secondsElapsed + shipData->animationFrequency;
                        }
                        else
                        {
                            // NOTE: GAME OVER
                            gameState->stage = -1;
                            shipData->isVisible = 0;
                            
                            if(gameState->score > gameState->topTenScores[9])
                            {
                                gameState->topTenScores[9] = gameState->score;
                                InsertionSort(gameState->topTenScores, sizeof(gameState->topTenScores) / sizeof(int));
                            }
                            
                            char writeBuffer[12 * 10] = {};
                            int err;
                            int startIndex = 0;
                            for(int i = 0; i < 10; i++)
                            {
                                err = snprintf(&writeBuffer[startIndex], 
                                               sizeof(writeBuffer), 
                                               "%d\n", 
                                               gameState->topTenScores[i]);
                                
                                for(int j = startIndex; j < 12 * 10; j++)
                                {
                                    if(writeBuffer[j] == '\0')
                                    {
                                        startIndex = j;
                                        break;
                                    }
                                }
                            }
                            WriteEntireFile(writeBuffer, sizeof(writeBuffer), "high_scores.txt");
                            break;
                        }
                    }
                    
                    for(int pIndex = 0; pIndex < projectileData->maxCount; ++pIndex)
                    {
                        if(projectileData->projectiles[pIndex].isActive)
                        {
                            glm::mat4 projectileCollisionMatrix = glm::mat4(1.0f);
                            projectileCollisionMatrix = glm::translate(projectileCollisionMatrix, glm::vec3(projectileData->projectiles[pIndex].position, 0.0f));
                            projectileCollisionMatrix = glm::scale(projectileCollisionMatrix, glm::vec3(projectileData->size, 1.0f));
                            
                            projectileCollisionBox = NewCollisionBox(projectileCollisionData);
                            projectileCollisionBox.topLeft = openGLState->projectionMatrix * projectileCollisionMatrix * projectileCollisionBox.topLeft;
                            projectileCollisionBox.bottomRight = openGLState->projectionMatrix * projectileCollisionMatrix * projectileCollisionBox.bottomRight;
                            
                            
                            bool collisionDetected = CheckCollision(projectileCollisionBox, asteroidCollisionBox);
                            if(collisionDetected)
                            {
                                // NOTE: Projectile & Asteroid collision
                                projectileData->projectiles[pIndex].isActive = 0;
                                projectileData->currentCount--;
                                asteroidData->asteroids[aIndex].isActive = 0;
                                asteroidData->currentCount--;
                                if(!asteroidData->asteroids[aIndex].isSmall)
                                {
                                    glm::vec2 spawnPosition = asteroidData->asteroids[aIndex].position;
                                    SpawnAsteroid(asteroidData, spawnPosition, glm::vec2(25.0f, 25.0f), true);
                                    SpawnAsteroid(asteroidData, spawnPosition, glm::vec2(25.0f, 25.0f), true);
                                    SpawnAsteroid(asteroidData, spawnPosition, glm::vec2(25.0f, 25.0f), true);
                                    gameState->score += 50;
                                }
                                else
                                {
                                    gameState->score += 100;
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    
    glClear(GL_COLOR_BUFFER_BIT);
    glUseProgram(openGLState->shaderProgram);
    glUniformMatrix4fv(openGLState->projectionUniform, 1, GL_FALSE,
                       glm::value_ptr(openGLState->projectionMatrix));
    
    if(gameState->stage > 0)
    {
        if(shipData->isVisible)
        {
            glm::vec4 shipPos = glm::vec4(triangle[0], triangle[1], 0.0f, 1.0f);
            DrawShip(*shipData, shipData->position, openGLState->rMatrixUniform, false);
            for(int positionIndex = 0; positionIndex < 6; ++positionIndex)
            {
                DrawShip(*shipData, shipData->wrappedPositions[positionIndex], openGLState->rMatrixUniform, false);
            }
        }
        
        glm::vec2 shipIndicatorPos = glm::vec2(25, windowDimensions.height - 25); 
        glm::mat4 shipModel;
        int livesLeft = gameState->livesLeft;
        for(int shipLives = 0; shipLives < gameState->maxLives; ++shipLives)
        {
            if(shipLives > livesLeft - 1)
            {
                break;
            }
            
            shipModel = glm::mat4(1.0f);
            shipModel = glm::translate(shipModel, glm::vec3(shipIndicatorPos, 0.0f));
            shipModel = glm::scale(shipModel, glm::vec3(25.0f, 25.0f, 1.0f));
            
            glUniformMatrix4fv(openGLState->rMatrixUniform, 1, GL_FALSE, glm::value_ptr(shipModel));
            glBindBuffer(GL_ARRAY_BUFFER, shipData->vertexBuffer);
            glEnableVertexAttribArray(0);
            glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);
            glDrawArrays(GL_TRIANGLES, 0, 6);
            glDisableVertexAttribArray(0);
            shipIndicatorPos.x += 50;
        }
        
        for(int projIndex = 0;
            projIndex < projectileData->maxCount;
            ++projIndex)
        {
            if(projectileData->projectiles[projIndex].isActive)
            {
                DrawProjectile(projectileData->projectiles[projIndex],
                               projectileData->projectiles[projIndex].position, 
                               shipData->size, 
                               openGLState->rMatrixUniform,
                               projectileData->vertexBuffer);
                
                for(int positionIndex = 0; positionIndex < 6; ++positionIndex)
                {
                    DrawProjectile(projectileData->projectiles[projIndex],
                                   projectileData->projectiles[projIndex].wrappedPositions[positionIndex], 
                                   shipData->size, 
                                   openGLState->rMatrixUniform,
                                   projectileData->vertexBuffer);
                }
            }
        }
    }
    
    int bufferIndex;
    for(int asteroidIndex = 0; asteroidIndex < asteroidData->maxCount; ++asteroidIndex)
    {
        if(asteroidData->asteroids[asteroidIndex].isActive)
        {
            DrawAsteroid(asteroidData->asteroids[asteroidIndex], 
                         asteroidData->asteroids[asteroidIndex].position,
                         openGLState->rMatrixUniform,
                         asteroidData->vertexAttributes,
                         asteroidData->bufferVertexCount,
                         asteroidData->vertexBuffers);
            for(int positionIndex = 0; positionIndex < 6; ++positionIndex)
            {
                DrawAsteroid(asteroidData->asteroids[asteroidIndex],
                             asteroidData->asteroids[asteroidIndex].wrappedPositions[positionIndex],
                             openGLState->rMatrixUniform,
                             asteroidData->vertexAttributes,
                             asteroidData->bufferVertexCount,
                             asteroidData->vertexBuffers);
            }
        }
    }
    
    glUseProgram(openGLState->fontShaderProgram);
    glUniform3f(openGLState->fontTextColorUniform, 1.0f, 1.0f, 1.0f);
    glUniformMatrix4fv(openGLState->fontProjectionUniform, 1, GL_FALSE,
                       glm::value_ptr(openGLState->fontProjectionMatrix));
    
    char scoreBuffer[11];
    
    if(gameState->stage == 0)
    {
        OpenGLRenderText(fontBuffer, 
                         "ASTEROIDS",
                         glm::vec2((windowDimensions.width / 2) - 245, windowDimensions.height / 2),
                         2.0f);
        
        OpenGLRenderText(fontBuffer, 
                         "Press Space",
                         glm::vec2((windowDimensions.width / 2) - 150, windowDimensions.height / 6),
                         1.0f);
    }
    
    else if(gameState->stage > 0)
    {
        int err = snprintf(scoreBuffer, sizeof(scoreBuffer), "%d", gameState->score);
        glm::vec2 textPos = glm::vec2(25.0f, windowDimensions.height - 100);
        
        OpenGLRenderText(fontBuffer, scoreBuffer, textPos, 1.0f);
    }
    else if(gameState->stage == -1)
    {
        glm::vec2 highScorePos = glm::vec2(0, (windowDimensions.height - 50.0f));
        int err;
        for(int i = 0; i < 10; ++i)
        {
            // TODO: Logging
            err = snprintf(scoreBuffer, 
                           sizeof(scoreBuffer), 
                           "%d", gameState->topTenScores[i]);
            OpenGLRenderText(fontBuffer, scoreBuffer, highScorePos, 1.0f);
            highScorePos.y -= 50.0f;
        }
        
        OpenGLRenderText(fontBuffer, 
                         "game over",
                         glm::vec2((windowDimensions.width / 2) - 245, windowDimensions.height / 2),
                         2.0f);
        
        OpenGLRenderText(fontBuffer,
                         "Press Space",
                         glm::vec2((windowDimensions.width / 2) - 145, windowDimensions.height / 6),
                         1.0f);
    }
    
    GLenum error;
    while(error = glGetError())
    {
        // TODO: Logging
        printf("OpenGL error: %d\n", error);
    }
}