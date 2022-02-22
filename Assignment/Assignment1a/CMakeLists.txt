# CMakeList.txt : CMake project for Assignment1a, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.13)

project(Assignment1a)

set(SOURCEFILES
	Assignment1a.cpp
	Input_processor.cpp
	Vector3.cpp
	Point3.cpp
	Color.cpp
	Sphere.cpp
	Ray.cpp
)

set(HEADERFILES
	Assignment1a.h
	Input_processor.h
	Vector3.h
	Point3.h
	Color.h
	Sphere.h
	Ray.h
)

# Add source to this project's executable.
add_executable (${PROJECT_NAME} ${SOURCEFILES} ${HEADERFILES})

# TODO: Add tests and install targets if needed.
set_target_properties(Assignment1a PROPERTIES VS_DEBUGGER_COMMAND_ARGUMENTS "../setup.txt")
