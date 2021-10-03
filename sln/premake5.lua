workspace "EverViewport"
	architecture "x64"
	configurations { "Debug", "Release" }
	location ""
	
	flags
	{
		"MultiProcessorCompile"
	}

include "../proj/EverViewport"
include "../proj/Test"