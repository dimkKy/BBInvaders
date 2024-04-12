// by Dmitry Kolontay


#include "Projectiles/LaserProjectileData.h"
#include "BBInvadersUtils.h"


void ULaserProjectileData::OnShooting(const FVector& from, const FVector& to, AActor* owner, AActor* target) const
{
	FHitResult result;
	using namespace BBInvadersUtils;
	//vfx?

	//requiest damage from gamemode?
	if (owner->GetWorld()->LineTraceSingleByChannel(result, from, to, ECC_Projectile)) {
		if (auto* actor{ result.GetActor() }; actor->CanBeDamaged()) {
			FBBIDamageEvent damageEvent{ EDT::Laser };

			result.GetActor()->TakeDamage(baseDamage, damageEvent, /*controller*/nullptr, owner);
		}
		
	}
}
