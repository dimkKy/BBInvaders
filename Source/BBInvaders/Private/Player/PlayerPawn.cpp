// by Dmitry Kolontay


#include "Player/PlayerPawn.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "CoreSystems/BBInvadersProjectile.h"
#include "CoreSystems/BBInvadersGameModeBase.h"
#include "Environment/PlanetaryThreatable.h"
#include "CoreSystems/AssetProvider.h"
#include "BBInvadersUtils.h"

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
	BBInvadersUtils::ConfigureDefaultCollision<true>(planet, ECC_WorldStatic,
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
	cameraArm->SetRelativeRotation(BBInvadersUtils::UpRotator);
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

	float planetHalfSize{ planet->GetStaticMesh()->GetBounds().GetSphere().W };
	float platformSize{ platform->GetStaticMesh()->GetBounds().GetSphere().W * 2.f };

	float cameraHalfHFOVTan{ FMath::Tan(camera->FieldOfView * PI / 360.f) };

	float cameraArmLength{ (planetHalfSize + platformSize) *
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
	auto* world{ GetWorld() };
	check(world);

	FActorSpawnParameters spawnParams;
	spawnParams.Owner = this;
	spawnParams.SpawnCollisionHandlingOverride =
		ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	world->SpawnActor<ABBInvadersProjectile>(
		world->GetSubsystem<UAssetProvider>()->projectileClass,
		platform->GetSocketTransform(BBInvadersUtils::muzzleSocket),
		spawnParams);
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
	float planetHalfSize{ planet->GetStaticMesh()->GetBounds().GetSphere().W };

	float mapYHalfSize{ FMath::Tan(camera->FieldOfView * PI / 360.f) * 
		(cameraArmLengthRange.Y + planetHalfSize) };

	return { mapYHalfSize * camera->AspectRatio, mapYHalfSize, planetHalfSize };
}

#if WITH_EDITOR
EDataValidationResult APlayerPawn::IsDataValid(TArray<FText>& ValidationErrors)
{
	Super::IsDataValid(ValidationErrors);

	if (!planet->GetStaticMesh()) {
		ValidationErrors.Add(FText::FromString("Invalid planetMesh"));
	}
	
	if (platform->GetStaticMesh()) {
		if (!platform->DoesSocketExist(BBInvadersUtils::muzzleSocket)) {
			ValidationErrors.Add(FText::FromString("socket was not found :" + BBInvadersUtils::muzzleSocket.ToString()));
		}
	}
	else {
		ValidationErrors.Add(FText::FromString("Invalid platformMesh"));
	}

	return ValidationErrors.Num() > 0 ?
		EDataValidationResult::Invalid : EDataValidationResult::Valid;
}
#endif

