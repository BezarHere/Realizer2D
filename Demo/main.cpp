#include <iostream>
#include <vector>
#include <string>
#include <array>
#include <list>
#include <fstream>
#define R2D_VERBOSE
#include "../include/Realizer2D.h"

using r2d::real_t;

void start_spaceships();
void start_circles();
void physics_circles(real_t delta);
void start_bath();
void start_collision_test();
void physics_collision_test(real_t delta);

enum class DemoType
{
	None,
	Spaceship,
	Circles,
	Bath,
	CollisionTest
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
	{ DemoType::None,					{ nullptr, nullptr, nullptr, nullptr } },
	{ DemoType::Spaceship,		{ start_spaceships, nullptr, nullptr, nullptr } },
	{ DemoType::Circles,			{ start_circles, nullptr, nullptr, physics_circles } },
	{ DemoType::Bath,					{ start_bath, nullptr, nullptr, nullptr } },
	{ DemoType::CollisionTest,{ start_collision_test, nullptr, nullptr, physics_collision_test } },
};

r2d::Object2D* cam;

class Bullet : public r2d::Object2D
{
public:
	Bullet(r2d::Vector2 ddir)
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

	const real_t speed{ 100.0f };
	r2d::Vector2 dir;
	real_t time{};
	const real_t lifetime{ 0.4f };
};

class Player : public r2d::Object2D
{
public:
	void shoot()
	{
		Bullet* p = new Bullet(velocity.normalized());
		p->setPosition(getGlobalPosition());
		p->addComponent(new r2d::components::CircleDrawer(4.0f));
		r2d::SceneTree::AddObject(p);
	}

