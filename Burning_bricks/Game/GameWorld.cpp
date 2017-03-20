#include "stdafx.h"
#include "GameWorld.h"
#include "DirectXTK/Inc/SimpleMath.h"

cGameWorld::cGameWorld()
{
}

void cGameWorld::WorldBeginning()
{
	KbCmds = std::make_shared<cKbCmdProductor>();
	KbCmds->CmdDownMaps[DIK_W] = std::make_shared<cCmdMoveW>(3, -5.0f);
	KbCmds->CmdDownMaps[DIK_S] = std::make_shared<cCmdMoveW>(3, 5.0f);
	KbCmds->CmdDownMaps[DIK_A] = std::make_shared<cCmdMoveW>(1, -5.0f);
	KbCmds->CmdDownMaps[DIK_D] = std::make_shared<cCmdMoveW>(1, 5.0f);
	/*KbCmds->CmdDownMaps[DIK_J] = cCmdBase(eJump);
	KbCmds->CmdDownMaps[DIK_ESCAPE] = cCmdBase(eTurnBack);
	KbCmds->CmdDownMaps[DIK_Q] = cCmdBase(eSkill);*/
	KbCmds->CmdUpMaps = KbCmds->CmdDownMaps;
	cLocator::InputLayer().GetKeyboard()->SetReceiver(KbCmds);
	MsCmds = std::make_shared<cMouseCmdProductor>();
	cLocator::InputLayer().GetMouse()->SetReceiver(MsCmds);
	m_SceneCanvas = cLocator::Graphics().NewCanvas(cLocator::Graphics().Width(), cLocator::Graphics().Height());
	m_CurScene = std::make_shared<cGameScene>();
	m_CurScene->CreateScene();
	m_Effect = std::make_shared<Gp::cEffect>(*cLocator::Graphics().Device(), "Effect.cso");
	auto& inputDesc = m_Effect->GetInputDesc();
	std::vector<D3D11_INPUT_ELEMENT_DESC> layoutDesc(3);
	layoutDesc[0] = { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 };
	layoutDesc[1] = { "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D10_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 };
	layoutDesc[2] = { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D10_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 };
	inputDesc[std::make_tuple(0, 0)] = std::make_shared<std::vector<D3D11_INPUT_ELEMENT_DESC>>(layoutDesc);

// 	m_Model = std::make_shared<cModel>(*cLocator::Graphics().Device());
// 	m_Model->LoadModel("Nanosuit2\\nanosuit2.3ds");
	//m_uModel = DirectX::Model::CreateFromSDKMESH(cLocator::Graphics().Device(), L"Teapot.sdkmesh", *m_EffFactory.get());
	
}

void cGameWorld::HandleInput()
{
	cLocator::InputLayer().UpdateDevice();
	auto& kbCmdQ = KbCmds->CmdQ;
	while (!kbCmdQ.empty()) {
		m_CurScene->DispatchCmd(kbCmdQ.front());
		kbCmdQ.pop();
	}
	auto& msCmdQ = MsCmds->CmdQ;
	while (!msCmdQ.empty()) {
		m_CurScene->DispatchCmd(msCmdQ.front());
		msCmdQ.pop();
	}
	m_CurScene->ProcessCmd();
}

void cGameWorld::Update()
{
	m_CurScene->Update();
}

void cGameWorld::Render()
{
 	float cc[4] = { 0,0.5,0.5,1 };
 	cLocator::Graphics().Clear(cc);
 	cLocator::Graphics().ClearDepthStencil(1, 0);
	m_CurScene->Render(m_SceneCanvas);
	// 	cLocator::Graphics().Draw(*m_Model.get(), *m_Effect.get());
	/*using namespace DirectX::SimpleMath;
	DirectX::XMMATRIX world = Matrix::Identity;
	DirectX::XMMATRIX view = Matrix::CreateLookAt(Vector3::UnitZ * 5, Vector3::Zero, Vector3::Up);
	DirectX::XMMATRIX proj = Matrix::CreatePerspectiveFieldOfView(
		DirectX::XM_PIDIV4,
		1.0f * cLocator::Graphics().Width() / cLocator::Graphics().Height(),
		0.01, 1000);*/
// 	m_uModel->UpdateEffects([](DirectX::IEffect* _eff) {
// 		DirectX::BasicEffect *eff = dynamic_cast<DirectX::BasicEffect *>(_eff);
// 		if (NULL != eff)
// 		{
// 			eff->SetDiffuseColor(Vector3(1, 0, 0));
// 		}
// 	});
	//m_uModel->Draw(cLocator::Graphics().Contex(), *m_States.get(), world, view, proj);
	cLocator::Graphics().Present();
}
