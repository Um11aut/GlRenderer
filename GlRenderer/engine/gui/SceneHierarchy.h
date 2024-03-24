#pragma once
#include "../Model.h"
#include "../CubeMap.h"
#include "../Entity.h"
#include <vector>

namespace Ui {
    void draw_scene_entities_controls(std::vector<std::unique_ptr<Entity>>& entities, std::unique_ptr<Camera>& camera);
}