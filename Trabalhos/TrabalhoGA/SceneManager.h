#pragma once
#include "Camera.h"
#include "ObjectReader.h"

class SceneManager {
public:
    SceneManager(Camera camera, ObjectReader reader);

    void initialize();
    void processInputs();
    void updateObjects();
    void updateCamera();
    void run();

private:
    Camera camera;
    ObjectReader reader;
};