#include "CollisionManager.h"
#include "ModelContainer.h"

int CollisionManager::AddContainer(ModelContainer* container)
{
    containers.push_back(container);
    return containers.size() - 1;
}

void CollisionManager::RemoveContainer(ModelContainer* container)
{
    auto containerLocation = find(containers.begin(), containers.end(), container);
    if (containerLocation != containers.end())
        containers.erase(containerLocation);
}

bool CollisionManager::IsColliding(ModelContainer* container, int index)
{
    // In our bounding box vertices, the 1st vertex contains the minimum
    //      values (minX, minY, and minZ), and the 7th vertex contains the
    //      maximum values (maxX, maxY, and maxZ)
    glm::vec3 targetMinVertices = container->GetMinimumVertices();
    glm::vec3 targetMaxVertices = container->GetMaximumVertices();

    vector<glm::vec3> boundingBox;
    glm::vec3 minVertices, maxVertices;
    for (int i = 0; i < containers.size(); i++)
    {
        if (i == index)
            continue;

        minVertices = containers[i]->GetMinimumVertices();
        maxVertices = containers[i]->GetMaximumVertices();

        if (targetMinVertices.x < maxVertices.x && targetMaxVertices.x > minVertices.x &&
            targetMinVertices.y < maxVertices.y && targetMaxVertices.y > minVertices.y &&
            targetMinVertices.z < maxVertices.z && targetMaxVertices.z > minVertices.z)
            return true;
    }

    return false;
}
