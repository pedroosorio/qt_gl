#ifndef PROPERTY_H
#define PROPERTY_H

#include "glm/glm.hpp"
#include <QOpenGLFunctions_4_0_Core>
#include <string>

extern QOpenGLFunctions_4_0_Core* GLctx;

template <typename T>
class Property{
public:
    Property(std::string n){
        name = n;
        dirty = false;
    }
    Property(T &p, std::string n){
        prop = p;
        name = n;
        dirty = true;
    }
    ~Property(){}

    void setUniformLocation(GLuint loc){
        location = loc;
    }
    void setProperty(T p){
        prop = p;
        dirty = true;
    }
    T& getProperty() { return prop; }
    bool isDirty() { return dirty; }
    void setDirty() { dirty = true; }
    void update(){
        if(!dirty) return;
        loadUniform(location, prop);
        dirty = false;
    }
protected:
    // Uniform Utility functions
    void loadUniform(GLuint loc, bool val)
    {
        GLctx->glUniform1f(loc, val*1.0f);
    }
    void loadUniform(GLuint loc, float val)
    {
        GLctx->glUniform1f(loc, val);
    }
    void loadUniform(GLuint loc, glm::vec3 val)
    {
        GLctx->glUniform3f(loc, val.x, val.y, val.z);
    }
    void loadUniform(GLuint loc, glm::mat4 val)
    {
        GLctx->glUniformMatrix4fv(loc, 1, GL_FALSE, &val[0][0]);
    }
private:
    T prop;
    std::string name;
    GLuint location;
    bool dirty;
};

#endif // PROPERTY_H
