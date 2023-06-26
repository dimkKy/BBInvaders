// by Dmitry Kolontay


#include "BBInvadersUtils.h"
#include "Camera/CameraComponent.h"
#include "GenericPlatform/GenericPlatformMisc.h"

float BBInvadersUtils::GetCameraNearPlane(const UCameraComponent& camera)
{
	float nearClipPlane{ camera.OrthoNearClipPlane };
	if (nearClipPlane <= 0.f || FMath::IsNearlyZero(nearClipPlane)) {
		if (IsValid(GEngine)) {
			nearClipPlane = GEngine->NearClipPlane;
		}
		else {
			nearClipPlane = defaultNearClipPlane;
		}
	}
	return nearClipPlane;
}

float BBInvadersUtils::RandomAngle()
{
	return FMath::FRandRange(-180.f, 180.f);
}

/*void BBInvadersUtils::SetupButtons(ChildOf<UObject> auto* object, UButton* button, TFunction<void(void)> onClickedFunc)
{
	check(button);
	button->SetClickMethod(EButtonClickMethod::PreciseClick);
	button->OnClicked.AddDynamic(object, onClickedFunc);
}*/
