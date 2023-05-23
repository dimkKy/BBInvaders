// by Dmitry Kolontay

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Invader.generated.h"

class UStaticMeshComponent;
class UStaticMesh;
class ABBInvadersProjectile;

UCLASS()
class BBINVADERS_API AInvader : public AActor
{
	GENERATED_BODY()
	
public:	
	AInvader();
	virtual void Tick(float DeltaTime) override;
	//void Init(UStaticMesh& newMesh, )
	void SetMesh(UStaticMesh& newMesh);

	void Shoot();

	void SetLookAt(const FVector& worldPos);

	UFUNCTION()
		void OnOverlapBegin(UPrimitiveComponent* component,
			AActor* otherActor, UPrimitiveComponent* otherComp,
			int32 otherIndex, bool bFromSweep, const FHitResult& result);
protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		UStaticMeshComponent* body;
};
