#include "../components/lighting.hpp"
#include "../ecs/entity.hpp"
#include "../deserialize-utils.hpp"


namespace our {
    void LightComponent::deserialize(const nlohmann::json &data){
        if (!data.is_object()){
            return;
        }

        diffuse_light = data.value("diffuse_light", diffuse_light);
        specular_light = data.value("specular_light", specular_light);
        ambient_light = data.value("ambient_light", ambient_light);

        if (data.value("lType","directional") == "directional"){
            type = our::lType::DIRECTIONAL;
        } 
        else if (data.value("lType","directional") == "point"){
            type = our::lType::POINT;
            attenuation.constant = data.value("attenuation_constant",attenuation.constant);
            attenuation.linear = data.value("attenuation_linear",attenuation.linear);
            attenuation.quad = data.value("attenuation_quadratic",attenuation.quad);
        }
        else if (data.value("lType","directional") == "spotlight"){
            type = our::lType::SPOTLIGHT;
            attenuation.constant = data.value("attenuation_constant",attenuation.constant);
            attenuation.linear = data.value("attenuation_linear",attenuation.linear);
            attenuation.quad = data.value("attenuation_quadratic",attenuation.quad);
            spotAngle.innerCone = data.value("spot_angle_inner", spotAngle.innerCone);
            spotAngle.outerCone = data.value("spot_angle_outer", spotAngle.outerCone);
        }
    }
} 
