uniform vec3 color;
uniform vec3 ambient;

uniform bool has_ao_map;
uniform float ao_intensity;
uniform sampler2D ao_map;

uniform float height_scale;
uniform bool has_height_map;
uniform sampler2D height_map;

uniform vec3 emissive;
uniform float emissive_scale;
uniform bool has_emissive_map;
uniform sampler2D emissive_map;

uniform bool has_env;
uniform samplerCube env_map;
uniform bool is_reflection;
uniform float reflectivity;
uniform float refraction_ration;

uniform bool has_map;
uniform sampler2D map;

uniform bool has_normal_map;
uniform sampler2D normal_map;

uniform float shininess;
uniform vec3 specular;
uniform bool has_specular_map;
uniform sampler2D specular_map;
