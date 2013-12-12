#ifndef SHADER_H
#define SHADER_H

#include <string> 
#include <vector>
#include <GL/glew.h>
#include <glm/glm.hpp>

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

    void setUniform(std::string const&, GLint)              const;
    void setUniform(std::string const&, GLuint)             const;
    void setUniform(std::string const&, GLfloat)            const;
    void setUniform(std::string const&, glm::vec2 const&) const;
    void setUniform(std::string const&, glm::vec3 const&) const;
    void setUniform(std::string const&, glm::vec4 const&) const;
    void setUniform(std::string const&, glm::mat4 const&) const;

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
