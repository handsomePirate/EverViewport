workspace "EverViewport"
	architecture "x64"
	configurations { "Debug", "Release" }
	location ""
	
	flags
	{
		"MultiProcessorCompile"
	}

include "../dependencies.lua"
	
include "../proj/EverViewport"
include "../proj/Test"