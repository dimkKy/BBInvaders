// by Dmitry Kolontay


#include "Environment/Invader.h"
#include "Components/StaticMeshComponent.h"
#include "CoreSystems/BBInvadersProjectile.h"
#include "BBInvadersUtils.h"
#include "Player/PlayerPawn.h"
#include "Player/BBInvadersPlayerState.h"
#if WITH_EDITOR
#include "Misc/DataValidation.h"
#endif

AInvader::AInvader() :
	body{ CreateDefaultSubobject<UStaticMeshComponent>("body") }
{
	PrimaryActorTick.bCanEverTick = false;
	SetRootComponent(body);

	using namespace BBInvadersUtils;
	ConfigureDefaultCollision<true>(body, ECC_Invader,
		ECC_Projectile, ECC_Pawn, ECC_WorldDynamic);

	//hide
}

void AInvader::SetMesh(UStaticMesh& newMesh)
{
	body->SetStaticMesh(&newMesh);
}

double AInvader::GetCollisionRadius() const
{
	check(body->GetStaticMesh());
	return body->GetStaticMesh()->GetBounds().GetSphere().W;
}

void AInvader::Shoot()
{
	auto* world{ GetWorld() };
	check(GetWorld() && false);

	/*ABBIProjectile::SpawnProjectile(*GetWorld(), 
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
EDataValidationResult AInvader::IsDataValid(FDataValidationContext& context) const
{
	Super::IsDataValid(context);

	if (GetWorld()) {
		if (body->GetStaticMesh()) {
			if (!body->DoesSocketExist(BBInvadersUtils::muzzleSocket)) {
				context.AddError(FText::FromString("socket was not found :" + BBInvadersUtils::muzzleSocket.ToString()));
			}
		}
		else {
			context.AddError(FText::FromString("no mesh assigned"));
		}
	}

	return context.GetNumErrors() > 0 ?
		EDataValidationResult::Invalid : EDataValidationResult::Valid;
}
#endif
