#version 330

in vec2 fragTexCoord;
in vec4 fragColor;

uniform sampler2D texture0;
uniform vec4 colDiffuse;

uniform uint states[4];
uniform vec4 colors[4];

out vec4 finalColor;

void main() {
  vec4 tex = texture(texture0, fragTexCoord);
  uint pixel = uint(tex.r * 255);

  if (pixel == states[0]) finalColor = colors[0];
  else if (pixel == states[1]) finalColor = colors[1];
  else if (pixel == states[2]) finalColor = colors[2];
  else if (pixel == states[3]) finalColor = colors[3];
  else discard;

  finalColor = finalColor * colDiffuse * fragColor;
}

