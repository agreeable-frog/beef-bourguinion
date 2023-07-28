#include "program.hh"
#include "mesh.hh"
#include "camera.hh"
#include "window.hh"
#include "scene.hh"
#include "lodepng.hh"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

struct StreamInfo {
    uint width;
    uint height;
    float hfov;
    float vfov;
    float heading;
    float pitch;
};

int main() {
    float framerate = 25.0f;
    float panoVfov = 3 * M_PI / 4;
    uint N = 13;
    int qualityActual = 0;
    int quality = 0;

    PanoramicCamera panoCamera({0.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f, 1.0f}, panoVfov,
                               N);
    auto mainWindow = RenderWindow(
        "window", 2000 * std::pow(2.0f, quality), 2000 * std::pow(2.0f, quality) / N * std::tan(panoVfov / 2) / std::tan(M_PI / N));

    auto roiInfo = StreamInfo{900, 600, 0.9f, 0.6f, 0, 0};
    auto roiWindow = SubWindow("ROI", roiInfo.width, roiInfo.height, &mainWindow);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();

    ImGui_ImplGlfw_InitForOpenGL(roiWindow, true);
    ImGui_ImplOpenGL3_Init();

    glfwMakeContextCurrent(mainWindow);

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
        float x = std::cos(t / 5.0f) * 8.0f;
        float y = std::sin(t / 1.0f) * 5.0f;
        float z = std::sin(t / 5.0f) * 8.0f;
        o->setPosition(glm::vec3{x, y, z});
    });
    scene.addObject(sphere1);
    scene.addObject(std::make_shared<Object>(m3, texDebug, glm::vec3{0.0f, 0.0f, 0.0f},
                                             glm::vec3{0.0f, M_PI_2, 0.0f}, 15.0f));

    glfwMakeContextCurrent(mainWindow);

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

    while (!glfwWindowShouldClose(mainWindow) && !glfwWindowShouldClose(roiWindow)) {
        static double lastFrameTime = glfwGetTime();
        double currentTime = glfwGetTime();
        double deltaTime = currentTime - lastFrameTime;
        if (deltaTime < (1.0 / framerate)) {
            continue;
        }
        lastFrameTime = currentTime;
        // FPS counter
        {
            static int counter = 0;
            static double timeRef = 0.0;
            counter++;
            timeRef += deltaTime;
            if (timeRef > 5.0) {
                double framerate = counter / timeRef;
                std::cout << "FRAMERATE : " << framerate << " fps\n";
                counter = 0;
                timeRef = 0.0;
            }
        }
        scene.update(deltaTime);
        glfwMakeContextCurrent(mainWindow);
        glUseProgram(programBasic.programId);
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, mainWindow.fbo());
        glViewport(0, 0, mainWindow.width(), mainWindow.height());
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glBindVertexArray(vao);
        for (int i = 0; i < panoCamera.nSplit(); i++) {
            glViewport((i / (float)panoCamera.nSplit()) * mainWindow.width(), 0,
                       mainWindow.width() / panoCamera.nSplit() + 1, mainWindow.height());
            auto tmpCam = panoCamera.internalCameras()[i];
            glUniformMatrix4fv(0, 1, GL_FALSE, glm::value_ptr(tmpCam.getProjectionMatrix()));
            glUniformMatrix4fv(1, 1, GL_FALSE, glm::value_ptr(tmpCam.getViewMatrix()));
            for (const auto& obj : scene.getObjects()) {
                glUniformMatrix4fv(2, 1, GL_FALSE, glm::value_ptr(obj->getModelMatrix()));
                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_2D, obj->getTexture()->getId());
                glDrawElements(GL_TRIANGLES, obj->getMesh()->getIndexSize(), GL_UNSIGNED_INT,
                               (void*)(obj->getMesh()->getIndexOffset() * sizeof(uint32_t)));
                glBindTexture(GL_TEXTURE_2D, 0);
            }
        }
        glBindVertexArray(0);

        glBindFramebuffer(GL_READ_FRAMEBUFFER, mainWindow.fbo());
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
        glViewport(0, 0, mainWindow.width(), mainWindow.height());
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glBlitFramebuffer(0, 0, mainWindow.width(), mainWindow.height(), 0, 0, mainWindow.width(),
                          mainWindow.height(), GL_COLOR_BUFFER_BIT, GL_NEAREST);

        glfwSwapBuffers(mainWindow);

        glfwMakeContextCurrent(roiWindow);

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        ImGui::SliderFloat("HFOV", &roiInfo.hfov, 0.0f, 2 * M_PI);
        ImGui::SliderFloat("VFOV", &roiInfo.vfov, 0.0f, M_PI / 2);
        ImGui::SliderFloat("Heading", &roiInfo.heading, -M_PI, M_PI);
        float pitchLimit = panoCamera.vfov() / 2 - roiInfo.vfov / 2;
        if (roiInfo.pitch > pitchLimit) roiInfo.pitch = pitchLimit;
        if (roiInfo.pitch < -pitchLimit) roiInfo.pitch = -pitchLimit;
        ImGui::SliderFloat("Pitch", &roiInfo.pitch, -pitchLimit, pitchLimit);
        ImGui::SliderInt("Subdivisions", (int*)&N, 7, 53);
        ImGui::SliderInt("Quality", &quality, -2, 2);
        N = N % 2 == 0 ? N + 1 : N;
        if (N != panoCamera.nSplit() || (quality != qualityActual)) {
            panoCamera.changeNSplit(N);
            qualityActual = quality;
            mainWindow.resize(2000 * std::pow(2.0f, quality),
                              2000 * std::pow(2.0f, quality) / N * std::tan(panoVfov / 2) / std::tan(M_PI / N));
        }

        glBindFramebuffer(GL_READ_FRAMEBUFFER, roiWindow.fbo());
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
        glViewport(0, 0, roiWindow.width(), roiWindow.height());
        float roiWidth = roiInfo.hfov / (2 * M_PI) * mainWindow.width();
        float roiHeight = std::tan(roiInfo.vfov / 2) / std::tan(panoVfov / 2) * mainWindow.height();
        float posX = mainWindow.width() / 2 * roiInfo.heading / M_PI;
        float posY = mainWindow.height() / 2 * std::tan(roiInfo.pitch) / std::tan(panoVfov / 2);
        float srcX0 = mainWindow.width() / 2 - roiWidth / 2 - posX;
        float srcY0 = mainWindow.height() / 2 - roiHeight / 2 + posY;
        float srcX1 = mainWindow.width() / 2 + roiWidth / 2 - posX;
        float srcY1 = mainWindow.height() / 2 + roiHeight / 2 + posY;
        glBlitFramebuffer(srcX0, srcY0, srcX1, srcY1, 0, 0, roiWindow.width(), roiWindow.height(),
                          GL_COLOR_BUFFER_BIT, GL_NEAREST);
        // the area spills on the left
        if (srcX0 < 0) {
            glBlitFramebuffer(mainWindow.width() + srcX0, srcY0, mainWindow.width(), srcY1, 0, 0,
                              roiWindow.width() * (-srcX0 / roiWidth) + 1, roiWindow.height(),
                              GL_COLOR_BUFFER_BIT, GL_NEAREST);
        }
        // the area spills on the right
        if (srcX1 > (mainWindow.width() - 1)) {
            glBlitFramebuffer(
                0, srcY0, srcX1 - mainWindow.width(), srcY1,
                roiWindow.width() - roiWindow.width() * (srcX1 - mainWindow.width()) / roiWidth - 1,
                0, roiWindow.width(), roiWindow.height(), GL_COLOR_BUFFER_BIT, GL_NEAREST);
        }
        glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(roiWindow);

        glfwPollEvents();
    }
    glDeleteBuffers(1, &vb);
    glDeleteBuffers(1, &ib);
    glDeleteVertexArrays(1, &vao);

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    return 0;
}