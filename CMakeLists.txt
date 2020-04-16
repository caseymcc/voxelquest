cmake_minimum_required(VERSION 3.1)

include(${CMAKE_CURRENT_LIST_DIR}/cmake/Hunter/HunterGate.cmake)
HunterGate(
    URL "https://github.com/cpp-pm/hunter/archive/v0.23.253.tar.gz"
    SHA1 "88ea6d37c897a81a080eb9ae0f69d7807bbb3c73"
    LOCAL
)

project(voxelquest)

option(VOXELQUEST_USE_SFML "Use SMFL for audio" OFF)
option(SFML_ROOT_DIRECTORY "Directory for SMFL" "")

hunter_add_package(CreateLaunchers)
find_package(CreateLaunchers CONFIG REQUIRED)

hunter_add_package(glbinding)
find_package(glbinding CONFIG REQUIRED)

hunter_add_package(glfw)
find_package(glfw3 REQUIRED)

hunter_add_package(imgui)
find_package(imgui CONFIG REQUIRED)

hunter_add_package(glm)
find_package(glm CONFIG REQUIRED)

hunter_add_package(Boost COMPONENTS system filesystem)
find_package(Boost COMPONENTS system filesystem REQUIRED)

hunter_add_package(PocoCpp)
find_package(Poco REQUIRED Foundation Util Net CONFIG)

hunter_add_package(PNG)
find_package(PNG CONFIG REQUIRED)

hunter_add_package(bullet)
find_package(bullet CONFIG REQUIRED)

#audio replacement
#hunter_add_package(stb)
#find_package(stb CONFIG REQUIRED)
#
#hunter_add_package(OpenAL)
#find_package(OpenAL CONFIG REQUIRED)

set(voxelquest_audio
    include/voxelquest/gameaudio.h
    source/gameaudio.cpp
    include/voxelquest/gamemusic.h
    source/gamemusic.cpp
    include/voxelquest/gamesound.h
    source/gamesound.cpp
)
source_group("audio" FILES ${voxelquest_audio})

set(voxelquest_entity
    include/voxelquest/gameent.h
    source/gameent.cpp
    include/voxelquest/gameentmanager.h
    source/gameentmanager.cpp
    include/voxelquest/gameplant.h
    source/gameplant.cpp
    include/voxelquest/gameplantnode.h
    source/gameplantnode.cpp
)
source_group("entity" FILES ${voxelquest_entity})

set(voxelquest_fileio
    include/voxelquest/fileio.h
    source/fileio.cpp
    include/voxelquest/imageloader.h
    source/imageloader.cpp
    include/voxelquest/json.h
    source/json.cpp
    include/voxelquest/jsonhelpers.h
    source/jsonhelpers.cpp
    include/voxelquest/spacebuffer.h
)
source_group("fileio" FILES ${voxelquest_fileio})

set(voxelquest_math
    include/voxelquest/bullethelpers.h
    source/bullethelpers.cpp
    include/voxelquest/ivec.h
    source/ivec.cpp
    include/voxelquest/mat.h
    source/mat.cpp
    include/voxelquest/vectors.h
    source/vectors.cpp
)
source_group("math" FILES ${voxelquest_math})

set(voxelquest_rendering
    include/voxelquest/dynbuffer.h
    source/dynbuffer.cpp
    include/voxelquest/fbos.h
    source/fbos.cpp
    include/voxelquest/glinfo.h
    source/glinfo.cpp
    include/voxelquest/materials.h
    source/materials.cpp
    include/voxelquest/myglhelper.h
    source/myglhelper.cpp
    include/voxelquest/myoglapp.h
    source/myoglapp.cpp
    include/voxelquest/myshapedrawer.h
    source/myshapedrawer.cpp
    include/voxelquest/oglrenderer.h
    source/oglrenderer.cpp
    include/voxelquest/raycastbar.h
    source/raycastbar.cpp
    include/voxelquest/renderer.h
    source/renderer.cpp
    include/voxelquest/shader.h
    source/shader.cpp
    include/voxelquest/tbos.h
    source/tbos.cpp
    include/voxelquest/texenums.h
    include/voxelquest/texture3d.h
    source/texture3d.cpp
    include/voxelquest/uniformbuffer.h
    source/uniformbuffer.cpp
    include/voxelquest/vbos.h
    source/vbos.cpp
    include/OpenGLWindow/SimpleCamera.cpp
)
source_group("rendering" FILES ${voxelquest_rendering})

