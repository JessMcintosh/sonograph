#ifdef GL_ES
// Set default precision to medium
precision mediump int;
precision mediump float;
#endif

uniform sampler3D textureData;
uniform float sampleSize;
uniform float sampleSpeed;
uniform float sampleFrequency;
uniform float e_sep;
uniform int numElements;

uniform float overexposure;
uniform float logfactor;
uniform float layer;

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
		//vec4 tex = texture3D(texture, vec3(tPos,1.0));
		//vec4 tex = texture(textureData, vec3(tPos,layer));

		vec4 sum_col = vec4(0,0,0,0);

		for(int i = 0; i < numElements; i++){
			sum_col += texture(textureData, vec3(tPos,(float(i)/float(numElements))));
		}
		//sum_col = sum_col / float(numElements);

		col_r = sum_col[0] * 2.0;
		col_i = sum_col[1] * 2.0;
		col_abs = sqrt(col_r*col_r + col_i*col_i);
		col_abs = overexposure * log(logfactor * col_abs + 1.0);
		gl_FragColor = vec4(col_abs, col_abs, col_abs, 1.0);
		//gl_FragColor = vec4(1.0,0.0,1.0,1.0);
		return;
	}

	// Begin FMC TFM B SCAN

	// Common for each emitting transducer

	// Position of the point in the plane in the real world 
	// (vec4 contains homogeneous 1.0 in fourth element)
	vec3 pos = vec3(wPos[0], wPos[1], wPos[2]);

	//float leftmost = -( float(numElements)/2.0 - 0.5);
	float leftmost = ( float(numElements)/2.0 - 0.5);

	vec4 average_col = vec4(0);

	// Iterate over all pulsing transducers
	for(int j = 0; j < numElements; j++){
		// Common for each receiving transducer

		float x_offset = leftmost - float(j);
		vec3 emitPos = vec3( (e_sep * x_offset), 0, 0);

		// Distance to emitting piezoelectric transducer 
		float distance = length(pos - emitPos);
		float time = distance / sampleSpeed;
		
		float element_contr_count = 0.0;

		// Loop through all receiving transducers
		for(int i = 0; i < numElements; i++){

			// Calculate the position of the i_th element, assume coplanar  (x axis)
			float xpos = (leftmost-float(i))*e_sep;
			
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
				vec4 tex = texture(textureData, vec3(index, 1.0-(float(i)/float(numElements)), float(j)/float(numElements)));
				col_r += tex[0];
				col_i += tex[1];
			}
		}
	}



	// Calculate which sample bin to index given the time
	//float bin = time * sampleFrequency;	
	
	// Then scale to within 0 .. 1
	//float index = bin / sampleSize;


	//col_r /= element_contr_count;
	//col_i /= element_contr_count;

	col_abs = sqrt(col_r*col_r + col_i*col_i);
	col_abs = overexposure * log(logfactor * col_abs + 1.0);

	gl_FragColor = vec4(col_abs, col_abs, col_abs, 1.0);


}
