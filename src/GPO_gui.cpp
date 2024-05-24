//imgui
#include "GpO.h"
#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"

void setupImGui(GLFWwindow* window) {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsDark();

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 130");
}

void renderImGui(int *scene_flag, int *model_flag, int *render_texture, int *color_levels, float *toon_border, vec3 *model_color) {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    // Ventanas ImGui
    ImGui::Begin("App selector");
    ImGui::SeparatorText("Shaders");
    ImGui::RadioButton("Pixel1", scene_flag, PIXEL1); ImGui::SameLine();
    ImGui::RadioButton("Pixel2", scene_flag, PIXEL2);
    if(*model_flag != SPIDER){ //TODO
        ImGui::RadioButton("Toon", scene_flag, TOON); ImGui::SameLine();
        ImGui::RadioButton("Phong", scene_flag, PHONG); ImGui::SameLine();
        ImGui::RadioButton("Blinn-Phong", scene_flag, BLINN);
        //ImGui::RadioButton("Goosh", scene_flag, GOOSH);
    }
    
    ImGui::SeparatorText("Models");
    ImGui::RadioButton("Spiderman", model_flag, SPIDER); ImGui::SameLine();
    ImGui::RadioButton("Fountain-Ball", model_flag, BALL);
    ImGui::RadioButton("Helmet", model_flag, HELMET); ImGui::SameLine();
    ImGui::RadioButton("Cat", model_flag, CAT);

    ImGui::SeparatorText("Options");
    if(*scene_flag != PIXEL1 && *scene_flag != PIXEL2){ // TODO PIXEL
        if(*model_flag != BALL){
            ImGui::Checkbox("Render textures", (bool *)render_texture); // Fountain ball doesn't have texture,
        }
        if(*render_texture == 0){ // color shouldn't be defined with active texture
            float col[3] = {model_color->x,model_color->y,model_color->z};
            ImGui::ColorEdit3("Model color", col);
            model_color->x = col[0];
            model_color->y = col[1];
            model_color->z = col[2];
        }
    } 
        

    if(*scene_flag == TOON){
        ImGui::SeparatorText("Toon shading params");
        ImGui::SliderFloat("Border", toon_border, 0.0f, 0.5f, "ratio = %.05f");
        ImGui::SliderInt("Color levels", color_levels, 3, 7);
    }
        
    ImGui::End();

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void terminateImGui(void) {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}