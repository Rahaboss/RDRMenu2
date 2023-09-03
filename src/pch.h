#pragma once

// C++ standard library
#include <iostream>
#include <string_view>
#include <vector>
#include <cassert>
#include <chrono>
#include <thread>
#include <atomic>

// Windows
#include <Windows.h>
#include <Psapi.h>
#include <d3d12.h>
#include <dxgi1_5.h>

// Third-party libraries
#include <Minhook.h>
#include <json.hpp>
#include <imgui.h>
#include <imgui_impl_dx12.h>
#include <imgui_impl_win32.h>

// Global variables
inline HMODULE g_Module{}; // DLL handle (used to exit thread)
inline HMODULE g_GameModule{}; // Game handle
inline uintptr_t g_BaseAddress{}; // Game base address
inline HANDLE g_MainThread{}; // Main thread handle (must be closed before exiting)
inline std::atomic_bool g_Running{ true }; // Set to false to eject

// Enable literal macros
using namespace std::literals::chrono_literals;

// Shortcut macros
#define LOG_HEX(v) std::hex << std::uppercase << "0x" << (uintptr_t)v << std::dec << std::nouppercase

// Toggles
#define TRY __try
#define EXCEPT __except(EXCEPTION_EXECUTE_HANDLER)
#define LOG_EXCEPTION() std::cout << "Caught exception in:\n\tFile: " << __FILE__ << ':' << __LINE__ << "\n\tFunction: " << __FUNCTION__ << ".\n";