	void update(real_t delta) override
	{
		cam->move(-getGlobalPosition().direction(cam->getGlobalPosition()) * delta * getGlobalPosition().distance(cam->getGlobalPosition()));
		r2d::Vector2 v;
		if (r2d::Engine::IsKeyPressed(r2d::Keyboard_t::W))
			v.y -= 1.0f;
		if (r2d::Engine::IsKeyPressed(r2d::Keyboard_t::S))
			v.y += 1.0f;
		if (r2d::Engine::IsKeyPressed(r2d::Keyboard_t::A))
			v.x -= 1.0f;
		if (r2d::Engine::IsKeyPressed(r2d::Keyboard_t::D))
			v.x += 1.0f;
		if (r2d::Engine::IsKeyJustPressed(r2d::Keyboard_t::Space))
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
	r2d::Vector2 velocity;

};

Player player{};
r2d::Object2D* player_ui;

void start_spaceships()
{
	//const std::string apps_img = "E:\\Assets\\visual studio\\Realizer2D\\apps.png";
	//player = Player();
	player.setName("player");
	r2d::Object2D* left_hand = new r2d::Object2D("left"), * right_hand = new r2d::Object2D("right");

	left_hand->setPosition({ -32.0f, 16.0f });
	right_hand->setPosition({ 32.0f, 16.0f });
	left_hand->addComponent(new r2d::components::CircleDrawer(8.0f));
	right_hand->addComponent(new r2d::components::CircleDrawer(8.0f));

	player.addChild(left_hand);
	player.addChild(right_hand);
	//player->addChild(apps_o);

	player.addComponent(new r2d::components::CircleDrawer(16.0f));
	((r2d::components::CircleDrawer*)player.getComponent(0))->setColor({ 255, 155, 55 });

	player.setZIndex(1);

	player.addToSceneTree();
	
	player_ui = new r2d::Object2D("ui");
	player_ui->addComponent(new r2d::components::GraphicalUI());
	player_ui->setUsesRelativeCoords(false);
	
	player_ui->addToSceneTree();
}

r2d::Object2D* circle;
r2d::components::CircleDrawer* drawer_cir = new r2d::components::CircleDrawer(128.0f);
void start_circles()
{
	circle = new r2d::Object2D();
	circle->addComponent(drawer_cir);
	r2d::SceneTree::AddObject(circle);
}

float cir_time;
void physics_circles(real_t delta)
{
	cir_time += delta;
}

r2d::Object2D* bath_platform;
r2d::Object2D* bath_balls;
void start_bath()
{
	bath_platform = new r2d::Object2D("platform");
	auto* phy = new r2d::components::StaticBody();
	bath_platform->addComponent(phy);
	phy->addCollidor(new r2d::RectangleCollidor({128.0f, 24.0f}));
	bath_platform->addComponent(new r2d::components::RectangleDrawer({ 128.0f, 24.0f }));
	bath_platform->setPosition(0.0f, 64.0f);
	bath_platform->addToSceneTree();
	phy->updateCache();
	PEEK(phy->getCollectiveAABB());
	for (const auto& p : phy->getCollidor(0)->getPoints())
	{
		PEEK(p);
	}
}

r2d::Object2D *collision_test_create_tringle(std::string name, bool kinetic)
{
	r2d::Object2D* traingle = new r2d::Object2D(name);
	r2d::Points_t points = {
		r2d::Vector2(r2d::Random::RandfRange(-8.0f, 8.0f), r2d::Random::RandfRange(-8.0f, 8.0f) - 16.0f),
		r2d::Vector2(r2d::Random::RandfRange(-8.0f, 8.0f) + 16.0f, r2d::Random::RandfRange(-8.0f, 8.0f) + 8.0f),
		r2d::Vector2(r2d::Random::RandfRange(-8.0f, 8.0f) - 16.0f, r2d::Random::RandfRange(-8.0f, 8.0f) + 8.0f),
	};
	auto drawer = new r2d::components::PolygonDrawer(points);
	traingle->addComponent(drawer);
	r2d::components::PhysicsBody* phy_body;
	if (kinetic)
		phy_body = (new r2d::components::KinematicBody());
	else
		phy_body = (new r2d::components::StaticBody());
	traingle->addComponent(phy_body);
	traingle->addToSceneTree();
	return traingle;
}

r2d::Object2D* traingles[2];
r2d::Object2D* camera_obj;
r2d::Points_t tri_points[32];
void start_collision_test()
{
	traingles[0] = collision_test_create_tringle("first", false);
	traingles[1] = collision_test_create_tringle("second", true);
	
	tri_points[0] = ((r2d::components::PolygonDrawer*)traingles[0]->getComponent(0))->getPoints();
	tri_points[1] = ((r2d::components::PolygonDrawer*)traingles[1]->getComponent(0))->getPoints();
	
	camera_obj = new r2d::Object2D("cam");
	r2d::components::Camera* cam = new r2d::components::Camera();
	cam->makeCurrent();
	cam->setCentered(true);
	camera_obj->addComponent(cam);
	camera_obj->addToSceneTree();
}

void physics_collision_test(real_t delta)
{
	traingles[1]->setPosition(r2d::Engine::GetMousePosition());
	/*r2d::Points_t p1 = tri_points[0];
	r2d::Points_t p2 = tri_points[1];
	for (r2d::Vector2& p : p1)
	{
		p += traingles[0]->getPosition();
	}
	for (r2d::Vector2& p : p2)
	{
		p += traingles[1]->getPosition();
	}
	PEEK(r2d::GJKTest(p1, p2));*/
}

const wchar_t* widen(const char* txt)
{
	size_t size = r2d::lengthof(txt);
	wchar_t* p = new wchar_t[size];
	for (size_t i{ 0 }; i < size; i++) p[i] = (wchar_t)txt[i];
	return p;
}

template <typename _T>
void print_ptr(const _T* const ptr, size_t size)
{
	for (size_t i{ 0 }; i < size; i++)
	{
		if (i)
			std::cout << ", ";
		std::cout << *(ptr + i);

	}
}

void create_cam()
{
	cam = new r2d::Object2D("cam");
	cam->addComponent(new r2d::components::Camera());
	cam->addToSceneTree();
	((r2d::components::Camera*)cam->getComponent(0))->makeCurrent();
	((r2d::components::Camera*)cam->getComponent(0))->setCentered(false);
}

int main(int argc, const char** argv)
{
	create_cam();
	r2d::Engine::SetOnInitAction(demos.at(demotype).init);
	r2d::Engine::SetProcessAction(demos.at(demotype).process);
	r2d::Engine::SetPhysicsProcessAction(demos.at(demotype).physics);
	r2d::VisualServer::SetDrawingAction(demos.at(demotype).drawer);
	if (r2d::Engine::Fire() != r2d::Error::Ok)
	{
		std::cerr << "FAILED TO RUN REALIZER 2D" << std::endl;
	}

	return EXIT_SUCCESS;
}

