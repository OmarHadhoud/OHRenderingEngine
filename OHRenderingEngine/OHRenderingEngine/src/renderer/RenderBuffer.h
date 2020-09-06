#ifndef RENDER_BUFFER_H
#define RENDER_BUFFER_H

#include "renderer/enums.h"

class RenderBuffer
{
public:
	RenderBuffer();
	~RenderBuffer();
	unsigned int GetId() const;
	//Binds the current render buffer to this
	void Bind() const;
	//Unbinds the render buffer
	void Unbind() const;
	//Creates the render buffer object
	void Create(float width, float height, BufferType bType) const;
private:
	unsigned int m_ID;
};

#endif	//RENDER_BUFFER_H
