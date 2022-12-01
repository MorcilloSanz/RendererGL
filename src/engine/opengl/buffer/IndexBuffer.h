#include <iostream>
#include <vector>

#include "Buffer.h"

class IndexBuffer : public Buffer {
public:
    IndexBuffer();
    IndexBuffer(const std::vector<unsigned int> _ndices);
    IndexBuffer(const IndexBuffer& indexBuffer);
    IndexBuffer(IndexBuffer&& indexBuffer) noexcept;
    IndexBuffer& operator=(const IndexBuffer& indexBuffer);
    ~IndexBuffer();
private:
    void initBuffer(std::vector<unsigned int> indices);
    void initBuffer() override;
public:
    void bind() override;
    void unbind() override;
    void updateIndices(const std::vector<unsigned int>& indices);
public:
    inline unsigned int* getIndicesData() { return (unsigned int*)glMapBuffer(GL_ELEMENT_ARRAY_BUFFER, GL_READ_ONLY); }
};