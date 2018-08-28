#include <jngl.hpp>

#include <Rocket/Core.h>

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

class Main : public jngl::Work {
public:
	void step() {
	}
	void draw() const {
		jngl::print("Hallo Welt!", 10, 10);
	}
};

int main() {
	Rocket::Core::SetRenderInterface(new RocketJNGLRenderer);
	jngl::showWindow("JNGL+libRocket", 1920, 1080, false);
	jngl::setWork(std::make_shared<Main>());
	jngl::mainLoop();
}
