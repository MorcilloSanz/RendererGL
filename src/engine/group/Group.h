#pragma once

#include <iostream>
#include <vector>
#include <memory>

#include "Polytope.h"

class Group {
private:
    std::vector<std::shared_ptr<Polytope>> polytopes;
    unsigned int primitive;
    float pointSize, lineWidth;
    bool showWire, visible;
    glm::mat4 modelMatrix;
public:
    Group(unsigned int _primitive, bool _showWire = false);
    Group();
    ~Group() = default;
public:
    inline void translate(const glm::vec3& v) { modelMatrix = glm::translate(modelMatrix, v); }
    inline void rotate(float degrees, const glm::vec3& axis) { modelMatrix = glm::rotate(modelMatrix, glm::radians(degrees), axis); }
    inline void scale(const glm::vec3& s) { modelMatrix = glm::scale(modelMatrix, s); }

    inline void add(const std::shared_ptr<Polytope>& polytope) { polytopes.push_back(polytope); }
    inline std::vector<std::shared_ptr<Polytope>>& getPolytopes() { return polytopes; }

    inline void setVisible(bool visible) { this->visible = visible; }
    inline bool isVisible() const { return visible; }

    inline void setShowWire(bool showWire) { this->showWire = showWire; }
    inline bool isShowWire() const { return showWire; }

    inline void setPrimitive(unsigned int primitive) { this->primitive = primitive; }
    inline unsigned int getPrimitive() const { return primitive; }

    inline void setModelMatrix(const glm::mat4& modelMatrix) { this->modelMatrix = modelMatrix; }
    inline glm::mat4& getModelMatrix() { return modelMatrix; }

    inline void setPointSize(float pointSize) { this->pointSize = pointSize; }
    inline void setLineWidth(float lineWidth) { this->lineWidth = lineWidth; }

    inline float getPointSize() const { return pointSize; }
    inline float getLineWidth() const { return lineWidth; }
};