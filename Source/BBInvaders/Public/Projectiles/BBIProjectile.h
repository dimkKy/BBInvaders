// by Dmitry Kolontay

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
//#include "Engine/DataTable.h"
#include "BBIProjectile.generated.h"

class UProjectileData;

UCLASS(Abstract)
class BBINVADERS_API ABBIProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	ABBIProjectile();
	virtual void Tick(float DeltaTime) override;

	static ABBIProjectile* SpawnProjectile(UWorld& w, 
		const FTransform& t, const UProjectileData& d, AActor* owner = nullptr);

	UFUNCTION()
		void OnOverlapBegin(UPrimitiveComponent* component, 
			AActor* otherActor, UPrimitiveComponent* otherComp, 
			int32 otherIndex, bool bFromSweep, const FHitResult& result);

	void SetProjectileData(const UProjectileData& data);

	const UProjectileData* GetProjectileData() const;

protected:
	virtual void BeginPlay() override;

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		UStaticMeshComponent* body;

	UPROPERTY()
		const UProjectileData* projectileData;

	static constexpr float lifespan{ 40.f };
	static constexpr int32 simulationInteractions{ 3 };

};
