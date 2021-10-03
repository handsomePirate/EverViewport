project "EverViewport"
	kind "StaticLib"
	staticruntime "off"
	language "C++"
	cppdialect "C++17"
	location ""
	targetdir "../../build/%{cfg.buildcfg}"
	objdir "/obj/%{cfg.buildcfg}"
	files { "../../src/**.hpp", "../../src/**.cpp" }

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