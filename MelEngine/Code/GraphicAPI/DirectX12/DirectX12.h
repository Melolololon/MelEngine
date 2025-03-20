#pragma once
#include<string>

#include<Windows.h>

#include<d3d12.h>
#include<dxgi1_6.h>
#include<d3dx12.h>
#include<DirectXMath.h>
#include<d3dcompiler.h>
#include<dxgidebug.h>

#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"dxgi.lib")
#pragma comment(lib,"d3dcompiler.lib")
#pragma comment(lib,"dxguid.lib")

//d3d12�̃T���v���ꗗ
//https://docs.microsoft.com/ja-jp/windows/win32/direct3d12/working-samples


#include"../../MelEngine/MelEngine/Code/Support/Math/Vector.h"
#include"../../MelEngine/MelEngine/Code/Struct/Color.h"

using namespace Microsoft::WRL;
class DirectX12
{
private:
	const std::wstring errorTxtTmp = L"�J���҂֕񍐂��Ă��������B�A�����X�A�J�E���g@Melolololon2000�ł��B";

private:
	// init�֌W
	// �R�}���h�L���[
	ComPtr<ID3D12CommandQueue> cmdQueue = nullptr;
	// �f�o�C�X
	ComPtr<ID3D12Device> dev = nullptr;
	// �X���b�v�`�F�[��
	ComPtr<IDXGISwapChain4> swapchain = nullptr;
	// �A���P�[�^�[
	ComPtr<ID3D12CommandAllocator> cmdAllocator = nullptr;
	// �R�}���h���X�g
	ComPtr<ID3D12GraphicsCommandList> cmdList = nullptr;

	ComPtr<ID3D12DescriptorHeap> rtvHeaps = nullptr;

	ComPtr<ID3D12Fence> fence = nullptr;
	UINT64 fenceVal = 0;
private:
	// �����_�[�^�[�Q�b�g
	//���C��
	ComPtr<ID3D12Resource> backBuffer[2] = { nullptr ,nullptr };
	D3D12_DESCRIPTOR_HEAP_DESC heapDesc;

private:
	// �ݒ�
	ME::Color rtColor = ME::Color(1, 1, 1, 1);

private:
#ifdef _DEBUG
	void CreateDebugController();
#endif // DEBUG

	void CreateFactory(ComPtr<IDXGIFactory6>& factry);
	void CreateDevice(const ComPtr<IDXGIFactory6>& factry);
	void CreateAllcater(const ComPtr<IDXGIFactory6>& factry);
	void CreateList(const ComPtr<IDXGIFactory6>& factry);
	void CreateQueue(const ComPtr<IDXGIFactory6>& factry);
	void CreateSwapChain(const ComPtr<IDXGIFactory6>& factry, const HWND& hwnd, const ME::Vector2& windowSize);
	void CreateDescHeap();
	void CreateBackBuffer(); 
	void CreateDepthBuffer(D3D12_DESCRIPTOR_HEAP_DESC dsvHeapDesc);
	void CreateFence();


public:
	void Initialize(const HWND& hwnd ,const ME::Vector2& windowSize,unsigned short buffer);
	void StartUpdate();
	void EndUpdate();
	
};

