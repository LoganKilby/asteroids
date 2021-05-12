/* date = April 23rd 2021 6:05 am */

#ifndef COLLISION_H
#define COLLISION_H

struct collision_box
{
    glm::vec4 topLeft;
    glm::vec4 bottomRight;
};

struct collision_data
{
    glm::vec4 regionOne[4];
    glm::vec4 regionTwo[4];
};

static bool CheckCollision(collision_box, collision_box);
static collision_box NewCollisionBox(float[20]);

inline bool
CheckAABBCollision(glm::vec2 posA, glm::vec2 sizeA, glm::vec2 posB, glm::vec2 sizeB)
{
    bool collisionX = posA.x + sizeA.x >= posB.x && posB.x + sizeB.x >= posA.x;
    bool collisionY = posA.y + sizeA.y >= posB.y && posB.y + sizeB.y >= posA.y;
    return collisionX && collisionY;
}

#endif //COLLISION_H
