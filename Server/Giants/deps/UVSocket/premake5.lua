workspace "UVSocket"
	configurations{ "Debug", "Release"}
	location "VSBuild"

project "UVSocket"
	kind "StaticLib"
	language "C++"
	location "VSBuild"
	architecture "x86_64"
	
	files {"UVSocket/**.h", "UVSocket/**.cpp"}
	includedirs{"libuv/include"}
	
	filter "configurations:Debug"
		targetdir "Debug"
		runtime "Debug"
	
	filter "configurations:Release"
		targetdir "Release"
		runtime "Release"
		flags {"NoIncrementalLink"}

project "TestServerSocket"
	kind "ConsoleApp"
	language "C++"
	location "VSBuild"
	architecture "x86_64"
	dependson {"UVSocket"}
	
	files {"TestServerSocket/**.h", "TestServerSocket/**.cpp"}
	includedirs{"libuv/include", "UVSocket/", "vld/include"}
	links {"libuv", "UVSocket", "ws2_32", "Psapi", "IPHLPAPI", "Userenv"}
	
	
	filter "configurations:Debug"
		links {"vld"}
		includedirs{"vld/include"}
		libdirs{"libuv/lib/Debug", "Debug", "vld/lib"}
		targetdir "VSBuild/Debug"
		runtime "Debug"
		flags {"Symbols"}
	
	filter "configurations:Release"
		libdirs{"libuv/lib/Release", "Release"}
		targetdir "VSBuild/Release"
		runtime "Release"
		flags {"NoIncrementalLink"}
		
project "TestClientSocket"
	kind "ConsoleApp"
	language "C++"
	location "VSBuild"
	architecture "x86_64"
	dependson {"UVSocket"}
	
	files {"TestClientSocket/**.h", "TestClientSocket/**.cpp"}
	includedirs{"libuv/include", "UVSocket/", "vld/include"}
	links {"libuv", "UVSocket", "ws2_32", "Psapi", "IPHLPAPI", "Userenv"}
	
	filter "configurations:Debug"
		links {"vld"}
		libdirs{"libuv/lib/Debug", "Debug", "vld/lib"}
		targetdir "VSBuild/Debug"
		runtime "Debug"
		flags {"Symbols"}
	
	filter "configurations:Release"
		libdirs{"libuv/lib/Release", "Release"}
		targetdir "VSBuild/Release"
		runtime "Release"
		flags {"NoIncrementalLink"}
		