uv =  i_uv;
norm =  mat3(transpose(inverse(model_matrix))) * i_norm;
pos = vec3(model_matrix * vec4(i_pos, 1.0));

gl_Position = projection_matrix * view_matrix * vec4(pos, 1.0);
