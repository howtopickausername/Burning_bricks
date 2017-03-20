#include "stdafx.h"
#include "ModelRes.h"
#include "Locator.h"
#include "GraphicsException.h"


void Gp::cModelRes::Init(ID3D11Device& device, DirectX::EffectFactory& effFactory, std::vector<std::wstring> resPaths)
{
	try {
		for (auto p : resPaths) {
			auto m = Gp::cModel::CreateModel(&device, p.c_str(), effFactory);
			m_Models.insert(std::make_pair(p, std::move(m)));
		}
	}
	catch (Gp::Exception e) {
		cLocator::Log() << e.what();
	}
}

DirectX::Model* Gp::cModelRes::GetModel(std::wstring name)
{
	auto targ = m_Models.find(name);
	if (targ != m_Models.end()) {
		return (targ->second).get();
	}
	else {
		return nullptr;
	}
}
