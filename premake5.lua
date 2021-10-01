workspace "EverViewport"
	architecture "x64"
	configurations { "Debug", "Release" }
	location "proj"

project "EverViewport"
	kind "ConsoleApp"
	staticruntime "off"
	language "C++"
	cppdialect "C++17"
	location "proj"
	callingconvention "FastCall"
	targetdir "./build/%{cfg.buildcfg}"
	objdir "proj/obj/%{cfg.buildcfg}"
	files { "src/**.hpp", "src/**.cpp", "src/**.glsl" }

	flags {
		"MultiProcessorCompile"
	}

	includedirs {
		"src",
		"$(VULKAN_SDK)/include"
	}

	links {
		"$(VULKAN_SDK)/lib/vulkan-1.lib",
		"$(VULKAN_SDK)/lib/shaderc_shared.lib"
	}

	filter "system:windows"
		systemversion "latest"
	filter{}
	
	filter "configurations:Debug"
		defines { "DEBUG" }
		symbols "On"

	filter "configurations:Release"
		defines { "RELEASE" }
		optimize "On"

	filter {}
