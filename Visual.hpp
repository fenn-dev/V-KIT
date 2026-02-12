// ImGui & GLFW Headers
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <GLFW/glfw3.h>

void RunVisualMode(Tokenizer& app, std::string initial_src) {
    if (!glfwInit()) return;

    GLFWwindow* window = glfwCreateWindow(1400, 900, "V-KIT | Vulpine Tree Inspector", nullptr, nullptr);
    if (!window) {
        glfwTerminate();
        return;
    }
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); 

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 130");

    ApplyVulpineStyle();

    char buffer[4096];
    initial_src.copy(buffer, sizeof(buffer) - 1);
    buffer[initial_src.length()] = '\0';

    std::vector<Token> cached_tokens = app.Tokenize(buffer);

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        ImGuiIO& io = ImGui::GetIO();
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        ImGui::DockSpaceOverViewport(0, ImGui::GetMainViewport());

        // 1. Source Editor
        ImGui::Begin("Vulpine Source Editor");
        if (ImGui::InputTextMultiline("##source", buffer, sizeof(buffer), ImVec2(-FLT_MIN, -50))) {
            // Optional: Live update could go here, but we'll stick to the button for safety
        }
        if (ImGui::Button("Tokenize Source", ImVec2(-FLT_MIN, 30))) {
            cached_tokens = app.Tokenize(buffer); 
        }
        ImGui::End();

        // 2. Tree Inspector
        ImGui::Begin("V-KIT Tree Inspector");
        
        std::string TreeRootName = "Module: root (" + std::to_string(cached_tokens.size()) + ")";

        if (ImGui::TreeNodeEx(TreeRootName.c_str(), ImGuiTreeNodeFlags_DefaultOpen)) {
            for (size_t i = 0; i < cached_tokens.size(); ++i) {
                auto& t = cached_tokens[i];
                
                // Determine color based on type
                ImVec4 textColor = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
                if (t.type == TokenType::MOVE) textColor = ImVec4(1.0f, 0.3f, 0.3f, 1.0f);
                else if (t.type == TokenType::IDENTIFIER) textColor = ImVec4(0.5f, 1.0f, 0.5f, 1.0f);
                else if ((int)t.type <= 10) textColor = ImVec4(0.3f, 0.7f, 1.0f, 1.0f);

                ImGui::PushStyleColor(ImGuiCol_Text, textColor);
                
                // Format the tree node label
                std::string node_name = "Token[" + std::to_string(i) + "]: " + t.lexeme;
                
                if (ImGui::TreeNode(node_name.c_str())) {
                    ImGui::PopStyleColor(); // Return to normal color for details
                    
                    ImGui::BulletText("Type:   %d", (int)t.type);
                    ImGui::BulletText("Pos:    Line %zu, Col %zu", t.row, t.column);
                    
                    // Specific logic for types
                    if (t.type == TokenType::IDENTIFIER) {
                        ImGui::TextColored(ImVec4(0.8f, 0.8f, 0.0f, 1.0f), "Action: Symbol Lookup required");
                    }

                    ImGui::TreePop();
                } else {
                    ImGui::PopStyleColor();
                }
            }
            ImGui::TreePop();
        }
        ImGui::End();

        // Rendering
        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(0.08f, 0.08f, 0.09f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        glfwSwapBuffers(window);
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwDestroyWindow(window);
    glfwTerminate();
}

void ApplyVulpineStyle() {
    auto& style = ImGui::GetStyle();

    // --- Layout & Spacing ---
    style.WindowPadding     = ImVec2(10, 10);
    style.FramePadding      = ImVec2(8, 6);
    style.ItemSpacing       = ImVec2(10, 8);
    style.IndentSpacing     = 25.0f;
    style.ScrollbarSize     = 15.0f;
    style.GrabMinSize       = 10.0f;

    // --- Rounding (Modern soft edges) ---
    style.WindowRounding    = 6.0f;
    style.ChildRounding     = 4.0f;
    style.FrameRounding     = 4.0f;
    style.PopupRounding     = 4.0f;
    style.TabRounding       = 4.0f;
    style.GrabRounding      = 4.0f;

    // --- Borders ---
    style.WindowBorderSize  = 1.0f;
    style.FrameBorderSize   = 0.0f; // Flat design
    style.PopupBorderSize   = 1.0f;

    // --- Vulpine Palette ---
    ImVec4* colors = style.Colors;
    
    // Backgrounds
    colors[ImGuiCol_WindowBg]             = ImVec4(0.10f, 0.10f, 0.12f, 1.00f); // Main deep charcoal
    colors[ImGuiCol_ChildBg]              = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    colors[ImGuiCol_PopupBg]              = ImVec4(0.08f, 0.08f, 0.10f, 0.94f);
    colors[ImGuiCol_Border]               = ImVec4(0.20f, 0.20f, 0.22f, 1.00f); // Subtle separators
    
    // Frames (Inputs, Checkboxes, etc)
    colors[ImGuiCol_FrameBg]              = ImVec4(0.16f, 0.16f, 0.18f, 1.00f);
    colors[ImGuiCol_FrameBgHovered]       = ImVec4(0.24f, 0.24f, 0.28f, 1.00f);
    colors[ImGuiCol_FrameBgActive]        = ImVec4(0.28f, 0.28f, 0.32f, 1.00f);
    
    // Tabs (The Docking UI)
    colors[ImGuiCol_Tab]                  = ImVec4(0.12f, 0.12f, 0.14f, 1.00f);
    colors[ImGuiCol_TabHovered]           = ImVec4(0.92f, 0.45f, 0.20f, 0.80f); // Vulpine Orange Tint
    colors[ImGuiCol_TabActive]            = ImVec4(0.92f, 0.45f, 0.20f, 1.00f); // Pure Vulpine Orange
    colors[ImGuiCol_TabUnfocused]         = ImVec4(0.08f, 0.08f, 0.09f, 1.00f);
    colors[ImGuiCol_TabUnfocusedActive]   = ImVec4(0.14f, 0.14f, 0.16f, 1.00f);

    // Headers (Tree Nodes)
    colors[ImGuiCol_Header]               = ImVec4(0.92f, 0.45f, 0.20f, 0.30f); // Orange highlight
    colors[ImGuiCol_HeaderHovered]        = ImVec4(0.92f, 0.45f, 0.20f, 0.50f);
    colors[ImGuiCol_HeaderActive]         = ImVec4(0.92f, 0.45f, 0.20f, 0.70f);

    // Buttons
    colors[ImGuiCol_Button]               = ImVec4(0.20f, 0.20f, 0.24f, 1.00f);
    colors[ImGuiCol_ButtonHovered]        = ImVec4(0.28f, 0.28f, 0.32f, 1.00f);
    colors[ImGuiCol_ButtonActive]         = ImVec4(0.92f, 0.45f, 0.20f, 1.00f);

    // Title Bars
    colors[ImGuiCol_TitleBg]              = ImVec4(0.08f, 0.08f, 0.09f, 1.00f);
    colors[ImGuiCol_TitleBgActive]        = ImVec4(0.08f, 0.08f, 0.09f, 1.00f);
    colors[ImGuiCol_TitleBgCollapsed]     = ImVec4(0.00f, 0.00f, 0.00f, 0.51f);
    
    // Text
    colors[ImGuiCol_Text]                 = ImVec4(0.90f, 0.90f, 0.92f, 1.00f);
    colors[ImGuiCol_TextDisabled]         = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
}