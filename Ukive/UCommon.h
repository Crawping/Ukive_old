#pragma once

#include <vector>
#include <string>
#include <sstream>
#include <mutex>
#include <list>
#include <stack>
#include <queue>
#include <atomic>
#include <map>
#include <cstdint>
#include <fstream>

#include "UUtility.h"
#include "DirectXMath\Inc\DirectXMath.h"
#include "DirectXMath\Inc\DirectXCollision.h"

using DirectX::XMVECTOR;
using DirectX::XMMATRIX;
using DirectX::XMFLOAT2;
using DirectX::XMFLOAT3;
using DirectX::XMFLOAT4;
using DirectX::XMFLOAT4X4;

#include <windows.h>
#include <windowsx.h>
#include <initguid.h>
#include <dwmapi.h>

#include <d2d1_2.h>
#include <d2d1effects_1.h>
#include <dwrite_2.h>

#include <d3d11_2.h>
#include <d3dcompiler.h>
#include <dxgi1_3.h>

#include <msctf.h>
#include <commCtrl.h>
#include <uianimation.h>
#include <wincodec.h>

#pragma comment(lib, "windowscodecs.lib")
#pragma comment(lib, "dwmapi.lib")
#pragma comment(lib, "comctl32.lib")
#pragma comment(lib, "d2d1.lib")
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "dwrite.lib")

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "dxgi.lib")


#define RH(x)    {HRESULT hr=(x); if(FAILED(hr)){ return hr;} }