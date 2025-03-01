<!--
- Copyright (c) 2023, Holochip Corporation
-
- SPDX-License-Identifier: Apache-2.0
-
- Licensed under the Apache License, Version 2.0 the "License";
- you may not use this file except in compliance with the License.
- You may obtain a copy of the License at
-
-     http://www.apache.org/licenses/LICENSE-2.0
-
- Unless required by applicable law or agreed to in writing, software
- distributed under the License is distributed on an "AS IS" BASIS,
- WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
- See the License for the specific language governing permissions and
- limitations under the License.
-
-->

# Mesh Shader Culling
![Mesh Shader Culling](./images/mesh_shader_culling.png)

## Overview

This sample demonstrates how to incorporate the Vulkan extension [```VK_EXT_mesh_shader```](https://www.khronos.org/registry/vulkan/specs/1.2-extensions/html/vkspec.html#VK_EXT_mesh_shader), and
introduces per primitive culling in a mesh shader.

## Contents

1) [enable the mesh shader extension](#enabling-mesh-shading) 
2) [create a mesh shading graphic
pipeline](#creating-pipeline) 
3) [generate a simple mesh using meshlets](#mesh-shader) 
4) [establish a basic cull logic for the meshlets.](#mesh-shader-culling)

## Enabling mesh shading

To enable the mesh shading feature, the following 
extensions are required: (NB: ```VK_API_VERSION_1_1``` is required as a base requirement)

1) ```VK_KHR_SPIRV_1_4_EXTENSION_NAME```
2) ```VK_EXT_MESH_SHADER_EXTENSION_NAME```
3) ```VK_KHR_SHADER_FLOAT_CONTROLS_EXTENSION_NAME```

To enable task shaders and mesh shaders enable the 
following flags from the 
```VkPhysicalDeviceMeshShaderFeaturesEXT``` feature.
1) ```taskShader```
2) ```meshShader```

## Pipeline Creation

When working with Mesh shader pipelines, Vertex Input 
State and Input Assembly state are ignored.  This is 
because the mesh pipeline has the responsibility of 
defining/creating the vertex information that the 
standard fragment pipeline utilizes.

The mesh pipeline can create its own vertices as is 
done in this sample.  Or it can receive them from the 
application the same way one would for compute shaders 
when working with models.

Thus, we disable the ```pVertexInputState``` and 
```pInputAssemblyState``` by setting them to NULL.

## Linking resources

In this sample, we use a UBO (Uniform Buffer Object) to 
set the settings for the culling.

```cpp
	struct UBO
	{
		float cull_center_x   = 0.0f;
		float cull_center_y   = 0.0f;
		float cull_radius     = 1.75f;
		float meshlet_density = 2.0f;
	} ubo_cull{};
```

* ```cull_center_x``` and ```cull_center_y``` determines 
the translation of the cull mask
* ```cull_radius```defines the size of the cull mask. 
* ```meshlet_density``` defines the total number of meshlets used for the sample.

## Task Shader

A task shader is an optional but recommended stage responsible for launching mesh shaders.  It has two purposes:

* Decide how many mesh shaders to launch in the workgroup.
* Create a task payload that mesh shaders will have read-only access to.
  * taskPayloadSharedEXT type variable in GLSL can exist at most once it gets implicitly used by EmitMeshTasksEXT 
    and behaves like shared memory in the task shader and like a read-only SSBO in a mesh shader.

In general, one should use task shaders anytime you use the mesh pipeline.  While it is "optional,"  their use is 
strongly encouraged to get the most out of the mesh shading pipeline.  The only time one wouldn't use them is for 
very simplistic scenes such as rendering a single triangle found in the [mesh shader sample](../mesh_shading).

```glsl
// Example of the data shared with its associated mesh shader:
// 1) define some structure if more than one variable data sharing is desired:
// Please note: GPU vendors recommend to use as little task payload as possible, eg. by packing the data to fewer bits etc.
struct SharedData
{
    vec4  positionTransformation;
    int   N;
    int   meshletsNumber;
    float subDimension;
    float cullRadius;
};
// 2) use the following variable with a storage class specifier to "establish the connection"
taskPayloadSharedEXT SharedData sharedData;
```

Once a variable is defined with taskPayloadSharedEXT in the task shader, it will be shared with the mesh shader when 
EmitMeshTasksEXT is called.

In general, the Mesh pipeline refers to a new pipeline 
which replaces everything before the fragment shader 
with an (optional) task shading stage that can call other mesh 
shading stages.

