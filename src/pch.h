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

// Windows
#include <Windows.h>
#include <Psapi.h>
#include <dxgi1_4.h>
#include <d3d12.h>

// Third-party libraries
#include <Minhook.h>
#include <imgui.h>
//#include <imgui_impl_vulkan.h> // Maybe one day
#include <imgui_impl_dx12.h>
#include <imgui_impl_win32.h>

// Global variables
inline HMODULE g_Module; // DLL handle (used to exit thread)
inline HMODULE g_GameModule; // Game handle
inline uintptr_t g_BaseAddress; // Game base address
inline HANDLE g_MainThread; // Main thread handle (must be closed before exiting)
inline std::atomic_bool g_Running = true; // Set to false to eject

// Enable literal macros
using namespace std::chrono_literals;
using namespace std::literals::string_view_literals;

// Shortcut Macros
#define LOG_HEX(v) std::hex << std::uppercase << "0x" << (uintptr_t)v << std::dec << std::nouppercase
#define TRY __try
#define EXCEPT __except(EXCEPTION_EXECUTE_HANDLER)
#define LOG_EXCEPTION() std::cout << "Caught exception in:\n\tFile " << __FILE__ << ':' << __LINE__ << "\n\tFunction " << __FUNCTION__ << '\n';
#define __ROL8__ _rotl64
#define __ROR8__ _rotr64

// Toggles
#define ENABLE_LARGE_STACK_ITEMS 0 // e.g. g_PedModelNameList
#define ENABLE_ANTI_ANTI_DEBUG 0
