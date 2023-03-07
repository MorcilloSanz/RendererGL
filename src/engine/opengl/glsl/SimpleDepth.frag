#version 330 core

/*
Since we have no color buffer and disabled the draw and read buffers, 
the resulting fragments do not require any processing so we can simply 
use an empty fragment shader
*/

void main()
{             
    // gl_FragDepth = gl_FragCoord.z;
}  