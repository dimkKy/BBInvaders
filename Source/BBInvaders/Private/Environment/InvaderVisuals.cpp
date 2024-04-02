// by Dmitry Kolontay


#include "Environment/InvaderVisuals.h"

FPrimaryAssetId UInvaderVisuals::GetPrimaryAssetId() const
{
	return { assetType, GetFName() };
}

UStaticMesh* UInvaderVisuals::GetStaticMesh() const
{
	return nullptr;
}

EInvaderType UInvaderVisuals::GetInvaderType() const
{
	return type;
}

int32 UInvaderVisuals::GetInvaderTypeI() const
{
	return static_cast<int32>(GetInvaderType());
}
