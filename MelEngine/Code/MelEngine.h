#pragma once

#include<Windows.h>

#include<memory>


#include"OS.h"
#include"GraphicAPI.h"

#include"GraphicAPI/DirectX12/DirectX12.h"


#include"Support/Math/Vector.h"

namespace ME 
{
	class MelEngine
	{
	private:

		OS os;
		HWND hwnd;
		
		GraphicAPI api;
		std::unique_ptr<DirectX12> dx12;


	private:

	public:
		bool Initialize(const ME::Vector2& windowSize, GraphicAPI api = GraphicAPI::DirectX12, OS os = OS::Windows);
		bool StartUpdate();
		bool EndUpdate();
		bool Draw();
	};
}
