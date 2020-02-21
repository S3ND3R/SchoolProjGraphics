/** CSci-4611 Assignment 2:  Car Soccer
 */

#include "car_soccer.h"
#include "config.h"


CarSoccer::CarSoccer() : GraphicsApp(1024,768, "Car Soccer") {
    // Define a search path for finding data files (images and shaders)
    searchPath_.push_back(".");
    searchPath_.push_back("./data");
    searchPath_.push_back(DATA_DIR_INSTALL);
    searchPath_.push_back(DATA_DIR_BUILD);
}

CarSoccer::~CarSoccer() {
}


Vector2 CarSoccer::joystick_direction() {
    Vector2 dir;
    if (IsKeyDown(GLFW_KEY_LEFT))
        dir[0]--;
    if (IsKeyDown(GLFW_KEY_RIGHT))
        dir[0]++;
    if (IsKeyDown(GLFW_KEY_UP))
        dir[1]++;
    if (IsKeyDown(GLFW_KEY_DOWN))
        dir[1]--;
    return dir;
}


void CarSoccer::OnSpecialKeyDown(int key, int scancode, int modifiers) {
    if (key == GLFW_KEY_SPACE) {
      ball_.Reset();
    }
}


void CarSoccer::UpdateSimulation(double timeStep) {
    // Here's where you shound do your "simulation", updating the positions of the
    // car and ball as needed and checking for collisions.  Filling this routine
    // in is the main part of the assignment.
    Vector2 dir = joystick_direction();
    float thrust = 150 * -dir[1];
    float drag = 5 * car_.speed();
    float newSpeed = (thrust - drag)* timeStep;
    car_.set_speed(car_.speed() + newSpeed);

    float turnRate = .06 * dir[0];
    float incAngle = turnRate * car_.speed() * timeStep;
    car_.set_angle(car_.angle() + incAngle);
    std::cout << car_.angle() << std::endl;
    Vector3 thrustV(car_.speed() * sin(car_.angle()), 0, car_.speed() * cos(car_.angle()));
    car_.set_velocity(thrustV);
    Point3 newPos = car_.position() + car_.velocity() * timeStep;
    car_.set_position(newPos);
    int colLoc = collision(car_);
    if (colLoc > 0) {
      reflect(&car_, colLoc);
    }
    //std::cout << car_.velocity() << std::endl;
    //car_.set_velocity(rotate(car_.velocity(), 45));




    // calculating ball position
    Vector3 newVel = ball_.velocity() + gravity_ * timeStep;
    ball_.set_velocity(newVel);
    Point3 newBalPos = ball_.position() + ball_.velocity() * timeStep;
    ball_.set_position(newBalPos);
    colLoc = collision(ball_);

    if (colLoc > 0) {
      reflect(&ball_, colLoc);
      ball_.set_velocity(ball_.velocity() * friction_);
    }

    if (carBallCollision(ball_, car_)) {
      float desDist = ball_.radius() + car_.collision_radius();
      Vector3 colVect = ball_.position() - car_.position();
      float colDist = colVect.Length();
      float amount = desDist - colDist;
      Vector3 colNorm = Vector3::Normalize(colVect);
      Point3 corP = ball_.position() - (colVect * (amount + .99));
      ball_.set_position(corP);
    }
}


void CarSoccer::InitOpenGL() {
    // Set up the camera in a good position to see the entire field
    projMatrix_ = Matrix4::Perspective(60, aspect_ratio(), 1, 1000);
    viewMatrix_ = Matrix4::LookAt(Point3(0,60,70), Point3(0,0,10), Vector3(0,1,0));

    // Set a background color for the screen
    glClearColor(0.8,0.8,0.8, 1);

    // Load some image files we'll use
    fieldTex_.InitFromFile(Platform::FindFile("pitch.png", searchPath_));
    crowdTex_.InitFromFile(Platform::FindFile("crowd.png", searchPath_));
}


