
#include "UI/DebugUI.h"

#include "Slate/CanvasPanel.h"
#include "Slate/VerticalBox.h"
#include "Slate/HorizontalBox.h"
#include "Slate/TextBlock.h"

#include "EngineSettings.h"

#include <Psapi.h>
#include <format>
#include <Pdh.h>
#include <tchar.h>
#include <Windows.h>

DebugUI::DebugUI(Object* inOwner, ID2D1RenderTarget* inRt2D, DirectX11& dx, DX::IMouseInterface* mouse)
	: UserWidget(inOwner, inRt2D, dx, mouse)
{
}
DebugUI::~DebugUI()
{
	if (pRootSlate != nullptr)
	{
		pRootSlate->ClearChildren();
	}
	pText_FPS.reset();
	pText_FPS = nullptr;

	pText_CPUUsed.reset();
	pText_CPUUsed = nullptr;
	pText_CPUCurrentUsed.reset();
	pText_CPUCurrentUsed = nullptr;

	pText_TotalVirtual.reset();
	pText_TotalVirtual = nullptr;
	pText_VirtualUsed.reset();
	pText_VirtualUsed = nullptr;
	pText_VirtualCurrentUsed.reset();
	pText_VirtualCurrentUsed = nullptr;

	pText_TotalPhys.reset();
	pText_TotalPhys = nullptr;
	pText_PhysUsed.reset();
	pText_PhysUsed = nullptr;
	pText_PhysCurrentUsed.reset();
	pText_PhysCurrentUsed = nullptr;
}

// ------------------------------------------------------------------------------------------------------------
// Main
// ------------------------------------------------------------------------------------------------------------
void DebugUI::NativeOnInitialized()
{
	const auto windowSize = EngineSettings::GetWindowSize();
	pRootSlate = std::make_shared<S_CanvasPanel>(windowSize, pRt2D);
	pRootSlate->SetPosition({ 0, 0 });

	FSlateInfos menuVBInfos;
	menuVBInfos.padding = { 5.f, 5.f, 0.f, 0.f };
	auto menuVB = std::make_shared<S_VerticalBox>(FVector2D(500.f, windowSize.y), pRt2D, menuVBInfos);
	pRootSlate->AddChild(menuVB);

	/* TextBlock */
	auto MakeTextBlock = [this, &menuVB](const std::wstring& label = L"")
	{
		FSlateInfos textInfos;
		textInfos.padding = { 1.f };
		FSlateFont font;
		FSlateTextAppearance appearance;
		appearance.color = FColor(255.f, 0.f, 0.f, 1.f);
		appearance.hAlign = EHorizontalAlignment::Left;

		auto textBlock = std::make_shared<S_TextBlock>(FVector2D(500.f, 30.f), pRt2D, textInfos, font, appearance);
		textBlock->SetText(label);
		menuVB->AddChild(textBlock);
		return std::move(textBlock);
	};

	// -------------------------------------------------------
	// State : FPS
	// -------------------------------------------------------
	pText_FPS = MakeTextBlock();

	// -------------------------------------------------------
	// State : CPU
	// -------------------------------------------------------
	InitCPUUsed();
	pText_CPUUsed = MakeTextBlock();
	InitCPUCurrentUsed();
	pText_CPUCurrentUsed = MakeTextBlock();

	// -------------------------------------------------------
	// State : virtual memory
	// -------------------------------------------------------
	/* total virtual memory */
	pText_TotalVirtual = MakeTextBlock();
	/* virtual memory used */
	pText_VirtualUsed = MakeTextBlock();
	/* virtual memory current used */
	pText_VirtualCurrentUsed = MakeTextBlock();

	// -------------------------------------------------------
	// State : physical memory
	// -------------------------------------------------------
	/* total physical memory */
	pText_TotalPhys = MakeTextBlock();
	/* physical memory used */
	pText_PhysUsed = MakeTextBlock();
	/* physical memory current used */
	pText_PhysCurrentUsed = MakeTextBlock();

	pRootSlate->UpdateWidget();
	UserWidget::NativeOnInitialized();
}

