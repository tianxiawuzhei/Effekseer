
#pragma once

//----------------------------------------------------------------------------------
// Include
//----------------------------------------------------------------------------------
#include "../../EffekseerRendererCommon/EffekseerRenderer.VertexBufferBase.h"
#include "EffekseerRendererGL.DeviceObject.h"
#include "EffekseerRendererGL.RendererImplemented.h"

//-----------------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------------
namespace EffekseerRendererGL
{
//-----------------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------------
class VertexBuffer : public DeviceObject, public ::EffekseerRenderer::VertexBufferBase
{
private:

	struct RingBuffer
	{
		GLuint id;
		size_t size;
	};

	std::vector<RingBuffer> buffers_;
	size_t bufferOffset_;

	GLuint m_buffer;

	uint32_t m_vertexRingStart;
	uint32_t m_vertexRingOffset;
	bool m_ringBufferLock;

	VertexBuffer(const Backend::GraphicsDeviceRef& graphicsDevice, int size, bool isDynamic);

public:
	virtual ~VertexBuffer();

	static VertexBuffer* Create(const Backend::GraphicsDeviceRef& graphicsDevice, int size, bool isDynamic);

	GLuint GetInterface();

public:
	void OnLostDevice() override;
	void OnResetDevice() override;

public:
	void Lock();
	bool RingBufferLock(int32_t size, int32_t& offset, void*& data, int32_t alignment) override;
	bool TryRingBufferLock(int32_t size, int32_t& offset, void*& data, int32_t alignment) override;

	void Unlock();

	bool IsValid();
};

//-----------------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------------
} // namespace EffekseerRendererGL
  //-----------------------------------------------------------------------------------
  //
  //-----------------------------------------------------------------------------------