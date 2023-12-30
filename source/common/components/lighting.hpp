#pragma once
#include "../ecs/component.hpp"
#include "../mesh/mesh.hpp"
#include "../asset-loader.hpp"


namespace our {
    enum class lType {
        DIRECTIONAL,
        POINT,
        SPOTLIGHT
    };

    struct atten {
        float constant = 1.0f;
        float linear = 0.0f;
        float quad = 0.0f;
    };

    struct angle {
        float innerCone = 0.0f;
        float outerCone = 0.1f;
    };

    class LightComponent : public Component {
        public:
            lType type = lType::DIRECTIONAL;

            glm::vec3 diffuse_light = {0,0,0};
            glm::vec3 specular_light = {0,0,0};
            glm::vec3 ambient_light = {0,0,0};

            atten attenuation;
            angle spotAngle;

            static std::string getID() { return "Light"; }

            void deserialize(const nlohmann::json& data) override;
            ~LightComponent(){}
    };
}