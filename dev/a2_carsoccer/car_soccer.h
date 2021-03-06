/** CSci-4611 Assignment 2:  Car Soccer
 */

#ifndef CAR_SOCCER_H_
#define CAR_SOCCER_H_

#include <mingfx.h>
using namespace mingfx;

#include "ball.h"
#include "car.h"


// The main class for the Car Soccer application
class CarSoccer : public GraphicsApp {
public:
    CarSoccer();
    virtual ~CarSoccer();

    /// This is called when special keys like SPACEBAR are pressed
    void OnSpecialKeyDown(int key, int scancode, int modifiers);

    /// This is called once each frame.  dt is "delta time", the time elapsed
    /// since the last call.
    void UpdateSimulation(double dt);

    /// This is called when it is time to initialize graphics objects, like
    /// texture files.
    void InitOpenGL();

    /// This is called once each frame, and you should draw the scene inside
    /// this function.
    void DrawUsingOpenGL();

    /// This is a little utility function that is helpful.  It treats the
    /// arrow keys like a joystick and returns the direction you are pressing
    /// as a 2D vector, taking into account the fact that you might be holding
    /// down more than one key at a time.
    Vector2 joystick_direction();

    // Feel free to add more functions here as needed.
    int collision(Ball b);

    int collision(Car c);

    void reflect(Ball *bptr, int n);

    void reflect(Car *cptr, int n);

    bool carBallCollision(Ball& b, Car& c);



private:

    // Simulation objects/parameters:

    // We suggest you start with the Car and Ball objects provided, adding new
    // member variables to those classes if you need to.  You'll probably want
    // to store some other data for the simulation here too, like some value
    // for gravity.
    Car car_;
    Ball ball_;

    Vector3 gravity_ = Vector3(0, -12.8, 0);
    const float friction_ = 0.70f;
    const float epsilon_ = 0.0099f;
    const float thrustCoef_ = 150;
    const float dragCoef_ = 5;
    const float turnCoef_ = .06f;

    const float maxX_ = 40;
    const float minX_ = -40;
    const float maxY_ = 35;
    const float minY_ = 0;
    const float maxZ_ = 50;
    const float minZ_ = -50;

    const float lPost_ = -10;
    const float rPost_ = 10;
    const float tPost_ = 10;


    // Support for drawing some simple shapes:
    QuickShapes quickShapes_;

    // Images to use as textures:
    Texture2D fieldTex_;
    Texture2D crowdTex_;

    // Control the computer graphics camera (we'll learn about this in a few weeks):
    Matrix4 modelMatrix_;
    Matrix4 viewMatrix_;
    Matrix4 projMatrix_;

    // A list of paths to search for data files (images):
    std::vector<std::string> searchPath_;
};


#endif
