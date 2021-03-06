/******************************************************************************
 * The MIT License (MIT)
 *
 * Copyright (c) 2016-2018 Baldur Karlsson
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 ******************************************************************************/

#include <QDebug>
#include "Code/QRDUtils.h"
#include "QRDInterface.h"

rdcstr CommonPipelineState::GetResourceLayout(ResourceId id)
{
  if(IsCaptureLoaded())
  {
    if(IsCaptureVK())
    {
      for(const VKPipe::ImageData &i : m_Vulkan->images)
      {
        if(i.resourceId == id)
          return i.layouts[0].name;
      }
    }

    if(IsCaptureD3D12())
    {
      for(const D3D12Pipe::ResourceData &r : m_D3D12->resourceStates)
      {
        if(r.resourceId == id)
          return r.states[0].name;
      }
    }
  }

  return lit("Unknown");
}

rdcstr CommonPipelineState::Abbrev(ShaderStage stage)
{
  if(IsCaptureD3D11() || (!IsCaptureLoaded() && defaultType == GraphicsAPI::D3D11) ||
     IsCaptureD3D12() || (!IsCaptureLoaded() && defaultType == GraphicsAPI::D3D12))
  {
    switch(stage)
    {
      case ShaderStage::Vertex: return lit("VS");
      case ShaderStage::Hull: return lit("HS");
      case ShaderStage::Domain: return lit("DS");
      case ShaderStage::Geometry: return lit("GS");
      case ShaderStage::Pixel: return lit("PS");
      case ShaderStage::Compute: return lit("CS");
      default: break;
    }
  }
  else if(IsCaptureGL() || (!IsCaptureLoaded() && defaultType == GraphicsAPI::OpenGL) ||
          IsCaptureVK() || (!IsCaptureLoaded() && defaultType == GraphicsAPI::Vulkan))
  {
    switch(stage)
    {
      case ShaderStage::Vertex: return lit("VS");
      case ShaderStage::Tess_Control: return lit("TCS");
      case ShaderStage::Tess_Eval: return lit("TES");
      case ShaderStage::Geometry: return lit("GS");
      case ShaderStage::Fragment: return lit("FS");
      case ShaderStage::Compute: return lit("CS");
      default: break;
    }
  }

  return lit("?S");
}

rdcstr CommonPipelineState::OutputAbbrev()
{
  if(IsCaptureGL() || (!IsCaptureLoaded() && defaultType == GraphicsAPI::OpenGL) || IsCaptureVK() ||
     (!IsCaptureLoaded() && defaultType == GraphicsAPI::Vulkan))
  {
    return lit("FB");
  }

  return lit("RT");
}

const D3D11Pipe::Shader &CommonPipelineState::GetD3D11Stage(ShaderStage stage)
{
  if(stage == ShaderStage::Vertex)
    return m_D3D11->vertexShader;
  if(stage == ShaderStage::Domain)
    return m_D3D11->domainShader;
  if(stage == ShaderStage::Hull)
    return m_D3D11->hullShader;
  if(stage == ShaderStage::Geometry)
    return m_D3D11->geometryShader;
  if(stage == ShaderStage::Pixel)
    return m_D3D11->pixelShader;
  if(stage == ShaderStage::Compute)
    return m_D3D11->computeShader;

  qCritical() << "Error - invalid stage " << (int)stage;
  return m_D3D11->computeShader;
}

const D3D12Pipe::Shader &CommonPipelineState::GetD3D12Stage(ShaderStage stage)
{
  if(stage == ShaderStage::Vertex)
    return m_D3D12->vertexShader;
  if(stage == ShaderStage::Domain)
    return m_D3D12->domainShader;
  if(stage == ShaderStage::Hull)
    return m_D3D12->hullShader;
  if(stage == ShaderStage::Geometry)
    return m_D3D12->geometryShader;
  if(stage == ShaderStage::Pixel)
    return m_D3D12->pixelShader;
  if(stage == ShaderStage::Compute)
    return m_D3D12->computeShader;

  qCritical() << "Error - invalid stage " << (int)stage;
  return m_D3D12->computeShader;
}

const GLPipe::Shader &CommonPipelineState::GetGLStage(ShaderStage stage)
{
  if(stage == ShaderStage::Vertex)
    return m_GL->vertexShader;
  if(stage == ShaderStage::Tess_Control)
    return m_GL->tessControlShader;
  if(stage == ShaderStage::Tess_Eval)
    return m_GL->tessEvalShader;
  if(stage == ShaderStage::Geometry)
    return m_GL->geometryShader;
  if(stage == ShaderStage::Fragment)
    return m_GL->fragmentShader;
  if(stage == ShaderStage::Compute)
    return m_GL->computeShader;

  qCritical() << "Error - invalid stage " << (int)stage;
  return m_GL->computeShader;
}

const VKPipe::Shader &CommonPipelineState::GetVulkanStage(ShaderStage stage)
{
  if(stage == ShaderStage::Vertex)
    return m_Vulkan->vertexShader;
  if(stage == ShaderStage::Tess_Control)
    return m_Vulkan->tessControlShader;
  if(stage == ShaderStage::Tess_Eval)
    return m_Vulkan->tessEvalShader;
  if(stage == ShaderStage::Geometry)
    return m_Vulkan->geometryShader;
  if(stage == ShaderStage::Fragment)
    return m_Vulkan->fragmentShader;
  if(stage == ShaderStage::Compute)
    return m_Vulkan->computeShader;

  qCritical() << "Error - invalid stage " << (int)stage;
  return m_Vulkan->computeShader;
}

rdcstr CommonPipelineState::GetShaderExtension()
{
  if(IsCaptureGL() || (!IsCaptureLoaded() && defaultType == GraphicsAPI::OpenGL) || IsCaptureVK() ||
     (!IsCaptureLoaded() && defaultType == GraphicsAPI::Vulkan))
  {
    return lit("glsl");
  }

  return lit("hlsl");
}

Viewport CommonPipelineState::GetViewport(int index)
{
  Viewport ret = {};

  if(IsCaptureLoaded())
  {
    if(IsCaptureD3D11() && index < m_D3D11->rasterizer.viewports.count())
    {
      return m_D3D11->rasterizer.viewports[index];
    }
    else if(IsCaptureD3D12() && index < m_D3D12->rasterizer.viewports.count())
    {
      return m_D3D12->rasterizer.viewports[index];
    }
    else if(IsCaptureGL() && index < m_GL->rasterizer.viewports.count())
    {
      return m_GL->rasterizer.viewports[index];
    }
    else if(IsCaptureVK() && index < m_Vulkan->viewportScissor.viewportScissors.count())
    {
      return m_Vulkan->viewportScissor.viewportScissors[index].vp;
    }
  }

  return ret;
}

