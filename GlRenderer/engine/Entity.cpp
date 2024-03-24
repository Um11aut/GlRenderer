#include "Entity.h"
#include <spdlog/spdlog.h>

void Entity::invoke() const
{
    spdlog::error("Invoking BASE Entity Class!! Invoke inherited entity!");
}

std::string Entity::get_name() const
{
    return "Not implemented!";
}
