#pragma once

#include "../ecs/world.hpp"
#include "../components/camera.hpp"
#include "../components/free-camera-controller.hpp"

#include "../application.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/trigonometric.hpp>
#include <glm/gtx/fast_trigonometry.hpp>

namespace our
{

    // The free camera controller system is responsible for moving every entity which contains a FreeCameraControllerComponent.
    // This system is added as a slightly complex example for how use the ECS framework to implement logic.
    // For more information, see "common/components/free-camera-controller.hpp"
    class FreeCameraControllerSystem
    {

        Application *app;          // The application in which the state runs
        bool mouse_locked = false; // Is the mouse locked
    private:
        bool isJumping = false;
        float jumpVelocity = 0.0f;
        float jumpHeight = 2.0f; // Adjust the jump height as needed
        float gravity = 9.8f;    // Adjust the gravity as needed

    public:
        // When a state enters, it should call this function and give it the pointer to the application
        void enter(Application *app)
        {
            this->app = app;
        }

        // This should be called every frame to update all entities containing a FreeCameraControllerComponent
        void update(World *world, float deltaTime)
        {
            // First of all, we search for an entity containing both a CameraComponent and a FreeCameraControllerComponent
            // As soon as we find one, we break
            CameraComponent *camera = nullptr;
            FreeCameraControllerComponent *controller = nullptr;
            for (auto entity : world->getEntities())
            {
                camera = entity->getComponent<CameraComponent>();
                controller = entity->getComponent<FreeCameraControllerComponent>();
                if (camera && controller)
                    break;
            }
            // If there is no entity with both a CameraComponent and a FreeCameraControllerComponent, we can do nothing so we return
            if (!(camera && controller))
                return;
            // Get the entity that we found via getOwner of camera (we could use controller->getOwner())
            Entity *entity = camera->getOwner();

            // If the left mouse button is pressed, we lock and hide the mouse. This common in First Person Games.
            if (app->getMouse().isPressed(GLFW_MOUSE_BUTTON_1) && !mouse_locked)
            {
                app->getMouse().lockMouse(app->getWindow());
                mouse_locked = true;
                // If the left mouse button is released, we unlock and unhide the mouse.
            }
            else if (!app->getMouse().isPressed(GLFW_MOUSE_BUTTON_1) && mouse_locked)
            {
                app->getMouse().unlockMouse(app->getWindow());
                mouse_locked = false;
            }

            // We get a reference to the entity's position and rotation
            glm::vec3 &position = entity->localTransform.position;
            glm::vec3 &rotation = entity->localTransform.rotation;

            // If the left mouse button is pressed, we get the change in the mouse location
            // and use it to update the camera rotation
            if (app->getMouse().isPressed(GLFW_MOUSE_BUTTON_1))
            {
                glm::vec2 delta = app->getMouse().getMouseDelta();
                // rotation.x -= delta.y * controller->rotationSensitivity; // The y-axis controls the pitch
                rotation.y -= delta.x * controller->rotationSensitivity; // The x-axis controls the yaw
            }

            // We prevent the pitch from exceeding a certain angle from the XZ plane to prevent gimbal locks
            if (rotation.x < -glm::half_pi<float>() * 0.99f)
                rotation.x = -glm::half_pi<float>() * 0.99f;
            if (rotation.x > glm::half_pi<float>() * 0.99f)
                rotation.x = glm::half_pi<float>() * 0.99f;
            // This is not necessary, but whenever the rotation goes outside the 0 to 2*PI range, we wrap it back inside.
            // This could prevent floating point error if the player rotates in single direction for an extremely long time.
            rotation.y = glm::wrapAngle(rotation.y);

            // We update the camera fov based on the mouse wheel scrolling amount
            float fov = camera->fovY + app->getMouse().getScrollOffset().y * controller->fovSensitivity;
            fov = glm::clamp(fov, glm::pi<float>() * 0.01f, glm::pi<float>() * 0.99f); // We keep the fov in the range 0.01*PI to 0.99*PI
            camera->fovY = fov;

            // We get the camera model matrix (relative to its parent) to compute the front, up and right directions
            glm::mat4 matrix = entity->localTransform.toMat4();

            glm::vec3 front = glm::vec3(matrix * glm::vec4(0, 0, -1, 0)),
                      up = glm::vec3(matrix * glm::vec4(0, 1, 0, 0)),
                      right = glm::vec3(matrix * glm::vec4(1, 0, 0, 0));

            glm::vec3 current_sensitivity = controller->positionSensitivity;
            // If the LEFT SHIFT key is pressed, we multiply the position sensitivity by the speed up factor
            if (app->getKeyboard().isPressed(GLFW_KEY_LEFT_SHIFT))
                current_sensitivity *= controller->speedupFactor;

            // We change the camera position based on the keys WASD/QE
            // S & W moves the player back and forth
            if (app->getKeyboard().isPressed(GLFW_KEY_W))
                position += front * (deltaTime * current_sensitivity.z);
            if (app->getKeyboard().isPressed(GLFW_KEY_S))
                position -= front * (deltaTime * current_sensitivity.z);
            // Q & E moves the player up and down
            // if(app->getKeyboard().isPressed(GLFW_KEY_Q)) position += up * (deltaTime * current_sensitivity.y);
            // if(app->getKeyboard().isPressed(GLFW_KEY_E)) position -= up * (deltaTime * current_sensitivity.y);
            // A & D moves the player left or right
            if (app->getKeyboard().isPressed(GLFW_KEY_D))
                position += right * (deltaTime * current_sensitivity.x);
            if (app->getKeyboard().isPressed(GLFW_KEY_A))
                position -= right * (deltaTime * current_sensitivity.x);
            if (app->getKeyboard().isPressed(GLFW_KEY_SPACE))
            {
                if (!isJumping)
                {
                    // Start the jump
                    isJumping = true;
                    jumpVelocity = std::sqrt(2.0f * gravity * jumpHeight);
                }
            }
            else
            {
                isJumping = false;
            }

            // Update jump physics
            if (isJumping)
            {
                // Move the camera up based on the jump velocity
                position += up * (deltaTime * jumpVelocity);

                // Update jump velocity based on gravity
                jumpVelocity -= gravity * deltaTime;

                // Check if the camera has reached the ground
                if (position.y <= 0.0f)
                {
                    // Reset position and jump state
                    position.y = 0.0f;
                    isJumping = false;
                    jumpVelocity = 0.0f; // Reset jump velocity
                }
            }
            else if (position.y > 0.0f)
            {
                // Descend if the camera is above the ground
                position -= up * (deltaTime * jumpVelocity);

                // Update descent velocity based on gravity
                jumpVelocity += gravity * deltaTime;

                // Clamp descent velocity to prevent excessive speed
                jumpVelocity = glm::max(jumpVelocity, -10.0f);

                // Check if the camera has reached the ground
                if (position.y <= 0.0f)
                {
                    // Reset position and descent state
                    position.y = 0.0f;
                    jumpVelocity = 0.0f; // Reset descent velocity
                }
            }
Entity *cat = nullptr;
Entity *platform = nullptr;

// Find the cat and platform entities
for (auto entity : world->getEntities()) {
    if (entity->name == "cat") {
        cat = entity;
        printf("cat found\n");
    }    
    if (entity->name == "platform") {
        platform = entity;
        printf("platform found\n");
    }
}

if (cat && platform) {
    // Get the camera entity
    if (entity) {
glm::vec3 catPos = cat->localTransform.position;
    glm::vec3 cameraPos = entity->localTransform.position;

    // Assuming the cat's position is relative to the camera
    glm::vec3 absoluteCatPos = cameraPos + catPos;

    printf("cat position: %f %f %f\n", absoluteCatPos.x, absoluteCatPos.y, absoluteCatPos.z);

    glm::vec3 platformPos = platform->localTransform.position;
    printf("platform position: %f %f %f\n", platformPos.x, platformPos.y, platformPos.z);

    float catRadius = cat->localTransform.radius;
    float platformRadius = platform->localTransform.radius;
    float distance = glm::distance(absoluteCatPos, platformPos);

    if (distance < catRadius + platformRadius) {
        // Collision detected
        // Move the camera (assuming it's the cat's parent entity) away from the platform
        glm::vec3 direction = glm::normalize(absoluteCatPos - platformPos);
        entity->localTransform.position += direction * (catRadius + platformRadius - distance);
    }
    }
}

        }

        // When the state exits, it should call this function to ensure the mouse is unlocked
        void exit()
        {
            if (mouse_locked)
            {
                mouse_locked = false;
                app->getMouse().unlockMouse(app->getWindow());
            }
        }
    };

}
