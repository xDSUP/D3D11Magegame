#include "stdafx.h"
#include "StaticMesh.h"

#include "D3D11_Framework.h"
#include "Util.h"



using namespace std;
using namespace D3D11_Framework;

struct Vertex
{
	XMFLOAT3 pos;
	XMFLOAT2 tex;
};

struct ConstantBuffer { XMMATRIX WVP; };


StaticMesh::StaticMesh()
{
	_vertexBuffer = nullptr;
	_vertexShader = nullptr;
	_pixelShader = nullptr;
	_indexBuffer = nullptr;
	_pConstantBuffer = nullptr;
	_layout = nullptr;
	_sampleState = nullptr;
	_texture = nullptr;
	Identity();
}

bool StaticMesh::Init(D3D11_Framework::Render* render, const wchar_t* name)
{
	_objMatrix = XMMatrixIdentity();
		_render = render;
	if (!_loadMS3DFile(name)) return false;
	if (!_initShader(w("MeshVS.hlsl"), w("MeshPS.hlsl"))) return false;
	return true;
}



bool StaticMesh::_loadMS3DFile(const wchar_t* name)
{
	unsigned short vertexCount = 0;
	unsigned short triangleCount = 0;
	unsigned short groupCount = 0;
	unsigned short materialCount = 0;
	MS3DVertex* pMS3DVertices = nullptr;
	MS3DTriangle* pMS3DTriangles = nullptr;
	MS3DGroup* pMS3DGroups = nullptr;
	MS3DMaterial* pMS3DMaterials = nullptr;

	ifstream fin;
	MS3DHeader header;

	fin.open(name, std::ios::binary);
	fin.read((char*)(&(header)), sizeof(header));
	if (header.version != 3 && header.version != 4)
		return false;

	fin.read((char*)(&vertexCount), sizeof(unsigned short));
	pMS3DVertices = new MS3DVertex[vertexCount];
	fin.read((char*)pMS3DVertices, vertexCount * sizeof(MS3DVertex));

	fin.read((char*)(&triangleCount), sizeof(unsigned short));
	pMS3DTriangles = new MS3DTriangle[triangleCount];
	fin.read((char*)pMS3DTriangles, triangleCount * sizeof(MS3DTriangle));

	fin.read((char*)(&groupCount), sizeof(unsigned short));
	pMS3DGroups = new MS3DGroup[groupCount];
	for (int i = 0; i < groupCount; i++)
	{
		fin.read((char*)&(pMS3DGroups[i].flags), sizeof(unsigned char));
		fin.read((char*)&(pMS3DGroups[i].name), sizeof(char[32]));
		fin.read((char*)&(pMS3DGroups[i].numTriangles), sizeof(unsigned short));
		unsigned short triCount = pMS3DGroups[i].numTriangles;
		pMS3DGroups[i].triangleIndices = new unsigned short[triCount];
		fin.read((char*)(pMS3DGroups[i].triangleIndices), sizeof(unsigned short) * triCount);
		fin.read((char*)&(pMS3DGroups[i].materialIndex), sizeof(char));
	}

	fin.read((char*)(&materialCount), sizeof(unsigned short));
	pMS3DMaterials = new MS3DMaterial[materialCount];
	fin.read((char*)pMS3DMaterials, materialCount * sizeof(MS3DMaterial));

	fin.close();

	// преобразуем загруженные структуры понятный
	// видюхе вид
	_indexCount = triangleCount * 3;
	// треугольники хранятся, как 3 индекса, поэтому *3
	WORD* indices = new WORD[_indexCount];
	if (!indices)
		return false;
	
	Vertex* vertices = new Vertex[vertexCount];
	if (!vertices)
		return false;

	for (int i = 0; i < triangleCount; i++)
	{
		indices[3 * i + 0] = pMS3DTriangles[i].vertexIndices[0];
		indices[3 * i + 1] = pMS3DTriangles[i].vertexIndices[1];
		indices[3 * i + 2] = pMS3DTriangles[i].vertexIndices[2];
	}

	for (int i = 0; i < vertexCount; i++)
	{
		vertices[i].pos.x = pMS3DVertices[i].vertex[0];
		vertices[i].pos.y = pMS3DVertices[i].vertex[1];
		vertices[i].pos.z = pMS3DVertices[i].vertex[2];

		for (int j = 0; j < triangleCount; j++)
		{
			if (i == pMS3DTriangles[j].vertexIndices[0])
			{
				vertices[i].tex.x = pMS3DTriangles[j].s[0];
				vertices[i].tex.y = pMS3DTriangles[j].t[0];
			}
			else if (i == pMS3DTriangles[j].vertexIndices[1])
			{
				vertices[i].tex.x = pMS3DTriangles[j].s[1];
				vertices[i].tex.y = pMS3DTriangles[j].t[1];
			}
			else if (i == pMS3DTriangles[j].vertexIndices[2])
			{
				vertices[i].tex.x = pMS3DTriangles[j].s[2];
				vertices[i].tex.y = pMS3DTriangles[j].t[2];
			}
			else
				continue;
			break;
		}
	}


	// одна текстура тк один материал
	if (!_loadTextures(CharToWChar(pMS3DMaterials[0].texture)))
		return false;

	_DELETE_ARRAY(pMS3DMaterials);
	if (pMS3DGroups != nullptr)
	{
		for (int i = 0; i < groupCount; i++)
			_DELETE_ARRAY(pMS3DGroups[i].triangleIndices);
		_DELETE_ARRAY(pMS3DGroups);
	}
	_DELETE_ARRAY(pMS3DTriangles);
	_DELETE_ARRAY(pMS3DVertices);

	// загружаем вершины
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(Vertex) * vertexCount;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;
	D3D11_SUBRESOURCE_DATA Data;
	ZeroMemory(&Data, sizeof(Data));
	Data.pSysMem = vertices;
	HRESULT hr = _render->_device->CreateBuffer(&bd, &Data, &_vertexBuffer);
	if (FAILED(hr))
		return false;

	// загружаем индексы
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(WORD) * _indexCount;
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bd.CPUAccessFlags = 0;
	Data.pSysMem = indices;
	hr = _render->_device->CreateBuffer(&bd, &Data, &_indexBuffer);
	if (FAILED(hr))
		return false;

	// загружаем конст буф
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(ConstantBuffer);
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = 0;
	hr = _render->_device->CreateBuffer(&bd, NULL, &_pConstantBuffer);
	if (FAILED(hr))
		return false;

	_DELETE_ARRAY(vertices);
	_DELETE_ARRAY(indices);
	
	return true;
}

