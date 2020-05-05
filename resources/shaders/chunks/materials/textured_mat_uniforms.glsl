uniform vec3 color;

uniform float color_intensity;

uniform sampler2D tex;

uniform bool has_alpha_map;
uniform sampler2D alpha_map;

uniform bool has_specular_map;
uniform sampler2D specular_map;

uniform float ao_intensity;

uniform bool has_ao_map;
uniform sampler2D ao_map;

uniform bool has_env;
uniform samplerCube env_map;

uniform bool is_relfection;

uniform float reflectivity;

uniform float refraction_ration;

uniform int mixing;

// uniform vec3 camera_pos;
