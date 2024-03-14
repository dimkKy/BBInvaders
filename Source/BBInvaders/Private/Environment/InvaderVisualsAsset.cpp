// by Dmitry Kolontay


#include "Environment/InvaderVisualsAsset.h"

FPrimaryAssetId UInvaderVisualsAsset::GetPrimaryAssetId() const
{
	return { assetType, GetFName() };
}

UStaticMesh* UInvaderVisualsAsset::GetStaticMesh() const
{
	return nullptr;
}

EInvaderType UInvaderVisualsAsset::GetInvaderType() const
{
	return type;
}

int32 UInvaderVisualsAsset::GetInvaderTypeI() const
{
	return static_cast<int32>(GetInvaderType());
}
