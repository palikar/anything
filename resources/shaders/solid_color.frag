#version 460 core

out vec4 frag_color;

uniform vec3 color;
uniform float opacity;
uniform float alpha_threshold;
uniform bool visible;


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

struct LightSetup {
    DirLight dir_light;
    AmbientLight ambient_light;

};

uniform vec3 camera_pos;

uniform LightSetup lighting;

in vec2 uv;
in vec3 pos;
in vec3 norm;

vec3 apply_dir_light(DirLight light, vec3 surface_color, vec3 normal, vec3 surface_pos, vec3 surface_to_camera)
{
    vec3 surface_to_light  = normalize(light.direction);
    float diffuse_coefficient = max(0.0, dot(normal, surface_to_light));
    vec3 diffuse = diffuse_coefficient * surface_color.rgb * light.color * light.intensity;

    return diffuse;

}

vec3 apply_ambient_light(AmbientLight light, vec3 surface_color)
{
    return light.intensity * surface_color.rgb;
}


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

    vec3 normal = normalize(norm);
    vec3 to_camera = normalize(pos - camera_pos);

    vec3 final_color = vec3(0.0);
    
    final_color += apply_dir_light(lighting.dir_light, frag_color.rgb, normal , pos, to_camera);
    final_color += apply_ambient_light(lighting.ambient_light, frag_color.rgb);

    
    frag_color = vec4(final_color.xzy, opacity);



// vec3 gamma = vec3(1.0/2.2);
    // frag_color = vec4(pow(frag_color.rgb, gamma), frag_color.a);

}
