// by Dmitry Kolontay

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
//#include "Engine/DataTable.h"
#include "BBIProjectile.generated.h"

class UProjectileData;

UCLASS()
class BBINVADERS_API ABBIProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	ABBIProjectile();
	virtual void Tick(float DeltaTime) override;

	static ABBIProjectile* SpawnProjectile(UWorld& w, 
		const FTransform& t, const UProjectileData& d, AActor* owner = nullptr);

	virtual void NotifyHit(class UPrimitiveComponent* myComp, AActor* other, 
		class UPrimitiveComponent* otherComp, bool bSelfMoved, FVector hitLoc, 
		FVector hitNormal, FVector normalImpulse, const FHitResult& Hit) override;

	void SetProjectileData(const UProjectileData& data);

	const UProjectileData* GetProjectileData() const;

protected:
	virtual void BeginPlay() override;

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		UStaticMeshComponent* body;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		UStaticMeshSocket* test;

	UPROPERTY()
		const UProjectileData* projectileData;

	static constexpr float lifespan{ 40.f };
	static constexpr int32 simulationInteractions{ 3 };

};
