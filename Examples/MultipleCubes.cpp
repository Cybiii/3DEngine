#include "Core/Camera.h"
#include "Core/Engine.h"
#include "Math/Math.h"
#include "Renderer/Renderer.h"
#include "Renderer/Shader.h"
#include <iostream>
#include <vector>


using namespace Engine;

struct CubeData {
  Vec3 position;
  Vec3 rotation;
  Vec3 scale;
  Vec3 color;
  float rotationSpeed;
};

int main() {
  // Initialize the engine
  if (!Engine::Engine::Initialize()) {
    std::cerr << "Failed to initialize engine!" << std::endl;
    return -1;
  }

  // Create renderer, shader, and camera
  Renderer renderer;
  Shader shader("Cube.vert", "Cube.frag");
  Camera camera;

  // Set up camera
  camera.SetPosition(Vec3(0.0f, 3.0f, 10.0f));
  camera.LookAt(Vec3(0.0f, 0.0f, 0.0f));
  camera.SetAspectRatio(800.0f, 600.0f);
  camera.SetFieldOfView(45.0f);

  // Create multiple cubes with different properties
  std::vector<CubeData> cubes = {// Center cube
                                 {Vec3(0.0f, 0.0f, 0.0f), Vec3(0.0f),
                                  Vec3(1.0f), Vec3(1.0f, 0.0f, 0.0f), 0.02f},
                                 // Orbiting cubes
                                 {Vec3(3.0f, 0.0f, 0.0f), Vec3(0.0f),
                                  Vec3(0.5f), Vec3(0.0f, 1.0f, 0.0f), 0.03f},
                                 {Vec3(-3.0f, 0.0f, 0.0f), Vec3(0.0f),
                                  Vec3(0.5f), Vec3(0.0f, 0.0f, 1.0f), 0.025f},
                                 {Vec3(0.0f, 3.0f, 0.0f), Vec3(0.0f),
                                  Vec3(0.7f), Vec3(1.0f, 1.0f, 0.0f), 0.015f},
                                 {Vec3(0.0f, -3.0f, 0.0f), Vec3(0.0f),
                                  Vec3(0.7f), Vec3(1.0f, 0.0f, 1.0f), 0.035f},
                                 {Vec3(0.0f, 0.0f, 3.0f), Vec3(0.0f),
                                  Vec3(0.8f), Vec3(0.0f, 1.0f, 1.0f), 0.01f},
                                 {Vec3(0.0f, 0.0f, -3.0f), Vec3(0.0f),
                                  Vec3(0.8f), Vec3(1.0f, 1.0f, 1.0f), 0.04f}};

  // Cube vertices (position only, we'll set color via uniform)
  float vertices[] = {
      // Front face
      -1.0f, -1.0f, 1.0f, // 0: Front bottom left
      1.0f, -1.0f, 1.0f,  // 1: Front bottom right
      1.0f, 1.0f, 1.0f,   // 2: Front top right
      -1.0f, 1.0f, 1.0f,  // 3: Front top left

      // Back face
      -1.0f, -1.0f, -1.0f, // 4: Back bottom left
      1.0f, -1.0f, -1.0f,  // 5: Back bottom right
      1.0f, 1.0f, -1.0f,   // 6: Back top right
      -1.0f, 1.0f, -1.0f   // 7: Back top left
  };

  unsigned int indices[] = {// Front face
                            0, 1, 2, 2, 3, 0,
                            // Back face
                            4, 6, 5, 6, 4, 7,
                            // Left face
                            4, 0, 3, 3, 7, 4,
                            // Right face
                            1, 5, 6, 6, 2, 1,
                            // Top face
                            3, 2, 6, 6, 7, 3,
                            // Bottom face
                            4, 5, 1, 1, 0, 4};

  // Create buffers
  VertexArray vao;
  VertexBuffer vbo(vertices, sizeof(vertices));
  IndexBuffer ibo(indices, sizeof(indices) / sizeof(unsigned int));

  // Set up vertex attributes
  vao.Bind();
  vbo.Bind();
  ibo.Bind();

  // Position attribute (location = 0)
  vao.AddAttribute(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);

  vao.Unbind();

  // Enable depth testing
  renderer.EnableDepthTesting();

  // Main loop
  float time = 0.0f;
  while (Engine::Engine::IsRunning()) {
    Engine::Engine::PollEvents();

    // Update time
    time += 0.016f; // Assuming ~60 FPS

    // Update camera to orbit around the scene
    float cameraAngle = time * 0.2f;
    Vec3 cameraPos = Vec3(Math::Cos(cameraAngle) * 10.0f, 5.0f,
                          Math::Sin(cameraAngle) * 10.0f);
    camera.SetPosition(cameraPos);
    camera.LookAt(Vec3(0.0f, 0.0f, 0.0f));

    // Render
    renderer.Clear();
    shader.Bind();

    Mat4 view = camera.GetViewMatrix();
    Mat4 projection = camera.GetProjectionMatrix();

    // Render each cube
    for (auto &cube : cubes) {
      // Update cube rotation
      cube.rotation.x += cube.rotationSpeed;
      cube.rotation.y += cube.rotationSpeed * 0.7f;
      cube.rotation.z += cube.rotationSpeed * 0.3f;

      // Create transformation matrix
      Mat4 translation = Mat4::Translation(cube.position);
      Mat4 rotationX = Mat4::RotationX(cube.rotation.x);
      Mat4 rotationY = Mat4::RotationY(cube.rotation.y);
      Mat4 rotationZ = Mat4::RotationZ(cube.rotation.z);
      Mat4 scale = Mat4::Scale(cube.scale);

      Mat4 model = translation * rotationZ * rotationY * rotationX * scale;
      Mat4 mvp = projection * view * model;

      // Set uniforms
      shader.SetUniformMat4("u_MVP", mvp);
      shader.SetUniformVec3("u_Color", cube.color);

      // Draw cube
      vao.Bind();
      renderer.DrawElements(GL_TRIANGLES, 36);
      vao.Unbind();
    }

    shader.Unbind();
    Engine::Engine::SwapBuffers();
  }

  // Cleanup
  Engine::Engine::Shutdown();
  return 0;
}