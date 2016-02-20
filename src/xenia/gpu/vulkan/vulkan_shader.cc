/**
 ******************************************************************************
 * Xenia : Xbox 360 Emulator Research Project                                 *
 ******************************************************************************
 * Copyright 2016 Ben Vanik. All rights reserved.                             *
 * Released under the BSD license - see LICENSE in the root for more details. *
 ******************************************************************************
 */

#include "xenia/gpu/vulkan/vulkan_shader.h"

#include "xenia/base/assert.h"
#include "xenia/base/logging.h"
#include "xenia/base/math.h"
#include "xenia/ui/vulkan/vulkan_util.h"

namespace xe {
namespace gpu {
namespace vulkan {

using xe::ui::vulkan::CheckResult;

VulkanShader::VulkanShader(VkDevice device, ShaderType shader_type,
                           uint64_t data_hash, const uint32_t* dword_ptr,
                           uint32_t dword_count)
    : Shader(shader_type, data_hash, dword_ptr, dword_count), device_(device) {}

VulkanShader::~VulkanShader() {
  if (shader_module_) {
    vkDestroyShaderModule(device_, shader_module_, nullptr);
    shader_module_ = nullptr;
  }
}

bool VulkanShader::Prepare() {
  assert_null(shader_module_);
  assert_true(is_valid());

  // Create the shader module.
  VkShaderModuleCreateInfo shader_info;
  shader_info.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
  shader_info.pNext = nullptr;
  shader_info.flags = 0;
  shader_info.codeSize = translated_binary_.size();
  shader_info.pCode =
      reinterpret_cast<const uint32_t*>(translated_binary_.data());
  auto err =
      vkCreateShaderModule(device_, &shader_info, nullptr, &shader_module_);
  CheckResult(err, "vkCreateShaderModule");

  return true;
}

}  // namespace vulkan
}  // namespace gpu
}  // namespace xe