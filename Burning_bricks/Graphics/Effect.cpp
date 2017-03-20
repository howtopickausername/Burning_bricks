#include "stdafx.h"
#include <vector>
#include <fstream>
#include <experimental/filesystem>
#include <direct.h>
#include "D3dx11effect.h"
#include "Effect.h"
#include "GraphicsException.h"
_GP_BEGIN;

cEffect::cEffect(ID3D11Device& device, std::string effPath)
{
	m_Effect = CreateEffect(device, effPath);
}

cEffect::~cEffect()
{
	RELEASE(m_Effect);
}

ID3D11InputLayout* cEffect::GetInputLayout(int techIdx, int passIdx)
{
	auto pos = std::make_tuple(techIdx, passIdx);
	auto pLayout = m_InputLayouts.find(pos);
	if (pLayout == m_InputLayouts.end()){
		ID3D11InputLayout* layout = CreateInputLayout(techIdx, passIdx);
		m_InputLayouts[pos] = layout;
		return layout;
	}
	else {
		return pLayout->second;
	}
}

void Gp::cEffect::BindVariable()
{
	m_mtxWorldVar = m_Effect->GetVariableByName("g_mtxWorld")->AsMatrix();
	assert(m_mtxWorldVar->IsValid());
	m_mtxProjVar = m_Effect->GetVariableByName("g_mtxView")->AsMatrix();
	assert(m_mtxProjVar->IsValid());
	m_mtxViewVar = m_Effect->GetVariableByName("g_mtxProj")->AsMatrix();
	assert(m_mtxViewVar->IsValid());
}

Gp::cEffect::InputDescT& Gp::cEffect::GetInputDesc()
{
	return m_InputDescElements;
}

ID3DX11Effect& Gp::cEffect::GetEffect() const
{
	return *m_Effect;
}

ID3DX11Effect* cEffect::CreateEffect(ID3D11Device& device, std::string effPath)
{
	std::ifstream file;
	file.exceptions(std::ofstream::failbit | std::ofstream::badbit);
	file.open(effPath, std::ios::binary | std::ios::in | std::ios::ate);
	auto size = file.tellg();
	file.seekg(0);
	std::vector<char> buf(size, 0);
	file.read(&buf[0], size);
	ID3DX11Effect* rtEff = nullptr;
	std::experimental::filesystem::path filepath(effPath);
	HRESULT hr = D3DX11CreateEffectFromMemory(&buf[0], buf.size(),
		0, &device, &rtEff, filepath.filename().string().c_str());
	if (FAILED(hr)){
		throw Gp::Exception("CreateEffect failed:" + effPath + std::to_string(hr));
	}
	return rtEff;
}

ID3D11InputLayout* cEffect::CreateInputLayout(int techIdx, int passIdx)
{
	ID3DX11EffectTechnique* technique = m_Effect->GetTechniqueByIndex(techIdx);
	if (!technique){
		throw std::invalid_argument("invalid techIdx:" + std::to_string(techIdx));
	}
	ID3DX11EffectPass* pass = technique->GetPassByIndex(passIdx);
	if (!pass){
		throw std::invalid_argument("invalid passIdx:" + std::to_string(passIdx));
	}
	D3DX11_PASS_DESC pd;
	pass->GetDesc(&pd);
	try {
		auto pInputDesc = m_InputDescElements.at(std::make_tuple(techIdx, passIdx));
		ID3D11InputLayout* rtLayout = nullptr;
		ID3D11Device* device = nullptr;
		HRESULT hr = m_Effect->GetDevice(&device);
		hr = device->CreateInputLayout(
			&(pInputDesc->at(0)), pInputDesc->size(),
			pd.pIAInputSignature, pd.IAInputSignatureSize,
			&rtLayout);
		if (FAILED(hr)){
			throw Gp::Exception("CreateInputLayout failed:" + std::to_string(hr));
		}
		BindVariable();
		return rtLayout;
	}
	catch (std::out_of_range& e) {
		throw std::invalid_argument("InputDesc dosen't exist:" + std::to_string(techIdx) + std::to_string(passIdx));
	}
}
_GP_END;