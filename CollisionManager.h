#pragma once

#include "Includes.h"

class ModelContainer;

class CollisionManager
{
    CollisionManager() { containers = vector<ModelContainer*>(); }
    vector<ModelContainer*> containers;

public:
    static CollisionManager& GetInstance()
    {
        static CollisionManager instance;
        return instance;
    }
    CollisionManager(CollisionManager const&) = delete;
    CollisionManager& operator=(CollisionManager const&) = delete;

    int AddContainer(ModelContainer* container);
    bool IsColliding(ModelContainer* container, int index);
};
