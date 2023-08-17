// by Dmitry Kolontay

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "OutOfAreaActorTracker.generated.h"

class UBoxComponent;

UCLASS()
class BBINVADERS_API AOutOfAreaActorTracker : public AActor
{
	GENERATED_BODY()
	
public:
	AOutOfAreaActorTracker();
	void SetTrackArea(const FTransform& transform, const FVector& halfSize);
	void SetTrackArea(const FVector& halfSize);

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
		void OnTrackEnd(UPrimitiveComponent* comp, AActor* other,
			UPrimitiveComponent* otherComp, int32 otherBodyIndex);

	UFUNCTION()
		void OnKillOverlapBegin(UPrimitiveComponent* comp, AActor* other,
			UPrimitiveComponent* otherComp, int32 otherBodyIndex, bool bFromSweep, const FHitResult& sweepResult);

	UPROPERTY(VisibleAnywhere)
		UBoxComponent* asteroidTrackBox;

	UPROPERTY(VisibleAnywhere)
		UBoxComponent* zKillBoxes[2];
	static const FVector zKillBoxExtent;

	UPROPERTY(VisibleAnywhere)
		UBoxComponent* xyKillBoxes[4];
	static const FVector xKillBoxExtent;
	static const FVector yKillBoxExtent;

};