Scissor CommonPipelineState::GetScissor(int index)
{
  Scissor ret = {};

  if(IsCaptureLoaded())
  {
    if(IsCaptureD3D11() && index < m_D3D11->rasterizer.viewports.count())
    {
      return m_D3D11->rasterizer.scissors[index];
    }
    else if(IsCaptureD3D12() && index < m_D3D12->rasterizer.viewports.count())
    {
      return m_D3D12->rasterizer.scissors[index];
    }
    else if(IsCaptureGL() && index < m_GL->rasterizer.viewports.count())
    {
      return m_GL->rasterizer.scissors[index];
    }
    else if(IsCaptureVK() && index < m_Vulkan->viewportScissor.viewportScissors.count())
    {
      return m_Vulkan->viewportScissor.viewportScissors[index].scissor;
    }
  }

  return ret;
}

const ShaderBindpointMapping &CommonPipelineState::GetBindpointMapping(ShaderStage stage)
{
  if(IsCaptureLoaded())
  {
    if(IsCaptureD3D11())
    {
      switch(stage)
      {
        case ShaderStage::Vertex: return m_D3D11->vertexShader.bindpointMapping;
        case ShaderStage::Domain: return m_D3D11->domainShader.bindpointMapping;
        case ShaderStage::Hull: return m_D3D11->hullShader.bindpointMapping;
        case ShaderStage::Geometry: return m_D3D11->geometryShader.bindpointMapping;
        case ShaderStage::Pixel: return m_D3D11->pixelShader.bindpointMapping;
        case ShaderStage::Compute: return m_D3D11->computeShader.bindpointMapping;
        default: break;
      }
    }
    else if(IsCaptureD3D12())
    {
      switch(stage)
      {
        case ShaderStage::Vertex: return m_D3D12->vertexShader.bindpointMapping;
        case ShaderStage::Domain: return m_D3D12->domainShader.bindpointMapping;
        case ShaderStage::Hull: return m_D3D12->hullShader.bindpointMapping;
        case ShaderStage::Geometry: return m_D3D12->geometryShader.bindpointMapping;
        case ShaderStage::Pixel: return m_D3D12->pixelShader.bindpointMapping;
        case ShaderStage::Compute: return m_D3D12->computeShader.bindpointMapping;
        default: break;
      }
    }
    else if(IsCaptureGL())
    {
      switch(stage)
      {
        case ShaderStage::Vertex: return m_GL->vertexShader.bindpointMapping;
        case ShaderStage::Tess_Control: return m_GL->tessControlShader.bindpointMapping;
        case ShaderStage::Tess_Eval: return m_GL->tessEvalShader.bindpointMapping;
        case ShaderStage::Geometry: return m_GL->geometryShader.bindpointMapping;
        case ShaderStage::Fragment: return m_GL->fragmentShader.bindpointMapping;
        case ShaderStage::Compute: return m_GL->computeShader.bindpointMapping;
        default: break;
      }
    }
    else if(IsCaptureVK())
    {
      switch(stage)
      {
        case ShaderStage::Vertex: return m_Vulkan->vertexShader.bindpointMapping;
        case ShaderStage::Tess_Control: return m_Vulkan->tessControlShader.bindpointMapping;
        case ShaderStage::Tess_Eval: return m_Vulkan->tessEvalShader.bindpointMapping;
        case ShaderStage::Geometry: return m_Vulkan->geometryShader.bindpointMapping;
        case ShaderStage::Fragment: return m_Vulkan->fragmentShader.bindpointMapping;
        case ShaderStage::Compute: return m_Vulkan->computeShader.bindpointMapping;
        default: break;
      }
    }
  }

  static ShaderBindpointMapping empty;

  return empty;
}

const ShaderReflection *CommonPipelineState::GetShaderReflection(ShaderStage stage)
{
  if(IsCaptureLoaded())
  {
    if(IsCaptureD3D11())
    {
      switch(stage)
      {
        case ShaderStage::Vertex: return m_D3D11->vertexShader.reflection;
        case ShaderStage::Domain: return m_D3D11->domainShader.reflection;
        case ShaderStage::Hull: return m_D3D11->hullShader.reflection;
        case ShaderStage::Geometry: return m_D3D11->geometryShader.reflection;
        case ShaderStage::Pixel: return m_D3D11->pixelShader.reflection;
        case ShaderStage::Compute: return m_D3D11->computeShader.reflection;
        default: break;
      }
    }
    else if(IsCaptureD3D12())
    {
      switch(stage)
      {
        case ShaderStage::Vertex: return m_D3D12->vertexShader.reflection;
        case ShaderStage::Domain: return m_D3D12->domainShader.reflection;
        case ShaderStage::Hull: return m_D3D12->hullShader.reflection;
        case ShaderStage::Geometry: return m_D3D12->geometryShader.reflection;
        case ShaderStage::Pixel: return m_D3D12->pixelShader.reflection;
        case ShaderStage::Compute: return m_D3D12->computeShader.reflection;
        default: break;
      }
    }
    else if(IsCaptureGL())
    {
      switch(stage)
      {
        case ShaderStage::Vertex: return m_GL->vertexShader.reflection;
        case ShaderStage::Tess_Control: return m_GL->tessControlShader.reflection;
        case ShaderStage::Tess_Eval: return m_GL->tessEvalShader.reflection;
        case ShaderStage::Geometry: return m_GL->geometryShader.reflection;
        case ShaderStage::Fragment: return m_GL->fragmentShader.reflection;
        case ShaderStage::Compute: return m_GL->computeShader.reflection;
        default: break;
      }
    }
    else if(IsCaptureVK())
    {
      switch(stage)
      {
        case ShaderStage::Vertex: return m_Vulkan->vertexShader.reflection;
        case ShaderStage::Tess_Control: return m_Vulkan->tessControlShader.reflection;
        case ShaderStage::Tess_Eval: return m_Vulkan->tessEvalShader.reflection;
        case ShaderStage::Geometry: return m_Vulkan->geometryShader.reflection;
        case ShaderStage::Fragment: return m_Vulkan->fragmentShader.reflection;
        case ShaderStage::Compute: return m_Vulkan->computeShader.reflection;
        default: break;
      }
    }
  }

  return NULL;
}

ResourceId CommonPipelineState::GetComputePipelineObject()
{
  if(IsCaptureLoaded() && IsCaptureVK())
  {
    return m_Vulkan->compute.pipelineResourceId;
  }
  else if(IsCaptureLoaded() && IsCaptureD3D12())
  {
    return m_D3D12->pipelineResourceId;
  }

  return ResourceId();
}

ResourceId CommonPipelineState::GetGraphicsPipelineObject()
{
  if(IsCaptureLoaded() && IsCaptureVK())
  {
    return m_Vulkan->graphics.pipelineResourceId;
  }
  else if(IsCaptureLoaded() && IsCaptureD3D12())
  {
    return m_D3D12->pipelineResourceId;
  }

  return ResourceId();
}

