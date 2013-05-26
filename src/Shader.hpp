#ifndef SHADER_H
#define SHADER_H

#include <string> 
#include <vector>
#include <GL/glew.h>

#include "vmath.hpp"

class Shader
{
public:
    static void bind(Shader const&);

    enum Type { Vertex,
                Fragment };

    Shader();
    Shader(std::string const& file, Type type);
    Shader(std::string const& vertexShader, std::string const& fragmentShader);
    virtual ~Shader();

    void loadFromFile(std::string const& file, Type type);

    void loadFromFile(std::string const& vertexShader, 
                      std::string const& fragmentShader); 

    void setUniform(std::string const&, GLint);
    void setUniform(std::string const&, GLuint);
    void setUniform(std::string const&, GLfloat);
    void setUniform(std::string const&, vmath::vec2 const&);
    void setUniform(std::string const&, vmath::vec3 const&);
    void setUniform(std::string const&, vmath::vec4 const&);
    void setUniform(std::string const&, vmath::mat4 const&);

    GLuint name()    const { return program_; }
    bool   isValid() const { return !invalid_; } 
private: 
    void   createProgram();
    GLuint compileShaderFromFile(std::string const& file, Type type); 
    void   compileProgram(std::vector<GLuint> const& shaderObjects);

    GLuint program_;
    bool   invalid_;

    Shader& operator=(Shader const&);
    Shader(Shader const&);
};
#endif /* SHADER_H */
