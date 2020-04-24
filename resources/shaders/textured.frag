#version 460 core

out vec4 frag_color;

// basic mesh material
uniform vec3 color;

uniform float color_intensity;

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

    if (!visible){
        discard;
    }

    
    float alpha_opacity = texture(alpha_map, uv).g;
    if (alpha_opacity == 0.0) {
        alpha_opacity = opacity;
    }
    if (alpha_threshold >= 0.0 && alpha_opacity < alpha_threshold){
        discard;
    }


    // base color
    vec3 final_color = texture(tex, uv).rgb;
    final_color = mix(final_color, color, color_intensity);

    // Ambient occlusion
    float ambient_strength = (texture(ao_map, uv).r - 1.0) * ao_intensity + 1.0;
    final_color *= ambient_strength;

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

    if (mixing == 0) {
        // add
        final_color += env_color.xyz * reflectivity * specular_strength;
    } else if (mixing == 1) {
        // mult
        final_color = mix(final_color, final_color * env_color.xyz, reflectivity * specular_strength);
    } else {
        // mix
        final_color = mix(final_color, env_color, reflectivity * specular_strength);
    }
        

    
    frag_color = vec4(final_color, alpha_opacity);
    // frag_color = vec4(normalize(normal)*0.5 + 0.5, alpha_opacity);

}