* A task shader (optional) is used to launch mesh shader workgroup(s)
* A mesh shader has the responsibility to generate 
  primitives and vertices.

More details about emitting a mesh task can be found in the attached article:

[Mesh Shading For Vulkan](https://www.khronos.org/blog/mesh-shading-for-vulkan)
[Introduction to Turing Mesh Shaders](https://developer.nvidia.com/blog/introduction-turing-mesh-shaders/)

GPU manufactures have recommended best practices for 
their hardware in setting the work group and mesh size 
number. Further reading can be found here:

* [Meshlet Size tradeoffs](https://zeux.io/2023/01/16/meshlet-size-tradeoffs/)
* NVIDIA - [Mesh Shaders in Turing](https://on-demand.gputechconf.com/gtc-eu/2018/pdf/e8515-mesh-shaders-in-turing.pdf)
* AMD - [Sampler feedback ultimate in Mesh shaders](https://gpuopen.com/wp-content/uploads/slides/AMD_RDNA2_DirectX12_Ultimate_SamplerFeedbackMeshShaders.pdf)
* [Timur's blog](https://timur.hu/blog/2022/mesh-and-task-shaders)

## Mesh Shader

Task and mesh shaders are executed in workgroups similar to compute shaders. Each task shader workgroup can launch 
many mesh shader workgroups. Each mesh shader workgroup is responsible for generating vertices and primitives. The 
API allows for any logic, but a typical application it is recommended to organize this around meshlets, which are a 
small group of vertices and primitives. Typically, each task shader invocation processes a group of meshlets and 
each mesh shader workgroup processes one meshlet.
The vertices and primitives generation process can be found in the following code:

```glsl
// Vertices:
gl_MeshVerticesEXT[k * 4 + 0].gl_Position = vec4(2.0 * sharedData.subDimension * unitVertex_0, 0.0f, 1.0f) + sharedData.positionTransformation + displacement;
gl_MeshVerticesEXT[k * 4 + 1].gl_Position = vec4(2.0 * sharedData.subDimension * unitVertex_1, 0.0f, 1.0f) + sharedData.positionTransformation + displacement;
gl_MeshVerticesEXT[k * 4 + 2].gl_Position = vec4(2.0 * sharedData.subDimension * unitVertex_2, 0.0f, 1.0f) + sharedData.positionTransformation + displacement;
gl_MeshVerticesEXT[k * 4 + 3].gl_Position = vec4(2.0 * sharedData.subDimension * unitVertex_3, 0.0f, 1.0f) + sharedData.positionTransformation + displacement;
// Indices
gl_PrimitiveTriangleIndicesEXT[k * 2 + 0] = unitPrimitive_0 + k * uvec3(4);
gl_PrimitiveTriangleIndicesEXT[k * 2 + 1] = unitPrimitive_1 + k * uvec3(4);
// Assigning the color output:
vec3 color = vec3(1.0f, 0.0f, 0.0f) * (k + 1) / sharedData.meshletsNumber;
outColor[k * 4 + 0] = color;
outColor[k * 4 + 1] = color;
outColor[k * 4 + 2] = color;
outColor[k * 4 + 3] = color;
```

More details of meshlets generation can be found in the attached article:

[Using Mesh Shaders for Professional Graphics](https://developer.nvidia.com/blog/using-mesh-shaders-for-professional-graphics/)

## Per-primitive culling

This sample uses a simple per-primitive cull functionality from the mesh 
shader. The intention in mesh shading is to only generate geometry that is relevant to the scene.

In this sample, a circular visual zone is centered at the origin, with an adjustable radius, controlled by the gui.
When a primitive moves out of the visual zone, its generation process will be skipped.

```glsl
// the actual position of each meshlet:
vec4 position = displacement + sharedData.positionTransformation;
float squareRadius = position.x * position.x + position.y * position.y;
// Cull Logic: only if the meshlet center position is within the view circle defined by the cull radius,
// then the meshlet will be generated.
if (squareRadius < sharedData.cullRadius * sharedData.cullRadius)
{
    // Generating meshlets
}
```

Please note that per mesh culling should be done in the task shader and used to prevent mesh shaders from even 
launching.  The simplistic culling method demonstrated here is not the most ideal use of culling in mesh shaders and 
infact is discouraged due to limited benefit.  Instead please opt for limiting the number of mesh shaders that 
require launching by doing the cull within the task shader.

More advanced culling solutions can be found in the following video:

[Culling with NVIDIA Mesh Shaders](https://www.youtube.com/watch?v=n3cnUHYGbpw)
