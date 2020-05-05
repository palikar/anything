vec2 parallax_mapping(vec2 texCoords, vec3 viewDir)
{
    // float height = 1.0f - texture(height_map, tex_coords).r;
    // vec2 p = viewDir.xy * (height * height_scale);
    // return tex_coords - p;

    // number of depth layers
    const float numLayers = 10;
    // calculate the size of each layer
    float layerDepth = 1.0 / numLayers;
    // depth of current layer
    float currentLayerDepth = 0.0;
    // the amount to shift the texture coordinates per layer (from vector P)
    vec2 P = viewDir.xy * height_scale; 
    vec2 deltaTexCoords = P / numLayers;

    vec2  currentTexCoords     = texCoords;
    float currentDepthMapValue = 1.0 - texture(height_map, currentTexCoords).r;
  
    while(currentLayerDepth < currentDepthMapValue)
    {
        // shift texture coordinates along direction of P
        currentTexCoords -= deltaTexCoords;
        // get depthmap value at current texture coordinates
        currentDepthMapValue =  1.0 - texture(height_map, currentTexCoords).r;  
        // get depth of next layer
        currentLayerDepth += layerDepth;  
    }

    return currentTexCoords;
}
