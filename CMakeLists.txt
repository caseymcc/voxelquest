cmake_minimum_required(VERSION 3.1)

include(${CMAKE_CURRENT_LIST_DIR}/cmake/Hunter/HunterGate.cmake)
HunterGate(
    URL "https://github.com/ruslo/hunter/archive/v0.23.204.tar.gz"
    SHA1 "32cfed254da901f6f184027d530d8da47e035b85"
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

set(voxelquest_include
    include/voxelquest/baseobject.h
    include/voxelquest/benchmarkdemo.h
    include/voxelquest/bullethelpers.h
    include/voxelquest/cache.h
    include/voxelquest/classdefs.h
    include/voxelquest/constants.h
    include/voxelquest/dynbuffer.h
    include/voxelquest/entenums.h
    include/voxelquest/enums.h
    include/voxelquest/fbos.h
    include/voxelquest/fileio.h
    include/voxelquest/fontwrapper.h
#    include/voxelquest/gameactor.h #does not appear to be used
#    include/voxelquest/gameactorjoint.h #does not appear to be used
    include/voxelquest/gameai.h
    include/voxelquest/gameaudio.h
    include/voxelquest/gameblock.h
#    include/voxelquest/gamecamera.h #does not appear to be used
    include/voxelquest/gamechunk.h
    include/voxelquest/gameent.h
    include/voxelquest/gameentmanager.h
#    include/voxelquest/gameentnode.h #does not appear to be used
    include/voxelquest/gamefluid.h
#    include/voxelquest/gamegeom.h #does not appear to be used
    include/voxelquest/gamegui.h
    include/voxelquest/gamelogic.h
    include/voxelquest/gamemusic.h
    include/voxelquest/gamenetwork.h
    include/voxelquest/gameoctree.h
    include/voxelquest/gameorg.h
    include/voxelquest/gameorgnode.h
#    include/voxelquest/gamepage.h #does not appear to be used
    include/voxelquest/gamepageholder.h
    include/voxelquest/gamephysics.h
    include/voxelquest/gamephysrig.h
    include/voxelquest/gameplant.h
    include/voxelquest/gameplantnode.h
#    include/voxelquest/gameragdoll.h #does not appear to be used
    include/voxelquest/gamesim.h
    include/voxelquest/gamesound.h
    include/voxelquest/gamestate.h
    include/voxelquest/gamevoxelwrap.h
    include/voxelquest/gameworld.h
    include/voxelquest/geom.h
    include/voxelquest/geompage.h
    include/voxelquest/glinfo.h
    include/voxelquest/helperfuncs.h
    include/voxelquest/hpclock.h
    include/voxelquest/imageloader.h
#    include/voxelquest/includes.h #removed
    include/voxelquest/ivec.h
    include/voxelquest/json.h
	include/voxelquest/jsonhelpers.h
    include/voxelquest/mat.h
    include/voxelquest/materials.h
    include/voxelquest/memorypool.h
    include/voxelquest/mydebugdrawer.h
    include/voxelquest/myglhelper.h
	include/voxelquest/myoglapp.h
    include/voxelquest/myshapedrawer.h
#    include/voxelquest/network.h #does not appear to be used
    include/voxelquest/object.h
	include/voxelquest/oglrenderer.h
    include/voxelquest/path.h
#    include/voxelquest/pooledresource.h #does not appear to be used
#    include/voxelquest/poolmanager.h #does not appear to be used
#    include/voxelquest/poolpagemanager.h #does not appear to be used
    include/voxelquest/raycastbar.h
	include/voxelquest/renderer.h
#    include/voxelquest/rhfactory.h #does not appear to be used
    include/voxelquest/rig.h
    include/voxelquest/settings.h
    include/voxelquest/shader.h
    include/voxelquest/simplex.h
#    include/voxelquest/singleton.h #removed
	include/voxelquest/stylesheet.h
    include/voxelquest/spacebuffer.h
    include/voxelquest/tbos.h
    include/voxelquest/texenums.h
    include/voxelquest/texture3d.h
    include/voxelquest/threadpoolwrapper.h
    include/voxelquest/threadwrapper.h
    include/voxelquest/timer.h
    include/voxelquest/types.h
    include/voxelquest/uicomponent.h
	include/voxelquest/uniformbuffer.h
    include/voxelquest/vbos.h
    include/voxelquest/vectors.h
    include/voxelquest/volumewrapper.h
    include/voxelquest/voxelbuffer.h
#    include/voxelquest/wsrequesthandler.h #does not appear to be used
#    include/voxelquest/wsserver.h #does not appear to be used
)

set(voxelquest_source
    source/baseobject.cpp
    source/benchmarkdemo.cpp
    source/bullethelpers.cpp
    source/cache.cpp
    source/constants.cpp
    source/dynbuffer.cpp
    source/fbos.cpp
    source/fileio.cpp
    source/fontwrapper.cpp
#    source/gameactor.cpp #does not appear to be used
    source/gameai.cpp
	source/gameaudio.cpp
    source/gameblock.cpp
#    source/gamecamera.cpp #does not appear to be used
    source/gamechunk.cpp
    source/gameent.cpp
    source/gameentmanager.cpp
#    source/gameentnode.cpp #does not appear to be used
    source/gamefluid.cpp
#    source/gamegeom.cpp #does not appear to be used
    source/gamegui.cpp
    source/gamelogic.cpp
    source/gamemusic.cpp
    source/gamenetwork.cpp
    source/gameoctree.cpp
    source/gameorg.cpp
    source/gameorgnode.cpp
#    source/gamepage.cpp #does not appear to be used
    source/gamepageholder.cpp
    source/gamephysics.cpp
    source/gamephysrig.cpp
    source/gameplant.cpp
    source/gameplantnode.cpp
#    source/gameragdoll.cpp #does not appear to be used
    source/gamesound.cpp
    source/gamestate.cpp
    source/gamevoxelwrap.cpp
    source/gameworld.cpp
    source/geom.cpp
    source/glinfo.cpp
    source/helperfuncs.cpp
    source/hpclock.cpp
    source/imageloader.cpp
#    source/includes.cpp #removed
    source/ivec.cpp
    source/json.cpp
	source/jsonhelpers.cpp
    source/main.cpp
    source/mat.cpp
    source/materials.cpp
    source/memorypool.cpp
    source/mydebugdrawer.cpp
    source/myglhelper.cpp
    source/myoglapp.cpp
    source/myshapedrawer.cpp
#    source/network.cpp #does not appear to be used
    source/object.cpp
	source/oglrenderer.cpp
#    source/pooledresource.cpp #does not appear to be used
#    source/poolmanager.cpp #does not appear to be used
#    source/poolpagemanager.cpp #does not appear to be used
    source/raycastbar.cpp
	source/renderer.cpp
#    source/rhfactory.cpp #does not appear to be used
	source/settings.cpp
    source/shader.cpp
    source/simplex.cpp
#    source/singleton.cpp #removed
	source/stylesheet.cpp
    source/tbos.cpp
    source/texture3d.cpp
    source/threadpoolwrapper.cpp
    source/threadwrapper.cpp
    source/timer.cpp
	source/uniformbuffer.cpp
    source/uicomponent.cpp
    source/vbos.cpp
    source/vectors.cpp
    source/volumewrapper.cpp
    source/voxelbuffer.cpp
#    source/wsrequesthandler.cpp #does not appear to be used
#    source/wsserver.cpp #does not appear to be used

    include/OpenGLWindow/SimpleCamera.cpp
)

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
set_source_files_properties(${voxelquest_shaders} PROPERTIES HEADER_FILE_ONLY TRUE)
source_group("shaders" FILES ${voxelquest_shaders})

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
#    stb::stb
#    OpenAL::OpenAL
)

include_directories("${CMAKE_CURRENT_LIST_DIR}/include")
include_directories("${SFML_ROOT_DIRECTORY}/include")

add_executable(voxelquest ${voxelquest_include} ${voxelquest_source} ${voxelquest_shaders})

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