void CarSoccer::DrawUsingOpenGL() {
    // Draw the crowd as a fullscreen background image
    quickShapes_.DrawFullscreenTexture(Color(1,1,1), crowdTex_);

    // Draw the field with the field texture on it.
    Color col(16.0/255.0, 46.0/255.0, 9.0/255.0);
    Matrix4 M = Matrix4::Translation(Vector3(0,-0.201,0)) * Matrix4::Scale(Vector3(50, 1, 60));
    quickShapes_.DrawSquare(modelMatrix_ * M, viewMatrix_, projMatrix_, col);
    M = Matrix4::Translation(Vector3(0,-0.2,0)) * Matrix4::Scale(Vector3(40, 1, 50));
    quickShapes_.DrawSquare(modelMatrix_ * M, viewMatrix_, projMatrix_, Color(1,1,1), fieldTex_);

    // Draw the car
    Color carcol(0.8, 0.2, 0.2);
    Matrix4 Mcar =
        Matrix4::Translation(car_.position() - Point3(0,0,0)) *
        Matrix4::RotationY(car_.angle()) *
        Matrix4::Scale(car_.size()) *
        Matrix4::Scale(Vector3(0.5,0.5,0.5));
    quickShapes_.DrawCube(modelMatrix_ * Mcar, viewMatrix_, projMatrix_, carcol);


    // Draw the ball
    Color ballcol(1,1,1);
    Matrix4 Mball =
        Matrix4::Translation(ball_.position() - Point3(0,0,0)) *
        Matrix4::Scale(Vector3(ball_.radius(), ball_.radius(), ball_.radius()));
    quickShapes_.DrawSphere(modelMatrix_ * Mball, viewMatrix_, projMatrix_, ballcol);


    // Draw the ball's shadow -- this is a bit of a hack, scaling Y by zero
    // flattens the sphere into a pancake, which we then draw just a bit
    // above the ground plane.
    Color shadowcol(0.2,0.4,0.15);
    Matrix4 Mshadow =
        Matrix4::Translation(Vector3(ball_.position()[0], -0.1, ball_.position()[2])) *
        Matrix4::Scale(Vector3(ball_.radius(), 0, ball_.radius())) *
        Matrix4::RotationX(90);
    quickShapes_.DrawSphere(modelMatrix_ * Mshadow, viewMatrix_, projMatrix_, shadowcol);

    // Draw the field boundary
    Color cBound = Color(1, 1, 1);
    std::vector<Point3> strip;
    strip.push_back(Point3(minX_, minY_, maxZ_));
    strip.push_back(Point3(minX_, maxY_, maxZ_));
    strip.push_back(Point3(maxX_, maxY_, maxZ_));
    strip.push_back(Point3(maxX_, minY_, maxZ_));
    strip.push_back(Point3(maxX_, maxY_, maxZ_));
    strip.push_back(Point3(maxX_, maxY_, minZ_));
    strip.push_back(Point3(maxX_, minY_, minZ_));
    strip.push_back(Point3(maxX_, maxY_, minZ_));
    strip.push_back(Point3(minX_, maxY_, minZ_));
    strip.push_back(Point3(minX_, minY_, minZ_));
    strip.push_back(Point3(minX_, maxY_, minZ_));
    strip.push_back(Point3(minX_, maxY_, maxZ_));
    quickShapes_.DrawLines(modelMatrix_, viewMatrix_, projMatrix_, cBound, strip, QuickShapes::LinesType::LINE_STRIP, 0.1);

    //Draw the goals
    Color cGoal = Color(0, 0, 1);
    std::vector<Point3> blines;
    for(int i = lPost_; i <= rPost_; i+= 2) {
      blines.push_back(Point3(i, tPost_, minZ_));
      blines.push_back(Point3(i, 0, minZ_));
    }
    for(int i = 2; i <= tPost_; i+= 2) {
      blines.push_back(Point3(lPost_, i, minZ_));
      blines.push_back(Point3(rPost_, i, minZ_));
    }

    quickShapes_.DrawLines(modelMatrix_, viewMatrix_, projMatrix_, cGoal, blines, QuickShapes::LinesType::LINES, 0.2);

    cGoal = Color(1, 0, 0);
    std::vector<Point3> rlines;
    for(int i = lPost_; i <= rPost_; i+= 2) {
      rlines.push_back(Point3(i, tPost_, maxZ_));
      rlines.push_back(Point3(i, 0, maxZ_));
    }
    for(int i = 2; i <= tPost_; i+= 2) {
      rlines.push_back(Point3(lPost_, i, maxZ_));
      rlines.push_back(Point3(rPost_, i, maxZ_));
    }
    quickShapes_.DrawLines(modelMatrix_, viewMatrix_, projMatrix_, cGoal, rlines, QuickShapes::LinesType::LINES, 0.2);


    // Debugging code that draws arrows for the velocity
    quickShapes_.DrawArrow(modelMatrix_, viewMatrix_, projMatrix_, Color(1,0,0), ball_.position(), ball_.velocity(), 0.1);
    quickShapes_.DrawArrow(modelMatrix_, viewMatrix_, projMatrix_, Color(0,1,0), car_.position(), car_.velocity(), 0.1);

}

int CarSoccer::collision(Ball b) {
  const Point3 bPos = b.position();
  const float x = bPos[0];
  const float y = bPos[1];
  const float z = bPos[2];
  const float r = b.radius();

  if ( x - r <= minX_ || x + r >= maxX_ ) {
    return 1;
  } else if (y - r <= minY_ || y + r >= maxY_) {
    return 2;
  } else if (z - r <= minZ_ || z + r >= maxZ_) {
    return 3;
  }
  return 0;
}

int CarSoccer::collision(Car c) {
  const Point3 cPos = c.position();
  const float x = cPos[0];
  const float y = cPos[1];
  const float z = cPos[2];
  const float r = c.collision_radius();

  if ( x - r <= minX_ || x + r >= maxX_ ) {
    return 1;
  } else if (y - r <= minY_ || y + r >= maxY_) {
    return 2;
  } else if (z - r <= minZ_ || z + r >= maxZ_) {
    return 3;
  }
  return 0;

}

