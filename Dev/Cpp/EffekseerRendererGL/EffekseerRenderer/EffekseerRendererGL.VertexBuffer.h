
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
	GLuint m_buffer;

	int currentSize_ = 0;
	int initialSize_ = 0;
	uint32_t m_vertexRingStart;
	bool m_ringBufferLock;

	VertexBuffer(const Backend::GraphicsDeviceRef& graphicsDevice, int initialSize, int size, bool isDynamic);

public:
	virtual ~VertexBuffer();

	static VertexBuffer* Create(const Backend::GraphicsDeviceRef& graphicsDevice, int initialSize, int size, bool isDynamic);

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