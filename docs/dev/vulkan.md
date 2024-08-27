# Vulkan

This document provides information related to the development of the Vulkan renderer.

## Installation

The easiest option is to install the [Vulkan SDK](https://vulkan.lunarg.com/), which will work on most platforms.
You can validate your local Vulkan installation with the following command.

```shell
vulkaninfo --summary
```

### Homebrew (macOS)

On macOS, it's possible to avoid the Vulkan SDK by using MoltenVK directly via Homebrew.

```shell
brew install molten-vk vulkan-validationlayers
```

However, this requires manually setting some additional environment variables.

```shell
export VULKAN_SDK=$(brew --prefix molten-vk)
export VK_ICD_FILENAMES=$VULKAN_SDK/share/vulkan/icd.d/MoltenVK_icd.json
export VK_LAYER_PATH=$VK_LAYER_PATH:$(brew --prefix vulkan-validationlayers)/share/vulkan/explicit_layer.d
export DYLD_FALLBACK_LIBRARY_PATH=$DYLD_FALLBACK_LIBRARY_PATH:$(brew --prefix vulkan-validationlayers)/lib
```

### Troubleshooting

If you experience problems with your Vulkan installation, such as issues with locating the validation layers, it can be useful to tell the Vulkan loader to emit debug messages.

```shell
export VK_LOADER_DEBUG=all
```
