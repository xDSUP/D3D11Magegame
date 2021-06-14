#include "stdafx.h"
#include "StaticMesh.h"
#include "ms3dspec.h"
#include <fstream>
#include "macros.h"
#include "Util.h"
#include "Shader.h"
#include "Buffer.h"

using namespace D3D11Framework;
using namespace std;



bool StaticMesh::InitBuffers(unsigned short VertexCount, unsigned short indexCount, unsigned short* indices, Vertex* vertices)
{
	m_vertexBuffer = Buffer::CreateVertexBuffer(m_render->GetDevice(), sizeof(Vertex) * VertexCount, false, vertices);
	if (!m_vertexBuffer)
		return false;

	m_indexCount = indexCount;
	m_indexBuffer = Buffer::CreateIndexBuffer(m_render->GetDevice(), sizeof(unsigned short) * indexCount, false, indices);
	if (!m_indexBuffer)
		return false;


	m_constantBuffer = Buffer::CreateConstantBuffer(m_render->GetDevice(), sizeof(ConstantBuffer), false);
	if (!m_constantBuffer)
		return false;

	return true;
}

StaticMesh::StaticMesh(Render *render)
{
	m_render = render;
	m_vertexBuffer = nullptr;
	m_indexBuffer = nullptr;
	m_constantBuffer = nullptr;
	m_shader = nullptr;
}

bool StaticMesh::Init(wchar_t *name)
{
	Identity();

	m_shader = new Shader(m_render);
	if ( !m_shader )
		return false;

	m_shader->AddInputElementDesc("POSITION", DXGI_FORMAT_R32G32B32_FLOAT);
	m_shader->AddInputElementDesc("NORMAL", DXGI_FORMAT_R32G32B32_FLOAT);
	m_shader->AddInputElementDesc("TEXCOORD", DXGI_FORMAT_R32G32_FLOAT);
	
	//m_shader->AddInputElementDesc("TANGENT", DXGI_FORMAT_R32G32B32_FLOAT);
	if ( !m_shader->CreateShader(L"meshVS.hlsl", L"meshPS.hlsl") )
		return false;

	if( !m_loadFromFile(name) )
		return false;

	return true;
}


void StaticMesh::Draw(CXMMATRIX viewmatrix)
{
	m_RenderBuffers();
	m_SetShaderParameters(viewmatrix);
	m_RenderShader();
}

void StaticMesh::m_RenderBuffers()
{
	unsigned int stride = sizeof(Vertex); 
	unsigned int offset = 0;
	m_render->m_pImmediateContext->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);
	m_render->m_pImmediateContext->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R16_UINT, 0);
	/*m_render->m_pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);*/
}

void StaticMesh::m_SetShaderParameters(CXMMATRIX viewmatrix)
{
	XMMATRIX WVP = m_objMatrix * viewmatrix * m_render->m_Projection;	
	ConstantBuffer cb;
	cb.WVP = XMMatrixTranspose(WVP);
	m_render->m_pImmediateContext->UpdateSubresource( m_constantBuffer, 0, NULL, &cb, 0, 0 );

	m_render->m_pImmediateContext->VSSetConstantBuffers(0, 1, &m_constantBuffer);
}

void StaticMesh::m_RenderShader()
{
	m_shader->Draw();
	m_render->m_pImmediateContext->DrawIndexed(m_indexCount, 0, 0);
}

void StaticMesh::Close()
{
	_RELEASE(m_indexBuffer);
	_RELEASE(m_vertexBuffer);
	_RELEASE(m_constantBuffer);
	_CLOSE(m_shader);
}

void StaticMesh::Translate(float x, float y, float z)
{
	m_objMatrix *= XMMatrixTranslation(x,y,z);
}

void StaticMesh::Rotate(float angle, float x, float y, float z)
{
	XMVECTOR v = XMVectorSet(x,y,z,0.0f);
	m_objMatrix *= XMMatrixRotationAxis(v,angle);
}

void StaticMesh::Scale(float x, float y, float z)
{
	m_objMatrix *= XMMatrixScaling(x,y,z);
}

void StaticMesh::Identity()
{
	m_objMatrix = XMMatrixIdentity();
}