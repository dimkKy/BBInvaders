// by Dmitry Kolontay


#include "Environment/Invader.h"
#include "Components/StaticMeshComponent.h"
#include "CoreSystems/BBInvadersProjectile.h"
#include "BBInvadersUtils.h"
#include "Player/PlayerPawn.h"

AInvader::AInvader() :
	body{ CreateDefaultSubobject<UStaticMeshComponent>("body") }
{
	PrimaryActorTick.bCanEverTick = false;
	SetRootComponent(body);

	body->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	//asteroids->bMultiBodyOverlap = true;
	body->SetGenerateOverlapEvents(true);
	body->SetCollisionObjectType(BBInvadersUtils::ECC_Invader);
	body->SetCollisionResponseToAllChannels(ECR_Ignore);
	body->SetCollisionResponseToChannel(BBInvadersUtils::ECC_Projectile, ECR_Overlap);

	body->OnComponentBeginOverlap.AddDynamic(this, &AInvader::OnOverlapBegin);
}

void AInvader::BeginPlay()
{
	Super::BeginPlay();
}

void AInvader::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//FTransform transform;
	//asteroids->GetInstanceTransform(, transform, true);
	//ast.AddToTranslation();
	//asteroids->UpdateInstanceTransform(0, transform, , , );
}

void AInvader::Shoot()
{
	FActorSpawnParameters spawnParams;
	spawnParams.Owner = this;
	spawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	GetWorld()->SpawnActor<ABBInvadersProjectile>(projectileClass, 
		body->GetSocketTransform(BBInvadersUtils::muzzleSocket), spawnParams);
}

void AInvader::OnOverlapBegin(UPrimitiveComponent* component, AActor* otherActor, UPrimitiveComponent* otherComp, int32 otherIndex, bool bFromSweep, const FHitResult& result)
{
	if (otherActor->IsA(APlayerPawn::StaticClass())) {
		Destroy();
		body->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}

