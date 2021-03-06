/** CSci-4611 Assignment 2:  Car Soccer
 */

#ifndef BALL_H_
#define BALL_H_

#include <mingfx.h>

/// Small data structure for a ball
class Ball {
public:

    /// The constructor sets the radius and calls Reset() to start the ball at
    /// the center of the field
    Ball() : radius_(2.6) {
        Reset();
    }

    /// Nothing special needed in the constructor
    virtual ~Ball() {}


    void Reset() {
        position_ = Point3(0, radius_, 0);
        float x = rand() % 50 + (-25);
        float y = rand() % 50 + (-25);
        float z = rand() % 50 + (-25);
        velocity_ = Vector3(x, y, z);
    }

    float radius() { return radius_; }

    Point3 position() { return position_; }
    void set_position(const Point3 &p) { position_ = p; }

    Vector3 velocity() { return velocity_; };
    void set_velocity(const Vector3 &v) { velocity_ = v; }


private:
    // You will probably need to store some additional data here, e.g., velocity

    Point3 position_;
    float radius_;
    Vector3 velocity_;
};

#endif
