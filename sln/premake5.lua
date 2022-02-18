workspace "EverViewport"
	architecture "x64"
	configurations { "Debug", "Release" }
	startproject "Test"
	location ""
	
	flags
	{
		"MultiProcessorCompile"
	}

include "../dependencies.lua"
	
include "../proj/EverViewport"
include "../proj/Test"