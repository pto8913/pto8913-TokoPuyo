
#include "UI/DebugUI.h"

#include "Slate/CanvasPanel.h"
#include "Slate/VerticalBox.h"
#include "Slate/HorizontalBox.h"
#include "Slate/TextBlock.h"

#include "EngineSettings.h"

#include <Psapi.h>
#include <format>
#include <Pdh.h>

DebugUI::DebugUI(Object* inOwner, DirectX11& dx, DX::IMouseInterface* mouse)
	: UserWidget(
		inOwner,
		dx,
		mouse,
		EngineSettings::GetWindowSize().x,
		EngineSettings::GetWindowSize().y
	)
{
	const auto windowSize = EngineSettings::GetWindowSize();
	pRootSlate = std::make_shared<S_CanvasPanel>(windowSize, GetRt2D());

	FSlateInfos menuVBInfos;
	menuVBInfos.padding = { 5.f, 5.f, 0.f, 0.f };
	auto menuVB = std::make_shared<S_VerticalBox>(FVector2D(500.f, windowSize.y), GetRt2D(), menuVBInfos);

	/* TextBlock */
	auto MakeTextBlock = [this, &menuVB](const std::wstring& label = L"")
	{
		FSlateInfos textInfos;
		textInfos.padding = { 1.f };
		auto textBlock = std::make_shared<S_TextBlock>(FVector2D(500.f, 30.f), GetRt2D(), textInfos);
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

}
DebugUI::DebugUI(DirectX11& dx, DX::IMouseInterface* mouse)
	: DebugUI(nullptr, dx, mouse)
{
}

DebugUI::~DebugUI()
{
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
		pText_FPS->SetText(std::format(L"FPS : {}", 1.f / deltaTime));
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
		pText_TotalVirtual->SetText(std::format(L"total virtual memory : {}", memInfo.ullTotalPageFile));
	}
	if (pText_VirtualUsed != nullptr)
	{
		pText_TotalVirtual->SetText(std::format(L"virtual memory used : {}", memInfo.ullTotalPageFile - memInfo.ullAvailPageFile));
	}
	if (pText_VirtualCurrentUsed != nullptr)
	{
		pText_VirtualCurrentUsed->SetText(std::format(L"virtual memory current used : {}", pmc.PrivateUsage));
	}

	// -------------------------------------------------------
	// State : physical memory
	// -------------------------------------------------------
	if (pText_TotalPhys != nullptr)
	{
		pText_TotalPhys->SetText(std::format(L"total physical memory : {}", memInfo.ullTotalPhys));
	}
	if (pText_PhysUsed != nullptr)
	{
		pText_TotalPhys->SetText(std::format(L"physical memory used : {}", memInfo.ullTotalPhys - memInfo.ullAvailPhys));
	}
	if (pText_PhysCurrentUsed != nullptr)
	{
		pText_PhysCurrentUsed->SetText(std::format(L"physical memory current used : {}", pmc.WorkingSetSize));
	}
}

// -------------------------------------------------------
// Main : CPU Used
// -------------------------------------------------------
static PDH_HQUERY cpuQuery;
static PDH_HCOUNTER cpuTotal;

void DebugUI::InitCPUUsed()
{
	PdhOpenQueryA(NULL, NULL, &cpuQuery);
	// You can also use L"\\Processor(*)\\% Processor Time" and get individual CPU values with PdhGetFormattedCounterArray()
	PdhAddEnglishCounterA(cpuQuery, "\\Processor(_Total)\\% Processor Time", NULL, &cpuTotal);
	PdhCollectQueryData(cpuQuery);
}

float DebugUI::GetCPUUsed()
{
	PDH_FMT_COUNTERVALUE counterVal;

	PdhCollectQueryData(cpuQuery);
	PdhGetFormattedCounterValue(cpuTotal, PDH_FMT_DOUBLE, NULL, &counterVal);
	return counterVal.doubleValue;
}

// -------------------------------------------------------
// Main : CPU Current Used
// -------------------------------------------------------
static ULARGE_INTEGER lastCPU, lastSysCPU, lastUserCPU;
static int numProcessors;
static HANDLE self;

void DebugUI::InitCPUCurrentUsed()
{
	SYSTEM_INFO sysInfo;
	FILETIME ftime, fsys, fuser;

	GetSystemInfo(&sysInfo);
	numProcessors = sysInfo.dwNumberOfProcessors;

	GetSystemTimeAsFileTime(&ftime);
	memcpy(&lastCPU, &ftime, sizeof(FILETIME));

	self = GetCurrentProcess();
	GetProcessTimes(self, &ftime, &ftime, &fsys, &fuser);
	memcpy(&lastSysCPU, &fsys, sizeof(FILETIME));
	memcpy(&lastUserCPU, &fuser, sizeof(FILETIME));
}

float DebugUI::GetCPUCurrentUsed()
{
	FILETIME ftime, fsys, fuser;
	ULARGE_INTEGER now, sys, user;
	double percent;

	GetSystemTimeAsFileTime(&ftime);
	memcpy(&now, &ftime, sizeof(FILETIME));

	GetProcessTimes(self, &ftime, &ftime, &fsys, &fuser);
	memcpy(&sys, &fsys, sizeof(FILETIME));
	memcpy(&user, &fuser, sizeof(FILETIME));
	percent = (sys.QuadPart - lastSysCPU.QuadPart) +
		(user.QuadPart - lastUserCPU.QuadPart);
	percent /= (now.QuadPart - lastCPU.QuadPart);
	percent /= numProcessors;
	lastCPU = now;
	lastUserCPU = user;
	lastSysCPU = sys;

	return percent * 100;
}