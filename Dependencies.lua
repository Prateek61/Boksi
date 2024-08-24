-- Boksi Dependencies

IncludeDir = {}
IncludeDir["GLFW"] = "%{wks.location}/Boksi/vendor/GLFW/GLFW/include"
IncludeDir["Glad"] = "%{wks.location}/Boksi/vendor/GLAD/include"
IncludeDir["glm"] = "%{wks.location}/Boksi/vendor/GLM"
IncludeDir["ImGui"] = "%{wks.location}/Boksi/vendor/imgui/imgui"
IncludeDir["spdlog"] = "%{wks.location}/Boksi/vendor/spdlog/include"

Library = {}

-- Windows
Library["WinSock"] = "Ws2_32.lib"
Library["WinMM"] = "Winmm.lib"
Library["WinVersion"] = "version.lib"
Library["BCrypt"] = "Bcrypt.lib"
Library["GDI32"] = "gdi32"
Library["User32"] = "user32"