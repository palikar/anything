#version 460 core

out vec4 frag_color;

// basic mesh material
uniform vec3 color;

uniform sampler2D tex;

uniform sampler2D alpha_map;

uniform sampler2D specular_map;

uniform float ao_intensity;

uniform sampler2D ao_map;

uniform samplerCube env_map;

uniform bool is_relfection;

uniform float reflectivity;

uniform float refraction_ration;

uniform vec3 camera_pos;

uniform int mixing;


// baisc material
uniform float opacity;

uniform float alpha_threshold;

uniform bool visible;

// from the fragment shader
in vec2 uv;
in vec3 pos;
in vec3 normal;


void main()
{


    // base color
    vec3 final_color = texture(tex, uv).rgb;
    final_color = mix(final_color, color, 0.5);

    // Ambient occlusion
    float ambient_strength = texture(ao_map, uv).r;
    final_color += ao_intensity * ambient_strength;

    // Environment
    vec3 I = normalize(pos - camera_pos);
    vec3 env_color;
    
    if (is_relfection) {
        vec3 R = reflect(I, normalize(normal));
        env_color = texture(env_map, R).rgb;
        
    } else {
        vec3 R = refract(I, normalize(normal), refraction_ration);
        env_color = texture(env_map, R).rgb;
    }
    
    float specular_strength = texture(specular_map, uv).r;
    if (specular_strength == 0.0) {
        specular_strength = 1.0;
    }
    
    final_color = mix(final_color, env_color, reflectivity * specular_strength);


    float opacity = texture(alpha_map, uv).g;
    if (opacity == 0.0) {
        opacity = 1.0;
    }
    
    frag_color = vec4(final_color, 1.0);





    
    
}
