#pragma once

// C++ standard library
#include <iostream>
#include <string>
#include <string_view>
#include <array>
#include <vector>
#include <cassert>
#include <atomic>
#include <chrono>
#include <thread>
#include <queue>
#include <functional>
#include <map>
#include <filesystem>
#include <fstream>
#include <sstream>

// Windows
#include <Windows.h>
#include <Psapi.h>
#include <dxgi1_5.h>
#include <d3d12.h>

// Third-party libraries
#include <Minhook.h>
#include <imgui.h>
#include <imgui_impl_vulkan.h>
#include <imgui_impl_dx12.h>
#include <imgui_impl_win32.h>
#include <json.hpp>

// User headers
#include "Settings.h"
#include "Logger.h"

// Global variables
inline HMODULE g_Module; // DLL handle (used to exit thread)
inline HMODULE g_GameModule; // Game handle
inline uintptr_t g_BaseAddress; // Game base address
inline HANDLE g_MainThread; // Main thread handle (must be closed before exiting)
inline std::atomic_bool g_Running = true; // Set to false to eject

// Enable literal macros
using namespace std::chrono_literals;
using namespace std::literals::string_view_literals;
using namespace std::literals::string_literals;

using json = nlohmann::json;

// Shortcut Macros
#define LOG_HEX(v) std::hex << std::uppercase << "0x" << (uintptr_t)v << std::dec << std::nouppercase
#define TRY __try
#define EXCEPT __except(EXCEPTION_EXECUTE_HANDLER)
#define LOG_EXCEPTION() LOG_TO_CONSOLE("Caught exception in:\n\tFile: %s:%d\n\tFunction: %s\n", __FILE__, __LINE__, __FUNCTION__);
#define __ROL8__ _rotl64
#define __ROR8__ _rotr64

// Toggles
#define ENABLE_ANTI_ANTI_DEBUG 0
#define ENABLE_VULKAN_RENDERER 0