rdcstr CommonPipelineState::GetShaderEntryPoint(ShaderStage stage)
{
  if(IsCaptureLoaded() && IsCaptureVK())
  {
    switch(stage)
    {
      case ShaderStage::Vertex: return m_Vulkan->vertexShader.entryPoint;
      case ShaderStage::Tess_Control: return m_Vulkan->tessControlShader.entryPoint;
      case ShaderStage::Tess_Eval: return m_Vulkan->tessEvalShader.entryPoint;
      case ShaderStage::Geometry: return m_Vulkan->geometryShader.entryPoint;
      case ShaderStage::Fragment: return m_Vulkan->fragmentShader.entryPoint;
      case ShaderStage::Compute: return m_Vulkan->computeShader.entryPoint;
      default: break;
    }
  }

  return "";
}

ResourceId CommonPipelineState::GetShader(ShaderStage stage)
{
  if(IsCaptureLoaded())
  {
    if(IsCaptureD3D11())
    {
      switch(stage)
      {
        case ShaderStage::Vertex: return m_D3D11->vertexShader.resourceId;
        case ShaderStage::Domain: return m_D3D11->domainShader.resourceId;
        case ShaderStage::Hull: return m_D3D11->hullShader.resourceId;
        case ShaderStage::Geometry: return m_D3D11->geometryShader.resourceId;
        case ShaderStage::Pixel: return m_D3D11->pixelShader.resourceId;
        case ShaderStage::Compute: return m_D3D11->computeShader.resourceId;
        default: break;
      }
    }
    else if(IsCaptureD3D12())
    {
      switch(stage)
      {
        case ShaderStage::Vertex: return m_D3D12->vertexShader.resourceId;
        case ShaderStage::Domain: return m_D3D12->domainShader.resourceId;
        case ShaderStage::Hull: return m_D3D12->hullShader.resourceId;
        case ShaderStage::Geometry: return m_D3D12->geometryShader.resourceId;
        case ShaderStage::Pixel: return m_D3D12->pixelShader.resourceId;
        case ShaderStage::Compute: return m_D3D12->computeShader.resourceId;
        default: break;
      }
    }
    else if(IsCaptureGL())
    {
      switch(stage)
      {
        case ShaderStage::Vertex: return m_GL->vertexShader.shaderResourceId;
        case ShaderStage::Tess_Control: return m_GL->tessControlShader.shaderResourceId;
        case ShaderStage::Tess_Eval: return m_GL->tessEvalShader.shaderResourceId;
        case ShaderStage::Geometry: return m_GL->geometryShader.shaderResourceId;
        case ShaderStage::Fragment: return m_GL->fragmentShader.shaderResourceId;
        case ShaderStage::Compute: return m_GL->computeShader.shaderResourceId;
        default: break;
      }
    }
    else if(IsCaptureVK())
    {
      switch(stage)
      {
        case ShaderStage::Vertex: return m_Vulkan->vertexShader.resourceId;
        case ShaderStage::Tess_Control: return m_Vulkan->tessControlShader.resourceId;
        case ShaderStage::Tess_Eval: return m_Vulkan->tessEvalShader.resourceId;
        case ShaderStage::Geometry: return m_Vulkan->geometryShader.resourceId;
        case ShaderStage::Fragment: return m_Vulkan->fragmentShader.resourceId;
        case ShaderStage::Compute: return m_Vulkan->computeShader.resourceId;
        default: break;
      }
    }
  }

  return ResourceId();
}

rdcstr CommonPipelineState::GetShaderName(ShaderStage stage)
{
  if(IsCaptureLoaded())
  {
    if(IsCaptureD3D11())
    {
      switch(stage)
      {
        case ShaderStage::Vertex: return m_Ctx.GetResourceName(m_D3D11->vertexShader.resourceId);
        case ShaderStage::Domain: return m_Ctx.GetResourceName(m_D3D11->domainShader.resourceId);
        case ShaderStage::Hull: return m_Ctx.GetResourceName(m_D3D11->hullShader.resourceId);
        case ShaderStage::Geometry:
          return m_Ctx.GetResourceName(m_D3D11->geometryShader.resourceId);
        case ShaderStage::Pixel: return m_Ctx.GetResourceName(m_D3D11->pixelShader.resourceId);
        case ShaderStage::Compute: return m_Ctx.GetResourceName(m_D3D11->computeShader.resourceId);
        default: break;
      }
    }
    else if(IsCaptureD3D12())
    {
      switch(stage)
      {
        case ShaderStage::Vertex:
          return m_Ctx.GetResourceName(m_D3D12->pipelineResourceId) + lit(" VS");
        case ShaderStage::Domain:
          return m_Ctx.GetResourceName(m_D3D12->pipelineResourceId) + lit(" DS");
        case ShaderStage::Hull:
          return m_Ctx.GetResourceName(m_D3D12->pipelineResourceId) + lit(" HS");
        case ShaderStage::Geometry:
          return m_Ctx.GetResourceName(m_D3D12->pipelineResourceId) + lit(" GS");
        case ShaderStage::Pixel:
          return m_Ctx.GetResourceName(m_D3D12->pipelineResourceId) + lit(" PS");
        case ShaderStage::Compute:
          return m_Ctx.GetResourceName(m_D3D12->pipelineResourceId) + lit(" CS");
        default: break;
      }
    }
    else if(IsCaptureGL())
    {
      switch(stage)
      {
        case ShaderStage::Vertex: return m_Ctx.GetResourceName(m_GL->vertexShader.shaderResourceId);
        case ShaderStage::Tess_Control:
          return m_Ctx.GetResourceName(m_GL->tessControlShader.shaderResourceId);
        case ShaderStage::Tess_Eval:
          return m_Ctx.GetResourceName(m_GL->tessEvalShader.shaderResourceId);
        case ShaderStage::Geometry:
          return m_Ctx.GetResourceName(m_GL->geometryShader.shaderResourceId);
        case ShaderStage::Fragment:
          return m_Ctx.GetResourceName(m_GL->fragmentShader.shaderResourceId);
        case ShaderStage::Compute:
          return m_Ctx.GetResourceName(m_GL->computeShader.shaderResourceId);
        default: break;
      }
    }
    else if(IsCaptureVK())
    {
      switch(stage)
      {
        case ShaderStage::Vertex: return m_Ctx.GetResourceName(m_Vulkan->vertexShader.resourceId);
        case ShaderStage::Domain:
          return m_Ctx.GetResourceName(m_Vulkan->tessControlShader.resourceId);
        case ShaderStage::Hull: return m_Ctx.GetResourceName(m_Vulkan->tessEvalShader.resourceId);
        case ShaderStage::Geometry:
          return m_Ctx.GetResourceName(m_Vulkan->geometryShader.resourceId);
        case ShaderStage::Pixel: return m_Ctx.GetResourceName(m_Vulkan->fragmentShader.resourceId);
        case ShaderStage::Compute: return m_Ctx.GetResourceName(m_Vulkan->computeShader.resourceId);
        default: break;
      }
    }
  }

  return "";
}

