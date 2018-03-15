attribute vec4 qt_Vertex;
uniform mat4 qt_ModelViewProjectionMatrix;

void main(void)
{
    gl_Position = qt_ModelViewProjectionMatrix * qt_Vertex;
}

#attribute vec4 qt_MultiTexCoord0;
#varying vec4 qt_TexCoord0;
#qt_TexCoord0 = qt_MultiTexCoord0;
