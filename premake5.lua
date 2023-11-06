local BUILD_DIR = path.join("build", _ACTION)
if _OPTIONS["cc"] ~= nil then
	BUILD_DIR = BUILD_DIR .. "_" .. _OPTIONS["cc"]
end

solution "parser"
	location(BUILD_DIR)
	startproject "parser"
	configurations { "Release", "Debug" }
	if os.is64bit() and not os.istarget("windows") then
		platforms "x86_64"
	else
		platforms { "x86", "x86_64" }
	end
	filter "configurations:Release"
		defines
		{
			"NDEBUG",
		}
		optimize "Full"
	filter "configurations:Debug*"
		defines
		{
			"_DEBUG",
		}
		optimize "Debug"
		symbols "On"
	filter "platforms:x86"
		architecture "x86"
	filter "platforms:x86_64"
		architecture "x86_64"

project "parser"
    kind "ConsoleApp"
	language "C++"
	files
	{
		"src/**",
	}
	filter "system:windows"
		systemversion "latest"
		cppdialect "C++17"
		staticruntime "On"
	filter "system:linux"
		pic "On"
		systemversion "latest"
		cppdialect "C++17"
		staticruntime "On"
	filter "configurations:Debug"
		runtime "Debug"
		symbols "on"
	filter "configurations:Release"
		runtime "Release"
		optimize "on"