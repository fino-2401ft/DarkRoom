#include "render.hpp"
#include <memory>

int main()
{
    // Use smart pointers to avoid memory leaks
    auto roomObjects = std::make_unique<Object>("roomObj");
    roomObjects->setPosition(0.0f, 0.0f, 0.0f);
    roomObjects->setScale(3.0f, 3.0f, 3.0f);
    roomObjects->setRotationVector(0, 1, 0);
    roomObjects->setAngle(0);
    roomObjects->setModelName("../resources/models/Room/room4walls.obj");

    auto door = std::make_unique<Object>("door");
    door->setPosition(0.0f, 0.0f, -0.05f);
    door->setScale(3.0f, 3.0f, 3.0f);
    door->setRotationVector(0, 1, 0);
    door->setAngle(0);
    door->setModelName("../resources/models/Door/door.obj");

    auto blueCard = std::make_unique<Object>("blueCard");
    blueCard->setPosition(-3.58f, 0.02f, -3.28f);
    blueCard->setScale(3.0f, 3.0f, 3.0f);
    blueCard->setRotationVector(0, 1, 0);
    blueCard->setAngle(35);
    blueCard->setModelName("../resources/models/Room/blueC.obj");

    auto redCard = std::make_unique<Object>("redCard");
    redCard->setPosition(3.69f, 1.10f, -4.17f);
    redCard->setScale(3.0f, 3.0f, 3.0f);
    redCard->setRotationVector(0, 1, 0);
    redCard->setAngle(0);
    redCard->setModelName("../resources/models/Room/redC.obj");

    auto greenCard = std::make_unique<Object>("greenCard");
    greenCard->setPosition(-3.36f, 4.79f, -4.86f);
    greenCard->setScale(3.0f, 3.0f, 3.0f);
    greenCard->setRotationVector(0, 1, 0);
    greenCard->setAngle(0);
    greenCard->setModelName("../resources/models/Room/greenC.obj");

    auto yellowCard = std::make_unique<Object>("yellowCard");
    yellowCard->setPosition(2.30f, 0.93f, 2.87f);
    yellowCard->setScale(3.0f, 3.0f, 3.0f);
    yellowCard->setRotationVector(0, 1, 0);
    yellowCard->setAngle(0);
    yellowCard->setModelName("../resources/models/Room/yellowC.obj");

    auto room = std::make_unique<Object>();
    room->addObject(roomObjects.get());
    room->addObject(door.get());
    room->addObject(blueCard.get());
    room->addObject(redCard.get());
    room->addObject(greenCard.get());
    room->addObject(yellowCard.get());

    visualisation::render render(room.get());
    return 0;
}
