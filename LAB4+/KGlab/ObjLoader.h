#pragma once


#include <list>
#include <windows.h>
#include <GL/gl.h>

struct ObjVertex
{
    double x = 0, y = 0, z = 0, w = 1;

    inline double* _ptr()
    {
        return reinterpret_cast<double*>(this);
    }
};

struct ObjTexCord
{
    double u = 0, v = 0, w = 1;

    inline double* _ptr()
    {
        return reinterpret_cast<double*>(this);
    }
};
struct ObjNormal
{
    double x = 0, y = 0, z = 0;
    inline double* _ptr()
    {
        return reinterpret_cast<double*>(this);
    }
};

struct ObjFace
{
    unsigned int VertexCount;

    std::list<ObjVertex> vertex;
    std::list<ObjTexCord> texCoord;
    std::list<ObjNormal> normal;
};

class ObjModel
{
    std::list<ObjFace> Faces;

    int listId = -1;

  public:
    ObjModel()
    {
    }
    ~ObjModel()
    {
        glDeleteLists(listId, 1);
    }

    inline void RenderModel(int mode)
    {
        for (std::list<ObjFace>::iterator it = Faces.begin(); it != Faces.end(); ++it)
        {
            glBegin(mode);

            bool f_n = (it->normal.size() != 0);
            bool f_t = (it->texCoord.size() != 0);
            auto it_n = it->normal.begin();
            auto it_t = it->texCoord.begin();

            for (auto j = it->vertex.begin(); j != it->vertex.end(); ++j)
            {
                if (f_n)
                {
                    glNormal3dv((it_n++)->_ptr());
                }

                if (f_t)
                {
                    glTexCoord2dv((it_t++)->_ptr());
                }

                glVertex4dv(j->_ptr());
            }

            glEnd();
        }

        Faces.clear();
    }
    inline void Draw(GLenum mode = 0x0009) // GL_POLYGON
    {
        glCallList(listId);
    }

    int LoadModel(const char* filename);

    // ObjFile(const ObjFile)
};
