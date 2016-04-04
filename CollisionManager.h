#pragma once

#include "Includes.h"

class ModelContainer;
///
/// \brief The CollisionManager class
///  Handles the collision detection of various objects in the scene
///
class CollisionManager
{
    CollisionManager() { containers = vector<ModelContainer*>(); }
    vector<ModelContainer*> containers;

public:
    ///
    /// \brief GetInstance
    /// \return
    ///
    static CollisionManager& GetInstance()
    {
        static CollisionManager instance;
        return instance;
    }
    ///
    /// \brief CollisionManager
    ///
    CollisionManager(CollisionManager const&) = delete;
    ///
    /// \brief operator =
    /// \return
    ///
    CollisionManager& operator=(CollisionManager const&) = delete;
    ///
    /// \brief AddContainer adds a container to check for collision detection
    /// \param container
    /// \return
    ///
    int AddContainer(ModelContainer* container);
    ///
    /// \brief RemoveContainer removes container from collision detection
    /// \param container
    ///
    void RemoveContainer(ModelContainer* container);
    ///
    /// \brief IsColliding Using bounding box vertices 
    /// \param container the container for which we want to check collision
    /// \param index the index of the object for collision detection
    /// \return
    ///
    bool IsColliding(ModelContainer* container, int index);
};
