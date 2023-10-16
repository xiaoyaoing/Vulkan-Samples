//
// Created by pc on 2023/10/16.
//

#include "vxgi.h"

#include "common/vk_common.h"

#include "rendering/pipeline_state.h"
#include "rendering/render_context.h"
#include "rendering/render_pipeline.h"
#include "rendering/subpasses/geometry_subpass.h"
#include "rendering/subpasses/lighting_subpass.h"
#include "scene_graph/node.h"


Vxgi::Vxgi()
{

}
void Vxgi::createVoxelizationPass()
{

	auto voxelizationPass = std::make_unique<vkb::GeometrySubpass>()
	pipeline.add_subpass()
}
