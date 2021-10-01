workspace "EverViewport"
	architecture "x64"
	configurations { "Debug", "Release" }
	location "proj"
	
	flags
	{
		"MultiProcessorCompile"
	}

project "EverViewport"
	kind "StaticLib"
	staticruntime "off"
	language "C++"
	cppdialect "C++17"
	location "proj/EverViewport"
	targetdir "./build/%{cfg.buildcfg}"
	objdir "proj/EverViewport/obj/%{cfg.buildcfg}"
	files { "src/**.hpp", "src/**.cpp" }

	filter "system:windows"
		systemversion "latest"
	filter{}
	
	filter "configurations:Debug"
		defines { "EW_DEBUG" }
		runtime "Debug"
		symbols "On"

	filter "configurations:Release"
		defines { "EW_RELEASE" }
		runtime "Release"
		optimize "On"

	filter {}
	
project "Test"
	kind "ConsoleApp"
	staticruntime "off"
	language "C++"
	cppdialect "C++17"
	location "proj/Test"
	targetdir "./Test/build/%{cfg.buildcfg}"
	objdir "proj/Test/obj/%{cfg.buildcfg}"
	files { "Test/src/**.hpp", "Test/src/**.cpp" }
	
	includedirs {
		"src",
		"Test/src"
	}
	
	links {
		"EverViewport"
	}
	
	filter "system:windows"
		systemversion "latest"
	filter{}
	
	filter "configurations:Debug"
		runtime "Debug"
		symbols "On"

	filter "configurations:Release"
		runtime "Release"
		optimize "On"

	filter {}
