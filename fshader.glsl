#ifdef GL_ES
// Set default precision to medium
precision mediump int;
precision mediump float;
#endif

uniform sampler2D texture;
uniform float sampleSize;
uniform float sampleSpeed;
uniform float sampleFrequency;
uniform float e_sep;
uniform int numElements;

uniform float overexposure;
uniform float logfactor;

varying vec4 wPos;
varying vec2 tPos;

uniform bool a_scan;

void main()
{
	// Overall colour
	float col_r = 0.0;
	float col_i = 0.0;
	float col_abs = 0.0;

	if(a_scan){
		// Set fragment color from texture
		//gl_FragColor = texture2D(texture, tPos);
		vec4 tex = texture2D(texture, tPos);
		col_r = tex[0] * 2.0;
		col_i = tex[1] * 2.0;
		col_abs = sqrt(col_r*col_r + col_i*col_i);
		col_abs = overexposure * log(logfactor * col_abs + 1.0);
		gl_FragColor = vec4(col_abs, col_abs, col_abs, 1.0);
		return;
	}

	float leftmost = -( float(numElements)/2.0 - 0.5);

	// Position of the point in the plane in the real world 
	// (vec4 contains homogeneous 1.0 in fourth element)
	vec3 pos = vec3(wPos[0], wPos[1], wPos[2]);

	// FOR CSM
	//vec3 emitPos = vec3( (e_sep * -31.5), 0, 0);
	vec3 emitPos = vec3( (e_sep * leftmost), 0, 0);
	//vec3 emitPos = vec3(-0.019845, 0, 0);
	//vec3 emitPos = vec3(-0.006615, 0, 0);

	// Initial estimation, distance to emitting piezoelectric transducer 
	float distance = length(pos - emitPos);
	float time = distance / sampleSpeed;

	// Calculate which sample bin to index given the time
	//float bin = time * sampleFrequency;	
	
	// Then scale to within 0 .. 1
	//float index = bin / sampleSize;

	float element_contr_count = 0.0;

	for(int i = 0; i < numElements; i++){

		// Calculate the position of the i_th element, assume coplanar  (x axis)
		float xpos = (leftmost+float(i))*e_sep;
		
		vec3 e_pos = vec3(xpos, 0.0, 0.0);
		distance = length(e_pos - pos);

		float e_time = distance / sampleSpeed;
		float echo_time = e_time + time;
		float bin = echo_time * sampleFrequency;
		float index = bin / sampleSize;

		if(index > 1.0){
			element_contr_count += 1.0;
		}
		else{
			element_contr_count += 1.0;
			// depending on how the textures laid out...
			//vec4 tex = texture2D(texture, tPos);
			vec4 tex = texture2D(texture, vec2(index, 1.0-(float(i)/float(numElements))));
			col_r += tex[0];
			col_i += tex[1];

		}
	}
	//col_r /= element_contr_count;
	//col_i /= element_contr_count;

	col_abs = sqrt(col_r*col_r + col_i*col_i);
	col_abs = overexposure * log(logfactor * col_abs + 1.0);

	gl_FragColor = vec4(col_abs, col_abs, col_abs, 1.0);


}
