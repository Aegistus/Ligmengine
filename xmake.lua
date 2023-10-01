add_rules("mode.debug", "mode.release")
add_rules("plugin.vsxmake.autoupdate")
add_requires("glfw")
add_requires("spdlog")
add_requires("soloud")
add_requires("wgpu-native", "glfw3webgpu")
add_requires("glm")
add_requires("stb")
add_requires("lua", "sol2")

set_policy("build.warning", true) -- show warnings
set_warnings("all") -- warn about many things

target("ligmengine")
    set_kind("static")
    set_languages("cxx20")

    -- Adds required packages.
    add_packages("glfw")
    add_packages("spdlog")
    add_packages("soloud")
    add_packages("wgpu-native", "glfw3webgpu")
    add_packages("glm", { public = true })
    add_packages("stb")
    add_packages("lua")
    add_packages("sol2", { public = true })

    -- Declare our engine's header path.
    -- This allows targets that depend on the engine to #include them.
   
    add_includedirs("src", {public = true})
    
    -- Add all .cpp files in the `src` directory.
    add_files("src/*.cpp")

target("helloworld")
    set_kind("binary")
    set_languages("cxx20")
    set_rundir("$(projectdir)")

    -- Adds required packages.
    add_packages("glfw")
    add_packages("spdlog")
    add_packages("soloud")
    add_packages("wgpu-native", "glfw3webgpu")

    add_deps("ligmengine")

    add_files("demo/helloworld.cpp")

