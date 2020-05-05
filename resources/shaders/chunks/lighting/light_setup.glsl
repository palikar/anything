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

uniform bool lighting_enabled;
uniform LightSetup lighting;
