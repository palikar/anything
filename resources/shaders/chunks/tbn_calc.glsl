vec3 T = normalize(vec3(model_matrix * vec4(i_tan,   0.0)));
vec3 B = normalize(vec3(model_matrix * vec4(i_bitan, 0.0)));
vec3 N = normalize(vec3(model_matrix * vec4(i_norm,  0.0)));
TBN = mat3(T, B, N);

tan_view_pos  = transpose(TBN) * camera_pos; 
tan_pos  = transpose(TBN) * pos;
