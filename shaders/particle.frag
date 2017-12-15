#version 330 core

in vec2 textureCoord;
in vec4 particleColor;

uniform sampler2D textureSamp;
	
void main()
{
        gl_FragColor =  particleColor * texture2D(textureSamp, textureCoord);
}
