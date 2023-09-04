#pragma once
#include "scene/Object.h"
#include "components/Graphical.h"
#include "servers/VisualServer.h"
#include "servers/PhysicsServer.h"
#include "servers/Input.h"
#include "utils/Random.h"
#include "scene/SceneTree.h"
#include "R2DInit.h"



#ifndef R2D_TITLE
// funy
#define R2D_TITLE "Dude, define 'R2D_TITLE' before the r2d::Engine for a window title!"
#endif // !R2D_TITLE

#ifndef R2D_CLEAR_CLR
#define R2D_CLEAR_CLR sf::Color(12U, 12U, 12U, 0U)
#endif // !R2D_CLEAR_CLR

_R2D_NAMESPACE_START_


class _R2D_Init;

class Engine final
{
	friend class R2DInit;
public:

public:
	friend class VisualServer;
	friend class PhysicsServer;
	friend class Object2D;

	// call to start the Realizer 2D's processes
	[[nodiscard]] static ErrorCode Fire();

	static void SetProcessAction(ProcessFunction_t action);
	static void SetPhysicsProcessAction(PhysicsFunction_t action);
	static void SetOnInitAction(Action_t action);

	static inline ProcessFunction_t GetProcessAction() { return s_processAction; }
	static inline PhysicsFunction_t GetPhysicsProcessAction() { return s_physicsAction; }
	static inline Action_t GetOnInitAction() { return s_onInitAction; }

	static void RegisterObject(Object2D* object);
	static void PopObject(Object2D* object);

	static uint32_t GetCurrentFrame();

private:
	Engine() = delete;
	Engine(const Engine& other) = delete;
	//inline Engine* operator new() = delete;
	static ErrorCode Init();

	static inline void Main();
	static void Final();

	static inline void Update();
	static inline void physics();
	static inline void draw();

	static inline void pollEvents();

private:
	// TODO: remove this
	static inline ObjID_t s_objCounter{ 0U };
	static inline std::unordered_map<ObjID_t, Object2D*> s_objsRegistery{};

	static inline ProcessFunction_t s_processAction{ nullptr };
	static inline PhysicsFunction_t s_physicsAction{ nullptr };
	static inline Action_t s_onInitAction{ nullptr };

	static inline uint32_t s_currentFrame{ 0U };

	static inline bool s_mainLoopLocked{ false };

	static inline std::chrono::steady_clock::time_point s_lastFrameTime{ std::chrono::high_resolution_clock::now() };
	static inline real_t s_physicsDeltaTime{ 1.0f / 60.0f };

	static inline bool s_created{ false };
};

_R2D_NAMESPACE_END_