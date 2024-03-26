#include "SceneHierarchy.h"
#include <imgui_internal.h>

void Ui::draw_scene_entities_controls(std::vector<std::unique_ptr<Entity>>& entities, std::unique_ptr<Camera>& camera)
{
    static bool show_add_model_popup = false;
    ImGui::Begin("Scene Entities");

    if (ImGui::BeginPopupContextWindow()) {
        if (ImGui::MenuItem("Model")) {
            show_add_model_popup = true;
        }
        if (ImGui::MenuItem("CubeMap")) {
            // Handle option 2 selection
        }
        ImGui::EndPopup();
    }

    // Display existing entities
    for (const auto& entity : entities) {
        std::string entityName = entity->get_name();
        if (ImGui::MenuItem(entityName.c_str())) {
            // Handle selection of entity here
        }
    }

    ImGui::End();

    if (show_add_model_popup)
    {
        ImGui::OpenPopup("Add Model Popup");
        if (ImGui::BeginPopupModal("Add Model Popup", &show_add_model_popup))
        {
            static char modelName[64] = "";

            ImGui::Text("Enter model details:");
            ImGui::InputText("Model Name", modelName, sizeof(modelName));

            if (ImGui::Button("Add") && strlen(modelName) > 0) // Check if name field is not empty
            {
                entities.emplace_back(std::make_unique<Model>(Model::WithResultOf([&camera](){
                    return Model::create({ camera, modelName, modelName});
                })));
                
                memset(modelName, 0, sizeof(modelName));
                
                ImGui::CloseCurrentPopup();
                show_add_model_popup = false;
            }

            ImGui::SameLine();
            
            if (ImGui::Button("Close"))
            {
                ImGui::CloseCurrentPopup();
                show_add_model_popup = false;
            }
            
            ImGui::EndPopup();
        }
    }
}
