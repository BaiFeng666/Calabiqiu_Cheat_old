#include "dx11.h"
#include "engine.h"

typedef LRESULT(WINAPI* pWndProc)(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
typedef HRESULT(WINAPI* pPresent)(IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags);
typedef HRESULT(WINAPI* pResize)(IDXGISwapChain* pSwapChain, UINT BufferCount, UINT Width, UINT Height, DXGI_FORMAT NewFormat, UINT SwapChainFlags);


ID3D11Device* g_pDevice = nullptr;//游戏的D3D指针
ID3D11DeviceContext* g_pDeviceContext = nullptr;
IDXGISwapChain* g_pSwapChain = nullptr;
ID3D11RenderTargetView* g_RenderTargetView = nullptr;

pPresent g_Present = nullptr;//游戏的原函数地址
pWndProc g_WndProc = nullptr;
pResize g_Resize = nullptr;


DWORD64* SwapVTable = nullptr;//交换链虚表
HWND g_hWnd = 0;



inline AActor* aimActor = nullptr;
static double TempPreviousDistance = 100000.f;

void GetDx11Ptr(IDXGISwapChain* pSwapChain);//函数声明
//HRESULT WINAPI Initialize(IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags);

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

void WriteVtb(int index, void* address)
{
	DWORD Protect;
	VirtualProtect(SwapVTable, 1024, PAGE_EXECUTE_READWRITE, &Protect);
	SwapVTable[index] = (DWORD64)address;
	VirtualProtect(SwapVTable, 1024, Protect, &Protect);
}

LRESULT WINAPI hkWndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{

	switch (msg)
	{
	case WM_KEYUP:
		if (wParam == VK_INSERT) {

		}
		break;
	}

	if (/*Options.OpenMenu && */ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam)) return true;

	return CallWindowProcA(g_WndProc, hWnd, msg, wParam, lParam);
}

//void DrawBone(AActor* actor, Bone bone1, Bone bone2, ImColor& color) {
//	Vector2 bone_pos[2];
//	if (!WorldToScreen(GetBoneLocation(actor->Mesh, bone1), bone_pos[0]))
//		return;
//	if (!WorldToScreen(GetBoneLocation(actor->Mesh, bone2), bone_pos[1]))
//		return;
//	ImGui::GetForegroundDrawList()->AddLine(ImVec2(bone_pos[0].x, bone_pos[0].y), ImVec2(bone_pos[1].x, bone_pos[1].y), color, 1.0f);
//}

HRESULT WINAPI hkPresent(IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags)
{
	static bool init = false;

	if (!init)
	{
		init = true;
		g_WndProc = (WNDPROC)SetWindowLongPtrA(g_hWnd, GWLP_WNDPROC, (LONG_PTR)hkWndProc);//接管消息窗口

		ImGui::CreateContext();

		ImGuiIO& io = ImGui::GetIO();
		io.ConfigFlags = ImGuiConfigFlags_NoMouseCursorChange;

		io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\msyh.ttc", 17.0f, NULL, io.Fonts->GetGlyphRangesChineseFull());

		GetDx11Ptr(pSwapChain);
		ImGui_ImplDX11_Init(g_pDevice, g_pDeviceContext);
		ImGui_ImplWin32_Init(g_hWnd);
	}

	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();


	UWorld* world = GetWorld();

	TArray<APlayerState*> actors;
	UGameInstance* game_instance;
	AController* localPlayer;

	if (!world || !world->GameState) {
		goto RenderEnd;
	}
	actors = world->GameState->PlayerArray;

	game_instance = world->OwningGameInstance;
	if (!game_instance) {
		goto RenderEnd;
	}

	if (!game_instance->LocalPlayers.Data || !game_instance->LocalPlayers.Data[0]) {
		goto RenderEnd;
	}
	localPlayer = game_instance->LocalPlayers.Data[0]->PlayerController;

	for (int i = 0; i < actors.Count; i++) {

		APlayerState* playerState = actors.Data[i];

		if (!playerState || !playerState->PawnPrivate || !playerState->PawnPrivate->Mesh || !localPlayer)
			continue;

		AActor* actor = playerState->PawnPrivate;

		if (actor->PlayerState->AttributeTeamID == localPlayer->pawn->PlayerState->AttributeTeamID)
			continue;

		Vector2 head_pos;
		//printf("head: %f %f %f\n", bone.x, bone.y, bone.z);
		//if (WorldToScreen(GetBoneLocation(actor->Mesh, Bone::Head), head_pos, localPlayer))
		//{
			//printf("head_pos: %f %f\n", head_pos.x, head_pos.y);
			//bool visible = actor->IsVisible(localPlayer->pawn, Bone::Head, localPlayer->PlayerCameraManager->LastFrameCameraCachePrivate.POV.Location);
			//printf("actor:%p X:%f Y:%f, visible:%s\n", actor, head_pos.x, head_pos.y, visible ? "yes" : "no");
			//ImGui::GetForegroundDrawList()->AddCircle(ImVec2(head_pos.x, head_pos.y), 10.f, enemyColor, 10, 1.0f);
		//}



		if (GetAsyncKeyState(VK_LBUTTON) && actor->PlayerState->CyAttributeSet->Health.CurrentValue > 0 && actor->IsVisible(localPlayer->pawn, Bone::Head, localPlayer->PlayerCameraManager->LastFrameCameraCachePrivate.POV.Location)) {
			Vector2 Screen;
			WorldToScreen(GetBoneLocation(actor->Mesh, Bone::Head), Screen, localPlayer);
			auto screenSize = ImGui::GetIO().DisplaySize;
			double Distance = sqrt(pow(Screen.x - (double)screenSize.x / 2, 2) + pow(Screen.y - (double)screenSize.y / 2, 2));
			if (Distance < 100 && Distance < TempPreviousDistance)
			{
				aimActor = actor;
				TempPreviousDistance = Distance;
			}
		}

		//自瞄
		if (GetAsyncKeyState(VK_LBUTTON))
		{
			if (localPlayer && aimActor) {
				Vector3 aimRota{};
				Vector3 EnemyPos = GetBoneLocation(aimActor->Mesh, Bone::Head);
				Vector3 CameraPos = localPlayer->PlayerCameraManager->LastFrameCameraCachePrivate.POV.Location;

				Vector3 diff{ EnemyPos.x - CameraPos.x,
						  EnemyPos.y - CameraPos.y,
						  EnemyPos.z - CameraPos.z,
				};

				aimRota.y = atan2(diff.y, diff.x) * 180 / 3.1415926;
				aimRota.x = atan2(diff.z, sqrt(diff.x * diff.x + diff.y * diff.y)) * 180 / 3.1415926;

				localPlayer->ViewY = aimRota.x < 0 ? (360.f + aimRota.x) : aimRota.x;
				localPlayer->ViewX = aimRota.y;
				
				//clear
				aimActor = nullptr;
				TempPreviousDistance = 100000.f;
			}
		}

	}

RenderEnd:

	ImGui::Render();
	g_pDeviceContext->OMSetRenderTargets(1, &g_RenderTargetView, NULL);
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

	return g_Present(pSwapChain, SyncInterval, Flags);
}

