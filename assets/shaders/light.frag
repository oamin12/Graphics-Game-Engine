#version 330 core

//types of lights are directional, point or spot
#define TYPE_DIRECTIONAL    0
#define TYPE_POINT          1
#define TYPE_SPOT           2
#define MAX_LIGHT_COUNT     8   // This will define the maximum number of lights


in Varyings {
    vec4 color;
    vec2 tex_coord;
    vec3 world;   // vertex position in the world space 
    vec3 view;    // vertex to eye vector in the world space
    vec3 normal;  // surface normal in the world space
} fsin;


struct Light {
    // This defines the type of the light. It can be directional, point or spot
    int type;    

    // These defines the colors of the light
    vec3 color;
 
    vec3 position;// Position is for point and spot lights.

    vec3 direction;// Direction is for directional and spot lights

    // Attentuation factors are used for point and spot lights to simulate the light intensity drop over distance
    vec3 attenuation;

    float inner_angle, outer_angle;  // Cone angles are used for spot lights the umbra and penumbra angles
};


struct TexturedMaterial {
    //albedo is the color of the object raw ss 
   sampler2D albedo_map;
   vec3 albedo_tint; //tint is the color of the object with lighting
   //specular is the color of the object with lighting
   sampler2D specular_map;
   vec3 specular_tint;
   //ambient occlusion is el loon lama yekoon fy 7etat dakhla w 7etat tal3a fa mesh kolo menawar zy ba3do
   sampler2D ambient_occlusion_map;
   //we use roughness to calculate the shininess of the object where shininess = 2/roughness^4 - 2
   sampler2D roughness_map;
   vec2 roughness_range;
   //emissive is the color of the object without lighting the object is a source of light bs mesh bynawar el 7awaleeh
   sampler2D emissive_map;
   vec3 emissive_tint;
};


// This is the material struct that will be used in the fragment shader to compute the lighting of the object
struct Material {
   vec3 diffuse;
   vec3 specular;
   vec3 ambient;
   vec3 emissive;
   float shininess;
};


// This function will sample the material maps and compute the material properties
Material sample_material(TexturedMaterial tex_mat, vec2 tex_coord){
   Material mat;
   mat.diffuse = texture(tex_mat.albedo_map, tex_coord).rgb;
   mat.specular = texture(tex_mat.specular_map, tex_coord).rgb;
   mat.emissive = texture(tex_mat.emissive_map, tex_coord).rgb;
   mat.ambient = mat.diffuse * texture(tex_mat.ambient_occlusion_map, tex_coord).r;
   float roughness = texture(tex_mat.roughness_map, tex_coord).r;
    //we use roughness to calculate the shininess of the object where shininess = 2/roughness^4 - 2
   mat.shininess = 2.0f/pow(clamp(roughness, 0.001f, 0.999f), 4.0f) - 2.0f;
   return mat;
}


// the material, light array and number of lights sent from the cpu c++ code
uniform TexturedMaterial tex_material;
uniform Light lights[MAX_LIGHT_COUNT];
uniform int light_count;
//the color of the fragment output on the screen
out vec4 frag_color;

void main() {
    
    //we need to normalize the normal and view vectors in the fragment shader
    vec3 normal = normalize(fsin.normal);  
    vec3 view = normalize(fsin.view);
    //we need to sample the material maps and compute the material properties
    Material material = sample_material(tex_material, fsin.tex_coord);

    // We will loop over the lights and compute the lighting for each light so we need to know how many lights we have
    int count = min(light_count, MAX_LIGHT_COUNT);

    // We will accumulate the result of all the lights in this variable
    // as the object may be affected by more than one light
    vec3 accumulated_light = vec3(0.0);

    // Now we will loop over all the lights
    for(int index = 0; index < count; index++){
        // We will get the light at the current index
        Light light = lights[index];
        // We will compute the light direction
        vec3 light_direction;
        // attenuation is the light intensity drop over distance
        float attenuation = 1;
        // If the light is directional, then its direction is fixed and we don't need to compute it
        if(light.type == TYPE_DIRECTIONAL)
            light_direction = light.direction; 
        else {
            // If not directional we need to compute the direction
            light_direction = fsin.world - light.position; // We compute the direction from the light to the vertex
            // this is r
            float distance = length(light_direction);
            // We normalize the direction
            light_direction /= distance;

            // And compute the attenuation using the attenuation factors
            // if attenuation is (1, 0, 0) then the light intensity will not drop over distance
            // if attenuation is (0, 1, 0) then the light intensity will drop linearly over distance
            // if attenuation is (0, 0, 1) then the light intensity will drop quadratically over distance
            attenuation *= 1.0f / dot(light.attenuation, vec3(1, distance, distance*distance));
            
            if(light.type == TYPE_SPOT){
                // If it is a spot light comput the angle attenuation
                float angle = acos(dot(light.direction, light_direction));
                // We use the smoothstep function to interpolate between the inner and outer angles
                attenuation *= smoothstep(light.outer_angle, light.inner_angle, angle);
            }
        }

        
        vec3 diffuse = material.diffuse * light.color * max(0.0f, dot(normal, -light.direction));
        vec3 specular = material.specular * light.color * pow(max(0.0f, dot(view, reflect(light.direction, normal))), material.shininess);
        vec3 ambient = material.ambient * light.color;

        // Then we accumulate the light components additively
        accumulated_light += (diffuse + specular + ambient) * attenuation ;
    }
    // Finally we output the final color of the fragment
    // the accumulated light is is added to the emissive color of the object
    frag_color = fsin.color * vec4(accumulated_light + material.emissive, texture(tex_material.albedo_map, fsin.tex_coord).a);
}