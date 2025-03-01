<!--
- Copyright (c) 2023, Thomas Atkinson
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

# Components

A component encapsulates all code relating to a specific goal. Components act as individual compile targets. This allows CMake to efficiently parallelize the compilation and link stages. A component should include the minimum amount of dependencies. Circular dependencies should be avoided.

## Core Component

Common interfaces can be used across the project and multiple components. These interfaces are defined in `components/core`. **Core** is the only component which does not follow the component pattern in its entirety. The only major difference between core and other components is the header prefix used is `core/<sub_dir>` instead of `components/core/<sub_dir>`.

See [core documentation](core/README.md) for more information.

## Create a new component

To create a new component add a new folder under `components/`. The folder name should relate to the components implementation - see current components for inspiration. The next instructs are to be carried out inside the `components/<component_name>` folder.

1. Create a directory named `include/components/<component_name>`. This contains all public headers which other components will have access too
2. Create a directory named `src`. This contains all private headers and source files. Components will not be able to include these.
3. Create a directory named `tests`. This contains all test files for this component
4. Create a `CMakeLists.txt`

### Add the Component Compile Target

Registering a component adds the `vkb__<component_name>` compile target. This target is also linked as a dependency to `vkb__components`.

```cmake
vkb__register_component(
    NAME <component_name>
    SRC
        src/<some_private_header>.hpp
        src/<some_source>.cpp
    LINK_LIBS
        <some_link_lib>
)
```

### Add a Test

Registering a test adds the `tests__<test_name>` compile target. This target is also linked as a dependency to `vkb__tests`.

```cmake
vkb__register_tests(
    NAME <test_name>
    SRC
        tests/<some_test>.test.cpp
    LINK_LIBS
        <some_link_libs>
)
```

### Compile Components

- To compile all components run cmake with `--target vkb__components`
- To compile a specific component run cmake with `--target vkb__<component_name>`
- To compile all tests run cmake with `--target vkb__tests`
- To compile a specific test run cmake with `--target tests__<test_name>`.