#include "MelEngine.h"

#include"Window/Windows/WindowsWindowCreater.h"


bool ME::MelEngine::Initialize(const ME::Vector2& windowSize,GraphicAPI api, OS os)
{
	bool result = false;

	this->api = api;
	this->os = os;

	// ウィンドウ作成
	switch (os)
	{
	case OS::Windows:
		WindowsWindowCreater wwc;
		result = wwc.Create();
		break;
	}

	// API初期化
	switch (api)
	{
	case GraphicAPI::DirectX12:
		dx12 = std::make_unique<DirectX12>();
		dx12->Initialize(hwnd, windowSize);
		break;

	case GraphicAPI::Vulkan:
		break;

	default:
		break;
	}

    return true;
}
