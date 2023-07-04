#glfw
add_subdirectory(glfw)

#glbinding
add_subdirectory(glbinding)

#glm
add_subdirectory(glm)

#stb
add_library(stb INTERFACE)
target_include_directories(stb
	INTERFACE stb)