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

    glEnable(GL_DEPTH_TEST);
    glDepthMask(GL_TRUE);

    glEnable(GL_CULL_FACE);
    glCullFace(GL_FRONT);
    glFrontFace(GL_CCW);

	const char* version = reinterpret_cast<const char*>(glGetString(GL_VERSION));
	spdlog::info("Loaded OpenGL with version: " + std::string(version));

    std::unique_ptr<Camera> camera = std::make_unique<Camera>(Camera::WithResultOf{
        []() {
            return Camera::create();
        }
    });

    std::unique_ptr<Gui> gui = std::make_unique<Gui>(Gui::WithResultOf{
        [&window, &camera]() {
            return Gui::create(window, camera);
        }
    });

    std::unique_ptr<CubeMap> cubemap = std::make_unique<CubeMap>(CubeMap::WithResultOf{
        [&camera]() {
            return CubeMap::create({
                .right = "resources/textures/cubemap/negx.jpg",
                .left = "resources/textures/cubemap/posx.jpg",
                
                .top = "resources/textures/cubemap/posy.jpg", // ok
                .bottom = "resources/textures/cubemap/negy.jpg", // ok
                
                .front = "resources/textures/cubemap/negz.jpg",
                .back = "resources/textures/cubemap/posz.jpg",
            }, camera, "map");
        }
    });

    std::unique_ptr<FrameBuffer> frame_buffer = std::make_unique<FrameBuffer>(FrameBuffer::WithResultOf{
        [&gui]() {
            spdlog::info("Created with size: {} {}", gui->get_scene_viewport_size()->x, gui->get_scene_viewport_size()->y);
            return FrameBuffer::create(gui->get_scene_viewport_size(), 1);
         }
    });

    std::vector<std::unique_ptr<Entity>> entities;

    entities.push_back(std::make_unique<Model>(
            Model::WithResultOf([&camera](){
                return Model::create(camera, "Main Cube");
            })
        )
    );

    entities.push_back(std::make_unique<CubeMap>(
            CubeMap::WithResultOf{[&camera]() {
                return CubeMap::create({
                    .right = "resources/textures/cubemap/negx.jpg",
                    .left = "resources/textures/cubemap/posx.jpg",
                    
                    .top = "resources/textures/cubemap/posy.jpg", // ok
                    .bottom = "resources/textures/cubemap/negy.jpg", // ok
                    
                    .front = "resources/textures/cubemap/negz.jpg",
                    .back = "resources/textures/cubemap/posz.jpg",
                }, camera, "CubeMap");
            }}
        )
    );

	return Renderer(M{
        std::move(camera),
        std::move(frame_buffer),
        std::move(gui),
        std::move(entities)
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

    for(const auto& entity : m.entities) {
        entity->invoke();
    }

    bool options_opened = true;
    Gui::draw_main_dockspace(&options_opened);
    m.gui->draw_camera_controls_window();
    m.gui->render_scene(m.frame_buffer->get_texture(), m.camera);
    Ui::draw_scene_entities_controls(m.entities, m.camera);
    // m.gui->draw_models_control(m.models, m.camera);

    m.frame_buffer->revoke();
    m.gui->invoke_end();

    glfwSwapInterval(1);
}

void Renderer::destroy() const
{
	spdlog::info("Renderer destructed!");
	
    // for(const auto& model : m.entities) {
    //     model->destroy();
    // }

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