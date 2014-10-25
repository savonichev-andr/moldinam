#ifndef __RENDERER_H
#define __RENDERER_H

#include <GL/glew.h>
#include <GL/gl.h>

#include <glm/glm.hpp>

class renderer {
    public:
        virtual void display() = 0;
};

class shader_renderer : public renderer {
    public:
        virtual void set_mvp( glm::mat4 _mvp );

    protected:
        GLuint create_shader( GLuint shader_type, const GLchar* source );
        virtual void use_mvp();

        GLuint unif_mvp;
        glm::mat4 mvp;
};

#endif
