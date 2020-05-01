#ifndef MESH_H
#define MESH_H
#include <glm/glm.hpp>
#include <GL/glew.h>
class Vertex
{
    public:
    Vertex(const glm::vec3 pos)
    {
        this->pos=pos;
    }
    inline glm::vec3& GetPos() { return pos; }
    private:
    glm::vec3 pos;
};
class Mesh
{
    public:
        Mesh(Vertex *,unsigned int);
        void Draw();
        virtual ~Mesh();
    protected:

    private:
    Mesh(const Mesh& other);
    void operator=(const Mesh& other);
    enum
    {
        POSITION_VB,
        NUM_BUFFERS
    };
    unsigned int draw_count;
    GLuint m_VertexArrayObject;
    GLuint m_VertexArrayBuffers[NUM_BUFFERS];
};

#endif // MESH_H
