#pragma once

#include <string>

struct Entity {
    virtual void invoke() const;
    virtual std::string get_name() const;
    virtual ~Entity() {}
};