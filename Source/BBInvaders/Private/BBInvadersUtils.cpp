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

double BBInvadersUtils::RandomAngle_Double()
{
	return FMath::FRandRange(-180., 180.);
}

FRotator BBInvadersUtils::RandomRotator()
{
	return { RandomAngle(), RandomAngle(), RandomAngle() };
}

float BBInvadersUtils::RandAbsRange(float absMax)
{
	return FMath::RandRange(-1.f * absMax, absMax);
}

double BBInvadersUtils::RandAbsRange(double absMax)
{
	return FMath::RandRange(-1. * absMax, absMax);
}

/*void BBInvadersUtils::SetupButtons(ChildOf<UObject> auto* object, UButton* button, TFunction<void(void)> onClickedFunc)
{
	check(button);
	button->SetClickMethod(EButtonClickMethod::PreciseClick);
	button->OnClicked.AddDynamic(object, onClickedFunc);
}*/
