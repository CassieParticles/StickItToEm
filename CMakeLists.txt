cmake_minimum_required (VERSION 3.10)

project (Game VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

#Add main source file
list(APPEND ALLSOURCES "src/Main.cpp")

#Add all game source files
list(APPEND ALLSOURCES "src/Game/GameLevel.cpp" "src/Game/GameLevel.h")
list(APPEND ALLSOURCES "src/Game/TerrainManager.cpp" "src/Game/TerrainManager.h")
list(APPEND ALLSOURCES "src/Game/Player.cpp" "src/Game/Player.h")
list(APPEND ALLSOURCES "src/Game/Weapon.cpp" "src/Game/Weapon.h")
list(APPEND ALLSOURCES "src/Game/WeaponManager.cpp" "src/Game/WeaponManager.h")
list(APPEND ALLSOURCES "src/Game/Bullet.cpp" "src/Game/Bullet.h")
list(APPEND ALLSOURCES "src/Game/BulletManager.cpp" "src/Game/BulletManager.h")

#Add all the additions to the engine files
list(APPEND ALLSOURCES "src/EngineAdditions/PlayerCollision.cpp" "src/EngineAdditions/PlayerCollision.h")

add_executable (Game ${ALLSOURCES} "src/EngineAdditions/PlayerCollision.h")

add_subdirectory(GEGL2D)

target_link_libraries(Game PUBLIC GEGL2D)

#Copy the shaders into the output directory
file(MAKE_DIRECTORY "${PROJECT_BINARY_DIR}/src/Shaders")

add_custom_target(copyShaders ALL COMMAND ${CMAKE_COMMAND} -E copy_directory 
"${CMAKE_SOURCE_DIR}/src/Shaders" "${CMAKE_BINARY_DIR}/src/Shaders" COMMENT
"Copy shaders to output file" VERBATIM)

add_dependencies(Game copyShaders)

#Copy the assets into the output directory
file(MAKE_DIRECTORY "${PROJECT_BINARY_DIR}/assets")

add_custom_target(copyAssets ALL COMMAND ${CMAKE_COMMAND} -E copy_directory 
"${CMAKE_SOURCE_DIR}/assets" "${CMAKE_BINARY_DIR}/assets" COMMENT
"Copy assets to output file" VERBATIM)

add_dependencies(Game copyAssets)