BoundVBuffer CommonPipelineState::GetIBuffer()
{
  ResourceId buf;
  uint64_t ByteOffset = 0;

  if(IsCaptureLoaded())
  {
    if(IsCaptureD3D11())
    {
      buf = m_D3D11->inputAssembly.indexBuffer.resourceId;
      ByteOffset = m_D3D11->inputAssembly.indexBuffer.byteOffset;
    }
    else if(IsCaptureD3D12())
    {
      buf = m_D3D12->inputAssembly.indexBuffer.resourceId;
      ByteOffset = m_D3D12->inputAssembly.indexBuffer.byteOffset;
    }
    else if(IsCaptureGL())
    {
      buf = m_GL->vertexInput.indexBuffer;
      ByteOffset = 0;    // GL only has per-draw index offset
    }
    else if(IsCaptureVK())
    {
      buf = m_Vulkan->inputAssembly.indexBuffer.resourceId;
      ByteOffset = m_Vulkan->inputAssembly.indexBuffer.byteOffset;
    }
  }

  BoundVBuffer ret;
  ret.resourceId = buf;
  ret.byteOffset = ByteOffset;

  return ret;
}

bool CommonPipelineState::IsStripRestartEnabled()
{
  if(IsCaptureLoaded())
  {
    if(IsCaptureD3D11())
    {
      // D3D11 this is always enabled
      return true;
    }
    else if(IsCaptureD3D12())
    {
      return m_D3D12->inputAssembly.indexStripCutValue != 0;
    }
    else if(IsCaptureGL())
    {
      return m_GL->vertexInput.primitiveRestart;
    }
    else if(IsCaptureVK())
    {
      return m_Vulkan->inputAssembly.primitiveRestartEnable;
    }
  }

  return false;
}

uint32_t CommonPipelineState::GetStripRestartIndex()
{
  if(IsCaptureLoaded())
  {
    if(IsCaptureD3D11() || IsCaptureVK())
    {
      // D3D11 or Vulkan this is always '-1'
      return UINT32_MAX;
    }
    else if(IsCaptureD3D12())
    {
      return m_D3D12->inputAssembly.indexStripCutValue;
    }
    else if(IsCaptureGL())
    {
      return qMin(UINT32_MAX, m_GL->vertexInput.restartIndex);
    }
  }

  return UINT32_MAX;
}

rdcarray<BoundVBuffer> CommonPipelineState::GetVBuffers()
{
  rdcarray<BoundVBuffer> ret;

  if(IsCaptureLoaded())
  {
    if(IsCaptureD3D11())
    {
      ret.resize(m_D3D11->inputAssembly.vertexBuffers.count());
      for(int i = 0; i < m_D3D11->inputAssembly.vertexBuffers.count(); i++)
      {
        ret[i].resourceId = m_D3D11->inputAssembly.vertexBuffers[i].resourceId;
        ret[i].byteOffset = m_D3D11->inputAssembly.vertexBuffers[i].byteOffset;
        ret[i].byteStride = m_D3D11->inputAssembly.vertexBuffers[i].byteStride;
      }
    }
    else if(IsCaptureD3D12())
    {
      ret.resize(m_D3D12->inputAssembly.vertexBuffers.count());
      for(int i = 0; i < m_D3D12->inputAssembly.vertexBuffers.count(); i++)
      {
        ret[i].resourceId = m_D3D12->inputAssembly.vertexBuffers[i].resourceId;
        ret[i].byteOffset = m_D3D12->inputAssembly.vertexBuffers[i].byteOffset;
        ret[i].byteStride = m_D3D12->inputAssembly.vertexBuffers[i].byteStride;
      }
    }
    else if(IsCaptureGL())
    {
      ret.resize(m_GL->vertexInput.vertexBuffers.count());
      for(int i = 0; i < m_GL->vertexInput.vertexBuffers.count(); i++)
      {
        ret[i].resourceId = m_GL->vertexInput.vertexBuffers[i].resourceId;
        ret[i].byteOffset = m_GL->vertexInput.vertexBuffers[i].byteOffset;
        ret[i].byteStride = m_GL->vertexInput.vertexBuffers[i].byteStride;
      }
    }
    else if(IsCaptureVK())
    {
      ret.resize(m_Vulkan->vertexInput.bindings.count());
      for(int i = 0; i < m_Vulkan->vertexInput.bindings.count(); i++)
      {
        ret[i].resourceId = i < m_Vulkan->vertexInput.vertexBuffers.count()
                                ? m_Vulkan->vertexInput.vertexBuffers[i].resourceId
                                : ResourceId();
        ret[i].byteOffset = i < m_Vulkan->vertexInput.vertexBuffers.count()
                                ? m_Vulkan->vertexInput.vertexBuffers[i].byteOffset
                                : 0;
        ret[i].byteStride = m_Vulkan->vertexInput.bindings[i].byteStride;
      }
    }
  }

  return ret;
}

