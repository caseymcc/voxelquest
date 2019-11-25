cmake_minimum_required(VERSION 3.1)

include(${CMAKE_CURRENT_LIST_DIR}/cmake/Hunter/HunterGate.cmake)
HunterGate(
    URL "https://github.com/ruslo/hunter/archive/v0.23.204.tar.gz"
    SHA1 "32cfed254da901f6f184027d530d8da47e035b85"
    LOCAL
)

project(voxelquest)

hunter_add_package(glbinding)
find_package(glbinding CONFIG REQUIRED)

hunter_add_package(glfw)
find_package(glfw3 REQUIRED)

#set(Boost_INCLUDE_DIR /usr/local/boost_1_53_0)
#set(Boost_LIBRARY_DIR /usr/local/boost_1_53_0/stage/lib)
hunter_add_package(Boost COMPONENTS system filesystem)
find_package(Boost COMPONENTS system filesystem REQUIRED)
#include_directories(${Boost_INCLUDE_DIR})
#link_directories(${Boost_LIBRARY_DIR})

hunter_add_package(PocoCpp)
find_package(Poco REQUIRED Foundation Util Net CONFIG)

#find_library(POCO_FOUNDATION PocoFoundation REQUIRED)
#find_library(POCO_NET PocoNet REQUIRED)
#find_library(POCO_UTIL PocoUtil REQUIRED)

#include_directories ("${PROJECT_SOURCE_DIR}/CompiledSource")
#add_subdirectory (CompiledSource)

hunter_add_package(PNG)
find_package(PNG CONFIG REQUIRED)

set(voxelquest_include
    include/classdefs.h
    include/dynbuffer.h
    include/enums.h
    include/fbos.h
    include/fontwrapper.h
    include/gameactor.h
    include/gameactorjoint.h
    include/gameai.h
    include/gameblock.h
    include/gamecamera.h
    include/gamechunk.h
    include/gameent.h
    include/gameentmanager.h
    include/gameentnode.h
    include/gamefluid.h
    include/gamegeom.h
    include/gamegui.h
    include/gamelogic.h
    include/gamemusic.h
    include/gamenetwork.h
    include/gameoctree.h
    include/gameorg.h
    include/gameorgnode.h
    include/gamepage.h
    include/gamepageholder.h
    include/gamephysics.h
    include/gamephysrig.h
    include/gameplant.h
    include/gameplantnode.h
    include/gameragdoll.h
    include/gamesim.h
    include/gamesound.h
    include/gamevoxelwrap.h
    include/gameworld.h
    include/geompage.h
    include/helperclasses.h
    include/helperfuncs.h
    include/imageloader.h
    include/includes.h
    include/ivec.h
    include/json.h
    include/mat.h
    include/myglhelper.h
    include/myshapedrawer.h
    include/network.h
    include/pooledresource.h
    include/poolmanager.h
    include/poolpagemanager.h
    include/rhfactory.h
    include/shader.h
    include/simplex.h
    include/singleton.h
    include/threadpoolwrapper.h
    include/timer.h
    include/uicomponent.h
    include/vectors.h
    include/volumewrapper.h
    include/wsrequesthandler.h
    include/wsserver.h
)

set(voxelquest_source
    source/dynbuffer.cpp
    source/fbos.cpp
    source/fontwrapper.cpp
    source/gameactor.cpp
    source/gameai.cpp
    source/gameblock.cpp
    source/gamecamera.cpp
    source/gamechunk.cpp
    source/gameent.cpp
    source/gameentmanager.cpp
    source/gameentnode.cpp
    source/gamefluid.cpp
    source/gamegeom.cpp
    source/gamegui.cpp
    source/gamelogic.cpp
    source/gamemusic.cpp
    source/gamenetwork.cpp
    source/gameoctree.cpp
    source/gameorg.cpp
    source/gameorgnode.cpp
    source/gamepage.cpp
    source/gamepageholder.cpp
    source/gamephysics.cpp
    source/gamephysrig.cpp
    source/gameplant.cpp
    source/gameplantnode.cpp
    source/gameragdoll.cpp
    source/gamesound.cpp
    source/gamevoxelwrap.cpp
    source/gameworld.cpp
    source/helperclasses.cpp
    source/helperfuncs.cpp
    source/imageloader.cpp
    source/includes.cpp
    source/ivec.cpp
    source/json.cpp
    source/mat.cpp
    source/myglhelper.cpp
    source/myshapedrawer.cpp
    source/network.cpp
    source/pooledresource.cpp
    source/poolmanager.cpp
    source/poolpagemanager.cpp
    source/rhfactory.cpp
    source/shader.cpp
    source/simplex.cpp
    source/singleton.cpp
    source/threadpoolwrapper.cpp
    source/timer.cpp
    source/uicomponent.cpp
    source/vectors.cpp
    source/volumewrapper.cpp
    source/wsrequesthandler.cpp
    source/wsserver.cpp
)

include_directories("${CMAKE_CURRENT_LIST_DIR}/include")
add_executable(voxelquest ${voxelquest_include} ${voxelquest_source})
find_package(OpenGL REQUIRED)

target_link_libraries(voxelquest 
    glbinding::glbinding
    Boost::system 
    Boost::filesystem 
    Poco::Foundation
    Poco::Util
    Poco::Net
    OpenGL::GL
    glfw
    PNG::png
)