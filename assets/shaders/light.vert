#version 330 core
//DONE_Y phase 2
layout(location = 0) in vec3 position;
layout(location = 1) in vec4 color;
layout(location = 2) in vec2 tex_coord;
layout(location = 3) in vec3 normal;

uniform mat4 object_to_world;//object to world matrix
uniform mat4 object_to_world_inv_transpose;//object to world inverse transpose matrix (for normals)
uniform mat4 view_projection;//view projection matrix VP
uniform vec3 camera_position;//camera position to compute view vector

out Varyings {
   //color of the vertex
   vec4 color;
   //texture coordinate of the vertex
   vec2 tex_coord;
   //world position of the vertex
   vec3 world;
   //view vector of the vertex
   vec3 view;
   //normal of the vertex
   vec3 normal;
} vsout;

void main() {
   //compute the world position of the vertex
   vsout.world = (object_to_world * vec4(position, 1.0f)).xyz;
   //compute the view vector of the vertex (from the vertex to the camera)
   vsout.view = camera_position - vsout.world;
   //compute the normal of the vertex 
   vsout.normal = normalize((object_to_world_inv_transpose * vec4(normal, 0.0f)).xyz);
   //compute the position of the vertex in Homogeneous Clip Space 
   gl_Position = view_projection * vec4(vsout.world, 1.0);
   //pass the color and the texture coordinate of the vertex to the fragment shader
   vsout.color = color;
   vsout.tex_coord = tex_coord;
}
