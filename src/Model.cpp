#include "Model.hpp"

#include "Consts.hpp"

#define BUFFER_OFFSET(offset) ((GLvoid*)(offset))

Model::Model(Shader const& shader)
    : shader_(shader)
{ 
    GLushort const vertexIndices[] = {
        0, 1, 2, 3, 4, 5, 6, 7,
        constants::primitiveRestartIndex,
        2, 4, 0, 6, 1, 7, 3, 5
    };

    GLfloat const vertexPositions[] = {
        -0.2f, -0.2f,  -0.2f, 1.0f,
         0.2f, -0.2f,  -0.2f, 1.0f,
        -0.2f,  0.2f,  -0.2f, 1.0f,
         0.2f,  0.2f,  -0.2f, 1.0f,
        -0.2f,  0.2f,   0.2f, 1.0f,
         0.2f,  0.2f,   0.2f, 1.0f,
        -0.2f, -0.2f,   0.2f, 1.0f,
         0.2f, -0.2f,   0.2f, 1.0f
    };

    GLfloat const vertexColours[] = {
        1.0f, 1.0f, 1.0f, 0.5f, //c0
        0.5f, 0.5f, 0.5f, 0.5f, //c1
        0.5f, 0.0f, 0.0f, 0.5f, //c2
        1.0f, 0.0f, 0.0f, 0.5f, //c3
        0.0f, 0.5f, 0.0f, 0.5f, //c4
        0.0f, 1.0f, 0.0f, 0.5f, //c5
        0.0f, 0.0f, 1.0f, 0.5f, //c6
        0.5f, 0.5f, 0.5f, 0.5f  //c7
    };

    //Create a Vertex Array Object to store subsequent state
    glGenVertexArrays(1,&vao_);
    glBindVertexArray(vao_); 

    glGenBuffers(1, &ebo_);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                 sizeof(vertexIndices),
                 vertexIndices,
                 GL_STATIC_DRAW);

    glGenBuffers(1, &vbo_);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_);
    glBufferData(GL_ARRAY_BUFFER,
                 sizeof(vertexPositions) + sizeof(vertexColours),
                 NULL,
                 GL_STATIC_DRAW);

    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertexPositions), vertexPositions);
    glBufferSubData(GL_ARRAY_BUFFER, sizeof(vertexPositions), sizeof(vertexColours), vertexColours);

    //Set up attributes
    GLuint g_positionID = glGetAttribLocation(shader_.name(), "vertex");
    GLuint g_colourID   = glGetAttribLocation(shader_.name(), "vColour");

    glVertexAttribPointer(g_positionID,
                          4,
                          GL_FLOAT,
                          GL_FALSE,
                          0,
                          BUFFER_OFFSET(0));
    glVertexAttribPointer(g_colourID,
                          4,
                          GL_FLOAT,
                          GL_FALSE,
                          0,
                          BUFFER_OFFSET(sizeof(vertexPositions)));

    glEnableVertexAttribArray(g_positionID);
    glEnableVertexAttribArray(g_colourID); 

    glBindVertexArray(0); 
}

Model::~Model()
{
    glDeleteBuffers(1, &vbo_);
    glDeleteBuffers(1, &ebo_);
    glDeleteVertexArrays(1, &vao_);
}

void Model::draw(vmath::mat4 const& projectViewModel) const
{
    Shader::bind(shader_);

    glBindVertexArray(vao_);
    glEnable(GL_PRIMITIVE_RESTART);

    //Transfer object position
    shader_.setUniform("project_view_model", 
                       projectViewModel);

    //Draw vertices
    glDrawElements(GL_TRIANGLE_STRIP, 17, GL_UNSIGNED_SHORT, 0);

    //Unbind
    glBindVertexArray(0); 
    glDisable(GL_PRIMITIVE_RESTART);
}