rdcarray<VertexInputAttribute> CommonPipelineState::GetVertexInputs()
{
  if(IsCaptureLoaded())
  {
    if(IsCaptureD3D11())
    {
      uint32_t byteOffs[128] = {};

      const rdcarray<D3D11Pipe::Layout> &layouts = m_D3D11->inputAssembly.layouts;

      rdcarray<VertexInputAttribute> ret;
      ret.resize(layouts.size());
      for(int i = 0; i < layouts.count(); i++)
      {
        QString semName = layouts[i].semanticName;

        bool needsSemanticIdx = false;
        for(int j = 0; j < layouts.count(); j++)
        {
          if(i != j && !semName.compare(layouts[j].semanticName, Qt::CaseInsensitive))
          {
            needsSemanticIdx = true;
            break;
          }
        }

        uint32_t offs = layouts[i].byteOffset;
        if(offs == UINT32_MAX)    // APPEND_ALIGNED
          offs = byteOffs[layouts[i].inputSlot];
        else
          byteOffs[layouts[i].inputSlot] = offs = layouts[i].byteOffset;

        byteOffs[layouts[i].inputSlot] +=
            layouts[i].format.compByteWidth * layouts[i].format.compCount;

        ret[i].name =
            semName + (needsSemanticIdx ? QString::number(layouts[i].semanticIndex) : QString());
        ret[i].vertexBuffer = (int)layouts[i].inputSlot;
        ret[i].byteOffset = offs;
        ret[i].perInstance = layouts[i].perInstance;
        ret[i].instanceRate = (int)layouts[i].instanceDataStepRate;
        ret[i].format = layouts[i].format;
        memset(&ret[i].genericValue, 0, sizeof(PixelValue));
        ret[i].used = false;
        ret[i].genericEnabled = false;

        if(m_D3D11->inputAssembly.bytecode != NULL)
        {
          rdcarray<SigParameter> &sig = m_D3D11->inputAssembly.bytecode->inputSignature;
          for(int ia = 0; ia < sig.count(); ia++)
          {
            if(!semName.compare(sig[ia].semanticName, Qt::CaseInsensitive) &&
               sig[ia].semanticIndex == layouts[i].semanticIndex)
            {
              ret[i].used = true;
              break;
            }
          }
        }
      }

      return ret;
    }
    else if(IsCaptureD3D12())
    {
      uint32_t byteOffs[128] = {};

      const rdcarray<D3D12Pipe::Layout> &layouts = m_D3D12->inputAssembly.layouts;

      rdcarray<VertexInputAttribute> ret;
      ret.resize(layouts.size());
      for(int i = 0; i < layouts.count(); i++)
      {
        QString semName = layouts[i].semanticName;

        bool needsSemanticIdx = false;
        for(int j = 0; j < layouts.count(); j++)
        {
          if(i != j && !semName.compare(QString(layouts[j].semanticName), Qt::CaseInsensitive))
          {
            needsSemanticIdx = true;
            break;
          }
        }

        uint32_t offs = layouts[i].byteOffset;
        if(offs == UINT32_MAX)    // APPEND_ALIGNED
          offs = byteOffs[layouts[i].inputSlot];
        else
          byteOffs[layouts[i].inputSlot] = offs = layouts[i].byteOffset;

        byteOffs[layouts[i].inputSlot] +=
            layouts[i].format.compByteWidth * layouts[i].format.compCount;

        ret[i].name =
            semName + (needsSemanticIdx ? QString::number(layouts[i].semanticIndex) : QString());
        ret[i].vertexBuffer = (int)layouts[i].inputSlot;
        ret[i].byteOffset = offs;
        ret[i].perInstance = layouts[i].perInstance;
        ret[i].instanceRate = (int)layouts[i].instanceDataStepRate;
        ret[i].format = layouts[i].format;
        memset(&ret[i].genericValue, 0, sizeof(PixelValue));
        ret[i].used = false;
        ret[i].genericEnabled = false;

        if(m_D3D12->vertexShader.reflection != NULL)
        {
          rdcarray<SigParameter> &sig = m_D3D12->vertexShader.reflection->inputSignature;
          for(int ia = 0; ia < sig.count(); ia++)
          {
            if(!semName.compare(sig[ia].semanticName, Qt::CaseInsensitive) &&
               sig[ia].semanticIndex == layouts[i].semanticIndex)
            {
              ret[i].used = true;
              break;
            }
          }
        }
      }

      return ret;
    }
    else if(IsCaptureGL())
    {
      const rdcarray<GLPipe::VertexAttribute> &attrs = m_GL->vertexInput.attributes;

      int num = 0;
      for(int i = 0; i < attrs.count(); i++)
      {
        int attrib = -1;
        if(m_GL->vertexShader.reflection != NULL)
          attrib = m_GL->vertexShader.bindpointMapping.inputAttributes[i];
        else
          attrib = i;

        if(attrib >= 0)
          num++;
      }

      int a = 0;
      rdcarray<VertexInputAttribute> ret;
      ret.resize(attrs.count());
      for(int i = 0; i < attrs.count() && a < num; i++)
      {
        ret[a].name = lit("attr%1").arg(i);
        memset(&ret[a].genericValue, 0, sizeof(PixelValue));
        ret[a].vertexBuffer = (int)attrs[i].vertexBufferSlot;
        ret[a].byteOffset = attrs[i].byteOffset;
        ret[a].perInstance =
            m_GL->vertexInput.vertexBuffers[attrs[i].vertexBufferSlot].instanceDivisor > 0;
        ret[a].instanceRate =
            (int)m_GL->vertexInput.vertexBuffers[attrs[i].vertexBufferSlot].instanceDivisor;
        ret[a].format = attrs[i].format;
        ret[a].used = true;
        ret[a].genericEnabled = false;

        if(m_GL->vertexShader.reflection != NULL)
        {
          int attrib = m_GL->vertexShader.bindpointMapping.inputAttributes[i];

          if(attrib >= 0 && attrib < m_GL->vertexShader.reflection->inputSignature.count())
            ret[a].name = m_GL->vertexShader.reflection->inputSignature[attrib].varName;

          if(attrib == -1)
            continue;

          if(!attrs[i].enabled)
          {
            uint32_t compCount = m_GL->vertexShader.reflection->inputSignature[attrib].compCount;
            CompType compType = m_GL->vertexShader.reflection->inputSignature[attrib].compType;

            for(uint32_t c = 0; c < compCount; c++)
            {
              if(compType == CompType::Float)
                ret[a].genericValue.floatValue[c] = attrs[i].genericValue.floatValue[c];
              else if(compType == CompType::UInt)
                ret[a].genericValue.uintValue[c] = attrs[i].genericValue.uintValue[c];
              else if(compType == CompType::SInt)
                ret[a].genericValue.intValue[c] = attrs[i].genericValue.intValue[c];
              else if(compType == CompType::UScaled)
                ret[a].genericValue.floatValue[c] = (float)attrs[i].genericValue.uintValue[c];
              else if(compType == CompType::SScaled)
                ret[a].genericValue.floatValue[c] = (float)attrs[i].genericValue.intValue[c];
            }

            ret[a].genericEnabled = true;

            ret[a].perInstance = false;
            ret[a].instanceRate = 0;
            ret[a].format.compByteWidth = 4;
            ret[a].format.compCount = compCount;
            ret[a].format.compType = compType;
            ret[a].format.type = ResourceFormatType::Regular;
            ret[a].format.srgbCorrected = false;
          }
        }

        a++;
      }

      return ret;
    }
    else if(IsCaptureVK())
    {
      const rdcarray<VKPipe::VertexAttribute> &attrs = m_Vulkan->vertexInput.attributes;

      int num = 0;
      for(int i = 0; i < attrs.count(); i++)
      {
        int attrib = -1;
        if(m_Vulkan->vertexShader.reflection != NULL)
        {
          if(attrs[i].location <
             (uint32_t)m_Vulkan->vertexShader.bindpointMapping.inputAttributes.count())
            attrib = m_Vulkan->vertexShader.bindpointMapping.inputAttributes[attrs[i].location];
        }
        else
          attrib = i;

        if(attrib >= 0)
          num++;
      }

      int a = 0;
      rdcarray<VertexInputAttribute> ret;
      ret.resize(num);
      for(int i = 0; i < attrs.count() && a < num; i++)
      {
        ret[a].name = lit("attr%1").arg(i);
        memset(&ret[a].genericValue, 0, sizeof(PixelValue));
        ret[a].vertexBuffer = (int)attrs[i].binding;
        ret[a].byteOffset = attrs[i].byteOffset;
        ret[a].perInstance = false;
        ret[a].instanceRate = 1;
        if(attrs[i].binding < (uint32_t)m_Vulkan->vertexInput.bindings.count())
        {
          ret[a].perInstance = m_Vulkan->vertexInput.bindings[attrs[i].binding].perInstance;
          ret[a].instanceRate = m_Vulkan->vertexInput.bindings[attrs[i].binding].instanceDivisor;
        }
        ret[a].format = attrs[i].format;
        ret[a].used = true;
        ret[a].genericEnabled = false;

        if(m_Vulkan->vertexShader.reflection != NULL)
        {
          int attrib = -1;

          if(attrs[i].location <
             (uint32_t)m_Vulkan->vertexShader.bindpointMapping.inputAttributes.count())
            attrib = m_Vulkan->vertexShader.bindpointMapping.inputAttributes[attrs[i].location];

          if(attrib >= 0 && attrib < m_Vulkan->vertexShader.reflection->inputSignature.count())
            ret[a].name = m_Vulkan->vertexShader.reflection->inputSignature[attrib].varName;

          if(attrib == -1)
            continue;
        }

        a++;
      }

      return ret;
    }
  }

  return rdcarray<VertexInputAttribute>();
}

