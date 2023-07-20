#pragma once

#include "Core/DirectXTex.h"
#include <d3d11.h>
#include <d3d11_2.h>
#include <d2d1.h>
#include <d2d1_1.h>
#include <wingdi.h>
#include <dxgi.h>
#include <sstream>

#include <D3DCompiler.h>
#include <DirectXMath.h>
#include <DirectXColors.h>

#include <Windows.h>
#include <wrl/client.h>

#include <assert.h>

#include <iostream>
#include <string>
#include <map>

#include "Utils/DirectXUtils.h"

#include "Delegate/Delegate.h"

#define _DEBUG 0

#if _DEBUG
#include <format>
#endif
