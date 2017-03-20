#pragma once
#include <memory>
#include <string>
#include <vector>
#include <map>
#include "d3d11.h"
#include "d3dx11effect.h"
#include "GraphicsDefine.h"

_GP_BEGIN;
class cEffect
{
public:
	typedef std::map<std::tuple<int, int>, std::shared_ptr<std::vector<D3D11_INPUT_ELEMENT_DESC>>> InputDescT;
	cEffect(ID3D11Device& device, std::string effPath);
	~cEffect();
	ID3D11InputLayout* GetInputLayout(int techIdx, int passIdx);
	ID3DX11Effect& GetEffect() const;
	InputDescT& GetInputDesc();
	void BindVariable();
	ID3DX11EffectMatrixVariable* m_mtxWorldVar;
	ID3DX11EffectMatrixVariable* m_mtxProjVar;
	ID3DX11EffectMatrixVariable* m_mtxViewVar;

private:
	ID3DX11Effect* CreateEffect(ID3D11Device& device, std::string effPath);
	ID3D11InputLayout* CreateInputLayout(int techIdx, int passIdx);
	ID3DX11Effect* m_Effect;
	std::map<std::tuple<int, int>, std::shared_ptr<std::vector<D3D11_INPUT_ELEMENT_DESC>>> m_InputDescElements;
	std::map<std::tuple<int, int>, ID3D11InputLayout*> m_InputLayouts;
};
typedef std::shared_ptr<cEffect> pEffect;
_GP_END;
