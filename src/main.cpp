#include "RocketJNGLRenderer.hpp"

#include <jngl.hpp>

#include <Rocket/Core.h>

class RocketJNGLSystem : public Rocket::Core::SystemInterface {
	float GetElapsedTime() override {
		return jngl::getTime();
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

		Rocket::Core::FontDatabase::LoadFontFace(
		    "subprojects/libRocket/Samples/assets/Delicious-Roman.otf");

		Rocket::Core::ElementDocument* document = context->LoadDocument(
		    "subprojects/libRocket/Samples/invaders/data/main_menu.rml");
		assert(document != nullptr);
		document->Show();
		document->RemoveReference();
		jngl::setBackgroundColor(0, 0, 0);
	}

	~Main() {
		context->RemoveReference();
		Rocket::Core::Shutdown();
	}

	void step() override {
		const auto mouse = jngl::getMousePos() +
		                   jngl::Vec2(jngl::getScreenWidth() / 2, jngl::getScreenHeight() / 2);
		context->ProcessMouseMove(mouse.x, mouse.y, 0);
		std::pair<jngl::mouse::Button, int> mapping[3] = { { jngl::mouse::Left, 0 },
			                                               { jngl::mouse::Right, 1 },
			                                               { jngl::mouse::Middle, 2 } };
		for (const auto& button : mapping) {
			if (jngl::mousePressed(button.first)) {
				context->ProcessMouseButtonDown(button.second, 0);
			} else if (!jngl::mouseDown(button.first)) {
				context->ProcessMouseButtonUp(button.second, 0);
			}
		}
		context->Update();
	}

	void draw() const override {
		jngl::translate(-jngl::getScreenWidth() / 2, - jngl::getScreenHeight() / 2);
		context->Render();
		jngl::setFontColor(255, 255, 255);
		jngl::print("Hello World from JNGL!", 100, 100);
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
