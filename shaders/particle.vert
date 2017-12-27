#version 330 core

layout (location = 0) in vec2 texCoord;
layout (location = 1) in vec2 particleBilboard;
layout (location = 2) in vec2 particlePosition;
layout (location = 3) in float particleSize;
layout (location = 4) in float particleSpin;
layout (location = 5) in vec4 pColor;

uniform vec2 pixelSize;
uniform float scale;

out vec2 textureCoord;
out vec4 particleColor;

void main()
{
        vec2 particlePos = vec2(particleBilboard * particleSize);// * pixelSize);
        float sinAngle = sin(radians(particleSpin));
        float cosAngle = cos(radians(particleSpin));
        particlePos = vec2(particlePos.x * cosAngle - particlePos.y * sinAngle, particlePos.x * sinAngle + particlePos.y * cosAngle);

        gl_Position = vec4( particlePos*pixelSize*scale + pixelSize*particlePosition*scale, 0.0f, 1.0f);

        textureCoord = texCoord;
        particleColor = pColor;
}
