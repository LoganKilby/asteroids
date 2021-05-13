/* date = March 24th 2021 3:35 pm */

#ifndef VERTEX_DATA_H
#define VERTEX_DATA_H

float collisionVertexData[] = 
{
    -0.5f, 0.5f, 0.0f, 1.0f,
    -0.5f, -0.5f, 0.0f, 1.0f,
    0.5f, -0.5f, 0.0f, 1.0f,
    0.5f, 0.5f, 0.0f, 1.0f,
    -0.5, 0.5f, 0.0f, 1.0f
};

float shipVertices[] = 
{
    0.0f, 0.5f, 0.0f, 1.0f,
    -0.5, -0.5, 0.0f, 1.0f,
    0.0f, -0.25f, 0.0f, 1.0f,
    0.0f, 0.5f, 0.0f, 1.0f,
    0.0f, -0.25f, 0.0f, 1.0f,
    0.5, -0.5, 0.0f, 1.0f
};

float thrusterVertices[] = 
{
    0.0f, -0.25f, 0.0f, 1.0f,
    0.0f, -0.5f, 0.0f, 1.0f,
    
    0.07f, -0.27f, 0.0f, 1.0f,
    0.07f, -0.45f, 0.0f, 1.0f,
    
    -0.07f, -0.27f, 0.0f, 1.0f,
    -0.07f, -0.45f, 0.0f, 1.0f
};

float projectileVertexData[] =
{
    0.0f, 0.5f, 0.0f, 1.0f,
    0.0f, 0.65f, 0.0f, 1.0f
};

float projectileCollisionData[] =
{
    -0.1f, 0.65f, 0.0f, 1.0f,
    -0.1f, 0.5f, 0.0f, 1.0f,
    0.1f, 0.5f, 0.0f, 1.0f,
    0.1f, 0.65f, 0.0f, 1.0f,
    -0.1f, 0.65f, 0.0f, 1.0f
};

float asteroidBig0[] =
{ // 14 vertices
    0.0f, 0.5f, 0.0f, 1.0f,
    -0.15f, 1.0f, 0.0f, 1.0f,
    -0.80f, 0.5f, 0.0f, 1.0f,
    
    -0.80f, 0.5f, 0.0f, 1.0f,
    -1.0f, 0.0f, 0.0f, 1.0f,
    0.0f, -1.0f, 0.0f, 1.0f,
    
    0.0f, -1.0f, 0.0f, 1.0f,
    0.5f, -0.5f, 0.0f, 1.0f,
    0.5f, 0.0f, 0.0f, 1.0f,
    
    0.5f, 0.0f, 0.0f, 1.0f,
    1.0f, 0.5f, 0.0f, 1.0f,
    0.5f, 0.87f, 0.0f, 1.0f,
    
    0.5f, 0.87f, 0.0f, 1.0f,
    0.0f, 0.5f, 0.0f, 1.0f
};

float asteroidCollisionVertex0[] = 
{
    -1.0f, 1.0f, 0.0f, 1.0f,
    -1.0f, -1.0f, 0.0f, 1.0f,
    1.0f, -1.0f, 0.0f, 1.0f,
    1.0f, 1.0f, 0.0f, 1.0f,
    -1.0f, 1.0f, 0.0f, 1.0f
};

float asteroidBig1[] = 
{ // 24 vertices
    0.0f, 1.0f, 0.0f, 1.0f,
    -0.5f, 1.0f, 0.0f, 1.0f,
    
    -0.5f, 1.0f, 0.0f, 1.0f,
    -0.5f, 0.5f, 0.0f, 1.0f,
    
    -0.5f, 0.5f, 0.0f, 1.0f,
    -1.0f, 0.0f, 0.0f, 1.0f,
    
    -1.0f, 0.0f, 0.0f, 1.0f,
    -0.8f, -0.5f, 0.0f, 1.0f,
    
    -0.8f, -0.5f, 0.0f, 1.0f,
    0.0f, -0.5f, 0.0f, 1.0f,
    
    0.0f, -0.5f, 0.0f, 1.0f,
    0.25f, -1.0f, 0.0f, 1.0f,
    
    0.25f, -1.0f, 0.0f, 1.0f,
    0.75f, -0.5f, 0.0f, 1.0f,
    
    0.75f, -0.5f, 0.0f, 1.0f,
    0.5f, 0.0f, 0.0f, 1.0f,
    
    0.5f, 0.0f, 0.0f, 1.0f,
    0.85f, 0.5f, 0.0f, 1.0f,
    
    0.85f, 0.5f, 0.0f, 1.0f,
    0.65f, 1.0f, 0.0f, 1.0f,
    
    0.65f, 1.0f, 0.0f, 1.0f,
    0.0f, 0.5f, 0.0f, 1.0f,
    
    0.0f, 0.5f, 0.0f, 1.0f,
    0.0f, 1.0f, 0.0f, 1.0f
};

float asteroidCollisionVertex1[] = 
{
    -1.0f, 1.0f, 0.0f, 1.0f,
    -1.0f, -1.0f, 0.0f, 1.0f,
    1.0f, -1.0f, 0.0f, 1.0f,
    1.0f, 1.0f, 0.0f, 1.0f,
    -1.0f, 1.0f, 0.0f, 1.0f,
};

float asteroidBig2[] = 
{ // 24 vertices
    0.0f, 0.5f, 0.0f, 1.0f,
    -1.0f, 0.5f, 0.0f, 1.0f,
    
    -1.0f, 0.5f, 0.0f, 1.0f,
    -1.0f, 0.0f, 0.0f, 1.0f,
    
    -1.0f, 0.0f, 0.0f, 1.0f,
    -0.80f, -0.5f, 0.0f, 1.0f,
    
    -0.80f, -0.5f, 0.0f, 1.0f,
    0.0f, -0.5f, 0.0f, 1.0f,
    
    0.0f, -0.5f, 0.0f, 1.0f,
    0.25f, -1.0f,  0.0f, 1.0f,
    
    0.25f, -1.0f, 0.0f, 1.0f,
    0.80f, -0.5f, 0.0f, 1.0f,
    
    0.80f, -0.5f, 0.0f, 1.0f,
    0.20f, 0.0f, 0.0f, 1.0f,
    
    0.20f, 0.0f, 0.0f, 1.0f,
    0.90f, 0.0f, 0.0f, 1.0f,
    
    0.90f, 0.0f, 0.0f, 1.0f, 
    0.90f, 0.5f, 0.0f, 1.0f,
    
    0.90f, 0.5f, 0.0f, 1.0f,
    0.5f, 1.0f, 0.0f, 1.0f,
    
    0.5f, 1.0f, 0.0f, 1.0f,
    -0.20f, 1.0f, 0.0f, 1.0f,
    
    -0.20f, 1.0f, 0.0f,1.0f,
    0.0f, 0.5f, 0.0f, 1.0f,
};

float asteroidCollisionVertex2[] = 
{
    -1.0f, 1.0f, 0.0f, 1.0f,
    -1.0f, -1.0f, 0.0f, 1.0f,
    0.90f, -1.0f, 0.0f, 1.0f,
    0.90f, 1.0f, 0.0f, 1.0f,
    -1.0f, 1.0f, 0.0f, 1.0f,
};

float fontVertices[] = 
{
    0, 0, 0.0f, 0.0f,            
    0, 0, 0.0f, 1.0f,
    0, 0, 1.0f, 1.0f,
    0, 0, 0.0f, 0.0f,
    0, 0, 1.0f, 1.0f,
    0, 0, 1.0f, 0.0f            
};

#endif //VERTEX_DATA_H
