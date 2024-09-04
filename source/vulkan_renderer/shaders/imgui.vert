#version 450 core

layout (location = 0) in vec2 v_pos;
layout (location = 1) in vec2 v_uv;
layout (location = 2) in vec4 v_color;

layout (location = 0) out VsOutput
{
  vec2 uv;
  vec4 color;
} Out;

layout (std140, push_constant) uniform PushConstants
{
  vec2 u_scale;
  vec2 u_translate;
};

void main()
{
  gl_Position = vec4(v_pos * u_scale + u_translate, 0.0, 1.0);

  Out.uv = v_uv;
  Out.color = v_color;
}
