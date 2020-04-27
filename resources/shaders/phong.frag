#version 460 core
#define MAX_LIGHT 5

struct DirLight {
    bool act;
    vec3 direction;
    vec3 color;
    float intensity;
};

struct AmbientLight {
    bool act;
    float intensity;
    vec3 color;
};

struct PointLight {
    bool act;

    vec3 color;
    vec3 position;
    
    float linear;
    float constant;
    float quadratic;
};

struct SpotLight {
    bool act;

    vec3 color;
    vec3 position;
    vec3 dir;
    
    float cut_off;
    float outer_cut_off;
};

struct LightSetup {
    DirLight dir_light;

    AmbientLight ambient_light;

    PointLight point_lights[MAX_LIGHT];
    SpotLight spot_lights[MAX_LIGHT];
};


// Output
out vec4 frag_color;

// Input
in vec2 uv;
in vec3 pos;
in vec3 normal;


// Phong material
uniform vec3 camera_pos;

uniform float ao_intensity;
uniform sampler2D ao_map;

uniform float bump_scale;
uniform  sampler2D bump_map;

uniform float color;

uniform float disp_scale;
uniform float disp_bias;
uniform  sampler2D displ_map;

uniform float emissive;
uniform float emissive_scale;
uniform sampler2D emissive_map;

uniform samplerCube env_map;

uniform sampler2D map;

uniform sampler2D normal_map;


// Basic  material
uniform vec3 color;
uniform float opacity;
uniform float alpha_threshold;
uniform bool visible;


// Lighting
uniform bool lighting_enabled;
uniform LightSetup lighting;


void main()
{
    
    if (!visible){
        discard;
    }
    
    if (opacity <= 0.0) {
        frag_color = vec4(color, 1.0);
    } else {
        if (alpha_threshold >= 0.0 && opacity < alpha_threshold){
            discard;
        }
        frag_color = vec4(color, opacity);
    }

    
    vec3 final_color = color;

    if (has_map) {
        final_color = mix(final_color, texture(map, uv).rgb, 0.5);
    }

    
    

    

    if (lighting_enabled) {
        vec3 normal = normalize(norm);
        vec3 to_camera = - normalize(pos - camera_pos);
        vec3 final_color = vec3(0.0);



        frag_color = vec4(final_color.xzy, opacity);
    }
    
} 
