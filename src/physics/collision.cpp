
#include <stdexcept>

#include "GameDataStructs.hpp"

struct CollisionRect {
  float x;
  float y;
  float w;
  float h;

  CollisionRect(const Entity& entity, const EntityDef& def) {
    w = def.width * def.colliderWidthFraction;
    h = def.height * def.colliderHeightFraction;
    x = entity.x + def.centerOffsetX - def.width / 2;
    y = entity.y + def.centerOffsetY - def.height / 2;
  }
};

struct CollisionCircle {
  float x;
  float y;
  float r;

  CollisionCircle(const Entity& entity, const EntityDef& def) {
    x = entity.x + def.centerOffsetX;
    y = entity.y + def.centerOffsetY;
    r = def.width * def.colliderWidthFraction / 2;
  }
};

static bool checkCollisionWithAxisAlignedBoundingBox(CollisionRect a, CollisionRect b) {
  return a.x + a.w > b.x &&  //
         a.x < b.x + b.w &&  //
         a.y + a.h > b.y &&  //
         a.y < b.y + b.h;
}

static bool checkCollisionWithDistance(CollisionCircle a, CollisionCircle b) {
  const float distance = sqrt(pow(b.x - a.x, 2) + pow(b.y - a.y, 2));
  return distance < a.r + b.r;
}

bool isColliding(const GameData& gameData, const Entity& a, const Entity& b) {
  const EntityDef& defA = gameData.entityDefs.at(a.defID);
  const EntityDef& defB = gameData.entityDefs.at(b.defID);

  if (defA.colliderType == "rect" && defB.colliderType == "rect") {
    return checkCollisionWithAxisAlignedBoundingBox({a, defA}, {b, defB});
  }

  if (defA.colliderType == "circle" && defB.colliderType == "circle") {
    return checkCollisionWithDistance({a, defA}, {b, defB});
  }

  throw std::runtime_error("Dumbass Engine developer forgot to support this collision case");
}
