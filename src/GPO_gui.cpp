// imgui
#include "GpO.h"
#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"

void setupImGui(GLFWwindow *window)
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    (void)io;
    ImGui::StyleColorsDark();

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 130");
}

void renderImGui(int *scene_flag, int *model_flag, int *render_texture, int *color_levels, float *toon_border, bool *pixelArtActive,
                 vec3 *model_color, float *b_lightness, float *y_lightness, float *alpha, float *beta, vec4 *light_coefs,
                 float *rejillaX, float *rejillaY, float *rejillaZ, bool *sombra)
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    ImGuiWindowFlags window_flags = ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoMove;
    ImVec2 display_size = ImGui::GetIO().DisplaySize;

    // First window in top right corner (only first use, can be moved)
    ImGui::SetNextWindowPos(ImVec2(display_size.x, 0), ImGuiCond_Always, ImVec2(1.0f, 0.0f));

    // First window: shader and model selector
    ImGui::Begin("Scenes", NULL, window_flags);
    ImGui::SeparatorText("Shaders");
    ImGui::RadioButton("None", scene_flag, NONE);
    if (*model_flag != JEEP && *model_flag != FOUNTAIN_BALL && *model_flag != SUZANNE)
    {
        ImGui::RadioButton("Texture Pixelation", scene_flag, PIXEL);
    }

    if (*model_flag != SPIDER && *model_flag != HALO) // SPIDER & HALO DOESN'T HAVE NORMALS
    {
        ImGui::RadioButton("Toon", scene_flag, TOON);
        ImGui::SameLine();
        ImGui::RadioButton("Phong", scene_flag, PHONG);
        ImGui::SameLine();
        ImGui::RadioButton("Blinn-Phong", scene_flag, BLINN);
        ImGui::SameLine();
        ImGui::RadioButton("Gooch", scene_flag, GOOCH);
    }

    ImGui::SeparatorText("Models");
    // Textured models
    // Without normal mapping
    if (*scene_flag != TOON && *scene_flag != PHONG && *scene_flag != BLINN && *scene_flag != GOOCH) // No normal mapping for these shaders
    {
        ImGui::RadioButton("Spiderman", model_flag, SPIDER);
        ImGui::SameLine();
        ImGui::RadioButton("Master Chief", model_flag, HALO);
    }

    // With normal mapping
    ImGui::RadioButton("Helmet", model_flag, HELMET);
    ImGui::SameLine();
    ImGui::RadioButton("Cat", model_flag, CAT);
    // Plain models
    if (*scene_flag != PIXEL) // No texture models can't admit texture pixelation
    {
        ImGui::RadioButton("Jeep", model_flag, JEEP);
        ImGui::SameLine();
        ImGui::RadioButton("Fountain-Ball", model_flag, FOUNTAIN_BALL);
        ImGui::SameLine();
        ImGui::RadioButton("Suzanne", model_flag, SUZANNE);
    }
    ImGui::End();

    // Second window pos: top left corner
    ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Always, ImVec2(0.0f, 0.0f));

    // Second window: Shader and texture options
    ImGui::Begin("Scene options", NULL, window_flags);
    if (*render_texture == 0 || (*model_flag == JEEP || *model_flag == FOUNTAIN_BALL || *model_flag == SUZANNE))
    { // color only can be defined with no texture
        float col[3] = {model_color->x, model_color->y, model_color->z};
        ImGui::ColorPicker3("Model color", col, ImGuiColorEditFlags_PickerHueWheel | ImGuiColorEditFlags_NoSidePreview | ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoAlpha);
        ImGui::ColorEdit3("Model color", col);
        model_color->x = col[0];
        model_color->y = col[1];
        model_color->z = col[2];
    }
    if (*model_flag != JEEP && *model_flag != FOUNTAIN_BALL && *model_flag != SUZANNE) // This models don't have texture
    {
        ImGui::Checkbox("Render textures", (bool *)render_texture);
    }

    ImGui::SeparatorText("Shader params");

    if (*scene_flag == PIXEL)
    {
        ImGui::SliderFloat("Pixel size X", rejillaX, 2, 100, "ratio = %.1f");
        ImGui::SliderFloat("Pixel size Y", rejillaY, 2, 100, "ratio = %.1f");
    }

    // PHONG AND BLINN COEFS
    if (*scene_flag == PHONG || *scene_flag == BLINN)
    {
        ImGui::SliderFloat("Static light", &light_coefs->x, 0.0f, 1.f, "ratio = %.05f");
        ImGui::SliderFloat("Diffuse light", &light_coefs->y, 0.0f, 1.f, "ratio = %.05f");
        ImGui::SliderFloat("Spec light", &light_coefs->z, 0.0f, 1.f, "ratio = %.05f");
        ImGui::SliderFloat("Spec apperture", &light_coefs->w, 0.0f, 10.f, "ratio = %.1f");
    }

    // TOON PARAMS
    if (*scene_flag == TOON)
    {
        ImGui::SliderFloat("Border", toon_border, 0.0f, 0.5f, "ratio = %.05f");
        ImGui::SliderInt("Color levels", color_levels, 3, 7);
    }

    // GOOCH PARAMS
    if (*scene_flag == GOOCH)
    {
        ImGui::SliderFloat("B_lightness", b_lightness, 0.0f, 1.f, "ratio = %.05f");
        ImGui::SliderFloat("Y_lightness", y_lightness, 0.0f, 1.f, "ratio = %.05f");
        ImGui::SliderFloat("Alpha", alpha, 0.0f, 1.f, "ratio = %.05f");
        ImGui::SliderFloat("Beta", beta, 0.0f, 1.f, "ratio = %.05f");
    }

    ImGui::SeparatorText("Postprocess params");
    ImGui::Checkbox("Pixel Art", pixelArtActive);
    if (*pixelArtActive)
    {
        ImGui::SliderFloat("Pixel size X", rejillaX, 2, 100, "ratio = %1.0f");
        ImGui::SliderFloat("Pixel size Y", rejillaY, 2, 100, "ratio = %1.0f");
        if (*pixelArtActive)
        {
            ImGui::Checkbox("Sombra", sombra);
            if (*sombra)
            {
                ImGui::SliderFloat("Sombra pixel", rejillaZ, 0, 16, "ratio = %.1f");
            }
        }
    }
    ImGui::End();

    // Third window pos: bottom right corner
    ImGui::SetNextWindowPos(ImVec2(display_size.x, display_size.y), ImGuiCond_Always, ImVec2(1.0f, 1.0f));

    // Third window: Info.
    window_flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav;
    ImGui::Begin("Info", NULL, window_flags);
    if (*scene_flag == TOON || *scene_flag == PHONG || *scene_flag == BLINN || *scene_flag == GOOCH)
        ImGui::Text("<- and -> to change light direction");
    if (*model_flag == HELMET || *model_flag == CAT || *model_flag == JEEP || *model_flag == SUZANNE)
        ImGui::Text("<space> to switch rotation");
    ImGui::Text("https://github.com/ehrlz/non-photorealistic-rendering.git");
    ImGui::End();

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void terminateImGui(void)
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}