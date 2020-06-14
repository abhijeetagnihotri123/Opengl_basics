#version 330 core
out vec4 FragColor;

  in  vec3 FragPos;
   in  vec3 Normal;
   in vec2 TexCoords;

uniform vec3 Position[4];
uniform vec3 Color[4];
uniform sampler2D diffuseTexture;
uniform vec3 viewPos;

void main()
{           
    vec3 color = texture(diffuseTexture,TexCoords).rgb;
    vec3 normal = normalize(Normal);
    // ambient
    vec3 ambient = 0.0 * color;
    // lighting
    vec3 lighting = vec3(0.0);
    for(int i = 0; i < 16; i++)
    {
        // diffuse
        vec3 lightDir = normalize(lights[i].Position - fs_in.FragPos);
        float diff = max(dot(lightDir, normal), 0.0);
        vec3 diffuse = Color[i] * diff * color;      
        vec3 result = diffuse;        
        // attenuation (use quadratic as we have gamma correction)
        float distance = length(fs_in.FragPos - Position[i]);
        result *= 1.0 / (distance * distance);
        lighting += result;
                
    }
    FragColor = vec4(ambient + lighting, 1.0);
}