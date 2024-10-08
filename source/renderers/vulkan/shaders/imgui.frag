#version 450 core

precision highp float;

layout (location = 0) in VsOutput
{
  vec2 uv;
  vec4 color;
} In;

layout (location = 0) out vec4 output_color;

layout (binding = 0) uniform sampler2D u_texture;

// See https://github.com/ocornut/imgui/issues/4890
vec4 to_linear(vec4 srgb)
{
  const bvec3 cutoff = lessThan(srgb.rgb, vec3(0.04045));

  const vec3 higher = pow((srgb.rgb + vec3(0.055)) / vec3(1.055), vec3(2.4));
  const vec3 lower = srgb.rgb / vec3(12.92);

  return vec4(mix(higher, lower, cutoff), srgb.a);
}

void main()
{
  output_color = to_linear(In.color) * to_linear(texture(u_texture, In.uv));
}
