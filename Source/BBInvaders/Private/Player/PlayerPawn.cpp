// by Dmitry Kolontay


#include "Player/PlayerPawn.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "CoreSystems/BBInvadersProjectile.h"
#include "CoreSystems/BBInvadersGameModeBase.h"
#include "BBInvadersUtils.h"
//#include "Kismet/GameplayStatics.h"

//MyGameMode* MyMode = Cast< MyGameMode>(UGameplayStatics::GetGameMode(GetWorld()));

APlayerPawn::APlayerPawn() :
	planet{ CreateDefaultSubobject<UStaticMeshComponent>("planet") },
	//platformArm{ CreateDefaultSubobject<USpringArmComponent>("platformArm") },
	platform{ CreateDefaultSubobject<UStaticMeshComponent>("platform") },
	cameraArm{ CreateDefaultSubobject<USpringArmComponent>("platformArm") },
	camera{ CreateDefaultSubobject<UCameraComponent>("camera") },
	cameraArmLengthRange{450.f, 550.f}
{
	PrimaryActorTick.bCanEverTick = true;
	SetRootComponent(planet);

	planet->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	planet->SetMobility(EComponentMobility::Stationary);
	planet->SetCollisionObjectType(ECC_WorldStatic);
	planet->SetCollisionResponseToAllChannels(ECR_Ignore);
	planet->SetCollisionResponseToChannel(BBInvadersUtils::ECC_Asteroid, ECR_Block);
	planet->SetCollisionResponseToChannel(BBInvadersUtils::ECC_Invader, ECR_Block);

	/*platformArm->SetupAttachment(planet);
	//redo
	platformArm->TargetArmLength = 10.f;
	platformArm->bDoCollisionTest = false;
	platformArm->bUsePawnControlRotation = false;*/

	//platform->SetStaticMesh(platformMesh);
	platform->BodyInstance.bEnableGravity = false;
	//platform->SetupAttachment(platformArm);
	platform->SetupAttachment(planet);
	platform->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	platform->SetCollisionObjectType(ECC_Pawn);
	platform->SetCollisionResponseToAllChannels(ECR_Ignore);
	platform->SetCollisionResponseToChannel(BBInvadersUtils::ECC_Projectile, ECR_Block);
	platform->SetCollisionResponseToChannel(BBInvadersUtils::ECC_Asteroid, ECR_Block);
	platform->SetCollisionResponseToChannel(BBInvadersUtils::ECC_Invader, ECR_Block);

	cameraArm->SetupAttachment(planet);
	cameraArm->SetRelativeRotation(BBInvadersUtils::UpRotator);
	cameraArm->TargetArmLength = 300.f;
	cameraArm->bDoCollisionTest = false;

	camera->SetupAttachment(cameraArm, USpringArmComponent::SocketName);
}

void APlayerPawn::OnConstruction(const FTransform& Transform)
{
	//check(planet->GetStaticMesh() && platform->GetStaticMesh());
	//camera->FieldOfView;

	//float veritcalHalfFov{ 0.f };
/*#if WITH_EDITOR
	if (!planet->GetStaticMesh() || !platform->GetStaticMesh()) {
		return Super::OnConstruction(Transform);
	}
#endif
	float planetHalfSize{ planet->GetStaticMesh()->GetBoundingBox().GetSize().X * 0.5f };
	float platformSize{ platform->GetStaticMesh()->GetBoundingBox().GetSize().X };
	float cameraHorHalfFOV{ camera->FieldOfView * PI / 360.f };

	float cameraArmLength{ (planetHalfSize + platformSize) *
		camera->AspectRatio / FMath::Tan(cameraHorHalfFOV) };
	minCameraArmLength = cameraArmLength * minZoomMultiplier;
	maxCameraArmLength = cameraArmLength * maxZoomMultiplier;

	cameraArm->TargetArmLength = minCameraArmLength;*/

	Super::OnConstruction(Transform);
}

