#include "Core/Camera.h"
#include "Core/Engine.h"
#include "Math/Math.h"
#include "Renderer/Renderer.h"
#include "Renderer/Shader.h"
#include <iostream>


using namespace Engine;

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
  camera.SetPosition(Vec3(5.0f, 5.0f, 5.0f));
  camera.LookAt(Vec3(0.0f, 0.0f, 0.0f));
  camera.SetAspectRatio(800.0f, 600.0f);
  camera.SetFieldOfView(45.0f);

  // Create a grid of cubes to provide reference points
  std::vector<Vec3> cubePositions;
  for (int x = -5; x <= 5; x += 2) {
    for (int z = -5; z <= 5; z += 2) {
      cubePositions.push_back(
          Vec3(static_cast<float>(x), 0.0f, static_cast<float>(z)));
    }
  }

  // Add some elevated cubes
  cubePositions.push_back(Vec3(0.0f, 2.0f, 0.0f));   // Center elevated
  cubePositions.push_back(Vec3(3.0f, 1.0f, 3.0f));   // Corner elevated
  cubePositions.push_back(Vec3(-3.0f, 1.5f, -3.0f)); // Other corner elevated

  // Cube vertices (position only)
  float vertices[] = {
      // Front face
      -0.5f, -0.5f, 0.5f, // 0: Front bottom left
      0.5f, -0.5f, 0.5f,  // 1: Front bottom right
      0.5f, 0.5f, 0.5f,   // 2: Front top right
      -0.5f, 0.5f, 0.5f,  // 3: Front top left

      // Back face
      -0.5f, -0.5f, -0.5f, // 4: Back bottom left
      0.5f, -0.5f, -0.5f,  // 5: Back bottom right
      0.5f, 0.5f, -0.5f,   // 6: Back top right
      -0.5f, 0.5f, -0.5f   // 7: Back top left
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

  // Camera animation parameters
  float time = 0.0f;
  int cameraMode = 0; // 0: Orbit, 1: Linear, 2: Figure-8, 3: Look Around
  float modeTimer = 0.0f;
  const float MODE_DURATION = 5.0f; // 5 seconds per mode

  std::cout << "Camera Demo Controls:" << std::endl;
  std::cout << "Mode 0: Orbital camera around scene" << std::endl;
  std::cout << "Mode 1: Linear movement through scene" << std::endl;
  std::cout << "Mode 2: Figure-8 flight pattern" << std::endl;
  std::cout << "Mode 3: Fixed position, rotating view" << std::endl;

  // Main loop
  while (Engine::Engine::IsRunning()) {
    Engine::Engine::PollEvents();

    // Update time
    time += 0.016f; // Assuming ~60 FPS
    modeTimer += 0.016f;

    // Switch camera modes every 5 seconds
    if (modeTimer >= MODE_DURATION) {
      modeTimer = 0.0f;
      cameraMode = (cameraMode + 1) % 4;

      std::cout << "Switching to camera mode " << cameraMode << std::endl;
    }

    // Update camera based on current mode
    Vec3 cameraPos, lookAtPos;

    switch (cameraMode) {
    case 0: // Orbital camera
    {
      float angle = time * 0.3f;
      float radius = 8.0f;
      cameraPos =
          Vec3(Math::Cos(angle) * radius, 3.0f + Math::Sin(time * 0.5f) * 2.0f,
               Math::Sin(angle) * radius);
      lookAtPos = Vec3(0.0f, 0.0f, 0.0f);
      break;
    }
    case 1: // Linear movement
    {
      float t = (Math::Sin(time * 0.5f) + 1.0f) * 0.5f; // 0 to 1
      cameraPos = Vec3(-8.0f + t * 16.0f, 2.0f, -8.0f + t * 16.0f);
      lookAtPos = Vec3(0.0f, 0.0f, 0.0f);
      break;
    }
    case 2: // Figure-8 pattern
    {
      float angle = time * 0.4f;
      cameraPos =
          Vec3(Math::Sin(angle) * 6.0f, 4.0f, Math::Sin(angle * 2.0f) * 4.0f);
      lookAtPos = Vec3(0.0f, 0.0f, 0.0f);
      break;
    }
    case 3: // Fixed position, rotating view
    {
      cameraPos = Vec3(0.0f, 8.0f, 0.0f); // High above scene
      float lookAngle = time * 0.6f;
      lookAtPos =
          Vec3(Math::Cos(lookAngle) * 3.0f, 0.0f, Math::Sin(lookAngle) * 3.0f);
      break;
    }
    }

    camera.SetPosition(cameraPos);
    camera.LookAt(lookAtPos);

    // Render
    renderer.Clear();
    shader.Bind();

    Mat4 view = camera.GetViewMatrix();
    Mat4 projection = camera.GetProjectionMatrix();

    // Render all cubes
    for (size_t i = 0; i < cubePositions.size(); ++i) {
      // Create transformation matrix
      Mat4 model = Mat4::Translation(cubePositions[i]);
      Mat4 mvp = projection * view * model;

      // Set color based on position (for visual variety)
      Vec3 color = Vec3(0.5f + 0.5f * Math::Sin(static_cast<float>(i) * 1.3f),
                        0.5f + 0.5f * Math::Sin(static_cast<float>(i) * 2.1f),
                        0.5f + 0.5f * Math::Sin(static_cast<float>(i) * 0.7f));

      // Set uniforms
      shader.SetUniformMat4("u_MVP", mvp);
      shader.SetUniformVec3("u_Color", color);

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