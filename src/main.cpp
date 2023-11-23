#include "RocketJNGLRenderer.hpp"

#include <cassert>
#include <jngl.hpp>
#include <jngl/init.hpp>

#include <RmlUi/Core.h>

class RocketJNGLSystem : public Rml::SystemInterface {
	double GetElapsedTime() override {
		return jngl::getTime();
	}
};

const int WINDOW_WIDTH = 1024;
const int WINDOW_HEIGHT = 768;

class Main : public jngl::Work, public Rml::EventListener {
public:
	Main() {
		Rml::SetSystemInterface(&system);
		Rml::SetRenderInterface(&renderer);
		Rml::Initialise();
		context =
		    Rml::CreateContext("main", Rml::Vector2i(WINDOW_WIDTH, WINDOW_HEIGHT));
		assert(context != nullptr);
		context->AddEventListener("click", this);

		const std::string fontFile = "subprojects/RmlUi/Samples/assets/LatoLatin-Regular.ttf";
		if (!Rml::LoadFontFace(fontFile)) {
			throw std::runtime_error("Failed to load font: " + fontFile);
		}

		Rml::ElementDocument* document =
		    context->LoadDocument("subprojects/RmlUi/Samples/invaders/data/main_menu.rml");
		assert(document != nullptr);
		document->Show();
		jngl::setBackgroundColor(0x000000_rgb);
	}

	~Main() {
		Rml::Shutdown();
	}

	void ProcessEvent(Rml::Event& event) override {
		const auto target = event.GetTargetElement();
		if (!target) {
			return;
		}
		const auto onclick = target->GetAttribute<std::string>("onclick", "");
		if (onclick == "exit") {
			jngl::quit();
		}
	}

	void step() override {
		const auto mouse = jngl::getMousePos() +
		                   jngl::Vec2(jngl::getScreenWidth() / 2, jngl::getScreenHeight() / 2);
		context->ProcessMouseMove(mouse.x, mouse.y, 0);
		for (auto& button : mapping) {
			if (jngl::mousePressed(button.jngl)) {
				context->ProcessMouseButtonDown(button.rocket, 0);
				button.down = true;
			} else if (!jngl::mouseDown(button.jngl) and button.down) {
				context->ProcessMouseButtonUp(button.rocket, 0);
				button.down = false;
			}
		}
		for (auto it = keysDown.begin(); it != keysDown.end();) {
			if (!jngl::keyDown(*it)) {
				context->ProcessKeyUp(convertKey(*it), getKeyModifierState());
				it = keysDown.erase(it);
			} else {
				++it;
			}
		}
		for (jngl::key::KeyType k = static_cast<jngl::key::KeyType>(0); k < jngl::key::Any;
		     k = static_cast<jngl::key::KeyType>(k + 1)) {
			if (jngl::keyPressed(k)) {
				context->ProcessKeyDown(convertKey(k), getKeyModifierState());
				keysDown.push_back(k);
			}
		}
		context->Update();
	}

	void draw() const override {
		jngl::translate(-jngl::getScreenWidth() / 2, -jngl::getScreenHeight() / 2);
		context->Render();
		jngl::setFontColor(255, 255, 255);
		jngl::print("Hello World from JNGL!", 100, 100);
	}

private:
	std::vector<jngl::key::KeyType> keysDown;
	RocketJNGLSystem system;
	RocketJNGLRenderer renderer;
	Rml::Context* context = nullptr;

	static Rml::Input::KeyIdentifier convertKey(jngl::key::KeyType key) {
		switch (key) {
			case jngl::key::Left:
				return Rml::Input::KI_LEFT;
			case jngl::key::Right:
				return Rml::Input::KI_RIGHT;
			case jngl::key::Up:
				return Rml::Input::KI_UP;
			case jngl::key::Down:
				return Rml::Input::KI_DOWN;
			case jngl::key::BackSpace:
				return Rml::Input::KI_BACK;
			case jngl::key::Delete:
				return Rml::Input::KI_DELETE;
			case jngl::key::End:
				return Rml::Input::KI_END;
			case jngl::key::Escape:
				return Rml::Input::KI_ESCAPE;
			case jngl::key::F1:
				return Rml::Input::KI_F1;
			case jngl::key::F2:
				return Rml::Input::KI_F2;
			case jngl::key::F3:
				return Rml::Input::KI_F3;
			case jngl::key::F4:
				return Rml::Input::KI_F4;
			case jngl::key::F5:
				return Rml::Input::KI_F5;
			case jngl::key::F6:
				return Rml::Input::KI_F6;
			case jngl::key::F7:
				return Rml::Input::KI_F7;
			case jngl::key::F8:
				return Rml::Input::KI_F8;
			case jngl::key::F9:
				return Rml::Input::KI_F9;
			case jngl::key::F10:
				return Rml::Input::KI_F10;
			case jngl::key::F11:
				return Rml::Input::KI_F11;
			case jngl::key::F12:
				return Rml::Input::KI_F12;
			case jngl::key::Home:
				return Rml::Input::KI_HOME;
			case jngl::key::ControlL:
				return Rml::Input::KI_LCONTROL;
			case jngl::key::ShiftL:
				return Rml::Input::KI_LSHIFT;
			case jngl::key::Pause:
				return Rml::Input::KI_PAUSE;
			case jngl::key::ControlR:
				return Rml::Input::KI_RCONTROL;
			case jngl::key::Return:
				return Rml::Input::KI_RETURN;
			case jngl::key::ShiftR:
				return Rml::Input::KI_RSHIFT;
			case jngl::key::Space:
				return Rml::Input::KI_SPACE;
			case jngl::key::Tab:
				return Rml::Input::KI_TAB;
			case jngl::key::Clear:
				return Rml::Input::KI_CLEAR;
			case jngl::key::CapsLock:
				return Rml::Input::KI_CAPITAL;
			case jngl::key::SuperL:
				return Rml::Input::KI_LWIN;
			case jngl::key::SuperR:
				return Rml::Input::KI_RWIN;
			case jngl::key::PageUp:
				return Rml::Input::KI_PRIOR;
			case jngl::key::PageDown:
				return Rml::Input::KI_NEXT;
			case jngl::key::AltL:
			case jngl::key::AltR:
			case jngl::key::Any:
				return Rml::Input::KI_UNKNOWN;
		}
		return Rml::Input::KI_UNKNOWN;
	}
	static int getKeyModifierState() {
		int modifiers = 0;
		if (jngl::keyDown(jngl::key::ShiftL) || jngl::keyDown(jngl::key::ShiftR)) {
			modifiers |= Rml::Input::KM_SHIFT;
		}
		if (jngl::keyDown(jngl::key::ControlL) || jngl::keyDown(jngl::key::ControlR)) {
			modifiers |= Rml::Input::KM_CTRL;
		}
		if (jngl::keyDown(jngl::key::AltL) || jngl::keyDown(jngl::key::AltR)) {
			modifiers |= Rml::Input::KM_ALT;
		}
		return modifiers;
	}

	struct MouseButton {
		jngl::mouse::Button jngl;
		int rocket;
		bool down = false;
	};
	MouseButton mapping[3] = { { jngl::mouse::Left, 0 },
		                       { jngl::mouse::Right, 1 },
		                       { jngl::mouse::Middle, 2 } };
};

jngl::AppParameters jnglInit() {
	jngl::AppParameters params;
	params.displayName = "JNGL+RmlUi";
	params.screenSize = { WINDOW_WIDTH, WINDOW_HEIGHT };
	params.start = []() {
		return std::make_shared<Main>();
	};
	return params;
}
