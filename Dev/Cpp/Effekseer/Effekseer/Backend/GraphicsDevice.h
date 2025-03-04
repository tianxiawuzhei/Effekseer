
#ifndef __EFFEKSEER_GRAPHICS_DEVICE_H__
#define __EFFEKSEER_GRAPHICS_DEVICE_H__

#include "../Effekseer.Base.Pre.h"
#include "../Effekseer.Color.h"
#include "../Utils/Effekseer.CustomAllocator.h"
#include <array>
#include <stdint.h>
#include <string>

namespace Effekseer
{
namespace Backend
{

class GraphicsDevice;
class VertexBuffer;
class IndexBuffer;
class UniformBuffer;
class Shader;
class VertexLayout;
class FrameBuffer;
class Texture;
class RenderPass;
class PipelineState;
class UniformLayout;

using GraphicsDeviceRef = RefPtr<GraphicsDevice>;
using VertexBufferRef = RefPtr<VertexBuffer>;
using IndexBufferRef = RefPtr<IndexBuffer>;
using UniformBufferRef = RefPtr<UniformBuffer>;
using ShaderRef = RefPtr<Shader>;
using VertexLayoutRef = RefPtr<VertexLayout>;
using FrameBufferRef = RefPtr<FrameBuffer>;
using TextureRef = RefPtr<Texture>;
using RenderPassRef = RefPtr<RenderPass>;
using PipelineStateRef = RefPtr<PipelineState>;
using UniformLayoutRef = RefPtr<UniformLayout>;

static const int32_t RenderTargetMax = 4;

enum class TextureFormatType
{
	R8G8B8A8_UNORM,
	B8G8R8A8_UNORM,
	R8_UNORM,
	R16_FLOAT,
	R32_FLOAT,
	R16G16_FLOAT,
	R16G16B16A16_FLOAT,
	R32G32B32A32_FLOAT,
	BC1,
	BC2,
	BC3,
	R8G8B8A8_UNORM_SRGB,
	B8G8R8A8_UNORM_SRGB,
	BC1_SRGB,
	BC2_SRGB,
	BC3_SRGB,

	//! You don't need to implement DepthTexture for a runtime
	D32,

	//! You don't need to implement DepthTexture for a runtime
	D24S8,

	//! You don't need to implement DepthTexture for a runtime
	D32S8,
	Unknown,
};

inline bool IsDepthTextureFormat(TextureFormatType format)
{
	return format == TextureFormatType::D24S8 ||
		   format == TextureFormatType::D32S8 ||
		   format == TextureFormatType::D32;
}

enum class IndexBufferStrideType
{
	Stride2,
	Stride4,
};

enum class UniformBufferLayoutElementType
{
	Vector4,
	Matrix44,
};

enum class ShaderStageType
{
	Vertex,
	Pixel,
};

struct UniformLayoutElement
{
	ShaderStageType Stage = ShaderStageType::Vertex;
	CustomString<char> Name;
	UniformBufferLayoutElementType Type;
	int32_t Count = 1;

