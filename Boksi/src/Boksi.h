#pragma once

// For use by Boksi applications

#include "Boksi/Core/Base.h"

#include "Boksi/Core/Application.h"
#include "Boksi/Core/Layer.h"
#include "Boksi/ImGui/ImGuiLayer.h"
#include "Boksi/ImGui/ImGuiFileBrowser.h"
#include "Boksi/Core/Log.h"
#include "Boksi/Core/Assert.h"
#include "Boksi/Core/Input.h"
#include "Boksi/Core/KeyCodes.h"
#include "Boksi/Core/MouseCodes.h"
#include "Boksi/World/World.h"
#include "Boksi/World/Material.h"
#include "Boksi/Core/Timestep.h"

#include "Boksi/Debug/Instrumentor.h"

// ---Resource-----------------
#include "Boksi/Resource/Buffer.h"
#include "Boksi/Resource/FileIO.h"
#include "Boksi/Resource/ResourceManager.h"

// ---Mesh---------------------
#include "Boksi/World/Mesh/VoxelMeshArray.h"
#include "Boksi/World/Mesh/VoxelMeshSVO.h"

// ---Renderer-----------------
#include "Boksi/Renderer/Renderer.h"
#include "Boksi/Renderer/RenderCommand.h"
#include "Boksi/Renderer/Buffer/Buffer.h"
#include "Boksi/Renderer/Shader.h"
#include "Boksi/Renderer/VertexArray.h"
#include "Boksi/Renderer/OrthographicCamera.h"
#include "Boksi/Renderer/Camera.h"
#include "Boksi/Renderer/Texture.h"
#include "Boksi/Renderer/VoxelRenderer.h"
// -----------------------------

// ---World--------------------
#include "Boksi/World/CameraController.h"
#include "Boksi/World/Mesh/VoxelMeshSVO.h"
#include "Boksi/World/ModelLoader.h"
#include "Boksi/World/Mesh/VoxelMeshArray.h"
#include "Boksi/World/Mesh/VoxelMeshModifier.h"
// -----------------------------

// ---Entity-------------------
#include "Boksi/Entity/Entity.h"
#include "Boksi/Entity/EntitiesArray.h"
#include "Boksi/Entity/Object.h"
// -----------------------------