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

set(voxelquest_include
    include/voxelquest/baseobject.h
    include/voxelquest/bullethelpers.h
    include/voxelquest/classdefs.h
    include/voxelquest/constants.h
    include/voxelquest/draw.h
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
    include/voxelquest/helperclasses.h
    include/voxelquest/helperfuncs.h
    include/voxelquest/imageloader.h
    include/voxelquest/includes.h
    include/voxelquest/ivec.h
    include/voxelquest/json.h
    include/voxelquest/mat.h
    include/voxelquest/memorypool.h
    include/voxelquest/myglhelper.h
    include/voxelquest/myshapedrawer.h
#    include/voxelquest/network.h #does not appear to be used
    include/voxelquest/object.h
    include/voxelquest/path.h
#    include/voxelquest/pooledresource.h #does not appear to be used
#    include/voxelquest/poolmanager.h #does not appear to be used
#    include/voxelquest/poolpagemanager.h #does not appear to be used
#    include/voxelquest/rhfactory.h #does not appear to be used
    include/voxelquest/settings.h
    include/voxelquest/shader.h
    include/voxelquest/simplex.h
    include/voxelquest/singleton.h
    include/voxelquest/spacebuffer.h
    include/voxelquest/tbos.h
    include/voxelquest/texenums.h
    include/voxelquest/threadpoolwrapper.h
    include/voxelquest/threadwrapper.h
    include/voxelquest/timer.h
    include/voxelquest/types.h
    include/voxelquest/uicomponent.h
    include/voxelquest/vbos.h
    include/voxelquest/vectors.h
    include/voxelquest/volumewrapper.h
    include/voxelquest/voxelbuffer.h
#    include/voxelquest/wsrequesthandler.h #does not appear to be used
#    include/voxelquest/wsserver.h #does not appear to be used
)

set(voxelquest_source
    source/baseobject.cpp
    source/bullethelpers.cpp
    source/constants.cpp
    source/draw.cpp
    source/dynbuffer.cpp
    source/fbos.cpp
    source/fileio.cpp
    source/fontwrapper.cpp
#    source/gameactor.cpp #does not appear to be used
    source/gameai.cpp
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
    source/helperclasses.cpp
    source/helperfuncs.cpp
    source/imageloader.cpp
    source/includes.cpp
    source/ivec.cpp
    source/json.cpp
    source/mat.cpp
    source/memorypool.cpp
    source/myglhelper.cpp
    source/myshapedrawer.cpp
#    source/network.cpp #does not appear to be used
    source/object.cpp
#    source/pooledresource.cpp #does not appear to be used
#    source/poolmanager.cpp #does not appear to be used
#    source/poolpagemanager.cpp #does not appear to be used
#    source/rhfactory.cpp #does not appear to be used
    source/shader.cpp
    source/simplex.cpp
    source/singleton.cpp
    source/tbos.cpp
    source/threadpoolwrapper.cpp
    source/threadwrapper.cpp
    source/timer.cpp
    source/uicomponent.cpp
    source/vbos.cpp
    source/vectors.cpp
    source/volumewrapper.cpp
    source/voxelbuffer.cpp
#    source/wsrequesthandler.cpp #does not appear to be used
#    source/wsserver.cpp #does not appear to be used
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
    bullet::BulletCollision
    bullet::BulletDynamics
    bullet::BulletSoftBody
    bullet::LinearMath
    glm
)