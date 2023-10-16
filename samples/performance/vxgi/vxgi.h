//
// Created by pc on 2023/10/16.
//
#pragma once
#include "ktx.h"

#include "api_vulkan_sample.h"
#include "scene_graph/components/camera.h"
class Vxgi : public vkb::VulkanSample
{
  public:
	Vxgi();
  private:
	vkb::RenderPipeline pipeline;
	void createVoxelizationPass();
};

std::unique_ptr<Vxgi> create_vxgi();
