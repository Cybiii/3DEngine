#include "Core/Camera.h"
#include "Core/Engine.h"
#include "Math/Math.h"
#include "Renderer/Renderer.h"
#include "Renderer/Shader.h"
#include <iostream>
#include <vector>


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
  camera.SetPosition(Vec3(10.0f, 5.0f, 10.0f));
  camera.LookAt(Vec3(0.0f, 0.0f, 0.0f));
  camera.SetAspectRatio(800.0f, 600.0f);
  camera.SetFieldOfView(45.0f);

  // Math demonstration objects
  struct MathObject {
    Vec3 position;
    Quaternion rotation;
    Vec3 scale;
    Vec3 color;
    Transform transform;
  };

  std::vector<MathObject> objects;

  // Create objects to demonstrate different math operations
  for (int i = 0; i < 8; ++i) {
    MathObject obj;

    // Position objects in a circle
    float angle = static_cast<float>(i) * 2.0f * Math::PI / 8.0f;
    obj.position = Vec3(Math::Cos(angle) * 5.0f, 0.0f, Math::Sin(angle) * 5.0f);

    // Different rotations
    obj.rotation = Quaternion::FromAxisAngle(Vec3::Up(), angle);

    // Different scales
    obj.scale = Vec3(0.5f + static_cast<float>(i) * 0.1f);

    // Different colors
    obj.color = Vec3(0.5f + 0.5f * Math::Sin(static_cast<float>(i) * 0.8f),
                     0.5f + 0.5f * Math::Sin(static_cast<float>(i) * 1.2f),
                     0.5f + 0.5f * Math::Sin(static_cast<float>(i) * 1.6f));

    // Initialize transform
    obj.transform.position = obj.position;
    obj.transform.rotation = obj.rotation;
    obj.transform.scale = obj.scale;

    objects.push_back(obj);
  }

  // Cube vertices
  float vertices[] = {
      // Front face
      -0.5f, -0.5f, 0.5f, // 0
      0.5f, -0.5f, 0.5f,  // 1
      0.5f, 0.5f, 0.5f,   // 2
      -0.5f, 0.5f, 0.5f,  // 3

      // Back face
      -0.5f, -0.5f, -0.5f, // 4
      0.5f, -0.5f, -0.5f,  // 5
      0.5f, 0.5f, -0.5f,   // 6
      -0.5f, 0.5f, -0.5f   // 7
  };

  unsigned int indices[] = {
      0, 1, 2, 2, 3, 0, // Front
      4, 6, 5, 6, 4, 7, // Back
      4, 0, 3, 3, 7, 4, // Left
      1, 5, 6, 6, 2, 1, // Right
      3, 2, 6, 6, 7, 3, // Top
      4, 5, 1, 1, 0, 4  // Bottom
  };

  // Create buffers
  VertexArray vao;
  VertexBuffer vbo(vertices, sizeof(vertices));
  IndexBuffer ibo(indices, sizeof(indices) / sizeof(unsigned int));

  vao.Bind();
  vbo.Bind();
  ibo.Bind();
  vao.AddAttribute(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
  vao.Unbind();

  renderer.EnableDepthTesting();

  std::cout << "Math Playground Demo:" << std::endl;
  std::cout << "- Quaternion rotations around Y-axis" << std::endl;
  std::cout << "- Transform interpolation between positions" << std::endl;
  std::cout << "- Matrix transformations (TRS)" << std::endl;
  std::cout << "- SIMD-optimized vector operations" << std::endl;
  std::cout << "- Camera view/projection matrices" << std::endl;

  // Main loop
  float time = 0.0f;
  while (Engine::Engine::IsRunning()) {
    Engine::Engine::PollEvents();

    time += 0.016f;

    // Update camera to orbit around the scene
    float cameraAngle = time * 0.2f;
    Vec3 cameraPos = Vec3(Math::Cos(cameraAngle) * 12.0f,
                          5.0f + Math::Sin(time * 0.5f) * 2.0f,
                          Math::Sin(cameraAngle) * 12.0f);
    camera.SetPosition(cameraPos);
    camera.LookAt(Vec3(0.0f, 0.0f, 0.0f));

    // Update objects with different math operations
    for (size_t i = 0; i < objects.size(); ++i) {
      MathObject &obj = objects[i];

      // Demonstrate quaternion rotations
      float rotSpeed = 0.02f + static_cast<float>(i) * 0.005f;
      Quaternion additionalRot =
          Quaternion::FromAxisAngle(Vec3::Up(), time * rotSpeed);
      obj.transform.rotation = additionalRot * obj.rotation;

      // Demonstrate transform interpolation
      Vec3 basePos = obj.position;
      Vec3 offsetPos =
          basePos +
          Vec3(0.0f, Math::Sin(time + static_cast<float>(i)) * 2.0f, 0.0f);
      obj.transform.position = basePos.Lerp(
          offsetPos,
          (Math::Sin(time * 2.0f + static_cast<float>(i)) + 1.0f) * 0.5f);

      // Demonstrate scale animation
      float scaleMultiplier =
          1.0f + 0.3f * Math::Sin(time * 3.0f + static_cast<float>(i) * 0.5f);
      obj.transform.scale = obj.scale * scaleMultiplier;
    }

    // Render
    renderer.Clear();
    shader.Bind();

    Mat4 view = camera.GetViewMatrix();
    Mat4 projection = camera.GetProjectionMatrix();

    // Render all objects
    for (const auto &obj : objects) {
      // Use the Transform class to create the model matrix
      Mat4 model = obj.transform.ToMatrix();
      Mat4 mvp = projection * view * model;

      shader.SetUniformMat4("u_MVP", mvp);
      shader.SetUniformVec3("u_Color", obj.color);

      vao.Bind();
      renderer.DrawElements(GL_TRIANGLES, 36);
      vao.Unbind();
    }

    // Render center reference cube
    Mat4 centerModel = Mat4::Scale(Vec3(0.2f));
    Mat4 centerMvp = projection * view * centerModel;
    shader.SetUniformMat4("u_MVP", centerMvp);
    shader.SetUniformVec3("u_Color", Vec3(1.0f, 1.0f, 1.0f));

    vao.Bind();
    renderer.DrawElements(GL_TRIANGLES, 36);
    vao.Unbind();

    shader.Unbind();
    Engine::Engine::SwapBuffers();
  }

  // Cleanup
  Engine::Engine::Shutdown();
  return 0;
}