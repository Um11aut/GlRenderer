#include "Renderer.h"
#include <spdlog/spdlog.h>

void Renderer::framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

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

    glEnable(GL_MULTISAMPLE);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glDepthMask(GL_TRUE);

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
        [&]() {
            int w, h;
            glfwGetWindowSize(window, &w, &h);

            return FrameBuffer::create(w, h);
         }
    });


    std::vector<std::unique_ptr<Model>> models;

    models.emplace_back(std::make_unique<Model>(Model::WithResultOf([&camera](){
        return Model::create(camera);
    })));

    models.emplace_back(std::make_unique<Model>(Model::WithResultOf([&camera](){
        return Model::create(camera);
    })));

    models[0]->set_position({ 10,10,10 });
    models[1]->set_position({100,2,3});

	return Renderer(M{
        std::move(camera),
        std::move(frame_buffer),
        std::move(gui),
        std::move(models)
	});
}


template<typename ...T>
inline constexpr void Renderer::invoke(std::unique_ptr<T>&... objects)
{
    (objects->invoke(), ...);
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

    glClearColor(0.2f, 0.2f, 0.2f, 0.2f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    for(const auto& model : m.models) {
        model->invoke();
    }

    m.gui->render_scene(m.frame_buffer->get_texture(), m.camera);

    FrameBuffer::revoke();

    m.gui->invoke_end();

    glfwSwapInterval(1);
}

void Renderer::destroy() const
{
	spdlog::info("Renderer destructed!");
	
    for(const auto& model : m.models) {
        model->destroy();
        spdlog::info("Destructing model!");
    }

    m.gui->destroy();
}

Renderer::Renderer(Renderer&& other) noexcept
{
	m = std::move(other.m);
}

Renderer::~Renderer()
{
}