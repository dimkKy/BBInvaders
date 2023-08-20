// by Dmitry Kolontay


#include "Environment/Invader.h"
#include "Components/StaticMeshComponent.h"
#include "CoreSystems/BBInvadersProjectile.h"
#include "BBInvadersUtils.h"
#include "Player/PlayerPawn.h"
#include "Player/BBInvadersPlayerState.h"

AInvader::AInvader() :
	body{ CreateDefaultSubobject<UStaticMeshComponent>("body") }
{
	PrimaryActorTick.bCanEverTick = false;
	SetRootComponent(body);

	BBInvadersUtils::ConfigureDefaultCollision<true>(body, BBInvadersUtils::ECC_Invader,
		BBInvadersUtils::ECC_Projectile, ECC_Pawn, ECC_WorldDynamic);
}

void AInvader::SetMesh(UStaticMesh& newMesh)
{
	body->SetStaticMesh(&newMesh);
}

void AInvader::Shoot()
{
	auto* world{ GetWorld() };
	check(GetWorld() && false);

	/*ABBInvadersProjectile::SpawnProjectile(*GetWorld(), 
		body->GetSocketTransform(BBInvadersUtils::muzzleSocket),
		, this);*/

}

EShooterType AInvader::GetShooterType() const
{
	return EShooterType::EST_Invader;
}

/*void AInvader::SetLookAt(const FVector& worldPos)
{
	FVector thisLocation{ GetActorLocation() };

	FVector toTarget{ worldPos - thisLocation };

	body->SetRelativeRotation(FRotator{ 0.f });

	SetActorLocationAndRotation(thisLocation,
		FRotationMatrix::MakeFromX(toTarget).Rotator());
}*/

float AInvader::GetOnPlanetCollisionDamage() const
{
	return 55.f;
}

int32 AInvader::GetOnKillBounty() const
{
	return 100;
}

void AInvader::BeginPlay()
{
	Super::BeginPlay();
	body->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnOverlapBegin);
}

void AInvader::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	onDestroyedDelegate.ExecuteIfBound(this);
	onDestroyedDelegate.Unbind();
	Super::EndPlay(EndPlayReason);
}

void AInvader::OnOverlapBegin(UPrimitiveComponent* component, AActor* otherActor,
	UPrimitiveComponent* otherComp, int32 otherIndex, bool bFromSweep, const FHitResult& result)
{
	if (otherActor->IsA(APlayerPawn::StaticClass())) {
		DestroyGiveReward();
	}
	else {
		if (APlayerPawn* otherOwner{ Cast<APlayerPawn>(otherActor->GetOwner()) }) {
			DestroyGiveReward(otherOwner);
		}
	}
}

void AInvader::DestroyGiveReward(APlayerPawn* bountyReceiver/* = nullptr*/)
{
	Destroy();
	body->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	if (IsValid(bountyReceiver)) {

	}
}

#if WITH_EDITOR
EDataValidationResult AInvader::IsDataValid(TArray<FText>& ValidationErrors)
{
	Super::IsDataValid(ValidationErrors);

	if (GetWorld()) {
		if (body->GetStaticMesh()) {
			if (!body->DoesSocketExist(BBInvadersUtils::muzzleSocket)) {
				ValidationErrors.Add(FText::FromString("socket was not found :" + BBInvadersUtils::muzzleSocket.ToString()));
			}
		}
		else {
			ValidationErrors.Add(FText::FromString("no mesh assigned"));
		}
	}

	return ValidationErrors.Num() > 0 ?
		EDataValidationResult::Invalid : EDataValidationResult::Valid;
}
#endif
