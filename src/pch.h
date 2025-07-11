#pragma once

// Code toggles
#define ENABLE_NATIVE_RETURN_SPOOFING 0
#define ENABLE_DEBUGGING 0
#define ENABLE_VULKAN_RENDERER 0

// Development / non-distribution toggles
#if !_DIST
#define USE_SOLUTION_DIR_AS_CONFIG_DIR 1
#define ENABLE_DEBUG_HOOKS 1
#define ENABLE_DEBUG_TAB 1
#define ENABLE_NETWORK_TAB 1 // Example network tab, no functionality
#endif // !_DIST

// C++ standard library
#include <iostream>
#include <vector>
#include <string>
#include <string_view>
#include <filesystem>
#include <fstream>
#include <queue>
#include <array>
#include <cassert>
#include <chrono>
#include <thread>
#include <atomic>
#include <sstream>
#include <random>
#include <execution>
#include <algorithm>

// Windows
#include <Windows.h>
#include <Psapi.h>
#include <d3d12.h>
#include <dxgi1_5.h>
#include <intrin.h>

// Third-party libraries
#include <Minhook.h>
#include <json.hpp>
#include <imgui.h>
#include <imgui_impl_win32.h>
#include <imgui_impl_dx12.h>
#if ENABLE_VULKAN_RENDERER
#include <imgui_impl_vulkan.h>
#include <vulkan/vulkan.h>
#endif // ENABLE_VULKAN_RENDERER

// User headers
#include "Util/Logging.h"

// Global variables
inline HMODULE g_Module{}; // DLL handle (used to exit thread)
inline HMODULE g_GameModule{}; // Game handle
inline uintptr_t g_BaseAddress{}; // Game base address
inline HANDLE g_MainThread{}; // Main thread handle (must be closed before exiting)
inline std::atomic_bool g_Running{ true }; // Set to false to eject

// Enable literal macros
using namespace std::literals::chrono_literals;

typedef nlohmann::json json;

// Shortcut macros
#define TRY __try
#define EXCEPT __except(EXCEPTION_EXECUTE_HANDLER)
#define LOG_EXCEPTION() LOG_TEXT("Caught exception in:\n\tFile: %s:%u\n\tFunction: %s.", __FILE__, __LINE__, __FUNCTION__)
#define TO_IDA(x) (((uintptr_t)(x)) - g_BaseAddress + 0x7FF66B4D0000)
