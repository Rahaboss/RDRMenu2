#pragma once

// C++ standard library
#include <iostream>
#include <string_view>
#include <vector>
#include <cassert>
#include <atomic>
#include <chrono>
#include <thread>

// Windows
#include <Windows.h>
#include <Psapi.h>
#include <dxgi1_4.h>
#include <d3d12.h>

// Third-party libraries
#include <Minhook.h>
#include <imgui.h>
//#include <imgui_impl_vulkan.h>
#include <imgui_impl_dx12.h>
#include <imgui_impl_win32.h>

// Global variables
inline HMODULE g_hmodule; // DLL handle (used to exit thread)
inline HMODULE g_game_module; // Game handle
inline uintptr_t g_base_address; // Game base address
inline HANDLE g_main_thread; // Main thread handle (must be closed before exiting)
inline std::atomic_bool g_running = true; // Set to false to eject

using namespace std::chrono_literals;
using namespace std::literals::string_view_literals;

#define LOG_HEX(v) std::hex << std::uppercase << (uintptr_t)v << std::dec << std::nouppercase
#define TRY __try
#define EXCEPT __except(EXCEPTION_EXECUTE_HANDLER)
#define LOG_EXCEPTION() printf("Caught exception in:\n\tFile %s:%d\n\tFunction %s\n", __FILE__, __LINE__, __FUNCTION__)