BoundCBuffer CommonPipelineState::GetConstantBuffer(ShaderStage stage, uint32_t BufIdx,
                                                    uint32_t ArrayIdx)
{
  ResourceId buf;
  uint64_t ByteOffset = 0;
  uint64_t ByteSize = 0;

  if(IsCaptureLoaded())
  {
    if(IsCaptureD3D11())
    {
      const D3D11Pipe::Shader &s = GetD3D11Stage(stage);

      if(s.reflection != NULL && BufIdx < (uint32_t)s.reflection->constantBlocks.count())
      {
        const Bindpoint &bind =
            s.bindpointMapping.constantBlocks[s.reflection->constantBlocks[BufIdx].bindPoint];

        if(bind.bind >= s.constantBuffers.count())
          return BoundCBuffer();

        const D3D11Pipe::ConstantBuffer &descriptor = s.constantBuffers[bind.bind];

        buf = descriptor.resourceId;
        ByteOffset = descriptor.vecOffset * 4 * sizeof(float);
        ByteSize = descriptor.vecCount * 4 * sizeof(float);
      }
    }
    else if(IsCaptureD3D12())
    {
      const D3D12Pipe::Shader &s = GetD3D12Stage(stage);

      if(s.reflection != NULL && BufIdx < (uint32_t)s.reflection->constantBlocks.count())
      {
        const Bindpoint &bind =
            s.bindpointMapping.constantBlocks[s.reflection->constantBlocks[BufIdx].bindPoint];

        if(bind.bindset >= s.spaces.count() ||
           bind.bind >= s.spaces[bind.bindset].constantBuffers.count())
          return BoundCBuffer();

        const D3D12Pipe::ConstantBuffer &descriptor =
            s.spaces[bind.bindset].constantBuffers[bind.bind];

        buf = descriptor.resourceId;
        ByteOffset = descriptor.byteOffset;
        ByteSize = descriptor.byteSize;
      }
    }
    else if(IsCaptureGL())
    {
      const GLPipe::Shader &s = GetGLStage(stage);

      if(s.reflection != NULL && BufIdx < (uint32_t)s.reflection->constantBlocks.count())
      {
        if(s.reflection->constantBlocks[BufIdx].bindPoint >= 0)
        {
          int uboIdx =
              s.bindpointMapping.constantBlocks[s.reflection->constantBlocks[BufIdx].bindPoint].bind;
          if(uboIdx >= 0 && uboIdx < m_GL->uniformBuffers.count())
          {
            const GLPipe::Buffer &b = m_GL->uniformBuffers[uboIdx];

            buf = b.resourceId;
            ByteOffset = b.byteOffset;
            ByteSize = b.byteSize;
          }
        }
      }
    }
    else if(IsCaptureVK())
    {
      const VKPipe::Pipeline &pipe =
          stage == ShaderStage::Compute ? m_Vulkan->compute : m_Vulkan->graphics;
      const VKPipe::Shader &s = GetVulkanStage(stage);

      if(s.reflection != NULL && BufIdx < (uint32_t)s.reflection->constantBlocks.count())
      {
        const Bindpoint &bind =
            s.bindpointMapping.constantBlocks[s.reflection->constantBlocks[BufIdx].bindPoint];

        if(s.reflection->constantBlocks[BufIdx].bufferBacked == false)
        {
          BoundCBuffer ret;
          // dummy value, it would be nice to fetch this properly
          ret.byteSize = 1024;
          return ret;
        }

        const VKPipe::BindingElement &descriptorBind =
            pipe.descriptorSets[bind.bindset].bindings[bind.bind].binds[ArrayIdx];

        buf = descriptorBind.resourceResourceId;
        ByteOffset = descriptorBind.byteOffset;
        ByteSize = descriptorBind.byteSize;
      }
    }
  }

  BoundCBuffer ret;

  ret.resourceId = buf;
  ret.byteOffset = ByteOffset;
  ret.byteSize = ByteSize;

  return ret;
}

