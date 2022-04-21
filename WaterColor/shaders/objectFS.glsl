#version 330

in vec3 normalVector;
in vec3 worldPosition;
vec3 viewPosition = vec3 (0.0, 0.0, 0.0); // camera position in world coords.
uniform vec3 objectColor = vec3(0.8, 1.0, 0.8); // green diffuse surface reflectance
uniform vec3 darkColor = vec3(0.0, 0.4, 0.0);


void main(){
	vec3 viewDirection = normalize(viewPosition - worldPosition);

	// if it is face to us, use light color
	// if it is side, use darker color
	float NdotV = max(dot(normalVector,viewDirection),0);
	vec3 result = max(objectColor * step(0.5, NdotV), darkColor * (1 - step(0.5, NdotV)));

	gl_FragColor = vec4 (result, 1.0);
}