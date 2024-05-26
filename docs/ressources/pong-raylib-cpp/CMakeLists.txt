# CMake minimum version and project details
cmake_minimum_required(VERSION 3.10)
project(competitive-pong)


# Set C++ standard
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)


# Source files
set(SOURCE_FILES
    src/main.cpp

    src/application/globals.cpp
    src/application/WindowManager.cpp

    src/game/core/Game.cpp
    src/game/core/game_states/GameOverState.cpp
    src/game/core/game_states/MenuState.cpp
    src/game/core/game_states/PlayingState.cpp 

    src/game/entities/Ball.cpp
    src/game/entities/BotPaddle.cpp
    src/game/entities/Paddle.cpp

    src/game/managers/AudioManager.cpp
)


# Common Include directories
set(INCLUDE_DIRS
    src/3rdparty/raylib
    src/application
    src/game/core
    src/game/core/game_states
    src/game/entities
    src/game/managers
)

# Output directory
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/build)



################################ DEBUG ################################
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    # Create the executable from the sources files
    add_executable(debug ${SOURCE_FILES} datasrc/assets/logo/logo.o)

    # Indicate where are all the header files
    target_include_directories(debug PRIVATE ${INCLUDE_DIRS})

    # Link external libraries 
    target_link_directories(debug PRIVATE src/3rdparty/raylib) # Indicate where is the .a file of the raylib library
    target_link_libraries(debug PRIVATE raylib opengl32 gdi32 winmm) # Indicate which library we need

    # Add compilation options
    target_compile_options(debug PRIVATE -Wall -Wextra -g) # -g -> give informations to the debugger
endif()
########################################################################


################################ RELEASE ################################
if(CMAKE_BUILD_TYPE STREQUAL "Release")
    # Create the executable from the sources files
    add_executable(release WIN32 ${SOURCE_FILES} datasrc/assets/logo/logo.o) # WIN32 indicate that we are building an app with a ui, not a terminal app (it will not open the terminal )

    # Indicate where are all the header files
    target_include_directories(release PRIVATE ${INCLUDE_DIRS})

    # Link external libraries 
    target_link_directories(release PRIVATE src/3rdparty/raylib) # Indicate where is the .a file of the raylib library
    target_link_libraries(release PRIVATE raylib opengl32 gdi32 winmm) # Indicate which library we need

    # Add compilation options
    target_compile_options(release PRIVATE -O3) # Optimize the program (takes longer to compile but runs faster)

    # Add some dll files inside the executable
    target_link_options(release PRIVATE -static -static-libstdc++ -static-libgcc)
endif()
##########################################################################






###############
# Remove the command line when running the program
#set_target_properties(release PROPERTIES LINK_FLAGS "-mwindows")

