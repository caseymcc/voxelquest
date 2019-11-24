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

set(voxelquest_include
    src/cpp/f00020_includes.h
    src/cpp/f00022_enums.h
    src/cpp/f00024_mat.h
    src/cpp/f00025_includes.h
    src/cpp/f00025_ivec.h
    src/cpp/f00026_enums.h
    src/cpp/f00026_vectors.h
    src/cpp/f00027_mat.h
    src/cpp/f00027_vectors.h
    src/cpp/f00028_gameent.h
    src/cpp/f00028_vectors.h
    src/cpp/f00029_gameent.h
    src/cpp/f00030_json.h
    src/cpp/f00040_png.h
    src/cpp/f00050_enums.h
    src/cpp/f00215_helperfuncs.h
    src/cpp/f00217_helperclasses.h
    src/cpp/f00220_timer.h
    src/cpp/f00230_network.h
    src/cpp/f00240_simplex.h
    src/cpp/f00250_imageloader.h
    src/cpp/f00251_fbos.h
    src/cpp/f00260_classdefs.h
    src/cpp/f00290_wsrequesthandler.hpp
    src/cpp/f00291_rhfactory.hpp
    src/cpp/f00292_wsserver.hpp
    src/cpp/f00293_gamesound.hpp
    src/cpp/f00294_gamemusic.hpp
    src/cpp/f00295_gamegeom.hpp
    src/cpp/f00296_gameactor.hpp
    src/cpp/f00297_gamecamera.hpp
    src/cpp/f00297_gameent.hpp
    src/cpp/f00300_singleton.hpp
    src/cpp/f00305_shader.hpp
    src/cpp/f00307_dynbuffer.hpp
    src/cpp/f00310_pooledresource.hpp
    src/cpp/f00315_poolmanager.hpp
    src/cpp/f00316_poolpagemanager.hpp
    src/cpp/f00320_fontwrapper.hpp
    src/cpp/f00323_gameoctree.hpp
    src/cpp/f00324_gamevoxelwrap.hpp
    src/cpp/f00325_uicomponent.hpp
    src/cpp/f00330_gamegui.hpp
    src/cpp/f00332_gamenetwork.hpp
    src/cpp/f00335_gameai.hpp
    src/cpp/f00337_gamefluid.hpp
    src/cpp/f00338_gameentnode.hpp
    src/cpp/f00338_gameorgnode.hpp
    src/cpp/f00339_gameent.hpp
    src/cpp/f00339_gameorg.hpp
    src/cpp/f00340_gameplantnode.hpp
    src/cpp/f00341_gameplant.hpp
    src/cpp/f00345_gameragdoll.hpp
    src/cpp/f00345_geompage.hpp
    src/cpp/f00346_gameactor.hpp
    src/cpp/f00346_gameactorjoint.hpp
    src/cpp/f00347_gameactor.hpp
    src/cpp/f00347_gamephysrig.hpp
    src/cpp/f00349_gameentmanager.hpp
    src/cpp/f00350_gamepage.hpp
    src/cpp/f00351_gamepageholder.hpp
    src/cpp/f00352_gameblock.hpp
    src/cpp/f00352_gamechunk.hpp
    src/cpp/f00353_gameblock.hpp
    src/cpp/f00355_volumewrapper.hpp
    src/cpp/f00360_threadpoolwrapper.hpp
    src/cpp/f00370_gamelogic.hpp
    src/cpp/f00373_myshapedrawer.hpp
    src/cpp/f00374_myglhelper.hpp
    src/cpp/f00375_gamephysics.hpp
    src/cpp/f00377_gamesim.hpp
    src/cpp/f00380_gameworld.hpp
    src/cpp/f00500_mainfuncs.h
)

set(voxelquest_source
    src/f_0010_includes.cpp
    src/f_0011_json.cpp
    src/f_0012_vectors.cpp
    src/f_0013_enums.cpp
    src/f_0015_helperfuncs.cpp
    src/f_0017_helperclasses.cpp
    src/f_0020_timer.cpp
    src/f_0030_network.cpp
    src/f_0040_simplex.cpp
    src/f_0050_imageloader.cpp
    src/f_0051_fbos.cpp
    src/f_0052_singleton.cpp
    src/f_0055_predefs.cpp
    src/f_0056_fpage3d.cpp
    src/f_0057_fpagemanager.cpp
    src/f_0058_gameui.cpp
    src/f_0060_main.cpp
)

add_executable(voxelquest_server ${voxelquest_include} ${voxelquest_source})
target_link_libraries(voxelquest_server 
    glbinding::glbinding
    Boost::system 
    Boost::filesystem 
    Poco::Foundation
    Poco::Util
    Poco::Net
    OpenGL::GL
    glfw
)

add_executable(voxelquest ${PROJECT_SOURCE_DIR}/CompiledSource/main.cpp)
find_package(OpenGL REQUIRED)
#find_package(GLUT REQUIRED)
#include_directories( ${OPENGL_INCLUDE_DIRS}  ${GLUT_INCLUDE_DIRS} )

#target_link_libraries(voxelquest ${Boost_LIBRARIES} ${POCO_FOUNDATION} ${POCO_NET} ${POCO_UTIL} ${OPENGL_LIBRARIES} ${GLUT_LIBRARY} )
target_link_libraries(voxelquest 
    glbinding::glbinding
    Boost::system 
    Boost::filesystem 
    Poco::Foundation
    Poco::Util
    Poco::Net
    OpenGL::GL
    glfw
)