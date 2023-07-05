#include "program.hh"
#include "mesh.hh"
#include "camera.hh"
#include "window.hh"
#include "scene.hh"

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

#define FRAMERATE 25

int main() {
    auto window = Window("window", 1024, 576);

    auto programBasic = Program("shaders/basic.vert", "shaders/basic.frag");
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glClearColor(0.1, 0.1, 0.1, 0.0);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    GLuint vb, ib;
    glGenBuffers(1, &vb);
    glGenBuffers(1, &ib);

    Scene scene;
    std::shared_ptr<Mesh> m1 = std::make_shared<CubeMesh>();
    std::shared_ptr<Mesh> m2 = std::make_shared<SphereMesh>(128, 128);
    std::shared_ptr<Mesh> m3 = std::make_shared<SkySphereMesh>(128, 128);
    std::shared_ptr<Texture> texDebug =
        std::make_shared<Texture>("../resources/debug.png", 1, true);
    std::shared_ptr<Texture> texRed = std::make_shared<UniformTexture>(glm::vec3{1.0f, 0.0f, 0.0f});
    scene.addMesh(m1);
    scene.addMesh(m2);
    scene.addMesh(m3);
    auto sphere1 = std::make_shared<Object>(m2, texRed, glm::vec3{0.0f, 0.0f, 4.0f});
    sphere1->setMoveFunction([](Object* o, double t) {
        float x = std::cos(t / 10.0f) * 8.0f;
        float y = 0.0f;
        float z = std::sin(t / 10.0f) * 8.0f;
        o->setPosition(glm::vec3{x, y, z});
    });
    scene.addObject(sphere1);
    scene.addObject(std::make_shared<Object>(m3, texDebug, glm::vec3{0.0f, 0.0f, 0.0f},
                                             glm::vec3{0.0f, M_PI_2, 0.0f}, 15.0f));

    /* SimplePerspectiveCamera camera({0.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f, 1.0f},
                                   (float)M_PI_4, (float)window.width() / (float)window.height());
     */
    float fovy = M_PI_4;
    float fovx = 2 * std::atan(16.0f / 9.0f * std::tan(M_PI_4 / 2));
    FrustumCamera camera = FrustumCamera::build({0.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f},
                                                {0.0f, 0.0f, 1.0f}, fovx, fovy, 0.0f, 0.0f);

    glBindBuffer(GL_ARRAY_BUFFER, vb);
    glBufferData(GL_ARRAY_BUFFER, scene.getVertexBuffer().size() * sizeof(Vertex),
                 scene.getVertexBuffer().data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                          (void*)offsetof(Vertex, normal));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, uv));
    glEnableVertexAttribArray(2);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ib);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, scene.getIndexBuffer().size() * sizeof(uint32_t),
                 scene.getIndexBuffer().data(), GL_STATIC_DRAW);

    glBindVertexArray(0);

    while (!glfwWindowShouldClose(window)) {
        static double lastFrameTime = glfwGetTime();
        double currentTime = glfwGetTime();
        double deltaTime = currentTime - lastFrameTime;
        if (deltaTime < (1.0 / FRAMERATE)) {
            continue;
        }
        lastFrameTime = currentTime;
        scene.update(deltaTime);
        glfwMakeContextCurrent(window);
        glfwPollEvents();
        camera.keyMove(window.keystates, deltaTime);
        camera.mouseDrag(window.mouseButtonStates, window.mouseMove, deltaTime);
        window.mouseMove = {0, 0};
        glUseProgram(programBasic.programId);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glViewport(0, 0, window.width(), window.height());
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glBindVertexArray(vao);
        glUniformMatrix4fv(0, 1, GL_FALSE, glm::value_ptr(camera.getProjectionMatrix()));
        glUniformMatrix4fv(1, 1, GL_FALSE, glm::value_ptr(camera.getViewMatrix()));
        for (const auto& obj : scene.getObjects()) {
            glUniformMatrix4fv(2, 1, GL_FALSE, glm::value_ptr(obj->getModelMatrix()));
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, obj->getTexture()->getId());
            glDrawElements(GL_TRIANGLES, obj->getMesh()->getIndexSize(), GL_UNSIGNED_INT,
                           (void*)(obj->getMesh()->getIndexOffset() * sizeof(uint32_t)));
            glBindTexture(GL_TEXTURE_2D, 0);
        }
        glBindVertexArray(0);
        glfwSwapBuffers(window);
    }
    glDeleteBuffers(1, &vb);
    glDeleteBuffers(1, &ib);
    glDeleteVertexArrays(1, &vao);
    return 0;
}