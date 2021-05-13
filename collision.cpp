// TODO: do collision checking
#include "collision.h"

static bool
CheckCollision(collision_box regionOne, collision_box regionTwo)
{
    bool xAxisCollision = false;
    bool yAxisCollision = false;
    
    if((regionOne.topLeft.x > regionTwo.topLeft.x &&
        regionOne.topLeft.x < regionTwo.bottomRight.x) ||
       (regionOne.bottomRight.x > regionTwo.topLeft.x && regionOne.bottomRight.x < regionTwo.bottomRight.x))
    {
        xAxisCollision = true;
    }
    
    if((regionOne.topLeft.y < regionTwo.topLeft.y && 
        regionOne.topLeft.y > regionTwo.bottomRight.y) ||
       (regionOne.bottomRight.y < regionTwo.topLeft.y && regionOne.bottomRight.y > regionTwo.bottomRight.y))
    {
        yAxisCollision = true;
    }
    
    return xAxisCollision && yAxisCollision;
}

static collision_box
NewCollisionBox(float collisionVertexData[20])
{
    collision_box result = {};
    result.topLeft = glm::vec4(collisionVertexData[0], collisionVertexData[1], 0.0f, 1.0f);
    result.bottomRight = glm::vec4(collisionVertexData[8], collisionVertexData[9], 0.0f, 1.0f);
    
    return result;
}
