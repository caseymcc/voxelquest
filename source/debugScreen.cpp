#include "voxelquest/debugScreen.h"
#include "voxelquest/gamestate.h"

#include <imgui.h>
#include <imgui_impl_opengl3.h>
#include <imgui_impl_glfw.h>

DebugScreen::DebugScreen()
{

}

void DebugScreen::initialize()
{
    // Setup style
    
}

void DebugScreen::terminate()
{

}

void DebugScreen::startBuild()
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}

void DebugScreen::setSize(int width, int height)
{
    m_width=width;
    m_height=height;
}

void DebugScreen::update()
{
    bool show=true;

    ImGui::SetNextWindowPos(ImVec2(0, 0));
    ImGui::SetNextWindowSize(ImVec2(450, 450));
    ImGui::Begin("Info", &show, 
        ImGuiWindowFlags_NoTitleBar|ImGuiWindowFlags_NoBackground|ImGuiWindowFlags_NoBringToFrontOnFocus|ImGuiWindowFlags_NoMove|ImGuiWindowFlags_NoResize);

//    glm::ivec3 regionIndex=world->getRegionIndex(m_renderingOptions->camera.getRegionHash());
//    const glm::vec3 &cameraPos=m_renderingOptions->camera.getPosition();
//    glm::ivec3 chunkIndex=world->getChunkIndex(cameraPos);
//
//    const glm::vec3 &playerPos=m_renderingOptions->player.getPosition();
    
//    ImGui::Text("Camera Pos: Region: %d, %d, %d  Chunk: %d, %d, %d (%0.2f, %0.2f, %0.2f)", 
//        regionIndex.x, regionIndex.y, regionIndex.z, chunkIndex.x, chunkIndex.y, chunkIndex.z, cameraPos.x, cameraPos.y, cameraPos.z);
//    ImGui::Text("Player Pos: Region: %d, %d, %d  Chunk: %d, %d, %d (%0.2f, %0.2f, %0.2f)", 
//        m_renderingOptions->playerRegionIndex.x, m_renderingOptions->playerRegionIndex.y, m_renderingOptions->playerRegionIndex.z, 
//        m_renderingOptions->playerChunkIndex.x, m_renderingOptions->playerChunkIndex.y, m_renderingOptions->playerChunkIndex.z, 
//        playerPos.x, playerPos.y, playerPos.z);
    ImGui::Text("Press \"`\" toggle mouse lock");
    ImGui::Text("Press \"ctrl-s\" Settings");
    ImGui::Text("Press \"ctrl-h\" Debug commands");

    ImGui::Text("\nApplication average %.3f ms/frame (%.1f FPS)", 1000.0f/ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

    ImGui::End();

    if(GameState::helpCommands)
        updateHelp();

    if(GameState::showMap)
        updateMap();

    if(GameState::showSettings)
        updateSettings();
}

void DebugScreen::updateControls()
{
//    bool show=true;
//
//    ImGui::Begin("Controls", &show, ImVec2(150, 400), 0.5f);
//
//    ImGui::Checkbox("Move player", &m_renderingOptions->move_player);
//    if(ImGui::Button("Reset to player"))
//        m_renderingOptions->resetCamera=true;
//    
//    ImGui::Checkbox("Regions", &m_renderingOptions->show_regions);
//    ImGui::Checkbox("Chunks", &m_renderingOptions->show_chunks);
//
//    ImGui::End();
}

void DebugScreen::updateHelp()
{
    bool show=true;

    ImGui::SetNextWindowPos(ImVec2(0.0f, 450.0f));
    ImGui::SetNextWindowSize(ImVec2(300.0f, 400.0f));
    ImGui::SetNextWindowBgAlpha(0.5f);
    ImGui::Begin("Debug Commands", &show, ImGuiWindowFlags_NoResize);

    ImGui::Text("Commands:");
//    ImGui::Text("  ctrl-d : Debug view");
    ImGui::Text("  ctrl-e : Reset camera");
    ImGui::Text("  ctrl-f : Toggle fog");
    ImGui::Text("");
    ImGui::Text("Controls:");
    ImGui::Text("       m : Toggle Map");
    ImGui::Text("       t : Render voxels");

    ImGui::End();

    if(!show)
        GameState::helpCommands=false;

}

void DebugScreen::updateMap()
{
    bool show=true;

    std::string mapFBOName="mapFBO";
    FBOWrapper *mapFBO=&FBOManager::getFBOByName(mapFBOName)->fbos[0];

    ImGui::SetNextWindowPos(ImVec2((float)(m_width-300), 0.0f));
    ImGui::Begin("Map", &show, ImVec2(300.0f, 300.0f));

    ImGui::Image((ImTextureID)mapFBO->color_tex, {(float)mapFBO->width, (float)mapFBO->height}, {0.0f, 0.0f}, {1.0f, 1.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, {1.0f, 1.0f, 1.0f, 0.5f});

    ImGui::End();

    if(!show)
        GameState::showMap=false;
}

void DebugScreen::updateSettings()
{
    bool show=true;

    ImGui::Begin("Settings", &show, ImVec2(300.0f, 400.0f), 0.5f);

    ImGui::SliderInt("Chunk radius", &g_settings.rasterChunkRadius, 10, 100);

    ImGui::End();

    if(!show)
        GameState::showSettings=false;
}

void DebugScreen::build()
{
    ImGui::Render();
}

void DebugScreen::draw()
{
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}