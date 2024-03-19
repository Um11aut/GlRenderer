#include "Renderer.h"
#include <spdlog/spdlog.h>


Renderer Renderer::create(GLFWwindow* window)
{
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		spdlog::error("Failed to initialize GLAD");
		glfwTerminate();
	}

	const char* version = reinterpret_cast<const char*>(glGetString(GL_VERSION));
	spdlog::info("Loaded OpenGL with version: " + std::string(version));

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    std::unique_ptr<Camera> camera = std::make_unique<Camera>(Camera::WithResultOf{
        []() {
            return Camera::create();
        }
    });

    std::unique_ptr<Gui> gui = std::make_unique<Gui>(Gui::WithResultOf{
        [&]() {
            return Gui::create(window, camera);
         }
    });

    std::unique_ptr<FrameBuffer> frame_buffer = std::make_unique<FrameBuffer>(FrameBuffer::WithResultOf{
        [&gui]() {
            spdlog::info("Created with size: {} {}", gui->get_scene_viewport_size()->x, gui->get_scene_viewport_size()->y);
            return FrameBuffer::create(gui->get_scene_viewport_size());
         }
    });

    std::vector<std::unique_ptr<Model>> models;

    models.emplace_back(std::make_unique<Model>(Model::WithResultOf([&camera](){
        return Model::create(camera);
    })));

    models.emplace_back(std::make_unique<Model>(Model::WithResultOf([&camera](){
        return Model::create(camera);
    })));

    models.emplace_back(std::make_unique<Model>(Model::WithResultOf([&camera](){
        return Model::create(camera);
    })));

    models.emplace_back(std::make_unique<Model>(Model::WithResultOf([&camera](){
        return Model::create(camera);
    })));

    models[0]->set_position({ 4, 0, 4 });
    models[1]->set_position({ 1, 2, 3 });
    models[2]->set_position({ 1, 6, 3 });
    models[3]->set_position({ 1, 15, 9 });

	return Renderer(M{
        std::move(camera),
        std::move(frame_buffer),
        std::move(gui),
        std::move(models)
	});
}

Renderer::Renderer(WithResultOf&& res)
{
	Renderer other = std::move(res.func());

	m = std::move(other.m);
}

void Renderer::draw()
{
    m.gui->invoke_start();

    m.frame_buffer->invoke();

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    for(const auto& model : m.models) {
        model->invoke();
    }

    m.gui->render_scene(m.frame_buffer->get_texture(), m.camera);

    m.frame_buffer->revoke();
    m.gui->invoke_end();

    glfwSwapInterval(1);
}

void Renderer::destroy() const
{
	spdlog::info("Renderer destructed!");
	
    for(const auto& model : m.models) {
        model->destroy();
    }

    m.gui->destroy();
    m.frame_buffer->destroy();
}

Renderer::Renderer(Renderer&& other) noexcept
{
	m = std::move(other.m);
}

Renderer::~Renderer()
{
}