set(voxelquest_ui
    include/voxelquest/debugScreen.h
    source/debugScreen.cpp
    include/voxelquest/gamegui.h
    source/gamegui.cpp
    include/voxelquest/initGlew.h
    source/initGlew.cpp
    include/voxelquest/stylesheet.h
    source/stylesheet.cpp
    include/voxelquest/uicomponent.h
    source/uicomponent.cpp
)
source_group("ui" FILES ${voxelquest_ui})

set(voxelquest_utils
    include/voxelquest/helperfuncs.h
    source/helperfuncs.cpp
    include/voxelquest/hpclock.h
    source/hpclock.cpp
    include/voxelquest/timer.h
    source/timer.cpp
)
source_group("utils" FILES ${voxelquest_utils})

set(voxelquest_source
    include/voxelquest/baseobject.h
    source/baseobject.cpp
    include/voxelquest/benchmarkdemo.h
    source/benchmarkdemo.cpp
    include/voxelquest/cache.h
    source/cache.cpp
    include/voxelquest/classdefs.h
    include/voxelquest/constants.h
    source/constants.cpp
    include/voxelquest/entenums.h
    include/voxelquest/enums.h
    include/voxelquest/fontwrapper.h
    source/fontwrapper.cpp
    include/voxelquest/gameai.h
    source/gameai.cpp
    include/voxelquest/gameblock.h
    source/gameblock.cpp
    include/voxelquest/gamechunk.h
    source/gamechunk.cpp
    include/voxelquest/gamefluid.h
    source/gamefluid.cpp
    include/voxelquest/gamelogic.h
    source/gamelogic.cpp
    include/voxelquest/gamenetwork.h
    source/gamenetwork.cpp
    include/voxelquest/gameoctree.h
    source/gameoctree.cpp
    include/voxelquest/gameorg.h
    source/gameorg.cpp
    include/voxelquest/gameorgnode.h
    source/gameorgnode.cpp
    include/voxelquest/gamepageholder.h
    source/gamepageholder.cpp
    include/voxelquest/gamephysics.h
    source/gamephysics.cpp
    include/voxelquest/gamephysrig.h
    source/gamephysrig.cpp
    include/voxelquest/gamesim.h
    include/voxelquest/gamestate.h
    source/gamestate.cpp
    include/voxelquest/gamevoxelwrap.h
    source/gamevoxelwrap.cpp
    include/voxelquest/gameworld.h
    source/gameworld.cpp
    include/voxelquest/geom.h
    source/geom.cpp
    include/voxelquest/geompage.h
    source/main.cpp
    include/voxelquest/memorypool.h
    source/memorypool.cpp
    include/voxelquest/mydebugdrawer.h
    source/mydebugdrawer.cpp
    include/voxelquest/object.h
    source/object.cpp
    include/voxelquest/path.h
    include/voxelquest/rig.h
    include/voxelquest/settings.h
    source/settings.cpp
    include/voxelquest/simplex.h
    source/simplex.cpp
    include/voxelquest/threadpoolwrapper.h
    source/threadpoolwrapper.cpp
    include/voxelquest/threadwrapper.h
    source/threadwrapper.cpp
    include/voxelquest/types.h
    include/voxelquest/volumewrapper.h
    source/volumewrapper.cpp
    include/voxelquest/voxelbuffer.h
    source/voxelbuffer.cpp
)
source_group("source" FILES ${voxelquest_source})