HRESULT WINAPI hkResize(IDXGISwapChain* pSwapChain, UINT BufferCount, UINT Width, UINT Height, DXGI_FORMAT NewFormat, UINT SwapChainFlags)
{
	if (g_pDevice != nullptr)//加锁,来防止未调用ReImGui函数之前再次执行导致的崩溃
	{
		g_pDevice->Release();
		g_pDevice = nullptr;
		g_RenderTargetView->Release();

		ImGui_ImplDX11_Shutdown();

		//WriteVtb(8, Initialize);
	}

	return g_Resize(pSwapChain, BufferCount, Width, Height, NewFormat, SwapChainFlags);
}


void GetDx11Ptr(IDXGISwapChain* pSwapChain)
{
	g_pSwapChain = pSwapChain;//取得交换链指针
	g_pSwapChain->GetDevice(__uuidof(g_pDevice), (void**)&g_pDevice);//取得设备指针
	g_pDevice->GetImmediateContext(&g_pDeviceContext);//取得上下文指针

	ID3D11Texture2D* renderTarget = nullptr;
	g_pSwapChain->GetBuffer(0, __uuidof(renderTarget), (void**)&renderTarget);
	g_pDevice->CreateRenderTargetView(renderTarget, nullptr, &g_RenderTargetView);
	renderTarget->Release();//以上为获取必要四个指针
}

bool DX11Hook()
{
	g_hWnd = FindWindowA("UnrealWindow", NULL);

	DXGI_SWAP_CHAIN_DESC sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.BufferCount = 2;
	sd.BufferDesc.Width = 0;
	sd.BufferDesc.Height = 0;
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.BufferDesc.RefreshRate.Numerator = 60;
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.OutputWindow = g_hWnd;
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;
	sd.Windowed = TRUE;
	sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

	UINT createDeviceFlags = 0;
	D3D_FEATURE_LEVEL featureLevel;
	const D3D_FEATURE_LEVEL featureLevelArray[2] = { D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_0, };
	if (D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, createDeviceFlags, featureLevelArray, 2, D3D11_SDK_VERSION, &sd, &g_pSwapChain, NULL, &featureLevel, NULL) != S_OK)
		return false;

	SwapVTable = (*(DWORD64**)g_pSwapChain);

	DWORD Protect;
	VirtualProtect(SwapVTable, 1024, PAGE_EXECUTE_READWRITE, &Protect);

	g_Present = (pPresent)SwapVTable[8];//取得游戏Present地址
	WriteVtb(8, hkPresent);
	g_Resize = (pResize)SwapVTable[13];//取得游戏ResizeBuffers地址
	WriteVtb(13, hkResize);

	g_pSwapChain->Release();

	return true;
}

void DX11UnHook() {
	// 还原游戏Present地址
	WriteVtb(8, g_Present);

	// 还原游戏ResizeBuffers地址
	WriteVtb(13, g_Resize);

	// 还原消息窗口处理函数
	SetWindowLongPtrA(g_hWnd, GWLP_WNDPROC, (LONG_PTR)g_WndProc);

	// 清理资源
	// ... (如果有其他资源需要清理，可以在这里添加相应的操作)
}