	//! Ignored in UniformBuffer
	int32_t Offset;
};

/**
	@brief	Layouts in an uniform buffer
	@note
	Only for OpenGL
*/
class UniformLayout
	: public ReferenceObject
{
private:
	CustomVector<CustomString<char>> textures_;
	CustomVector<UniformLayoutElement> elements_;

public:
	UniformLayout(CustomVector<CustomString<char>> textures, CustomVector<UniformLayoutElement> elements)
		: textures_(std::move(textures))
		, elements_(std::move(elements))
	{
	}
	virtual ~UniformLayout() = default;

	const CustomVector<CustomString<char>>& GetTextures() const
	{
		return textures_;
	}

	const CustomVector<UniformLayoutElement>& GetElements() const
	{
		return elements_;
	}
};

class VertexBuffer
	: public ReferenceObject
{
public:
	VertexBuffer() = default;
	virtual ~VertexBuffer() = default;

	virtual void UpdateData(const void* src, int32_t size, int32_t offset) = 0;
};

class IndexBuffer
	: public ReferenceObject
{
protected:
	IndexBufferStrideType strideType_ = {};
	int32_t elementCount_ = {};

public:
	IndexBuffer() = default;
	virtual ~IndexBuffer() = default;

	virtual void UpdateData(const void* src, int32_t size, int32_t offset) = 0;

	IndexBufferStrideType GetStrideType() const
	{
		return strideType_;
	}
	int32_t GetElementCount() const
	{
		return elementCount_;
	}
};

class VertexLayout
	: public ReferenceObject
{
public:
	VertexLayout() = default;
	virtual ~VertexLayout() = default;
};

class UniformBuffer
	: public ReferenceObject
{
public:
	UniformBuffer() = default;
	virtual ~UniformBuffer() = default;
};

class PipelineState
	: public ReferenceObject
{
public:
	PipelineState() = default;
	virtual ~PipelineState() = default;
};

enum class TextureUsageType : uint32_t
{
	None = 0,
	//! You don't need to implement RenderTarget flag for a runtime
	RenderTarget = 1 << 0,
	Array = 1 << 1,
	External = 1 << 2,
};

inline TextureUsageType operator|(TextureUsageType lhs, TextureUsageType rhs)
{
	return static_cast<TextureUsageType>(static_cast<uint32_t>(lhs) | static_cast<uint32_t>(rhs));
}

inline TextureUsageType operator&(TextureUsageType lhs, TextureUsageType rhs)
{
	return static_cast<TextureUsageType>(static_cast<uint32_t>(lhs) & static_cast<uint32_t>(rhs));
}

struct TextureParameter
{
	TextureUsageType Usage = TextureUsageType::None;
	TextureFormatType Format = TextureFormatType::R8G8B8A8_UNORM;
	int32_t Dimension = 2;
	std::array<int32_t, 3> Size = {1, 1, 1};
	int32_t MipLevelCount = 1;

	//! You don't need to implement SampleCount for a runtime
	int SampleCount = 1;
};

class Texture
	: public ReferenceObject
{
protected:
	TextureParameter param_;

public:
	Texture() = default;
	virtual ~Texture() = default;

	TextureParameter GetParameter() const
	{
		return param_;
	}
};

class Shader
	: public ReferenceObject
{
public:
	Shader() = default;
	virtual ~Shader() = default;
};

class ComputeBuffer
	: public ReferenceObject
{
public:
	ComputeBuffer() = default;
	virtual ~ComputeBuffer() = default;
};

class FrameBuffer
	: public ReferenceObject
{
public:
	FrameBuffer() = default;
	virtual ~FrameBuffer() = default;
};

class RenderPass
	: public ReferenceObject
{
public:
	RenderPass() = default;
	virtual ~RenderPass() = default;
};

enum class TextureWrapType
{
	Clamp,
	Repeat,
};

enum class TextureSamplingType
{
	Linear,
	Nearest,
};

struct DrawParameter
{
public:
	static const int TextureSlotCount = 8;

	VertexBufferRef VertexBufferPtr;
	IndexBufferRef IndexBufferPtr;
	PipelineStateRef PipelineStatePtr;

	UniformBufferRef VertexUniformBufferPtr;
	UniformBufferRef PixelUniformBufferPtr;

	int32_t TextureCount = 0;
	std::array<TextureRef, TextureSlotCount> TexturePtrs;
	std::array<TextureWrapType, TextureSlotCount> TextureWrapTypes;
	std::array<TextureSamplingType, TextureSlotCount> TextureSamplingTypes;

	int32_t PrimitiveCount = 0;
	int32_t InstanceCount = 0;
};

enum class VertexLayoutFormat
{
	R32_FLOAT,
	R32G32_FLOAT,
	R32G32B32_FLOAT,
	R32G32B32A32_FLOAT,
	R8G8B8A8_UNORM,
	R8G8B8A8_UINT,
};

struct VertexLayoutElement
{
	VertexLayoutFormat Format;

	//! only for OpenGL
	CustomString<char> Name;

	//! only for DirectX
	CustomString<char> SemanticName;

