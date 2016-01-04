#ifdef GL_ES
// Set default precision to medium
precision mediump int;
precision mediump float;
#endif

uniform mat4 mvp_matrix;
uniform mat4 m_matrix;

attribute vec4 a_position;
attribute vec4 a_pos_real;
attribute vec2 a_texcoord;

varying vec4 wPos;
varying vec2 tPos;
//! [0]
void main()
{
    // Calculate vertex position in screen space
    gl_Position = mvp_matrix * a_position;
	wPos = m_matrix * a_pos_real;
	tPos = a_texcoord;
	//wPos = a_pos_real;
}
//! [0]
