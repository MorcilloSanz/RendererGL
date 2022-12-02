#include <iostream>
#include <vector>

#include "Buffer.h"

class IndexBuffer : public Buffer {
private:
    size_t length;
public:
    IndexBuffer();
    IndexBuffer(const std::vector<unsigned int> indices);
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
    std::vector<unsigned int> getIndices();
};