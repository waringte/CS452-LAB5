#version 120

uniform mat4 Matrix;
uniform mat4 viewMatrix;
uniform mat4 modelMatrix;

attribute vec2 texcoord;
varying vec2 f_texcoord;

attribute vec3 in_position;

attribute vec4 in_color;
varying vec4 pass_color;

void main(){
  gl_Position=Matrix*viewMatrix*modelMatrix*vec4(in_position,1.0);
  pass_color=in_color;
  f_texcoord = texcoord;
}