	//! only for DirectX
	int32_t SemanticIndex = 0;
};

enum class TopologyType
{
	Triangle,
	Line,
	Point,
};

enum class CullingType
{
	Clockwise,
	CounterClockwise,
	DoubleSide,
};

enum class BlendEquationType
{
	Add,
	Sub,
	ReverseSub,
	Min,
	Max,
};

enum class BlendFuncType
{
	Zero,
	One,
	SrcColor,
	OneMinusSrcColor,
	SrcAlpha,
	OneMinusSrcAlpha,
	DstAlpha,
	OneMinusDstAlpha,
	DstColor,
	OneMinusDstColor,
};

enum class DepthFuncType
{
	Never,
	Less,
	Equal,
	LessEqual,
	Greater,
	NotEqual,
	GreaterEqual,
	Always,
};

enum class CompareFuncType
{
	Never,
	Less,
	Equal,
	LessEqual,
	Greater,
	NotEqual,
	GreaterEqual,
	Always,
};

struct PipelineStateParameter
{
	TopologyType Topology = TopologyType::Triangle;

	CullingType Culling = CullingType::DoubleSide;

	bool IsBlendEnabled = true;

	BlendFuncType BlendSrcFunc = BlendFuncType::SrcAlpha;
	BlendFuncType BlendDstFunc = BlendFuncType::OneMinusSrcAlpha;
	BlendFuncType BlendSrcFuncAlpha = BlendFuncType::SrcAlpha;
	BlendFuncType BlendDstFuncAlpha = BlendFuncType::OneMinusSrcAlpha;

	BlendEquationType BlendEquationRGB = BlendEquationType::Add;
	BlendEquationType BlendEquationAlpha = BlendEquationType::Add;

	bool IsDepthTestEnabled = false;
	bool IsDepthWriteEnabled = false;
	DepthFuncType DepthFunc = DepthFuncType::Less;

