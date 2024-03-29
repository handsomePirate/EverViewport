project "Test"
	kind "ConsoleApp"
	staticruntime "off"
	language "C++"
	cppdialect "C++17"
	location ""
	targetdir "../../Test/build/%{cfg.buildcfg}"
	objdir "obj/%{cfg.buildcfg}"
	files { "../../Test/src/**.hpp", "../../Test/src/**.cpp" }
	
	includedirs {
		SoftwareCoreInclude,
		EverViewportInclude
	}
	
	links {
		"EverViewport",
		"SoftwareCore"
	}
	
	filter "system:linux"
		links {
			"X11"
		}
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
