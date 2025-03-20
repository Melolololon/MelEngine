#include "DirectX12.h"

#include<../../MelEngine/MelEngine/Code/OSAPI/Windows/DebugSupport/ErrorProcess.h>

void DirectX12::CreateDebugController()
{

	HRESULT result = S_OK;
	ComPtr<ID3D12Debug> debugController = nullptr;

	if (SUCCEEDED(result = D3D12GetDebugInterface(IID_PPV_ARGS(&debugController))))
	{
		debugController->EnableDebugLayer();
	}
	else
	{
		ME::ErrorProcess::GetInstance()->StartErroeProcess(L"デバッグレイヤーの生成に失敗しました。" + errorTxtTmp, true);
	}
}

void DirectX12::CreateFactory(ComPtr<IDXGIFactory6>& factry)
{
	HRESULT result = CreateDXGIFactory1(IID_PPV_ARGS(&factry));
	if (result != S_OK)ME::ErrorProcess::GetInstance()->StartErroeProcess(L"DXGIFactoryの生成に失敗。", true);
}

void DirectX12::CreateDevice(const ComPtr<IDXGIFactory6>& factry)
{
	std::vector<IDXGIAdapter*> adapters;
	IDXGIAdapter* tmpAdapter = nullptr;
	for (int i = 0; factry->EnumAdapters(i, &tmpAdapter) != DXGI_ERROR_NOT_FOUND; i++)
	{
		adapters.push_back(tmpAdapter);
	}

	std::vector<std::wstring> strDescs(adapters.size());
	bool useOnboard = true;
	for (int i = 0; i < (int)adapters.size(); i++)
	{
		DXGI_ADAPTER_DESC adesc{};
		adapters[i]->GetDesc(&adesc);
		strDescs[i] = adesc.Description;

		if (strDescs[i].find(L"Microsoft") == std::wstring::npos)
		{
			useOnboard = false;
		}
	}

	for (int i = 0; i < (int)adapters.size(); i++)
	{
		if (strDescs[i].find(L"Microsoft") != std::wstring::npos && useOnboard)
		{
			tmpAdapter = adapters[i];
			break;
		}
		else
		{
			tmpAdapter = adapters[i];
			break;
		}
	}

	if (!tmpAdapter)ME::ErrorProcess::GetInstance()->StartErroeProcess(L"アダプターが見つかりませんでした。" + errorTxtTmp, true);
	



	D3D_FEATURE_LEVEL levels[] =
	{
		D3D_FEATURE_LEVEL_12_1,
		D3D_FEATURE_LEVEL_12_0,
		D3D_FEATURE_LEVEL_11_1,
		D3D_FEATURE_LEVEL_11_0,
	};

	D3D_FEATURE_LEVEL featureLevel;

	HRESULT result = S_OK;

	for (int i = 0; i < _countof(levels); i++)
	{
		result = D3D12CreateDevice(tmpAdapter, levels[i], IID_PPV_ARGS(&dev));
		if (result == S_OK)
		{
			featureLevel = levels[i];
			break;
		}
	}

	if (!tmpAdapter)ME:: ErrorProcess::GetInstance()->StartErroeProcess(L"デバイスの生成に失敗しました。" + errorTxtTmp, true);

	tmpAdapter->Release();

}

void DirectX12::CreateAllcater(const ComPtr<IDXGIFactory6>& factry)
{
	HRESULT result = S_OK;
	result = dev->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&cmdAllocator));
	if (result != S_OK)ME::ErrorProcess::GetInstance()->StartErroeProcess(L"コマンドアロケーターの生成に失敗しました。" + errorTxtTmp, true);

}

void DirectX12::CreateList(const ComPtr<IDXGIFactory6>& factry)
{
	HRESULT result = S_OK;
	result = dev->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, cmdAllocator.Get(), nullptr, IID_PPV_ARGS(&cmdList));
	if (result != S_OK)ME::ErrorProcess::GetInstance()->StartErroeProcess(L"コマンドリストの生成に失敗しました。" + errorTxtTmp, true);
}

