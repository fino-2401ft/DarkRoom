// src/Features/camera.cpp
#include "../includes/Features/camera.hpp"
#include <cmath>   // sin, cos

Camera::Camera(VecMat::vec3 position, VecMat::vec3 up, float yaw, float pitch)
    : Position(position),
      WorldUp(up),
      Front(VecMat::vec3(0.0f, 0.0f, -1.0f)),
      Yaw(yaw),
      Pitch(pitch),
      MovementSpeed(CAM_SPEED),
      MouseSensitivity(CAM_SENSITIVITY),
      Zoom(CAM_ZOOM),
      freeCamMode(false)
{
    updateCameraVectors();
}

VecMat::mat4 Camera::GetViewMatrix() const {
    return VecMat::lookAt(Position, Position + Front, Up);
}

VecMat::vec3 Camera::GetHandPosition() const {
    // Đèn pin / nến nằm ngay trong tay người chơi
    return Position
           + Front * 0.6f      // ra phía trước
           + Right * 0.35f      // lệch sang phải một chút
           + Up * (-0.4f);      // thấp hơn mắt (dùng + với số âm để tránh ambiguous operator)
}

void Camera::ProcessKeyboard(Camera_Movement direction, float deltaTime) {
    float velocity = MovementSpeed * deltaTime;
    VecMat::vec3 newPos = Position;

    if (direction == FORWARD)   newPos += Front * velocity;
    if (direction == BACKWARD)  newPos -= Front * velocity;
    if (direction == LEFT)      newPos -= Right * velocity;
    if (direction == RIGHT)     newPos += Right * velocity;
    if (direction == UP)        newPos += WorldUp * velocity;
    if (direction == DOWN)      newPos -= WorldUp * velocity;

    if (freeCamMode || isInsideRoom(newPos)) {
        Position = newPos;
    }
}

void Camera::ProcessMouseMovement(float xoffset, float yoffset, bool constrainPitch) {
    xoffset *= MouseSensitivity;
    yoffset *= MouseSensitivity;

    Yaw   += xoffset;
    Pitch += yoffset;

    if (constrainPitch) {
        if (Pitch > 89.0f)  Pitch = 89.0f;
        if (Pitch < -89.0f) Pitch = -89.0f;
    }

    updateCameraVectors();
}

void Camera::ProcessMouseScroll(float yoffset) {
    Zoom -= yoffset;
    if (Zoom < 1.0f)  Zoom = 1.0f;
    if (Zoom > 90.0f) Zoom = 90.0f;
}

void Camera::updateCameraVectors() {
    VecMat::vec3 front;
    front.x = cos(to_radians(Yaw)) * cos(to_radians(Pitch));
    front.y = sin(to_radians(Pitch));
    front.z = sin(to_radians(Yaw)) * cos(to_radians(Pitch));
    Front = VecMat::normalize(front);

    Right = VecMat::normalize(VecMat::cross(Front, WorldUp));
    Up    = VecMat::normalize(VecMat::cross(Right, Front));
}

bool Camera::isInsideRoom(const VecMat::vec3& pos) const {
    return (pos.x >= ROOM_MIN.x && pos.x <= ROOM_MAX.x &&
            pos.y >= ROOM_MIN.y && pos.y <= ROOM_MAX.y &&
            pos.z >= ROOM_MIN.z && pos.z <= ROOM_MAX.z);
}