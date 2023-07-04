add_executable(OpenGLpbr main.cpp Camera.cpp Drawable.cpp Earth.cpp Light.cpp Material.cpp Mesh.cpp  PhongMaterial.cpp Scene.cpp Shader.cpp Sphere.cpp SphereMesh.cpp Timer.cpp Transform.cpp)
target_link_libraries(OpenGLpbr 
	PUBLIC glfw
	PUBLIC glbinding
	PUBLIC glbinding-aux
	PUBLIC glm
	PUBLIC stb)