set(voxelquest_shaders
    glsl/BasicLimbShader.c
    glsl/BasicPrimShader.c
    glsl/BillboardShader.c
    glsl/BlitPointShader.c
    glsl/BlitShader.c
    glsl/BlurShader.c
    glsl/BoxShader.c
    glsl/Combine3Shader.c
    glsl/CombineShader.c
    glsl/CopyShader.c
    glsl/CopyShader2.c
    glsl/CopyShader3.c
    glsl/CylBBShader.c
    glsl/DilateShader.c
    glsl/DownSampleShader.c
    glsl/DownScaleShader.c
    glsl/FogShader.c
    glsl/FXAAShader.c
    glsl/GeomShader.c
    glsl/GridShader.c
    glsl/GUIShader.c
    glsl/HDRShader.c
    glsl/HolderShader.c
    glsl/HolderShaderOld.c
    glsl/LightShader.c
    glsl/MapBorderShader.c
    glsl/MapLand.c
    glsl/MedianShader.c
    glsl/MergeShader.c
    glsl/NearestShader.c
    glsl/NoiseShader.c
    glsl/OctShader.c
    glsl/PointShader.c
    glsl/PolyCombineShader.c
    glsl/PolyShader.c
    glsl/PostLightingShader.c
    glsl/PreLightingShader.c
    glsl/PrimShader.c
    glsl/PrimShader2.c
    glsl/PrimShader3.c
    glsl/RadiosityCombineShader.c
    glsl/RadiosityShader.c
    glsl/RasterShader.c
    glsl/RayFuncs.c
    glsl/RenderFBOShader.c
    glsl/RoadShader.c
    glsl/SampleFuncs.c
    glsl/ShadowMapShader.c
    glsl/Simplex2D.c
    glsl/SkeletonShader.c
    glsl/SolidCombineShader.c
    glsl/SphereShader.c
    glsl/TerGenShader.c
    glsl/TerHeightFunc.c
    glsl/TerrainMix.c
    glsl/TopoShader.c
    glsl/VBOShader.c
    glsl/WaterShader.c
    glsl/WaterShaderCombine.c
    glsl/WaveFuncs.c
    glsl/WaveHeightShader.c
    glsl/WorldSpaceShader.c
)
source_group("shaders" FILES ${voxelquest_shaders})
set_source_files_properties(${voxelquest_shaders} PROPERTIES HEADER_FILE_ONLY TRUE)

set(voxelquest_libraries
    glbinding::glbinding
    Boost::system 
    Boost::filesystem 
    Poco::Foundation
    Poco::Util
    Poco::Net
    OpenGL::GL
    glfw
    PNG::png
    bullet::Bullet3Common
    bullet::BulletCollision
    bullet::BulletDynamics
    bullet::BulletSoftBody
    bullet::LinearMath
    glm
    imgui::imgui
#    stb::stb
#    OpenAL::OpenAL
)

include_directories("${CMAKE_CURRENT_LIST_DIR}/include")
include_directories("${SFML_ROOT_DIRECTORY}/include")

add_executable(voxelquest 
    ${voxelquest_audio}
    ${voxelquest_entity}
    ${voxelquest_fileio}
    ${voxelquest_math}
    ${voxelquest_rendering}
    ${voxelquest_ui}
    ${voxelquest_utils}
    ${voxelquest_source}
    ${voxelquest_shaders}
)

find_package(OpenGL REQUIRED)

target_link_libraries(voxelquest 
    ${voxelquest_libraries}
)

if(VOXELQUEST_USE_SFML)
    target_link_libraries(voxelquest 
        "${SFML_ROOT_DIRECTORY}/build/lib/Debug/sfml-audio-d.lib"
    )
    target_compile_definitions(voxelquest PUBLIC VOXELQUEST_USE_SFML=1)
endif()

include(CreateLaunchers)
include(${CMAKE_CURRENT_LIST_DIR}/cmake/targetInfo.cmake)

set(vs_enviroment_dir)
get_target_link_directories(vs_enviroment_dir ${voxelquest_libraries})

create_target_launcher(voxelquest
    RUNTIME_LIBRARY_DIRS "${vs_enviroment_dir}" "${SFML_ROOT_DIRECTORY}/build/lib/Debug" "${SFML_ROOT_DIRECTORY}/extlibs/bin/x64"
    WORKING_DIRECTORY "${CMAKE_CURRENT_LIST_DIR}"
)