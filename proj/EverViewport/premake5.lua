EverViewportInclude = path.getabsolute("../../include", os.getcwd())

project "EverViewport"
	kind "StaticLib"
	staticruntime "off"
	language "C++"
	cppdialect "C++17"
	location ""
	targetdir "../../build/%{cfg.buildcfg}"
	objdir "obj/%{cfg.buildcfg}"
	files { "../../src/**.hpp", "../../src/**.cpp", "../../include/**.hpp" }
	
	flags {
		"MultiProcessorCompile"
	}
	
	includedirs {
		SoftwareCoreInclude,
		EverViewportInclude
	}
	
	links {
		"SoftwareCore"
	}
	
	filter "system:windows"
		systemversion "latest"
	filter{}
	
	filter "configurations:Debug"
		defines { "DEBUG" }
		runtime "Debug"
		symbols "On"

	filter "configurations:Release"
		defines { "RELEASE" }
		runtime "Release"
		optimize "On"

	filter {}