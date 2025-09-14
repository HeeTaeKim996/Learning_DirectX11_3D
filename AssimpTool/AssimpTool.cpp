#include "pch.h"
#include "AssimpTool.h"
#include "Converter.h"

void AssimpTool::Init()
{
	CreateModelData(L"Tower", L"Tower");
	//CreateModelDataByName(L"Tank", L"Tank");
	CreateModelData(L"Kachujin", L"Mesh");


	CreateAnimationData(L"Kachujin", L"Idle");
	CreateAnimationData(L"Kachujin", L"Run");
	CreateAnimationData(L"Kachujin", L"Slash");
}

void AssimpTool::Update()
{

}

void AssimpTool::Render()
{

}

void AssimpTool::CreateModelData(wstring folerPath, wstring meshName)
{
	// �� �Ծ��� �ʿ��ѵ�, Assets ���Ͽ� �ش� FBX ���� ����, FBX �𵨸�� ������ ���ϸ� �ʿ�
	wstring fbxPath = folerPath + L"/" + meshName + L".fbx";
	wstring exportPath = folerPath + L"/" + folerPath;

	shared_ptr<Converter> converter = make_shared<Converter>();
	converter->ReadAssetFile(fbxPath);
	converter->ExportMaterialData(exportPath);
	converter->ExportModelData(exportPath);
}

void AssimpTool::CreateAnimationData(wstring folerPath, wstring animationName)
{
	wstring fbxPath = folerPath + L"/" + animationName + L".fbx";
	wstring exportPath = folerPath + L"/" + animationName;

	shared_ptr<Converter> converter = make_shared<Converter>();
	converter->ReadAssetFile(fbxPath);
	converter->ExportAnimationData(exportPath);
}
