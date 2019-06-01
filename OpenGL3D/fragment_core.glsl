#version 460

struct Material {
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	sampler2D diffuseTexture;
	sampler2D specularTexture;
};

in vec3 vs_position;
in vec3 vs_color;
in vec2 vs_texcoord;
in vec3 vs_normal;

out vec4 fs_color;

uniform Material material;

uniform vec3 lightPos0;
uniform vec3 camPosition;

vec3 calcuateDiffuse(Material material, vec3 vs_position, vec3 vs_normal, vec3 lightPos0) {
	vec3 posToLightDirectionalVec = normalize(lightPos0 - vs_position);
	vec3 diffuseColor = vec3(1.f, 1.f, 1.f);
	float diffuse = clamp(dot(posToLightDirectionalVec, vs_normal), 0, 1);
	return material.diffuse * diffuse;
}

vec3 calcuateSpecular(Material material, vec3 vs_position, vec3 vs_normal, vec3 lightPos0, vec3 camPosition) {
	vec3 lightToPosDirectionalVec = normalize(vs_position - lightPos0);
	vec3 reflectDirectionalVec = normalize(reflect(lightToPosDirectionalVec, normalize(vs_normal)));
	vec3 posToViewDirectionalVec = normalize(camPosition - vs_position);
	float specular = pow(max(dot(posToViewDirectionalVec, reflectDirectionalVec), 0), 30);
	return material.specular * specular * texture(material.specularTexture, vs_texcoord).rgb; 
}

void main() {
	//fs_color = vec4(vs_color, 1.f);
	//fs_color = texture(texture0, vs_texcoord) * texture(material.specularTexture, vs_texcoord) *  vec4(vs_color, 1.f);

	//Ambient light
	vec3 ambientFinal = material.ambient;

	//Diffuse light
	vec3 diffuseFinal = calcuateDiffuse(material, vs_position, vs_normal, lightPos0);

	//Specular Light
	vec3 specularFinal = calcuateSpecular(material, vs_position, vs_normal, lightPos0, camPosition);

	//Attenuation


	//Final Light
	fs_color = texture(material.diffuseTexture, vs_texcoord) *  (vec4(ambientFinal, 1.f) + vec4(diffuseFinal, 1.f) + vec4(specularFinal, 1.f));
}