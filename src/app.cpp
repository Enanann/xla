#include "app.hpp"
#include "batch/batch_processor.hpp"
#include "imgui_system.hpp"
#include "input_manager.hpp"
#include "renderer.hpp"
#include <memory>

void Application::run() {
    Application::initWindow();
    Application::initRenderer();
    Application::initBatchProcessor();
    Application::initImGuiSystem();
    Application::initInputManager();
    Application::mainLoop();
    Application::cleanup();
}

void Application::initWindow() {
    mWindow.create();
}

void Application::initRenderer() {
    mRenderer = std::make_unique<Renderer>(mWindow);
}

void Application::initBatchProcessor() {
    mBatchProcessor = std::make_unique<BatchProcessor>(mRenderer.get());
}

void Application::initImGuiSystem() {
    mImGuiSystem = std::make_unique<ImGuiSystem>(mRenderer.get(),  mBatchProcessor.get());
}

void Application::initInputManager() {
    mInputRenderer = std::make_unique<InputManager>(mWindow, *mRenderer);
}

void Application::mainLoop() {
    while (!glfwWindowShouldClose(mWindow.getGLFWHandle())) {
        mWindow.update();
        mInputRenderer->processInput();
        // mRenderer->drawImGui();
        mImGuiSystem->newFrame();
        mImGuiSystem->render();
        mRenderer->draw();
        mBatchProcessor->update();
    }
    mRenderer->getDevice().getVkHandle().waitIdle();
    mRenderer->cleanup();
}

void Application::cleanup() {
    mImGuiSystem->cleanup();
    // glfwTerminate();
    // ncnn::destroy_gpu_instance();
}
