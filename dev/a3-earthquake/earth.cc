/** CSci-4611 Assignment 3:  Earthquake
 */

#include "earth.h"
#include "config.h"

#include <vector>

// for M_PI constant
#define _USE_MATH_DEFINES
#include <math.h>


Earth::Earth() {
}

Earth::~Earth() {
}

void Earth::Init(const std::vector<std::string> &search_path) {
    // init shader program
    shader_.Init();

    // init texture: you can change to a lower-res texture here if needed
    earth_tex_.InitFromFile(Platform::FindFile("earth-2k.png", search_path));

    // init geometry
    const int nslices = 40;
    const int nstacks = 40;
    const float epsilon = .000000999;

    // TODO: This is where you need to set the vertices and indiceds for earth_mesh_.
    std::vector<unsigned int> indices;
    std::vector<Point2> tex_coords;

    // Construct the vertecies and index arrays for plane
    int base = 4 + 2 * (nstacks - 1);
    for (float x = -M_PI; x < M_PI + epsilon; x += 2 * M_PI / nslices) {
      for (float y = -(M_PI / 2); y < (M_PI / 2) + epsilon; y += M_PI / nstacks) {
        float u = (x + M_PI) / (2* M_PI);
        float v = (y + (M_PI / 2)) / M_PI;
        p_vertices_.push_back(Point3(x,y,0));
        tex_coords.push_back(Point2(u,-v));
        p_normals_.push_back(Vector3(0,0,1));
      }
      //skip in the first case where there are not sufficient vertices
      int i = p_vertices_.size();
      if ( x > (-M_PI + epsilon)) {
        for (int n = 0; n < nstacks; n++) {
          //bottom
          indices.push_back(i - (base - n));
          indices.push_back(i - ((base / 2) - n));
          indices.push_back(i - ((base / 2 - 1) - n));

          //top
          indices.push_back(i - (base - n));
          indices.push_back(i - ((base / 2 - 1) - n));
          indices.push_back(i - ((base - 1) - n));
        }
      }
    }

    // Set the vertices and normals for sphere
    for (float x = -M_PI; x < M_PI + epsilon; x += 2 * M_PI / nslices) {
      for (float y = -(M_PI / 2); y < (M_PI / 2) + epsilon; y += M_PI / nstacks) {
        float u = (x + M_PI) / (2* M_PI);
        float v = (y + (M_PI / 2)) / M_PI;
        float sphereX = cos(y) * sin(x);
        float sphereY = sin(y);
        float sphereZ = cos(y) * cos(x);
        s_vertices_.push_back(Point3(sphereX,sphereY,sphereZ));
        s_normals_.push_back(Vector3(sphereX,sphereY,sphereZ).ToUnit());
      }
    }

    //Set the mesh
    earth_mesh_.SetVertices(p_vertices_);
    earth_mesh_.SetNormals(p_normals_);
    earth_mesh_.SetIndices(indices);
    earth_mesh_.SetTexCoords(0,tex_coords);

    // Update the GPU with the mesh
    earth_mesh_.UpdateGPUMemory();
}



void Earth::Draw(const Matrix4 &model_matrix, const Matrix4 &view_matrix, const Matrix4 &proj_matrix) {
    // Define a really bright white light.  Lighting is a property of the "shader"
    DefaultShader::LightProperties light;
    light.position = Point3(10,10,10);
    light.ambient_intensity = Color(1,1,1);
    light.diffuse_intensity = Color(1,1,1);
    light.specular_intensity = Color(1,1,1);
    shader_.SetLight(0, light);

    // Adust the material properties, material is a property of the thing
    // (e.g., a mesh) that we draw with the shader.  The reflectance properties
    // affect the lighting.  The surface texture is the key for getting the
    // image of the earth to show up.
    DefaultShader::MaterialProperties mat;
    mat.ambient_reflectance = Color(0.5, 0.5, 0.5);
    mat.diffuse_reflectance = Color(0.75, 0.75, 0.75);
    mat.specular_reflectance = Color(0.75, 0.75, 0.75);
    mat.surface_texture = earth_tex_;

    // Draw the earth mesh using these settings
    if (earth_mesh_.num_triangles() > 0) {
        shader_.Draw(model_matrix, view_matrix, proj_matrix, &earth_mesh_, mat);
    }
}


Point3 Earth::LatLongToSphere(double latitude, double longitude) const {
    // TODO: We recommend filling in this function to put all your
    // lat,long --> sphere calculations in one place.
    float qlat = GfxMath::ToRadians(latitude);
    float qlon = GfxMath::ToRadians(longitude);
    float sphereX = cos(qlat) * sin(qlon);
    float sphereY = sin(qlat);
    float sphereZ = cos(qlat) * cos(qlon);
    return Point3(sphereX,sphereY,sphereZ);
}

Point3 Earth::LatLongToPlane(double latitude, double longitude) const {
    // TODO: We recommend filling in this function to put all your
    // lat,long --> plane calculations in one place.
    float qlat = GfxMath::ToRadians(latitude);
    float qlon = GfxMath::ToRadians(longitude);
    return Point3(qlon,qlat,0);
}



void Earth::DrawDebugInfo(const Matrix4 &model_matrix, const Matrix4 &view_matrix, const Matrix4 &proj_matrix) {
    // This draws a cylinder for each line segment on each edge of each triangle in your mesh.
    // So it will be very slow if you have a large mesh, but it's quite useful when you are
    // debugging your mesh code, especially if you start with a small mesh.
    for (int t=0; t<earth_mesh_.num_triangles(); t++) {
        std::vector<unsigned int> indices = earth_mesh_.triangle_vertices(t);
        std::vector<Point3> loop;
        loop.push_back(earth_mesh_.vertex(indices[0]));
        loop.push_back(earth_mesh_.vertex(indices[1]));
        loop.push_back(earth_mesh_.vertex(indices[2]));
        quick_shapes_.DrawLines(model_matrix, view_matrix, proj_matrix,
            Color(1,1,0), loop, QuickShapes::LinesType::LINE_LOOP, 0.005);
    }
}

void Earth::InitSphere() {
  earth_mesh_.SetVertices(s_vertices_);
  earth_mesh_.SetNormals(s_normals_);
  earth_mesh_.UpdateGPUMemory();
}

void Earth::InitPlane() {
  earth_mesh_.SetVertices(p_vertices_);
  earth_mesh_.SetNormals(p_normals_);
  earth_mesh_.UpdateGPUMemory();
}
