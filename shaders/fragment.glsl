#version 330 core

// Interpolated values from the vertex shaders
in vec2 UV;
in vec3 fragPosition_worldspace;
in vec3 EyeDirection_cameraspace;
in vec3 LightDirection_cameraspace;
in vec3 Normal_cameraspace;
in vec3 LightPosition_worldspace_out;
in float LightIntensity_out;

// Ouput data
out vec4 color;

// Values that stay constant for the whole mesh.
uniform sampler2D myTextureSampler;

void main(){
    float distance = length(fragPosition_worldspace - LightPosition_worldspace_out);

    //reflection coefficient
    vec3 R = reflect( normalize(LightDirection_cameraspace), normalize(Normal_cameraspace) );
    float cosAlpha = clamp( dot(-1*normalize(EyeDirection_cameraspace),R),0,1);

    // Output color = color of the texture at the specified UV
    vec3 diffuse = texture( myTextureSampler, UV ).rgb/(distance*distance) * LightIntensity_out;
    vec3 specular = LightIntensity_out * pow(cosAlpha, 10) * vec3(1,1,1) / (distance*distance);
    vec3 result = diffuse*(dot(normalize(LightDirection_cameraspace), normalize(Normal_cameraspace)) + 0.1) + specular;
    color = vec4(result, 1.0);
}
