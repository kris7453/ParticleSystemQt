#version 330 core

layout (location = 0) in vec2 texCoord;
layout (location = 1) in vec2 particleBilboard;
layout (location = 2) in vec2 particlePosition;
layout (location = 3) in float particleSize;
layout (location = 4) in vec4 pColor;

uniform vec2 pixelSize;

out vec2 textureCoord;
out vec4 particleColor;

void main()
{
        //vec2 particlePos = vec2();

        gl_Position = vec4(particleBilboard * pixelSize * particleSize + pixelSize*particlePosition, 0.0f, 1.0f);

        textureCoord = texCoord;
        particleColor = pColor;
}
