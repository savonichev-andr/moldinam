#include <GL/glew.h>

#include "particles_renderer.h"
#include <iostream>

ParticleRenderer::ParticleRenderer()
    : pointSize(1.0f)
    , particleRadius(0.125f * 0.5f)
    , program(0)
    , VBO(0)
{
    setup_program();
}

ParticleRenderer::~ParticleRenderer() {}

void
ParticleRenderer::set_particles_positions(const std::vector<glm::vec3>& pos, glm::vec3 area_size)
{
    positions.resize(pos.size());

    glm::mat3 scale_matrix = get_particles_scale_matrix(area_size);

    // positions are scaling to the range [0, 2] and then shifted to the range
    // [-1, 1]
    for (size_t i = 0; i < pos.size(); i++) {
        positions[i] = scale_matrix * pos[i] + glm::vec3(-1.0, -1.0, -1.0);
    }
}

glm::mat3 ParticleRenderer::get_particles_scale_matrix(glm::vec3 area_size)
{
    glm::mat3 scale_matrix;
    scale_matrix[0][0] = 2 / area_size.x;
    scale_matrix[1][1] = 2 / area_size.y;
    scale_matrix[2][2] = 2 / area_size.z;

    return scale_matrix;
}

void ParticleRenderer::setup_program()
{
    GLuint vertex_shader = create_shader(GL_VERTEX_SHADER, vertex_shader_source);
    GLuint fragment_shader = create_shader(GL_FRAGMENT_SHADER, fragment_shader_source);

    program = glCreateProgram();
    glAttachShader(program, vertex_shader);
    glAttachShader(program, fragment_shader);
    glLinkProgram(program);

    unif_mvp = glGetUniformLocation(program, "mvp");
    attrib_vertex = glGetAttribLocation(program, "coord");
}

void ParticleRenderer::_drawPoints()
{
    if (!VBO) {
        glBegin(GL_POINTS);
        {
            for (size_t i = 0; i < positions.size(); ++i) {
                glVertex3fv(glm::value_ptr(positions[i]));
            }
        }
        glEnd();
    } else {
        glBindBufferARB(GL_ARRAY_BUFFER_ARB, VBO);
        glVertexPointer(4, GL_FLOAT, 0, 0);
        glEnableClientState(GL_VERTEX_ARRAY);

        glDrawArrays(GL_POINTS, 0, positions.size());

        glBindBufferARB(GL_ARRAY_BUFFER_ARB, 0);
        glDisableClientState(GL_VERTEX_ARRAY);
        glDisableClientState(GL_COLOR_ARRAY);
    }
}

#define STRINGIFY(A) #A

// vertex shader
const char* ParticleRenderer::vertex_shader_source = STRINGIFY(uniform float pointRadius; // point size in world space
                                                               uniform float pointScale; // scale to calculate size in pixels
                                                               uniform float densityScale; uniform float densityOffset;
                                                               uniform mat4 mvp; void main() {
      // calculate window-space point size
      vec3 posEye = vec3(gl_ModelViewMatrix * vec4(gl_Vertex.xyz, 1.0));
      float dist = length(posEye);
      gl_PointSize = 20.0; // pointRadius * (pointScale / dist);

      gl_TexCoord[0] = gl_MultiTexCoord0;
      gl_Position = mvp * vec4(gl_Vertex.xyz, 1.0);
      gl_FrontColor = gl_Color;
});

// pixel shader for rendering points as shaded spheres
const char* ParticleRenderer::fragment_shader_source = STRINGIFY(vec3 lightDir = vec3(0.577, 0.577, 0.577); void main() {
      // calculate normal from texture coordinates
      vec3 N;
      N.xy = gl_TexCoord[0].xy * vec2(2.0, -2.0) + vec2(-1.0, 1.0);
      float mag = dot(N.xy, N.xy);
      if (mag > 1.0)
        discard; // kill pixels outside circle
      N.z = sqrt(1.0 - mag);

      // calculate lighting
      float diffuse = max(0.05, dot(lightDir, N));

      gl_FragColor = gl_Color * diffuse;
});

void ParticleRenderer::display()
{
    glEnable(GL_POINT_SPRITE_ARB);
    glTexEnvi(GL_POINT_SPRITE_ARB, GL_COORD_REPLACE_ARB, GL_TRUE);
    glEnable(GL_VERTEX_PROGRAM_POINT_SIZE_NV);
    glDepthMask(GL_TRUE);
    glEnable(GL_DEPTH_TEST);

    glUseProgram(program);

    use_mvp();
    /* glUniform1f( glGetUniformLocation(program, "pointScale"), window_h /
   * tan(fov*0.5*M_PI/180.0) ); */
    /* glUniform1f( glGetUniformLocation(program, "pointRadius"), particleRadius
   * ); */
    glUniform1f(glGetUniformLocation(program, "pointScale"), 0.01f);
    glUniform1f(glGetUniformLocation(program, "pointRadius"), 0.0001);

    glColor3f(1, 1, 1);
    _drawPoints();

    glUseProgram(0);
    glDisable(GL_POINT_SPRITE_ARB);
}