	ShaderRef ShaderPtr;
	VertexLayoutRef VertexLayoutPtr;
	FrameBufferRef FrameBufferPtr;
};

/**
	@brief	Render texture
	@note
	You don't need to implement it to run Effekseer Runtime
*/
struct RenderTextureParameter
{
	TextureFormatType Format = TextureFormatType::R8G8B8A8_UNORM;
	std::array<int32_t, 2> Size;
	int SamplingCount = 1;
};

/**
	@brief	Render texture
	@note
	You don't need to implement it to run Effekseer Runtime
*/
struct DepthTextureParameter
{
	TextureFormatType Format = TextureFormatType::R8G8B8A8_UNORM;
	std::array<int32_t, 2> Size;
	int SamplingCount = 1;
};

class GraphicsDevice
	: public ReferenceObject
{
public:
	GraphicsDevice() = default;
	virtual ~GraphicsDevice() = default;

	/**
		@brief	Create VertexBuffer
		@param	size	the size of buffer
		@param	initialData	the initial data of buffer. If it is null, not initialized.
		@param	isDynamic	whether is the buffer dynamic? (for DirectX9, 11 or OpenGL)
		@return	VertexBuffer
	*/
	virtual VertexBufferRef CreateVertexBuffer(int32_t size, const void* initialData, bool isDynamic)
	{
		return VertexBufferRef{};
	}

	/**
		@brief	Create IndexBuffer
		@param	elementCount	the number of element
		@param	initialData	the initial data of buffer. If it is null, not initialized.
		@param	stride	stride type
		@return	IndexBuffer
	*/
	virtual IndexBufferRef CreateIndexBuffer(int32_t elementCount, const void* initialData, IndexBufferStrideType stride)
	{
		return IndexBufferRef{};
	}

	/**
		@brief	Update content of a vertex buffer
		@param	buffer	buffer
		@param	size	the size of updated buffer
		@param	offset	the offset of updated buffer
		@param	data	updating data
		@return	Succeeded in updating?
	*/
	virtual bool UpdateVertexBuffer(VertexBufferRef& buffer, int32_t size, int32_t offset, const void* data)
	{
		return false;
	}

	/**
		@brief	Update content of a index buffer
		@param	buffer	buffer
		@param	size	the size of updated buffer
		@param	offset	the offset of updated buffer
		@param	data	updating data
		@return	Succeeded in updating?
	*/
	virtual bool UpdateIndexBuffer(IndexBufferRef& buffer, int32_t size, int32_t offset, const void* data)
	{
		return false;
	}

	/**
		@brief	Update content of an uniform buffer
		@param	buffer	buffer
		@param	size	the size of updated buffer
		@param	offset	the offset of updated buffer
		@param	data	updating data
		@return	Succeeded in updating?
	*/
	virtual bool UpdateUniformBuffer(UniformBufferRef& buffer, int32_t size, int32_t offset, const void* data)
	{
		return false;
	}

	/**
		@brief	Create VertexLayout
		@param	elements	a pointer of array of vertex layout elements
		@param	elementCount	the number of elements
	*/
	virtual VertexLayoutRef CreateVertexLayout(const VertexLayoutElement* elements, int32_t elementCount)
	{
		return RefPtr<VertexLayout>{};
	}

	/**
		@brief	Create UniformBuffer
		@param	size	the size of buffer
		@param	initialData	the initial data of buffer. If it is null, not initialized.
		@return	UniformBuffer
	*/
	virtual UniformBufferRef CreateUniformBuffer(int32_t size, const void* initialData)
	{
		return UniformBufferRef{};
	}

	virtual PipelineStateRef CreatePipelineState(const PipelineStateParameter& param)
	{
		return PipelineStateRef{};
	}

	virtual FrameBufferRef CreateFrameBuffer(const TextureFormatType* formats, int32_t formatCount, TextureFormatType depthFormat)
	{
		return FrameBufferRef{};
	}

	virtual RenderPassRef CreateRenderPass(FixedSizeVector<TextureRef, RenderTargetMax>& textures, TextureRef& depthTexture)
	{
		return RenderPassRef{};
	}

	virtual TextureRef CreateTexture(const TextureParameter& param, const CustomVector<uint8_t>& initialData = CustomVector<uint8_t>())
	{
		return TextureRef{};
	}

	virtual TextureRef CreateRenderTexture(const RenderTextureParameter& param)
	{
		return TextureRef{};
	}

	virtual TextureRef CreateDepthTexture(const DepthTextureParameter& param)
	{
		return TextureRef{};
	}

	virtual bool CopyTexture(TextureRef& dst, TextureRef& src, const std::array<int, 3>& dstPos, const std::array<int, 3>& srcPos, const std::array<int, 3>& size, int32_t dstLayer, int32_t srcLayer)
	{
		return false;
	}

	/**
		@brief	Create Shader from key
		@param	key	a key which specifies a shader
		@return	Shader
	*/
	virtual ShaderRef CreateShaderFromKey(const char* key)
	{
		return ShaderRef{};
	}

	virtual ShaderRef CreateShaderFromCodes(const CustomVector<StringView<char>>& vsCodes, const CustomVector<StringView<char>>& psCodes, UniformLayoutRef layout = nullptr)
	{
		return ShaderRef{};
	}

	virtual ShaderRef CreateShaderFromBinary(const void* vsData, int32_t vsDataSize, const void* psData, int32_t psDataSize)
	{
		return ShaderRef{};
	}

	/**
		@brief	Create ComputeBuffer
		@param	size	the size of buffer
		@param	initialData	the initial data of buffer. If it is null, not initialized.
		@return	ComputeBuffer
	*/
	// virtual ComputeBuffer* CreateComputeBuffer(int32_t size, const void* initialData)
	// {
	// 	return nullptr;
	// }

	virtual void Draw(const DrawParameter& drawParam)
	{
	}

	virtual void BeginRenderPass(RenderPassRef& renderPass, bool isColorCleared, bool isDepthCleared, Color clearColor)
	{
	}

	virtual void EndRenderPass()
	{
	}

	virtual std::string GetDeviceName() const
	{
		return "";
	}
};

inline int32_t GetVertexLayoutFormatSize(VertexLayoutFormat format)
{
	int32_t size = 0;
	if (format == Effekseer::Backend::VertexLayoutFormat::R8G8B8A8_UINT || format == Effekseer::Backend::VertexLayoutFormat::R8G8B8A8_UNORM)
	{
		size = 4;
	}
	else if (format == Effekseer::Backend::VertexLayoutFormat::R32_FLOAT)
	{
		size = sizeof(float) * 1;
	}
	else if (format == Effekseer::Backend::VertexLayoutFormat::R32G32_FLOAT)
	{
		size = sizeof(float) * 2;
	}
	else if (format == Effekseer::Backend::VertexLayoutFormat::R32G32B32_FLOAT)
	{
		size = sizeof(float) * 3;
	}
	else if (format == Effekseer::Backend::VertexLayoutFormat::R32G32B32A32_FLOAT)
	{
		size = sizeof(float) * 4;
	}
	else
	{
		assert(0);
	}

	return size;
}

} // namespace Backend
} // namespace Effekseer

#endif