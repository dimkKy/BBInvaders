// by Dmitry Kolontay


#include "Player/PlayerPawn.h"
#include "Player/BBInvadersPlayerController.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "CoreSystems/BBInvadersProjectile.h"
#include "CoreSystems/Shooter.h"
#include "CoreSystems/BBInvadersGameModeBase.h"
#include "Environment/PlanetaryThreatable.h"
#include "BBInvadersUtils.h"
#if WITH_EDITOR
#include "Misc/DataValidation.h"
#endif

APlayerPawn::APlayerPawn() :
	planet{ CreateDefaultSubobject<UStaticMeshComponent>("planet") },
	platform{ CreateDefaultSubobject<UStaticMeshComponent>("platform") },
	cameraArm{ CreateDefaultSubobject<USpringArmComponent>("platformArm") },
	camera{ CreateDefaultSubobject<UCameraComponent>("camera") },
	currentRotationSpeed{0.f}, cameraArmLengthRange{450.f, 550.f}, health{maxHealth}
{
	PrimaryActorTick.bCanEverTick = true;

	SetRootComponent(planet);
	planet->SetMobility(EComponentMobility::Stationary);
	BBInvadersUtils::ConfigureDefaultCollision<true>(planet, ECC_WorldDynamic,
		BBInvadersUtils::ECC_Asteroid, BBInvadersUtils::ECC_Invader);
	
	//
	platform->SetupAttachment(planet);
	platform->BodyInstance.bEnableGravity = false;
	BBInvadersUtils::ConfigureDefaultCollision<true>(platform, ECC_Pawn,
		BBInvadersUtils::ECC_Projectile,
		BBInvadersUtils::ECC_Asteroid,
		BBInvadersUtils::ECC_Invader);
	//
	cameraArm->SetupAttachment(planet);
	cameraArm->SetRelativeRotation(BBInvadersUtils::UpRotation);
	cameraArm->TargetArmLength = 300.f;
	cameraArm->bDoCollisionTest = false;
	cameraArm->SetCanEverAffectNavigation(false);

	camera->SetupAttachment(cameraArm, USpringArmComponent::SocketName);
}

void APlayerPawn::PostInitializeComponents()
{
#if WITH_EDITOR
	if (!planet->GetStaticMesh() || !platform->GetStaticMesh()) {
		return Super::PostInitializeComponents();
	}
#endif

	double planetHalfSize{ planet->GetStaticMesh()->GetBounds().GetSphere().W };
	double platformSize{ platform->GetStaticMesh()->GetBounds().GetSphere().W * 2. };

	float cameraHalfHFOVTan{ FMath::Tan(camera->FieldOfView / 360.f * UE_PI) };

	float cameraArmLength{ static_cast<float>(planetHalfSize + platformSize) *
		camera->AspectRatio / cameraHalfHFOVTan };

	cameraArmLengthRange.X = cameraArmLength * minZoomMultiplier;
	cameraArmLengthRange.Y = cameraArmLength * maxZoomMultiplier;

	cameraArm->TargetArmLength = cameraArmLengthRange.X;

	Super::PostInitializeComponents();
	planet->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnOverlapBegin);
	platform->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnOverlapBegin);
}

void APlayerPawn::MoveRight(float value)
{
	Accelerate(value);
}

void APlayerPawn::MoveLeft(float value)
{
	Accelerate(-1.f * value);
}

void APlayerPawn::Accelerate(float value)
{
	bool bHasInput{ !FMath::IsNearlyZero(value) };
	float currentAcc{ bHasInput ? (value * acceleration) : CalcDamping() };
	currentRotationSpeed = FMath::Clamp(
		currentRotationSpeed + (GetWorld()->GetDeltaSeconds() * currentAcc), 
		-1.f * maxSpeed, maxSpeed);
}

void APlayerPawn::ZoomIn(float value)
{
	if (!FMath::IsNearlyZero(value)) {
		ZoomCamera(-1.f * value);
	}
}

void APlayerPawn::ZoomOut(float value)
{
	if (!FMath::IsNearlyZero(value)) {
		ZoomCamera(value);
	}
}

void APlayerPawn::ZoomInAction()
{
	ZoomCamera(-1.f * zoomActionValue);
}

void APlayerPawn::ZoomOutAction()
{
	ZoomCamera(zoomActionValue);
}

