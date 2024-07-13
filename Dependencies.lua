-- Boksi Dependencies

IncludeDir = {}
IncludeDir["GLFW"] = "%{wks.location}/Boksi/vendor/GLFW/GLFW/include"
IncludeDir["Glad"] = "%{wks.location}/Boksi/vendor/GLAD/include"
IncludeDir["glm"] = "%{wks.location}/Boksi/vendor/GLM"
IncludeDir["spdlog"] = "%{wks.location}/Boksi/vendor/spdlog/include"

Library = {}

-- Windows
Library["WinSock"] = "Ws2_32.lib"
Library["WinMM"] = "Winmm.lib"
Library["WinVersion"] = "version.lib"
Library["BCrypt"] = "Bcrypt.lib"