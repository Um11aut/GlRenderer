#version 460 core

in vec2 texCoord;
uniform sampler2D txt;

out vec4 color;

void main(){
  color = texture(txt, texCoord);
}