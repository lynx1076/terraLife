#version 330

in vec2 fragTexCoord;
in vec4 fragColor;

uniform sampler2D texture0;
uniform vec4 colDiffuse;

uniform int states[3];
uniform vec4 colors[3];

out vec4 finalColor;

void main() {
  vec4 tex = texture(texture0, fragTexCoord);
  int pixel = int(tex.r * 255);

  if (pixel == states[0]) finalColor = colors[0];
  else if (pixel == states[1]) finalColor = colors[1];
  else if (pixel == states[2]) finalColor = colors[2];
  else discard;

  finalColor = finalColor * colDiffuse * fragColor;
}

