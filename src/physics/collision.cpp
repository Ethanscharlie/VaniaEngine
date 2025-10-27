
#include <stdexcept>

#include "GameDataStructs.hpp"

struct CollisionRect {
  float x;
  float y;
  float w;
  float h;

  CollisionRect(const Entity& entity, const EntityDef& def) {
    x = entity.x + def.colliderOffsetX;
    y = entity.y + def.colliderOffsetY;
    w = def.width * def.colliderWidthFraction;
    h = def.height * def.colliderHeightFraction;
  }
};

struct CollisionCircle {
  float x;
  float y;
  float r;

  CollisionCircle(const Entity& entity, const EntityDef& def) {
    x = entity.x + def.colliderOffsetX;
    y = entity.y + def.colliderOffsetY;
    r = def.width * def.colliderWidthFraction;
  }
};

static bool checkCollisionWithAxisAlignedBoundingBox(CollisionRect a, CollisionRect b) {
  return a.x + a.w > b.x &&  //
         a.x < b.x + b.w &&  //
         a.y + a.h > b.y &&  //
         a.y < b.y + b.h;
}

bool isColliding(const GameData& gameData, const Entity& a, const Entity& b) {
  const EntityDef& defA = gameData.entityDefs.at(a.defID);
  const EntityDef& defB = gameData.entityDefs.at(b.defID);

  if (defA.colliderType == "rect" && defB.colliderType == "rect") {
    return checkCollisionWithAxisAlignedBoundingBox({a, defA}, {b, defB});
  }

  throw std::runtime_error("Dumbass Engine developer forgot to support this collision case");
}
