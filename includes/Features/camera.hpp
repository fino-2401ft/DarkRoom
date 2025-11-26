// includes/Features/camera.hpp
#pragma once

#include "../VecMat/vector.hpp"
#include "../VecMat/matrix.hpp"

enum Camera_Movement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT,
    UP,
    DOWN
};

constexpr float CAM_YAW         = -90.0f;
constexpr float CAM_PITCH       =   0.0f;
constexpr float CAM_SPEED       =   5.0f;
constexpr float CAM_SENSITIVITY =   0.1f;
constexpr float CAM_ZOOM        =  45.0f;

// Giới hạn phòng – vec3 không phải literal type → dùng static const
static const VecMat::vec3 ROOM_MIN = VecMat::vec3(-4.0f,  1.5f, -4.9f);
static const VecMat::vec3 ROOM_MAX = VecMat::vec3( 4.4f,  7.0f,  5.2f);

class Camera {
public:
    VecMat::vec3 Position;
    VecMat::vec3 Front;
    VecMat::vec3 Up;
    VecMat::vec3 Right;
    VecMat::vec3 WorldUp;

    float Yaw;
    float Pitch;
    float MovementSpeed;
    float MouseSensitivity;
    float Zoom;

    bool freeCamMode = false;   // true = bay tự do (khi thắng game)

    // Constructor
    Camera(VecMat::vec3 position = VecMat::vec3(0.0f, 2.0f, 5.0f),
           VecMat::vec3 up       = VecMat::vec3(0.0f, 1.0f, 0.0f),
           float yaw = CAM_YAW,
           float pitch = CAM_PITCH);

    VecMat::mat4 GetViewMatrix() const;
    VecMat::vec3 GetHandPosition() const;   // vị trí tay cầm đèn pin / nến

    void ProcessKeyboard(Camera_Movement direction, float deltaTime);
    void ProcessMouseMovement(float xoffset, float yoffset, bool constrainPitch = true);
    void ProcessMouseScroll(float yoffset);

    void ToggleFreeMode() { freeCamMode = !freeCamMode; }

private:
    void updateCameraVectors();
    bool isInsideRoom(const VecMat::vec3& pos) const;
};