void DirectX12::CreateQueue(const ComPtr<IDXGIFactory6>& factry)
{
	HRESULT result = S_OK;
	D3D12_COMMAND_QUEUE_DESC cmdQueueDesc{};
	result = dev->CreateCommandQueue(&cmdQueueDesc, IID_PPV_ARGS(&cmdQueue));
	if (result != S_OK)ME::ErrorProcess::GetInstance()->StartErroeProcess(L"コマンドキューの生成に失敗しました。" + errorTxtTmp, true);
}

void DirectX12::CreateSwapChain(const ComPtr<IDXGIFactory6>& factry, const HWND& hwnd, const ME::Vector2& windowSize)
{
	HRESULT result = S_OK;
	ComPtr<IDXGISwapChain1>swapchain1;
	DXGI_SWAP_CHAIN_DESC1 swapchainDesc{};
	swapchainDesc.Width = windowSize.x;
	swapchainDesc.Height = windowSize.y;
	swapchainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapchainDesc.SampleDesc.Count = 1;
	swapchainDesc.BufferUsage = DXGI_USAGE_BACK_BUFFER;
	swapchainDesc.BufferCount = 2;
	swapchainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	swapchainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
	result = factry->CreateSwapChainForHwnd(
		cmdQueue.Get(),
		hwnd,
		&swapchainDesc,
		nullptr,
		nullptr,
		&swapchain1
	);
	swapchain1.As(&swapchain);

	if (result != S_OK)ME::ErrorProcess::GetInstance()->StartErroeProcess(L"スワップチェーンの生成に失敗しました。"+errorTxtTmp, true);
}

void DirectX12::CreateDescHeap()
{
	HRESULT result = S_OK;
	
	dev->CreateDescriptorHeap(&heapDesc, IID_PPV_ARGS(&rtvHeaps));

	if (result != S_OK)ME::ErrorProcess::GetInstance()->StartErroeProcess(L"ディスクリプタヒープの生成に失敗しました。" + errorTxtTmp, true);

}

void DirectX12::CreateBackBuffer()
{
	HRESULT result = S_OK;
	for (int i = 0; i < heapDesc.NumDescriptors; i++)
	{
		result = swapchain->GetBuffer(i, IID_PPV_ARGS(&backBuffer[i]));
		D3D12_CPU_DESCRIPTOR_HANDLE handle = rtvHeaps->GetCPUDescriptorHandleForHeapStart();
		handle.ptr += i * dev->GetDescriptorHandleIncrementSize(heapDesc.Type);
		dev->CreateRenderTargetView
		(
			backBuffer[i].Get(),
			nullptr,
			handle
		);

		if (result != S_OK)ME::ErrorProcess::GetInstance()->StartErroeProcess(L"バックバッファの生成に失敗しました。" + errorTxtTmp, true);
	}
}

void DirectX12::CreateDepthBuffer(D3D12_DESCRIPTOR_HEAP_DESC dsvHeapDesc)
{
	/*HRESULT result = S_OK;
	result = dev->CreateDescriptorHeap
	(
		&dsvHeapDesc,
		IID_PPV_ARGS(&depthHeap)
	);
	if (result != S_OK)ErrorProcess::GetInstance()->StartErroeProcess(L"深度バッファの生成に失敗。", true);

	D3D12_CPU_DESCRIPTOR_HANDLE depthHeapHandle = depthHeap->GetCPUDescriptorHandleForHeapStart();


	D3D12_RESOURCE_DESC depthResDesc = CD3DX12_RESOURCE_DESC::Tex2D
	(
		DXGI_FORMAT_D32_FLOAT,
		winWidth,
		winHeight,
		1, 0, 1, 0,
		D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL
	);

	CreateBuffer::GetInstance()->CreateDepthBufferSet(CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT), depthResDesc, CD3DX12_CLEAR_VALUE(DXGI_FORMAT_D32_FLOAT, 1.0f, 0), depthHeapHandle, depthBufferSet);*/

}