rdcarray<BoundResourceArray> CommonPipelineState::GetReadOnlyResources(ShaderStage stage)
{
  rdcarray<BoundResourceArray> ret;

  if(IsCaptureLoaded())
  {
    if(IsCaptureD3D11())
    {
      const D3D11Pipe::Shader &s = GetD3D11Stage(stage);

      ret.reserve(s.srvs.size());

      for(int i = 0; i < s.srvs.count(); i++)
      {
        Bindpoint key(0, i);
        BoundResource val;

        val.resourceId = s.srvs[i].resourceResourceId;
        val.firstMip = (int)s.srvs[i].firstMip;
        val.firstSlice = (int)s.srvs[i].firstSlice;
        val.typeHint = s.srvs[i].viewFormat.compType;

        ret.push_back(BoundResourceArray(key, {val}));
      }

      return ret;
    }
    else if(IsCaptureD3D12())
    {
      const D3D12Pipe::Shader &s = GetD3D12Stage(stage);

      for(int space = 0; space < s.spaces.count(); space++)
      {
        for(int reg = 0; reg < s.spaces[space].srvs.count(); reg++)
        {
          const D3D12Pipe::View &bind = s.spaces[space].srvs[reg];
          Bindpoint key(space, reg);
          BoundResource val;

          // consider this register to not exist - it's in a gap defined by sparse root signature
          // elements
          if(bind.rootElement == ~0U)
            continue;

          val.resourceId = bind.resourceId;
          val.firstMip = (int)bind.firstMip;
          val.firstSlice = (int)bind.firstSlice;
          val.typeHint = bind.viewFormat.compType;

          ret.push_back(BoundResourceArray(key, {val}));
        }
      }

      return ret;
    }
    else if(IsCaptureGL())
    {
      ret.reserve(m_GL->textures.size());

      for(int i = 0; i < m_GL->textures.count(); i++)
      {
        Bindpoint key(0, i);
        BoundResource val;

        val.resourceId = m_GL->textures[i].resourceId;
        val.firstMip = (int)m_GL->textures[i].firstMip;
        val.firstSlice = (int)m_GL->textures[i].firstSlice;
        val.typeHint = CompType::Typeless;

        ret.push_back(BoundResourceArray(key, {val}));
      }

      return ret;
    }
    else if(IsCaptureVK())
    {
      const rdcarray<VKPipe::DescriptorSet> &descsets = stage == ShaderStage::Compute
                                                            ? m_Vulkan->compute.descriptorSets
                                                            : m_Vulkan->graphics.descriptorSets;

      ShaderStageMask mask = MaskForStage(stage);

      for(int set = 0; set < descsets.count(); set++)
      {
        const VKPipe::DescriptorSet &descset = descsets[set];
        for(int slot = 0; slot < descset.bindings.count(); slot++)
        {
          const VKPipe::DescriptorBinding &bind = descset.bindings[slot];
          if((bind.type == BindType::ImageSampler || bind.type == BindType::InputAttachment ||
              bind.type == BindType::ReadOnlyImage || bind.type == BindType::ReadOnlyTBuffer) &&
             (bind.stageFlags & mask) == mask)
          {
            ret.push_back(BoundResourceArray());
            ret.back().bindPoint = Bindpoint(set, slot);

            rdcarray<BoundResource> &val = ret.back().resources;
            val.resize(bind.descriptorCount);

            for(uint32_t i = 0; i < bind.descriptorCount; i++)
            {
              val[i].resourceId = bind.binds[i].resourceResourceId;
              val[i].firstMip = (int)bind.binds[i].firstMip;
              val[i].firstSlice = (int)bind.binds[i].firstSlice;
              val[i].typeHint = bind.binds[i].viewFormat.compType;
            }
          }
        }
      }

      return ret;
    }
  }

  return ret;
}

rdcarray<BoundResourceArray> CommonPipelineState::GetReadWriteResources(ShaderStage stage)
{
  rdcarray<BoundResourceArray> ret;

  if(IsCaptureLoaded())
  {
    if(IsCaptureD3D11())
    {
      if(stage == ShaderStage::Compute)
      {
        ret.reserve(m_D3D11->computeShader.uavs.size());

        for(int i = 0; i < m_D3D11->computeShader.uavs.count(); i++)
        {
          Bindpoint key(0, i);
          BoundResource val;

          val.resourceId = m_D3D11->computeShader.uavs[i].resourceResourceId;
          val.firstMip = (int)m_D3D11->computeShader.uavs[i].firstMip;
          val.firstSlice = (int)m_D3D11->computeShader.uavs[i].firstSlice;
          val.typeHint = m_D3D11->computeShader.uavs[i].viewFormat.compType;

          ret.push_back(BoundResourceArray(key, {val}));
        }
      }
      else
      {
        int uavstart = (int)m_D3D11->outputMerger.uavStartSlot;

        ret.reserve(m_D3D11->outputMerger.uavs.size() + qMax(0, uavstart));

        // up to UAVStartSlot treat these bindings as empty.
        for(int i = 0; i < uavstart; i++)
        {
          Bindpoint key(0, i);
          BoundResource val;

          ret.push_back(BoundResourceArray(key, {val}));
        }

        for(int i = 0; i < m_D3D11->outputMerger.uavs.count() - uavstart; i++)
        {
          Bindpoint key(0, i + uavstart);
          BoundResource val;

          val.resourceId = m_D3D11->outputMerger.uavs[i].resourceResourceId;
          val.firstMip = (int)m_D3D11->outputMerger.uavs[i].firstMip;
          val.firstSlice = (int)m_D3D11->outputMerger.uavs[i].firstSlice;
          val.typeHint = m_D3D11->outputMerger.uavs[i].viewFormat.compType;

          ret.push_back(BoundResourceArray(key, {val}));
        }
      }
    }
    else if(IsCaptureD3D12())
    {
      const D3D12Pipe::Shader &s = GetD3D12Stage(stage);

      for(int space = 0; space < s.spaces.count(); space++)
      {
        for(int reg = 0; reg < s.spaces[space].uavs.count(); reg++)
        {
          const D3D12Pipe::View &bind = s.spaces[space].uavs[reg];
          Bindpoint key(space, reg);
          BoundResource val;

          // consider this register to not exist - it's in a gap defined by sparse root signature
          // elements
          if(bind.rootElement == ~0U)
            continue;

          val.resourceId = bind.resourceId;
          val.firstMip = (int)bind.firstMip;
          val.firstSlice = (int)bind.firstSlice;
          val.typeHint = bind.viewFormat.compType;

          ret.push_back(BoundResourceArray(key, {val}));
        }
      }
    }
    else if(IsCaptureGL())
    {
      ret.reserve(m_GL->images.size());

      for(int i = 0; i < m_GL->images.count(); i++)
      {
        Bindpoint key(0, i);
        BoundResource val;

        val.resourceId = m_GL->images[i].resourceId;
        val.firstMip = (int)m_GL->images[i].mipLevel;
        val.firstSlice = (int)m_GL->images[i].slice;
        val.typeHint = m_GL->images[i].imageFormat.compType;

        ret.push_back(BoundResourceArray(key, {val}));
      }
    }
    else if(IsCaptureVK())
    {
      const rdcarray<VKPipe::DescriptorSet> &descsets = stage == ShaderStage::Compute
                                                            ? m_Vulkan->compute.descriptorSets
                                                            : m_Vulkan->graphics.descriptorSets;

      ShaderStageMask mask = MaskForStage(stage);

      for(int set = 0; set < descsets.count(); set++)
      {
        const VKPipe::DescriptorSet &descset = descsets[set];
        for(int slot = 0; slot < descset.bindings.count(); slot++)
        {
          const VKPipe::DescriptorBinding &bind = descset.bindings[slot];
          if((bind.type == BindType::ReadWriteBuffer || bind.type == BindType::ReadWriteImage ||
              bind.type == BindType::ReadWriteTBuffer) &&
             (bind.stageFlags & mask) == mask)
          {
            ret.push_back(BoundResourceArray());
            ret.back().bindPoint = Bindpoint(set, slot);

            rdcarray<BoundResource> &val = ret.back().resources;
            val.resize(bind.descriptorCount);

            for(uint32_t i = 0; i < bind.descriptorCount; i++)
            {
              val[i].resourceId = bind.binds[i].resourceResourceId;
              val[i].firstMip = (int)bind.binds[i].firstMip;
              val[i].firstSlice = (int)bind.binds[i].firstSlice;
              val[i].typeHint = bind.binds[i].viewFormat.compType;
            }
          }
        }
      }
    }
  }

  return ret;
}

