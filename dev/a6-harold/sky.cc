/** CSci-4611 Assignment 6: Harold
 */

#include "sky.h"


Sky::Sky() {
  radius_ = 1500.0;
}


Sky::~Sky() {

}

void Sky::Init(ShaderProgram *stroke3d_shaderprog) {
    stroke3d_shaderprog_ = stroke3d_shaderprog;
}


/// Projects a 2D normalized screen point (e.g., the mouse position in normalized
/// device coordinates) to a 3D point on the "sky", which is really a huge sphere
/// (radius = 1500) that the viewer is inside.  This function should always return
/// true since any screen point can successfully be projected onto the sphere.
/// sky_point is set to the resulting 3D point.  Note, this function only checks
/// to see if the ray passing through the screen point intersects the sphere; it
/// does not check to see if the ray hits the ground or anything else first.
bool Sky::ScreenPtHitsSky(const Matrix4 &view_matrix, const Matrix4 &proj_matrix,
                        const Point2 &normalized_screen_pt, Point3 *sky_point)
{
    Matrix4 camera = view_matrix.Inverse();
    Point3 eye = camera.ColumnToPoint3(3);

    Point3 mouseIn3d = GfxMath::ScreenToNearPlane(view_matrix,
                                                  proj_matrix,
                                                  normalized_screen_pt);
    Ray pickRay(eye, (mouseIn3d - eye).ToUnit());
    float t;
    return pickRay.IntersectSphere(Point3(0,0,0), radius_, &t, sky_point);
}

/// Creates a new sky stroke mesh by projecting each vertex of the 2D mesh
/// onto the sky dome and saving the result as a new 3D mesh.
void Sky::AddSkyStroke(const Matrix4 &view_matrix, const Matrix4 &proj_matrix,
                       const Mesh &stroke2d_mesh, const Color &stroke_color)
{
  std::vector<Point3> skyVertices;
  Mesh skyMesh(stroke2d_mesh);
  Point3 v;  // vertex that holds the current screen mesh vertex & conversion

  // loop through the vertex of the copy of stroke2d_mesh and convert them to 3d
  for(int i = 0; i < skyMesh.num_vertices(); i++) {
    v = skyMesh.vertex(i);
    Point2 screenPt(v.x(), v.y());
    ScreenPtHitsSky(view_matrix, proj_matrix, screenPt, &v);
    skyVertices.push_back(v);
  }
  skyMesh.SetVertices(skyVertices);
  skyMesh.UpdateGPUMemory();

  SkyStroke skyS;
  skyS.mesh = skyMesh;
  skyS.color = stroke_color;
  strokes_.push_back(skyS);
}


/// Draws all of the sky strokes
void Sky::Draw(const Matrix4 &view_matrix, const Matrix4 &proj_matrix) {

    // Precompute matrices needed in the shader
    Matrix4 model_matrix; // identity
    Matrix4 modelview_matrix = view_matrix * model_matrix;

    // Draw sky meshes
    stroke3d_shaderprog_->UseProgram();
    stroke3d_shaderprog_->SetUniform("modelViewMatrix", modelview_matrix);
    stroke3d_shaderprog_->SetUniform("projectionMatrix", proj_matrix);
    for (int i=0; i<strokes_.size(); i++) {
        stroke3d_shaderprog_->SetUniform("strokeColor", strokes_[i].color);
        strokes_[i].mesh.Draw();
    }
    stroke3d_shaderprog_->StopProgram();
}
