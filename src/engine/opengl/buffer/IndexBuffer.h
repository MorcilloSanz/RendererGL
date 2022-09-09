#include <iostream>
#include <vector>

#include "Buffer.h"

class IndexBuffer : public Buffer {
private:
    std::vector<unsigned int> indices;
public:
    IndexBuffer();
    IndexBuffer(const std::vector<unsigned int> _indices);
    IndexBuffer(const IndexBuffer& indexBuffer);
    IndexBuffer(IndexBuffer&& indexBuffer) noexcept;
    IndexBuffer& operator=(const IndexBuffer& indexBuffer);
    ~IndexBuffer();
private:
    void initBuffer() override;
public:
    void bind() override;
    void unbind() override;
    void updateIndices(const std::vector<unsigned int>& indices, bool copy2memory = false);
public:
    inline std::vector<unsigned int>& getIndices() {
        return indices;
    }

    inline unsigned int* getMapBuffer() {
        return (unsigned int*)glMapBuffer(GL_ELEMENT_ARRAY_BUFFER, GL_READ_ONLY);
    }
};