BoundResource CommonPipelineState::GetDepthTarget()
{
  if(IsCaptureLoaded())
  {
    if(IsCaptureD3D11())
    {
      BoundResource ret;
      ret.resourceId = m_D3D11->outputMerger.depthTarget.resourceResourceId;
      ret.firstMip = (int)m_D3D11->outputMerger.depthTarget.firstMip;
      ret.firstSlice = (int)m_D3D11->outputMerger.depthTarget.firstSlice;
      ret.typeHint = m_D3D11->outputMerger.depthTarget.viewFormat.compType;
      return ret;
    }
    else if(IsCaptureD3D12())
    {
      BoundResource ret;
      ret.resourceId = m_D3D12->outputMerger.depthTarget.resourceId;
      ret.firstMip = (int)m_D3D12->outputMerger.depthTarget.firstMip;
      ret.firstSlice = (int)m_D3D12->outputMerger.depthTarget.firstSlice;
      ret.typeHint = m_D3D12->outputMerger.depthTarget.viewFormat.compType;
      return ret;
    }
    else if(IsCaptureGL())
    {
      BoundResource ret;
      ret.resourceId = m_GL->framebuffer.drawFBO.depthAttachment.resourceId;
      ret.firstMip = (int)m_GL->framebuffer.drawFBO.depthAttachment.mipLevel;
      ret.firstSlice = (int)m_GL->framebuffer.drawFBO.depthAttachment.slice;
      ret.typeHint = CompType::Typeless;
      return ret;
    }
    else if(IsCaptureVK())
    {
      const VKPipe::RenderPass &rp = m_Vulkan->currentPass.renderpass;
      const VKPipe::Framebuffer &fb = m_Vulkan->currentPass.framebuffer;

      if(rp.depthstencilAttachment >= 0 && rp.depthstencilAttachment < fb.attachments.count())
      {
        BoundResource ret;
        ret.resourceId = fb.attachments[rp.depthstencilAttachment].imageResourceId;
        ret.firstMip = (int)fb.attachments[rp.depthstencilAttachment].firstMip;
        ret.firstSlice = (int)fb.attachments[rp.depthstencilAttachment].firstSlice;
        ret.typeHint = fb.attachments[rp.depthstencilAttachment].viewFormat.compType;
        return ret;
      }

      return BoundResource();
    }
  }

  return BoundResource();
}

rdcarray<BoundResource> CommonPipelineState::GetOutputTargets()
{
  rdcarray<BoundResource> ret;

  if(IsCaptureLoaded())
  {
    if(IsCaptureD3D11())
    {
      ret.resize(m_D3D11->outputMerger.renderTargets.count());
      for(int i = 0; i < m_D3D11->outputMerger.renderTargets.count(); i++)
      {
        ret[i].resourceId = m_D3D11->outputMerger.renderTargets[i].resourceResourceId;
        ret[i].firstMip = (int)m_D3D11->outputMerger.renderTargets[i].firstMip;
        ret[i].firstSlice = (int)m_D3D11->outputMerger.renderTargets[i].firstSlice;
        ret[i].typeHint = m_D3D11->outputMerger.renderTargets[i].viewFormat.compType;
      }
    }
    else if(IsCaptureD3D12())
    {
      ret.resize(m_D3D12->outputMerger.renderTargets.count());
      for(int i = 0; i < m_D3D12->outputMerger.renderTargets.count(); i++)
      {
        ret[i].resourceId = m_D3D12->outputMerger.renderTargets[i].resourceId;
        ret[i].firstMip = (int)m_D3D12->outputMerger.renderTargets[i].firstMip;
        ret[i].firstSlice = (int)m_D3D12->outputMerger.renderTargets[i].firstSlice;
        ret[i].typeHint = m_D3D12->outputMerger.renderTargets[i].viewFormat.compType;
      }
    }
    else if(IsCaptureGL())
    {
      ret.resize(m_GL->framebuffer.drawFBO.drawBuffers.count());
      for(int i = 0; i < m_GL->framebuffer.drawFBO.drawBuffers.count(); i++)
      {
        int db = m_GL->framebuffer.drawFBO.drawBuffers[i];

        if(db >= 0)
        {
          ret[i].resourceId = m_GL->framebuffer.drawFBO.colorAttachments[db].resourceId;
          ret[i].firstMip = (int)m_GL->framebuffer.drawFBO.colorAttachments[db].mipLevel;
          ret[i].firstSlice = (int)m_GL->framebuffer.drawFBO.colorAttachments[db].slice;
          ret[i].typeHint = CompType::Typeless;
        }
      }
    }
    else if(IsCaptureVK())
    {
      const VKPipe::RenderPass &rp = m_Vulkan->currentPass.renderpass;
      const VKPipe::Framebuffer &fb = m_Vulkan->currentPass.framebuffer;

      int idx = 0;

      ret.resize(rp.colorAttachments.count() + rp.resolveAttachments.count());
      for(int i = 0; i < rp.colorAttachments.count(); i++)
      {
        if(rp.colorAttachments[i] < (uint32_t)fb.attachments.count())
        {
          ret[idx].resourceId = fb.attachments[rp.colorAttachments[i]].imageResourceId;
          ret[idx].firstMip = (int)fb.attachments[rp.colorAttachments[i]].firstMip;
          ret[idx].firstSlice = (int)fb.attachments[rp.colorAttachments[i]].firstSlice;
          ret[idx].typeHint = fb.attachments[rp.colorAttachments[i]].viewFormat.compType;
        }

        idx++;
      }

      for(int i = 0; i < rp.resolveAttachments.count(); i++)
      {
        if(rp.resolveAttachments[i] < (uint32_t)fb.attachments.count())
        {
          ret[idx].resourceId = fb.attachments[rp.resolveAttachments[i]].imageResourceId;
          ret[idx].firstMip = (int)fb.attachments[rp.resolveAttachments[i]].firstMip;
          ret[idx].firstSlice = (int)fb.attachments[rp.resolveAttachments[i]].firstSlice;
          ret[idx].typeHint = fb.attachments[rp.resolveAttachments[i]].viewFormat.compType;
        }

        idx++;
      }
    }
  }

  return ret;
}
