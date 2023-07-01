#include <iostream>
#include "../include/Realizer2D.h"
#include <vector>
#include <string>
#include <array>
#include <list>

constexpr size_t obj_count = 20;
uint32_t counter = 0;
r2d::Object2D* objects[obj_count];

void process()
{
	counter++;
	sf::Vector2f velocity;
	r2d::Engine* engine = r2d::Engine::Singleton();
	if (engine->isKeyPressed(r2d::Keyboard_t::W))
		velocity.y -= 1.0f;
	if (engine->isKeyPressed(r2d::Keyboard_t::S))
		velocity.y += 1.0f;
	if (engine->isKeyPressed(r2d::Keyboard_t::A))
		velocity.x -= 1.0f;
	if (engine->isKeyPressed(r2d::Keyboard_t::D))
		velocity.x += 1.0f;
	if (velocity.x != 0.0f || velocity.y != 0.0f)
	{
		r2d::VisualServer::Singleton()->moveView(velocity);
	}
}

void start()
{
	//r2d::VisualServer::Singleton()->setViewCentered(false);
		auto t = std::chrono::steady_clock::now();
	for (int i = 0; i < obj_count; i++) {
		objects[i] = new r2d::Object2D();
		objects[i]->installComponent(new r2d::components::CircleDrawer(r2d::random::randf_range(16.0f, 32.0f), 80));
		objects[i]->setPosition(r2d::random::randf_range(0.0f, 500.0f), r2d::random::randf_range(0.0f, 500.0f));
	}
		
}

int main(int argc, const char** argv)
{

	r2d::ObjectComponent2D* p = new r2d::components::RectangleDrawer();

	r2d::Engine::SetOnInitAction(start);
	r2d::Engine::SetProcessAction(process);
	r2d::Engine::Fire();
	return EXIT_SUCCESS;
}

