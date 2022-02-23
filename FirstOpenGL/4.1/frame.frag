#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D screenTexture;

uniform bool invert;
uniform bool gray;
uniform bool sharpen;
uniform bool blur;
uniform bool edge;

vec4 InvertEffect(vec4 texColor)
{
    return vec4(vec3(1.0 - texColor), 1.0);
}

vec4 GrayEffect(vec4 texColor)
{
    float average = 0.2126 * texColor.r + 0.7152 * texColor.g + 0.0722 * texColor.b;
	return vec4(average, average, average, 1.0);
}

// 3x3��
vec4 KernelEffect(float[9] kernel)
{
    float offset = 1.0 / 300.0; // ����ƫ���������Զ���
	vec2 offsets[9] = vec2[](
        vec2(-offset,  offset), // ����
        vec2( 0.0f,    offset), // ����
        vec2( offset,  offset), // ����
        vec2(-offset,  0.0f),   // ��
        vec2( 0.0f,    0.0f),   // ��
        vec2( offset,  0.0f),   // ��
        vec2(-offset, -offset), // ����
        vec2( 0.0f,   -offset), // ����
        vec2( offset, -offset)  // ����
    );

    vec3 sampleTex[9];
    for(int i = 0; i < 9; i++)
    {
        sampleTex[i] = vec3(texture(screenTexture, TexCoords.st + offsets[i]));
    }
    vec3 col = vec3(0.0);
    for(int i = 0; i < 9; i++)
        col += sampleTex[i] * kernel[i];

    return vec4(col, 1.0);
}

vec4 SharpenEffect()
{
    float sharpenKernel[9] = float[](
        -1, -1, -1,
        -1,  9, -1,
        -1, -1, -1
    );

    return KernelEffect(sharpenKernel);
}

vec4 BlurEffect()
{
    float blurKernel[9] = float[](
        1.0 / 16, 2.0 / 16, 1.0 / 16,
        2.0 / 16, 4.0 / 16, 2.0 / 16,
        1.0 / 16, 2.0 / 16, 1.0 / 16  
    );

    return KernelEffect(blurKernel);
}

vec4 EdgeDetectionEffect()
{
    float edgeDetectionKernel[9] = float[](
        1.0,  1.0, 1.0,
        1.0, -8.0, 1.0,
        1.0,  1.0, 1.0  
    );

    return KernelEffect(edgeDetectionKernel);
}

void main()
{
	vec4 texColor = texture(screenTexture, TexCoords);
	// ���к���
	// ����
	if(invert)
	{
		FragColor = InvertEffect(texColor);
	}
	// �Ҷ�
	else if(gray)
	{
		FragColor = GrayEffect(texColor);
	}
	// �񻯾����3x3
	else if(sharpen)
	{
        FragColor = SharpenEffect();
	}
    // ģ��
    else if(blur)
    {
        FragColor = BlurEffect();
    }
    // ��Ե���Edge-detection
    else if(edge)
    {
        FragColor = EdgeDetectionEffect();
    }
	// ԭ�����
	else
    {
        FragColor = texColor;
    }
}