void DirectX12::CreateFence()
{
	HRESULT result = S_OK;
	fence = nullptr;
	result = dev->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&fence));
	if (result != S_OK)ME::ErrorProcess::GetInstance()->StartErroeProcess(L"フェンスの生成に失敗しました。" + errorTxtTmp, true);
}

void DirectX12::Initialize(const HWND& hwnd, const ME::Vector2& windowSize, unsigned short buffer)
{
#ifdef _DEBUG
	CreateDebugController();
#endif // _DEBUG

	ComPtr<IDXGIFactory6> factory = nullptr;
	CreateFactory(factory);
	CreateDevice(factory);
	CreateAllcater(factory);
	CreateList(factory);
	CreateQueue(factory);
	CreateSwapChain(factory, hwnd, windowSize);

	heapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
	heapDesc.NumDescriptors = buffer;
	CreateDescHeap();

	//深度用ヒープ作成
	D3D12_DESCRIPTOR_HEAP_DESC dsvHeapDesc{};
	dsvHeapDesc.NumDescriptors = 1;
	dsvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
	CreateDepthBuffer(dsvHeapDesc);


}

void DirectX12::StartUpdate()
{
	UINT bbIndex = swapchain->GetCurrentBackBufferIndex();
	//RenderTarget::DrawBegin();
}

void DirectX12::EndUpdate()
{
	//RenderTarget::AllDraw();
	
	//板ポリをバックバッファーに描画する準備
	D3D12_RESOURCE_BARRIER barrierDesc = {};
	UINT bbIndex = swapchain->GetCurrentBackBufferIndex();
	barrierDesc.Transition.pResource = backBuffer[bbIndex].Get();
	barrierDesc.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT;
	barrierDesc.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;
	cmdList->ResourceBarrier(1, &barrierDesc);

	D3D12_CPU_DESCRIPTOR_HANDLE rtvH = rtvHeaps->GetCPUDescriptorHandleForHeapStart();
	rtvH.ptr += bbIndex * dev->GetDescriptorHandleIncrementSize(heapDesc.Type);
	//D3D12_CPU_DESCRIPTOR_HANDLE dsvH = depthHeap.Get()->GetCPUDescriptorHandleForHeapStart();
	cmdList->OMSetRenderTargets(1, &rtvH, false, /*&dsvH*/nullptr);

	//画面のクリア
	float clearColors[4] = { rtColor.r,rtColor.g,rtColor.b,rtColor.a };
	cmdList->ClearRenderTargetView(rtvH, clearColors, 0, nullptr);
	//cmdList->ClearDepthStencilView(dsvH, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);

	//RenderTarget::MainRTDraw();

	//ImguiManager::GetInstance()->Draw();
#pragma region RTVからPRESENTへ
	//TextWriteクラスで自動的に変更するからいらない

	/*barrierDesc.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;
	barrierDesc.Transition.StateAfter = D3D12_RESOURCE_STATE_PRESENT;
	cmdList->ResourceBarrier(1, &barrierDesc);*/

	//barrierDesc.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;
	//barrierDesc.Transition.StateAfter = D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE;
	//cmdList->ResourceBarrier(1, &barrierDesc);
#pragma endregion



#pragma region 実行


	cmdList->Close();
	ID3D12CommandList* cmdLists[] = { cmdList.Get() };
	cmdQueue->ExecuteCommandLists(_countof(cmdLists), cmdLists);

	cmdQueue->Signal(fence.Get(), ++fenceVal);
	if (fence->GetCompletedValue() != fenceVal)
	{
		HANDLE event = CreateEvent(nullptr, false, false, nullptr);
		fence->SetEventOnCompletion(fenceVal, event);
		WaitForSingleObject(event, INFINITE);
		CloseHandle(event);
	}


	cmdAllocator->Reset();
	cmdList->Reset(cmdAllocator.Get(), nullptr);

	//(VSYNC,定数)
	swapchain->Present(1, 0);
#pragma endregion
	//DrawManager::GetInstance()->Reset();
}