int MemoryDiv = 1048576;
void DebugUI::Tick(DirectX11& dx, float deltaTime)
{
	UserWidget::Tick(dx, deltaTime);

	MEMORYSTATUSEX memInfo;
	memInfo.dwLength = sizeof(MEMORYSTATUSEX);
	GlobalMemoryStatusEx(&memInfo);

	PROCESS_MEMORY_COUNTERS_EX pmc;
	GetProcessMemoryInfo(GetCurrentProcess(), (PROCESS_MEMORY_COUNTERS*)&pmc, sizeof(pmc));

	// -------------------------------------------------------
	// State : FPS
	// -------------------------------------------------------
	if (pText_FPS != nullptr)
	{
		pText_FPS->SetText(std::format(L"FPS : {}", (int)(1.f / deltaTime)));
	}

	// -------------------------------------------------------
	// State : CPU
	// -------------------------------------------------------
	if (pText_CPUUsed != nullptr)
	{
		pText_CPUUsed->SetText(std::format(L"CPU Used : {}", GetCPUUsed()));
	}
	if (pText_CPUCurrentUsed != nullptr)
	{
		pText_CPUCurrentUsed->SetText(std::format(L"CPU current used : {}", GetCPUCurrentUsed()));
	}

	// -------------------------------------------------------
	// State : virtual memory
	// -------------------------------------------------------
	if (pText_TotalVirtual != nullptr)
	{
		pText_TotalVirtual->SetText(std::format(L"total virtual memory : {}", memInfo.ullTotalPageFile / MemoryDiv));
	}
	if (pText_VirtualUsed != nullptr)
	{
		pText_VirtualUsed->SetText(std::format(L"virtual memory used : {}", (memInfo.ullTotalPageFile - memInfo.ullAvailPageFile) / MemoryDiv));
	}
	if (pText_VirtualCurrentUsed != nullptr)
	{
		pText_VirtualCurrentUsed->SetText(std::format(L"virtual memory current used : {}", pmc.PrivateUsage / MemoryDiv));
	}

	// -------------------------------------------------------
	// State : physical memory
	// -------------------------------------------------------
	if (pText_TotalPhys != nullptr)
	{
		pText_TotalPhys->SetText(std::format(L"total physical memory : {}", memInfo.ullTotalPhys / MemoryDiv));
	}
	if (pText_PhysUsed != nullptr)
	{
		pText_PhysUsed->SetText(std::format(L"physical memory used : {}", (memInfo.ullTotalPhys - memInfo.ullAvailPhys) / MemoryDiv));
	}
	if (pText_PhysCurrentUsed != nullptr)
	{
		pText_PhysCurrentUsed->SetText(std::format(L"physical memory current used : {}", pmc.WorkingSetSize / MemoryDiv));
	}
}

// -------------------------------------------------------
// Main : CPU Used
// -------------------------------------------------------
static PDH_HQUERY cpuQuery;
static PDH_HCOUNTER cpuTotal;

void DebugUI::InitCPUUsed()
{
	//PdhOpenQuery(NULL, NULL, &cpuQuery);
	//// You can also use L"\\Processor(*)\\% Processor Time" and get individual CPU values with PdhGetFormattedCounterArray()
	//PdhAddEnglishCounter(cpuQuery, L"\\Processor(_Total)\\% Processor Time", NULL, &cpuTotal);
	//PdhCollectQueryData(cpuQuery);
}

float DebugUI::GetCPUUsed()
{
	//PDH_FMT_COUNTERVALUE counterVal;

	//PdhCollectQueryData(cpuQuery);
	//PdhGetFormattedCounterValue(cpuTotal, PDH_FMT_DOUBLE, NULL, &counterVal);
	//return counterVal.doubleValue;
	return 0;
}

// -------------------------------------------------------
// Main : CPU Current Used
// -------------------------------------------------------
static ULARGE_INTEGER lastCPU, lastSysCPU, lastUserCPU;
static int numProcessors;
static HANDLE self;

void DebugUI::InitCPUCurrentUsed()
{
	//SYSTEM_INFO sysInfo;
	//FILETIME ftime, fsys, fuser;

	//GetSystemInfo(&sysInfo);
	//numProcessors = sysInfo.dwNumberOfProcessors;

	//GetSystemTimeAsFileTime(&ftime);
	//memcpy(&lastCPU, &ftime, sizeof(FILETIME));

	//self = GetCurrentProcess();
	//GetProcessTimes(self, &ftime, &ftime, &fsys, &fuser);
	//memcpy(&lastSysCPU, &fsys, sizeof(FILETIME));
	//memcpy(&lastUserCPU, &fuser, sizeof(FILETIME));
}

float DebugUI::GetCPUCurrentUsed()
{
	//FILETIME ftime, fsys, fuser;
	//ULARGE_INTEGER now, sys, user;
	//double percent;

	//GetSystemTimeAsFileTime(&ftime);
	//memcpy(&now, &ftime, sizeof(FILETIME));

	//GetProcessTimes(self, &ftime, &ftime, &fsys, &fuser);
	//memcpy(&sys, &fsys, sizeof(FILETIME));
	//memcpy(&user, &fuser, sizeof(FILETIME));
	//percent = (sys.QuadPart - lastSysCPU.QuadPart) +
	//	(user.QuadPart - lastUserCPU.QuadPart);
	//percent /= (now.QuadPart - lastCPU.QuadPart);
	//percent /= numProcessors;
	//lastCPU = now;
	//lastUserCPU = user;
	//lastSysCPU = sys;

	//return percent * 100;
	return 0;
}