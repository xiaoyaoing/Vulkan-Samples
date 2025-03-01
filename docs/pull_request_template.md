## Description

Please include a summary of the change, new sample or fixed issue. Please also include relevant motivation and context.
Please read the [contribution guidelines](https://github.com/KhronosGroup/Vulkan-Samples/tree/master/CONTRIBUTING.md)

Fixes #<issue number>

## General Checklist:

Please ensure the following points are checked:

- [ ] My code follows the [coding style](https://github.com/KhronosGroup/Vulkan-Samples/tree/master/CONTRIBUTING.md#Code-Style)
- [ ] I have reviewed file [licenses](https://github.com/KhronosGroup/Vulkan-Samples/tree/master/CONTRIBUTING.md#Copyright-Notice-and-License-Template)
- [ ] I have commented any added functions (in line with Doxygen)
- [ ] I have commented any code that could be hard to understand
- [ ] My changes do not add any new compiler warnings
- [ ] My changes do not add any new validation layer errors or warnings
- [ ] I have used existing framework/helper functions where possible

- [ ] My changes do not add any regressions
- [ ] I have tested every sample to ensure everything runs correctly
- [ ] This PR describes the scope and expected impact of the changes I am making

 Note: The Samples CI runs a number of checks including:
 - [ ] I have updated the header Copyright to reflect the current year (CI build will fail if Copyright is out of date)
 - [ ] My changes build on Windows, Linux, macOS and Android. Otherwise I have [documented any exceptions](https://github.com/KhronosGroup/Vulkan-Samples/tree/master/CONTRIBUTING.md#General-Requirements)
  
## Sample Checklist

If your PR contains a new or modified sample, these further checks must be carried out *in addition* to the General Checklist:
- [ ] I have tested the sample on at least one compliant Vulkan implementation
- [ ] If the sample is vendor-specific, I have [tagged it appropriately](https://github.com/KhronosGroup/Vulkan-Samples/tree/master/CONTRIBUTING.md#General-Requirements)
- [ ] I have stated on what implementation the sample has been tested so that others can test on different implementations and platforms
- [ ] Any dependent assets have been merged and published in downstream modules
- [ ] For new samples, I have added a paragraph with a summary to the appropriate chapter in the [samples readme](./../samples/README.md)
- [ ] For new samples, I have added a tutorial README.md file to guide users through what they need to know to implement code using this feature. For example, see [conditional_rendering](./../samples/extensions/conditional_rendering)