void APlayerPawn::PostInitializeComponents()
{
#if WITH_EDITOR
	if (!planet->GetStaticMesh() || !platform->GetStaticMesh()) {
		return Super::PostInitializeComponents();
	}
#endif

	float planetHalfSize{ planet->GetStaticMesh()->GetBoundingBox().GetSize().X * 0.5f };
	float platformSize{ platform->GetStaticMesh()->GetBoundingBox().GetSize().X };
	//float cameraHorHalfFOV{ camera->FieldOfView * PI / 360.f };
	float cameraHalfHFOVTan{ FMath::Tan(camera->FieldOfView * PI / 360.f) };

	float cameraArmLength{ (planetHalfSize + platformSize) *
		camera->AspectRatio / cameraHalfHFOVTan };
	cameraArmLengthRange.X = cameraArmLength * minZoomMultiplier;
	cameraArmLengthRange.Y = cameraArmLength * maxZoomMultiplier;

	cameraArm->TargetArmLength = cameraArmLengthRange.X;

	/*if (auto* world{ GetWorld() }) {
		float mapYHalfSize{ cameraHalfHFOVTan * (cameraArmLength + planetHalfSize) };

		CastChecked<ABBInvadersGameModeBase>(world->GetAuthGameMode())->
			SetMapHalfSize({ mapYHalfSize * camera->AspectRatio, mapYHalfSize });
	}*/
	Super::PostInitializeComponents();
}

// Called when the game starts or when spawned
void APlayerPawn::BeginPlay()
{
	Super::BeginPlay();
	
}

void APlayerPawn::MoveRight(float value)
{
	Accelerate(value);
}

void APlayerPawn::MoveLeft(float value)
{
	Accelerate(-1.f * value);
}

/*void APlayerPawn::Stop()
{
	
	check(false);
}*/

void APlayerPawn::Accelerate(float value)
{
	bool bHasInput{ !FMath::IsNearlyZero(value) };
	float currentAcceleration{ bHasInput ? (value * acceleration) : CalcDamping() };
	currentRotationSpeed = FMath::Clamp(
		currentRotationSpeed + (GetWorld()->GetDeltaSeconds() * currentAcceleration), 
			-1.f * maxSpeed, maxSpeed);
}

void APlayerPawn::ZoomIn(float value)
{
	if (!FMath::IsNearlyZero(value)) {
		GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, FString::Printf(TEXT("ZoomIn")));
		ZoomCamera(-1.f * value);
	}
}

void APlayerPawn::ZoomOut(float value)
{
	if (!FMath::IsNearlyZero(value)) {
		GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, FString::Printf(TEXT("ZoomOut")));
		ZoomCamera(value);
	}
}

void APlayerPawn::ZoomCamera(float value)
{
	cameraArm->TargetArmLength = FMath::Clamp(
		cameraArm->TargetArmLength + (GetWorld()->GetDeltaSeconds() * value * zoomSpeed),
		cameraArmLengthRange.X, cameraArmLengthRange.Y);
}

void APlayerPawn::Shoot()
{
	FActorSpawnParameters spawnParams;
	spawnParams.Owner = this;
	spawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	GetWorld()->SpawnActor<ABBInvadersProjectile>(projectileClass, 
		platform->GetSocketTransform(BBInvadersUtils::muzzleSocket), spawnParams);
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

// Called every frame
void APlayerPawn::Tick(float DeltaTime)
{
	auto rotation{ platform->GetRelativeRotation() };
	rotation.Yaw += DeltaTime * currentRotationSpeed;
	GEngine->AddOnScreenDebugMessage(-1, DeltaTime, FColor::Red, FString::Printf(TEXT("%.6f"), currentRotationSpeed));
	platform->SetRelativeLocationAndRotation(platform->GetRelativeLocation(), rotation);

	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void APlayerPawn::SetupPlayerInputComponent(UInputComponent* InputComp)
{
	Super::SetupPlayerInputComponent(InputComp);
	InputComp->BindAxis("MoveRight", this, &APlayerPawn::MoveRight);
	InputComp->BindAxis("MoveLeft", this, &APlayerPawn::MoveLeft);

	InputComp->BindAxis("ZoomIn", this, &APlayerPawn::ZoomIn);
	InputComp->BindAxis("ZoomOut", this, &APlayerPawn::ZoomOut);

	InputComp->BindAction("Shoot", IE_Released, this, &APlayerPawn::Shoot);
}

FVector APlayerPawn::CalcMapHalfSize() const
{
	float planetHalfSize{ planet->GetStaticMesh()->GetBoundingBox().GetSize().X * 0.5f};

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
	if (!projectileClass) {
		ValidationErrors.Add(FText::FromString("Invalid projectileClass"));
	}

	return ValidationErrors.Num() > 0 ?
		EDataValidationResult::Invalid : EDataValidationResult::Valid;
}
#endif