void CarSoccer::reflect(Ball *bptr, int n) {
  float x = bptr->position()[0];
  float y = bptr->position()[1];
  float z = bptr->position()[2];
  float r = bptr->radius();
  Vector3 d = bptr->velocity();
  Vector3 reflect = bptr->velocity();
  Vector3 norm = Vector3(0,0,0);
  float dot;
  float over;

  if (n == 1) {
    if (x + r >= maxX_) {
      // std::cout << "in x: " << bptr->position()[0] <<std::endl;
      over = (x + r) - maxX_;
      bptr->set_position(Point3((maxX_ - (over + 3)), y, z));
      //bptr->position()[0] = maxX_ - bptr->radius();
      // std::cout << "in x: " << bptr->position()[0] <<std::endl;
      norm = Vector3(-1,0,0);
    } else {
      over = (x - r) - minX_;
      bptr->set_position(Point3((minX_ - (over - 3)), y, z));
      norm = Vector3(1,0,0);
    }
  } else if (n == 2) {
    if (y + r >= maxY_) {
      over = (y + r) - maxY_;
      bptr->set_position(Point3(x,(maxY_ - (over + 3)),z));
      norm = Vector3(0,-1,0);
    } else {
      over = (y - r) - minY_;
      bptr->set_position(Point3(x,(minY_ - (over - 3)),z));
      norm = Vector3(0,1,0);
    }
  } else if (n == 3) {
    if (z + r >= maxZ_) {
      over = (z + r) - maxZ_;
      bptr->set_position(Point3(x, y, (maxZ_ - (over + 3))));
      norm = Vector3(0,0,-1);
    } else {
      over = (z - r) - minZ_;
      bptr->set_position(Point3(x, y, (minZ_ - (over - 3))));
      norm = Vector3(0,0,1);
    }
  }
  dot = d.Dot(norm);
  reflect = d - (2 * norm * dot);
  bptr->set_velocity(reflect);
}

void CarSoccer::reflect(Car *cptr, int n) {
  float x = cptr->position()[0];
  float y = cptr->position()[1];
  float z = cptr->position()[2];
  float r = cptr->collision_radius();
  Vector3 d = cptr->velocity();
  Vector3 reflect = cptr->velocity();
  Vector3 norm = Vector3(0,0,0);
  float dot;
  float over;

  if (n == 1) {
    if (x + r >= maxX_) {
      // std::cout << "in x: " << bptr->position()[0] <<std::endl;
      over = (x + r) - maxX_;
      cptr->set_position(Point3((maxX_ - (over + 3)),y,z));
      //bptr->position()[0] = maxX_ - bptr->radius();
      // std::cout << "in x: " << bptr->position()[0] <<std::endl;
      norm = Vector3(-1,0,0);
    } else {
      over = (x - r) - minX_;
      cptr->set_position(Point3((minX_ - (over - 3)), y, z));
      norm = Vector3(1,0,0);
    }
  } else if (n == 2) {
    if (y + r >= maxY_) {
      over = (y + r) - maxY_;
      cptr->set_position(Point3(x,(maxY_ - (over + 3)),z));
      norm = Vector3(0,-1,0);
    } else {
      over = (y - r) - minY_;
      cptr->set_position(Point3(x,(minY_ - (over - 3)),z));
      norm = Vector3(0,1,0);
    }
  } else if (n == 3) {
    if (z + r >= maxZ_) {
      over = (z + r) - maxZ_;
      cptr->set_position(Point3(x, y, (maxZ_ - (over + 3))));
      norm = Vector3(0,0,-1);
    } else {
      over = (z - r) - minZ_;
      cptr->set_position(Point3(x, y, (minZ_ - (over - 3))));
      norm = Vector3(0,0,1);
    }
  }
  dot = d.Dot(norm);
  reflect = d - (2 * norm * dot);
  cptr->set_velocity(reflect);
}

Vector3 CarSoccer::rotate(Vector3 v, float angle){
  float x = v[0];
  float z = v[2];
  //std::cout << "before:" << x << " and," << z << std::endl;
  angle = GfxMath::ToRadians(angle);
  float cosVal = cos(angle);
  float sinVal = sin(angle);

  float xp = x * cosVal - z * sinVal;
  float zp = x * sinVal + z * cosVal;
  //std::cout << "after:" << xp << " ," << zp << std::endl;
  return Vector3(xp, 0, zp);
}

bool CarSoccer::carBallCollision(Ball& b, Car& c){
  float safeDist = b.radius() + c.collision_radius();
  Vector3 curDistVect = b.position() - c.position();
  float curDist = curDistVect.Length();
  if (curDist <= safeDist) {
    return true;
  }
  return false;
}
