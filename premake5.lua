workspace "RDRMenu2"
	architecture "x86_64"
	configurations {
		"Debug", -- Regular debug build
		"Release", -- Build with optimizations and debug information
		"Dist" -- Build with maximum optimization and no debug information meant for distribution
	}

	defines {
		"_CRT_SECURE_NO_WARNINGS",
		"_USE_MATH_DEFINES", -- Enable macros like M_PI in cmath
		"NOMINMAX", -- std::min, std::max
		"WIN32_LEAN_AND_MEAN" -- Exclude rarely used stuff
	}

	-- Locations
	startproject "RDRMenu2"
	targetdir "bin/%{cfg.buildcfg}/%{prj.name}"
	objdir "int/%{cfg.buildcfg}/%{prj.name}"

	-- Optimizations
	vectorextensions "AVX2"
	floatingpoint "Fast"
	staticruntime "Off" -- Sets Run-Time Library to MultiThreadedDLL
	flags {
		"MultiProcessorCompile",
		"NoManifest" -- Disable creation of Manifest XML file
	}

	-- Build configuration specifics
	filter "configurations:Debug"
		symbols "On"
		optimize "Off"

	filter "configurations:Release"
		symbols "On"
		optimize "Speed" -- All optimizations favoring speed
		flags {
			"LinkTimeOptimization"
		}

	filter "configurations:Dist"
		symbols "Off" -- Completely disable debug information
		optimize "Speed" -- All optimizations favoring speed
		flags {
			"LinkTimeOptimization"
		}

	project "RDRMenu2"
		kind "SharedLib" -- .dll
		language "C++"
		cppdialect "C++17"
		location "src" -- Project file location

		pchheader "pch.h"
		pchsource "src/pch.cpp" -- Must be defined relative to the script
		forceincludes "pch.h"

		defines {
			"NOGDI"
		}

		-- Source files
		files {
			"src/**.hpp",
			"src/**.h",
			"src/**.cpp",
			"src/**.c",
			"src/**.asm"
		}

		-- .lib locations
		libdirs {
			"bin/lib/%{cfg.buildcfg}",
			"%VULKAN_SDK%/lib"
		}
		
		-- .lib links
		links {
			"MinHook",
			"ImGui",
			"vulkan-1"
		}

		includedirs {
			"src",
			"lib/MinHook/include",
			"lib/ImGui",
			"lib/ImGui/backends",
			"lib/json/single_include/nlohmann",
			"%VULKAN_SDK%/include"
		}

		disablewarnings {
			"4838", -- Narrowing conversion
			"26812", -- Prefer "enum class"
			"26819", -- Unannotated fallthrough between switch labels
			"28020", -- The expression 'expr' is not true at this call
			"33011" -- Unchecked lower bound for enum key/source as index
		}

	project "ImGui"
		kind "StaticLib" -- .lib
		language "C++"
		cppdialect "C++11"
		location "src" -- .vcxproj file location

		files {
			"lib/%{prj.name}/*.cpp",
			"lib/%{prj.name}/*.h",
			"lib/%{prj.name}/backends/imgui_impl_vulkan.*",
			"lib/%{prj.name}/backends/imgui_impl_dx12.*",
			"lib/%{prj.name}/backends/imgui_impl_win32.*"
		}

		includedirs {
			"lib/%{prj.name}",
			"%VULKAN_SDK%/include"
		}
		
		disablewarnings {
			"26812", -- Prefer "enum class"
			"28020", -- The expression ... is not true at this call
			"33011" -- Unchecked lower bound for enum key/source as index
		}

	project "MinHook"
		kind "StaticLib" -- .lib
		language "C"
		location "src" -- Project file location

		files {
			"lib/%{prj.name}/src/**.c",
			"lib/%{prj.name}/src/**.h",
			"lib/%{prj.name}/src/include/*"
		}

		includedirs {
			"lib/%{prj.name}/include"
		}
