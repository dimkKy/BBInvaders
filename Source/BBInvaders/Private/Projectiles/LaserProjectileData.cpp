// by Dmitry Kolontay


#include "Projectiles/LaserProjectileData.h"
#include "BBInvadersUtils.h"


void ULaserProjectileData::OnShooting(const FVector& from, const FVector& to, AActor* owner, AActor* target) const
{
	check(owner);
	FHitResult result;
	using namespace BBInvadersUtils;
	//vfx?
	check(target == nullptr);
	//requiest damage from gamemode?
	if (owner->GetWorld()->LineTraceSingleByChannel(result, from, to, ECC_Projectile)) {
		if (auto* actor{ result.GetActor() }; actor->CanBeDamaged()) {

			result.GetActor()->TakeDamage(baseDamage, BBIDamageEvents::Laser, /*controller*/nullptr, owner);
		}
		
	}
}
