cmake_minimum_required(VERSION 3.20)


project(UniT
	VERSION 0.1.0
	LANGUAGES CXX
)

add_library(UniT_UniT INTERFACE)
set_property(TARGET UniT_UniT PROPERTY CXX_STANDARD 26)

target_include_directories(UniT_UniT INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include)
add_library(UniT::UniT ALIAS UniT_UniT)


if (PROJECT_IS_TOP_LEVEL)
	add_executable(UniT_examples examples/main.cpp)
	set_property(TARGET UniT_examples PROPERTY CXX_STANDARD 26)
	target_link_libraries(UniT_examples UniT::UniT)
endif()
