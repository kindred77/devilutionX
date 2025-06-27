#include "MapCellInfo.h" #include <iostream>
MapCellInfo::MapCellInfo() : backIndex(0), backImage(0), middleIndex(0), middleImage(0), frontIndex(0), frontImage(0), doorIndex(0), doorOffset(0), frontAnimationFrame(0), frontAnimationTick(0), middleAnimationFrame(0), middleAnimationTick(0), tileAnimationImage(0), tileAnimationOffset(0), tileAnimationFrames(0), light(0), unknown(0), fishingCell(false) {}
void MapCellInfo::addObject(const MapObjectPtr& ob) { if (!ob) return;
    if (!cellObjects.empty()) {
        cellObjects.insert(cellObjects.begin(), ob);
    } else {
        cellObjects.push_back(ob);
    }

    sort();
}
void MapCellInfo::removeObject(const MapObjectPtr& ob) { if (cellObjects.empty()) return;
    auto it = std::find(cellObjects.begin(), cellObjects.end(), ob);
    if (it != cellObjects.end()) {
        cellObjects.erase(it);
    }

    if (cellObjects.empty()) {
        cellObjects.clear();
    } else {
        sort();
    }
}
MapCellInfo::MapObjectPtr MapCellInfo::findObject(long objectID) const { for (const auto& obj : cellObjects) { if (obj && obj->getObjectID() == objectID) { return obj; } } return nullptr; }
void MapCellInfo::drawObjects(void* surface) const { if (cellObjects.empty()) return;
    for (const auto& obj : cellObjects) {
        if (!obj || obj->isDead()) continue;

        obj->draw(surface);
    }
}
void MapCellInfo::drawDeadObjects(void* surface) const { if (cellObjects.empty()) return;
    for (const auto& obj : cellObjects) {
        if (!obj || !obj->isDead()) continue;

        if (obj->getRace() == ObjectType::Monster) {
            switch (dynamic_cast<MonsterObject*>(obj.get())->getBaseImage()) {
                case PalaceWallLeft:
                case PalaceWall1:
                case PalaceWall2:
                case SSabukWall1:
                case SSabukWall2:
                case SSabukWall3:
                case HellLord:
                    continue;
                default:
                    break;
            }
        }

        obj->draw(surface);
    }
}
void MapCellInfo::sort() { if (cellObjects.size() <= 1) return;
    std::sort(cellObjects.begin(), cellObjects.end(),
              [](const MapObjectPtr& ob1, const MapObjectPtr& ob2) -> bool {
                  if (!ob1 || !ob2) return false;

                  // Item 优先于非 Item
                  if (ob1->getRace() == ObjectType::Item && ob2->getRace() != ObjectType::Item)
                      return true;
                  if (ob2->getRace() == ObjectType::Item && ob1->getRace() != ObjectType::Item)
                      return false;

                  // Spell 优先于非 Spell
                  if (ob1->getRace() == ObjectType::Spell && ob2->getRace() != ObjectType::Spell)
                      return true;
                  if (ob2->getRace() == ObjectType::Spell && ob1->getRace() != ObjectType::Spell)
                      return false;

                  // Dead 状态比较
                  if (ob2->isDead() != ob1->isDead()) {
                      return ob2->isDead();
                  }

                  // ID 比较
                  return ob1->getObjectID() < ob2->getObjectID();
              });
}