# CMakeList.txt : CMake project for OrderbookDemo, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project ("OrderbookDemo")

# Add source to this project's executable.
add_executable (OrderbookDemo "src/mainWindows.cpp"  "src/Order.cpp" "src/Trade.cpp" "src/Orderbook.cpp" "src/Terminal.cpp" "src/pch.cpp" "src/Enums.cpp")

# Enable automatic precompiled headers
target_precompile_headers(OrderbookDemo PRIVATE include/pch.hpp)

include_directories(include)
target_include_directories(OrderbookDemo PRIVATE "/include")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET OrderbookDemo PROPERTY CXX_STANDARD 20)
endif()

# TODO: Add tests and install targets if needed.
