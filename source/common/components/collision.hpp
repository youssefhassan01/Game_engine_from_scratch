#pragma once

#include "../ecs/component.hpp"
#include <glm/glm.hpp>

namespace our {
    
    struct CollisionBoundary {
        glm::vec2 x_Boundary; //right vector
        glm::vec2 y_Boundary; //up vector
        float z_position; //depth of position
    };
    class CollisionComponent : public Component {
    public:
        std::vector<CollisionBoundary> Walls;
        glm::vec2 x_Boundary = {0, 0}; 
        glm::vec2 y_Boundary = {0, 0}; 
        glm::vec2 z_Boundary = {0, 0}; 
        int WallsNumber = 0;
        int projectilesNumber = 0;
        static std::string getID() { return "Collision"; }
        void deserialize(const nlohmann::json& data) override;
    };

}