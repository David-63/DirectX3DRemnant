#pragma once

#include <vector>
#include <list>
#include <map>
#include <string>

using std::vector;
using std::list;
using std::map;
using std::make_pair;
using std::wstring;
using std::string;

#include <typeinfo>
#include <assert.h>
#include <iostream>

// FileSystem
#define _SILENCE_EXPERIMENTAL_FILESYSTEM_DEPRECATION_WARNING
#include <experimental/filesystem>
using namespace std::experimental;
using namespace std::experimental::filesystem;



#include <wrl.h>
using namespace Microsoft::WRL;

#include <d3d11.h>					// DirectX11
#include <d3dcompiler.h>			// Shader ÄÄÆÄÀÏ
#include <DirectXMath.h>			// DX Math
#include <DirectXPackedVector.h>	

using namespace DirectX;
using namespace DirectX::PackedVector;

#pragma comment(lib, "d3d11")
#pragma comment(lib, "d3dcompiler")


#include "SimpleMath.h"
using namespace DirectX::SimpleMath;

typedef Vector2 Vec2;
typedef Vector3 Vec3;
typedef Vector4 Vec4;

// Fbx Loader
#include <FBXLoader/fbxsdk.h>
#include <PhysX/PxPhysicsAPI.h>

#ifdef _DEBUG
#pragma comment(lib, "FBXLoader/x64/debug/libfbxsdk-md.lib")

#pragma comment(lib, "PhysX/Debug/PhysXExtensions_static_64.lib")
#pragma comment(lib, "PhysX/Debug/PhysXPvdSDK_static_64.lib")
#pragma comment(lib, "PhysX/Debug/PhysXVehicle_static_64.lib")
#pragma comment(lib, "PhysX/Debug/PhysXVehicle2_static_64.lib")
#pragma comment(lib, "PhysX/Debug/PhysXCharacterKinematic_static_64.lib")
#pragma comment(lib, "PhysX/Debug/PhysXCooking_64.lib")
#pragma comment(lib, "PhysX/Debug/PVDRuntime_64.lib")
#pragma comment(lib, "PhysX/Debug/SceneQuery_static_64.lib")
#pragma comment(lib, "PhysX/Debug/PhysX_64.lib")
#pragma comment(lib, "PhysX/Debug/PhysXCommon_64.lib")
#pragma comment(lib, "PhysX/Debug/PhysXFoundation_64.lib")

#else
#pragma comment(lib, "FBXLoader/x64/release/libfbxsdk-md.lib")

#pragma comment(lib, "PhysX/Release/PhysXExtensions_static_64.lib")
#pragma comment(lib, "PhysX/Release/PhysXPvdSDK_static_64.lib")
#pragma comment(lib, "PhysX/Release/PhysXVehicle_static_64.lib")
#pragma comment(lib, "PhysX/Release/PhysXVehicle2_static_64.lib")
#pragma comment(lib, "PhysX/Release/PhysXCharacterKinematic_static_64.lib")
#pragma comment(lib, "PhysX/Release/PhysXCooking_64.lib")
#pragma comment(lib, "PhysX/Release/PVDRuntime_64.lib")
#pragma comment(lib, "PhysX/Release/SceneQuery_static_64.lib")
#pragma comment(lib, "PhysX/Release/PhysX_64.lib")
#pragma comment(lib, "PhysX/Release/PhysXCommon_64.lib")
#pragma comment(lib, "PhysX/Release/PhysXFoundation_64.lib")

#endif

#include "define.h"
#include "struct.h"
#include "func.h"

#include "CSingleton.h"



static void AssertEx(bool _expression, const std::wstring& _message)
{
	if (_expression)
		return;

	MessageBoxW(NULL, _message.c_str(), L"Assert", MB_OK);
	assert(_expression);
}

