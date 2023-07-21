#include "global.h"
#include "R2DInit.h"
#include "Engine.h"
#include "servers/ResourcesServer.h"
#include "servers/PhysicsServer.h"
#include "servers/VisualServer.h"

_R2D_NAMESPACE_START_
#define DoInit(class_name) Error class_name## _error = class_name::Init(); \
if (class_name## _error != Error::Ok) { throw std::runtime_error(("Failed to initialize " #class_name \
" with error: ") + GetErrorName(class_name## _error)); } \
else { std::cout << "initializing " << #class_name << ':' << (std::string(#class_name).size() < 8 ? "\t\t" : "\t") << "completed!" << std::endl; }

class R2DInit
{
public:
	R2DInit()
	{
		if (called)
			throw std::runtime_error("can't manualy create  init R2D processes.");
		called = true;
		_r2d_print("Initializing R2D processes.");
		DoInit(Engine);
		DoInit(ResourcesServer);
		DoInit(VisualServer);
		DoInit(PhysicsServer);
	}
private:
	static inline bool called{ false };
} __init__;

_R2D_NAMESPACE_END_