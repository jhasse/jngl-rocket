#include <jngl.hpp>

#include <Rocket/Core.h>

class RocketJNGLSystem : public Rocket::Core::SystemInterface {
	float GetElapsedTime() override {
		return jngl::getTime();
	}
};

class RocketJNGLRenderer : public Rocket::Core::RenderInterface {
public:
	void RenderGeometry(Rocket::Core::Vertex* vertices, int num_vertices, int* indices,
	                    int num_indices, Rocket::Core::TextureHandle texture,
	                    const Rocket::Core::Vector2f& translation) override {
		// TODO
	}

	void EnableScissorRegion(bool) override {
		// TODO
	}

	void SetScissorRegion(int, int, int, int) override {
		// TODO
	}
};

const int WINDOW_WIDTH = 1024;
const int WINDOW_HEIGHT = 768;

class Main : public jngl::Work {
public:
	Main() {
		Rocket::Core::SetSystemInterface(&system);
		Rocket::Core::SetRenderInterface(&renderer);
		Rocket::Core::Initialise();
		context = Rocket::Core::CreateContext("main",
		                                      Rocket::Core::Vector2i(WINDOW_WIDTH, WINDOW_HEIGHT));
		assert(context != nullptr);
	}

	~Main() {
		context->RemoveReference();
		Rocket::Core::Shutdown();
	}

	void step() {
	}
	void draw() const {
		jngl::print("Hallo Welt!", 10, 10);
	}

private:
	RocketJNGLSystem system;
	RocketJNGLRenderer renderer;
	Rocket::Core::Context* context = nullptr;
};

JNGL_MAIN_BEGIN {
	jngl::showWindow("JNGL+libRocket", WINDOW_WIDTH, WINDOW_HEIGHT, false);
	jngl::setWork(std::make_shared<Main>());
	jngl::mainLoop();
} JNGL_MAIN_END
