#include <iostream>
#include "../include/Realizer2D.h"
#include <vector>
#include <string>
#include <array>
#include <list>
#include <fstream>

using r2d::real_t;

void start_spaceships();
void start_circles();
void physics_circles(real_t delta);

enum class DemoType
{
	None,
	Spaceship,
	Circles
};

constexpr DemoType demotype = DemoType::Spaceship;

struct DemoData
{
	r2d::Action_t init;
	r2d::ProcessFunction_t process;
	r2d::DrawerFunction_t drawer;
	r2d::PhysicsFunction_t physics;
};

const std::unordered_map<DemoType, DemoData> demos
{
	{ DemoType::None,      { nullptr, nullptr, nullptr, nullptr } },
	{ DemoType::Spaceship, { start_spaceships, nullptr, nullptr, nullptr } },
	{ DemoType::Circles,	 { start_circles, nullptr, nullptr, physics_circles } },
};


class Bullet : public r2d::Object2D
{
public:
	Bullet(r2d::Vector2f_t ddir)
		: dir(ddir)
	{

	}

	void update(real_t delta) override
	{
		move(dir * speed * delta);
		time += delta;
		if (time >= lifetime)
		{
			kill();
		}
	}

	const real_t speed{ 100.0 };
	r2d::Vector2f_t dir;
	real_t time{};
	const real_t lifetime{ 0.4 };
};

class Player : public r2d::Object2D
{
public:
	void shoot()
	{
		Bullet* p = new Bullet(velocity.normalized());
		p->setPosition(getGlobalPosition());
		p->installComponent(new r2d::components::CircleDrawer(4.0f));
		r2d::SceneTree::AddObject(p);
	}

	void update(real_t delta) override
	{
		r2d::Engine* engine = r2d::Engine::Singleton();
		r2d::Vector2f_t v;
		if (engine->isKeyPressed(r2d::Keyboard_t::W))
			v.y -= 1.0f;
		if (engine->isKeyPressed(r2d::Keyboard_t::S))
			v.y += 1.0f;
		if (engine->isKeyPressed(r2d::Keyboard_t::A))
			v.x -= 1.0f;
		if (engine->isKeyPressed(r2d::Keyboard_t::D))
			v.x += 1.0f;
		if (engine->isKeyJustPressed(r2d::Keyboard_t::Space))
		{
			shoot();
		}
		if (v.x != 0.0f || v.y != 0.0f)
		{
			velocity += v;
		}
			velocity *= real_t(0.94f);
		move(velocity);
		setRotation(rad2deg(velocity.angle()) + 90.0f);
	}

private:
	r2d::Vector2f_t velocity;

};


Player* player;

void start_spaceships()
{
	const std::string apps_img = "E:\\Assets\\visual studio\\Realizer2D\\apps.png";
	player = new Player();
	player->setName("player");
	r2d::Object2D *left_hand = new r2d::Object2D("left"), * right_hand = new r2d::Object2D("right");
	r2d::Object2D* apps_o = new r2d::Object2D("apps");

	left_hand->setPosition({ -32.0f, 16.0f });
	right_hand->setPosition({ 32.0f, 16.0f });
	left_hand->installComponent(new r2d::components::CircleDrawer(8.0f));
	right_hand->installComponent(new r2d::components::CircleDrawer(8.0f));

	sf::Texture* p = new sf::Texture();
	p->loadFromFile(apps_img);
	apps_o->installComponent(new r2d::components::SpriteDrawer(p));

	player->addChild(left_hand);
	player->addChild(right_hand);
	//player->addChild(apps_o);

	player->installComponent(new r2d::components::CircleDrawer(16.0f, 24));

	player->addToSceneTree();
	apps_o->addToSceneTree();
}

r2d::Object2D* circle;
r2d::components::CircleDrawer* drawer_cir = new r2d::components::CircleDrawer(128.0f, 8);
void start_circles()
{
	circle = new r2d::Object2D();
	circle->installComponent(drawer_cir);
	r2d::SceneTree::AddObject(circle);
}

float cir_time;
void physics_circles(real_t delta)
{
	cir_time += delta;
	drawer_cir->setSegmentsCount(uint16_t(abs(sin(cir_time / 2.5f) * 32.0f)) + 16U);
}

const wchar_t* widen(const char* txt)
{
	size_t size = lengthof(txt);
	wchar_t* p = new wchar_t[size];
	for (size_t i{ 0 }; i < size; i++) p[i] = (wchar_t)txt[i];
	return p;
}

int main(int argc, const char** argv)
{
	
	r2d::Engine::SetOnInitAction(demos.at(demotype).init);
	r2d::Engine::SetProcessAction(demos.at(demotype).process);
	r2d::Engine::SetPhysicsProcessAction(demos.at(demotype).physics);
	r2d::Engine::SetDrawingAction(demos.at(demotype).drawer);
	r2d::Engine::Fire();

	return EXIT_SUCCESS;
}