bool StaticMesh::_loadTextures(const wchar_t* name)
{
	HRESULT hr = D3DX11CreateShaderResourceViewFromFile(
	_render->_device, name, NULL, NULL, &_texture, NULL);
	if (FAILED(hr)) return false;

	return true;
}


bool StaticMesh::_initShader(wchar_t* vsFilename, wchar_t* psFilename)
{
	ID3DBlob* vertexShaderBuffer = nullptr;
	HRESULT hr = _render->_compileShaderFromFile(vsFilename, "main", "vs_4_0", &vertexShaderBuffer);
	if (FAILED(hr))
		return false;

	ID3DBlob* pixelShaderBuffer = nullptr;
	HRESULT result = _render->_compileShaderFromFile(psFilename, "main", "ps_4_0", &pixelShaderBuffer);
	if (FAILED(hr))
		return false;

	result = _render->_device->CreateVertexShader(vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), NULL, &_vertexShader);
	if (FAILED(result))
		return false;

	result = _render->_device->CreatePixelShader(pixelShaderBuffer->GetBufferPointer(), pixelShaderBuffer->GetBufferSize(), NULL, &_pixelShader);
	if (FAILED(result))
		return false;

	D3D11_INPUT_ELEMENT_DESC polygonLayout[2];
	polygonLayout[0].SemanticName = "POSITION";
	polygonLayout[0].SemanticIndex = 0;
	polygonLayout[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	polygonLayout[0].InputSlot = 0;
	polygonLayout[0].AlignedByteOffset = 0;
	polygonLayout[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[0].InstanceDataStepRate = 0;
	
	polygonLayout[1].SemanticName = "TEXCOORD";
	polygonLayout[1].SemanticIndex = 0;
	polygonLayout[1].Format = DXGI_FORMAT_R32G32_FLOAT;
	polygonLayout[1].InputSlot = 0;
	polygonLayout[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	polygonLayout[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[1].InstanceDataStepRate = 0;

	unsigned int numElements = sizeof(polygonLayout) / sizeof(polygonLayout[0]);

	result = _render->_device->CreateInputLayout(polygonLayout, numElements, vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), &_layout);
	if (FAILED(result))
		return false;

	_RELEASE(vertexShaderBuffer);
	_RELEASE(pixelShaderBuffer);

	D3D11_SAMPLER_DESC samplerDesc;
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.MipLODBias = 0.0f;
	samplerDesc.MaxAnisotropy = 1;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	samplerDesc.BorderColor[0] = 0;
	samplerDesc.BorderColor[1] = 0;
	samplerDesc.BorderColor[2] = 0;
	samplerDesc.BorderColor[3] = 0;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	result = _render->_device->CreateSamplerState(&samplerDesc, &_sampleState);
	if (FAILED(result))
		return false;


	return true;
}

void StaticMesh::Draw(CXMMATRIX viewMatrix)
{
	_renderBuffers();
	_setShaderParameters(viewMatrix);
	_renderShader();
}


void StaticMesh::_renderBuffers() const
{
	unsigned int stride = sizeof(Vertex);
	unsigned int offset = 0;
	_render->_context->IASetVertexBuffers(0, 1, &_vertexBuffer, &stride, &offset);
	_render->_context->IASetIndexBuffer(_indexBuffer, DXGI_FORMAT_R16_UINT, 0);
	_render->_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	
}

void StaticMesh::_setShaderParameters(CXMMATRIX viewMatrix)
{
	XMMATRIX WVP = _objMatrix * viewMatrix * _render->_projection;
	ConstantBuffer cb;
	cb.WVP = XMMatrixTranspose(WVP);
	_render->_context->UpdateSubresource(_pConstantBuffer, 0, NULL, 
		&cb, 0, 0);
	_render->_context->VSSetConstantBuffers(0, 1, &_pConstantBuffer);
	_render->_context->PSSetShaderResources(0, 1, &_texture);
}

void StaticMesh::_renderShader()
{
	_render->_context->IASetInputLayout(_layout);
	_render->_context->VSSetShader(_vertexShader, NULL, 0);
	_render->_context->PSSetShader(_pixelShader, NULL, 0);
	_render->_context->PSSetSamplers(0, 1, &_sampleState);
	_render->_context->DrawIndexed(_indexCount, 0, 0);
}

void StaticMesh::Close()
{
	_RELEASE(_texture);
	_RELEASE(_indexBuffer);
	_RELEASE(_vertexBuffer);
	_RELEASE(_pConstantBuffer);
	_RELEASE(_sampleState);
	_RELEASE(_layout);
	_RELEASE(_pixelShader);
	_RELEASE(_vertexShader);
}

void StaticMesh::Translate(float x, float y, float z)
{
	_objMatrix *= XMMatrixTranslation(x, y, z);
}

void StaticMesh::Rotate(float angle, float x, float y, float z)
{
	XMVECTOR v = XMVectorSet(x, y, z, 0.0f);
	_objMatrix *= XMMatrixRotationAxis(v, angle);
}

void StaticMesh::Scale(float x, float y, float z)
{
	_objMatrix *= XMMatrixScaling(x, y, z);
}

void StaticMesh::Identity()
{
	_objMatrix = XMMatrixIdentity();
}