void APlayerPawn::ZoomCamera(float value)
{
	cameraArm->TargetArmLength = FMath::Clamp(
		cameraArm->TargetArmLength + (GetWorld()->GetDeltaSeconds() * value * zoomSpeed),
		cameraArmLengthRange.X, cameraArmLengthRange.Y);
}

void APlayerPawn::Shoot()
{
	check(GetWorld());
	if (auto* projectile{ GetController<ABBInvadersPlayerController>()->GetSelectedProjectile() }) {
		ABBInvadersProjectile::SpawnProjectile(*GetWorld(),
			platform->GetSocketTransform(BBInvadersUtils::muzzleSocket),
			*projectile, this);
	}	
}

EShooterType APlayerPawn::GetShooterType() const
{
	return EShooterType::EST_PlayerOnly;
}

float APlayerPawn::CalcDamping() const
{
	//current speed
	/*if (currentRotationSpeed >= 0.f) {
		return -1.f * dampingMultiplier * currentRotationSpeed * currentRotationSpeed;
	}
	else {
		return dampingMultiplier * currentRotationSpeed * currentRotationSpeed;
	}*/
	//FMath::Sqrt()?
	return -1.f * dampingMultiplier * currentRotationSpeed;
}

void APlayerPawn::OnOverlapBegin(UPrimitiveComponent* comp, AActor* otherActor, 
	UPrimitiveComponent* otherComp, int32 otherIndex, bool bFromSweep, const FHitResult& result)
{
	if (comp == platform && otherActor->GetOwner() != this) {
		return OnGameOver();
	}

	if (auto* thread{Cast<IPlanetaryThreatable>(otherActor)}) {
		health -= thread->GetOnPlanetCollisionDamage();
		if (health <= 0.f) {
			return OnGameOver();
		}
	}
}

void APlayerPawn::OnGameOver()
{
	GetWorld()->GetAuthGameMode<ABBInvadersGameModeBase>()->OnGameOver();
}

void APlayerPawn::Tick(float DeltaTime)
{
	auto rotation{ platform->GetRelativeRotation() };
	rotation.Yaw += DeltaTime * currentRotationSpeed;
	platform->SetRelativeLocationAndRotation(platform->GetRelativeLocation(), rotation);
	health += healthRegenRate * DeltaTime;
	if (health > maxHealth) {
		health = maxHealth;
	}

	Super::Tick(DeltaTime);
}

void APlayerPawn::SetupPlayerInputComponent(UInputComponent* InputComp)
{
	Super::SetupPlayerInputComponent(InputComp);

	InputComp->BindAxis("MoveRight", this, &APlayerPawn::MoveRight);
	InputComp->BindAxis("MoveLeft", this, &APlayerPawn::MoveLeft);

	InputComp->BindAxis("ZoomIn", this, &APlayerPawn::ZoomIn);
	InputComp->BindAxis("ZoomOut", this, &APlayerPawn::ZoomOut);

	InputComp->BindAction("ZoomInAction", IE_Pressed, this, &APlayerPawn::ZoomInAction);
	InputComp->BindAction("ZoomOUtAction", IE_Pressed, this, &APlayerPawn::ZoomOutAction);

	InputComp->BindAction("Shoot", IE_Released, this, &APlayerPawn::Shoot);
}

FVector APlayerPawn::CalcMapHalfSize() const
{
	double planetHalfSize{ planet->GetStaticMesh()->GetBounds().GetSphere().W };

	double mapYHalfSize{ FMath::Tan(camera->FieldOfView / 360. * UE_PI ) *
		(cameraArmLengthRange.Y + planetHalfSize) };

	return { mapYHalfSize, mapYHalfSize * camera->AspectRatio, planetHalfSize };
}

#if WITH_EDITOR
EDataValidationResult APlayerPawn::IsDataValid(FDataValidationContext& context) const
{
	Super::IsDataValid(context);

	if (!planet->GetStaticMesh()) {
		context.AddError(FText::FromString("Invalid planetMesh"));
	}
	
	if (platform->GetStaticMesh()) {
		if (!platform->DoesSocketExist(BBInvadersUtils::muzzleSocket)) {
			context.AddError(FText::FromString("socket was not found :"
				+ BBInvadersUtils::muzzleSocket.ToString()));
		}
	}
	else {
		context.AddError(FText::FromString("Invalid platformMesh"));
	}

	return context.GetNumErrors() > 0 ?
		EDataValidationResult::Invalid : EDataValidationResult::Valid;
}
#endif

