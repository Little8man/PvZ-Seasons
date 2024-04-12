#include "Board.h"
#include "Plant.h"
#include "Zombie.h"
#include "Cutscene.h"
#include "Projectile.h"
#include "../LawnApp.h"
#include "../Resources.h"
#include "../GameConstants.h"
#include "../Sexy.TodLib/TodFoley.h"
#include "../Sexy.TodLib/TodDebug.h"
#include "../Sexy.TodLib/Reanimator.h"
#include "../Sexy.TodLib/Attachment.h"

ProjectileDefinition gProjectileDefinition[] = {  //0x69F1C0
	{ ProjectileType::PROJECTILE_PEA,           0,  20,	0				},
	{ ProjectileType::PROJECTILE_SNOWPEA,       0,  20,	0				},
	{ ProjectileType::PROJECTILE_CABBAGE,       0,  40,	0				},
	{ ProjectileType::PROJECTILE_MELON,         0,  80,	0				},
	{ ProjectileType::PROJECTILE_PUFF,          0,  20,	0				},
	{ ProjectileType::PROJECTILE_WINTERMELON,   0,  80,	0				},
	{ ProjectileType::PROJECTILE_FIREPEA,		0,  40,	0				},
	{ ProjectileType::PROJECTILE_STAR,          0,  20,	0				},
	{ ProjectileType::PROJECTILE_SPIKE,         0,  20,	1				},
	{ ProjectileType::PROJECTILE_BASKETBALL,    0,  75,	0				},
	{ ProjectileType::PROJECTILE_KERNEL,        0,  20,	0				},
	{ ProjectileType::PROJECTILE_COBBIG,        0,  300,0				},
	{ ProjectileType::PROJECTILE_BUTTER,        0,  40,	0				},
	//{ ProjectileType::PROJECTILE_ZOMBIE_PEA,  0,  20,	0				},
	{ ProjectileType::PROJECTILE_LETTUCE,       0,  40,	0				},
	{ ProjectileType::PROJECTILE_PLASMAPEA,     0,  60,	0				},
	{ ProjectileType::PROJECTILE_PEPPER,		0,  80,	0				},
	{ ProjectileType::PROJECTILE_MINT,			0,  40,	NUM_MAX_PIERCE	},
	{ ProjectileType::PROJECTILE_BEE,			0,  20,	0				},
	{ ProjectileType::PROJECTILE_ICEPEA,		0,  20,	0				},
	{ ProjectileType::PROJECTILE_GOOPEA,		0,  10,	0				},
	{ ProjectileType::PROJECTILE_BOMB,			0,  0,	0				},
	{ ProjectileType::PROJECTILE_FUTURESTAR,    0,  20,	3				},
	{ ProjectileType::PROJECTILE_SUN,			0,  320,0				},
	{ ProjectileType::PROJECTILE_MOLD,          0,  20,	0				},
	{ ProjectileType::PROJECTILE_BIGPLASMAPEA,  0,  0,	0				},
	{ ProjectileType::PROJECTILE_ARROW,         0,  130,0				},
	{ ProjectileType::PROJECTILE_SPECIALARROW,  0,  300,0				},
	{ ProjectileType::PROJECTILE_ASPARAGUS,     0,  40,	3				},
	{ ProjectileType::PROJECTILE_MCARROW,		0,  40,	0				},
	{ ProjectileType::PROJECTILE_FIREBALL,		0,  40,	NUM_MAX_PIERCE	},
	{ ProjectileType::PROJECTILE_PLASMABALL,	0,  60,	NUM_MAX_PIERCE	},
	{ ProjectileType::PROJECTILE_BOOMERANG,		0,  20,	2				},
	{ ProjectileType::PROJECTILE_ROCKPEA,       0,  50,	0				},
	{ ProjectileType::PROJECTILE_SEASTARFRUIT,	0,  900,NUM_MAX_PIERCE	},
	{ ProjectileType::PROJECTILE_SPORE,         0,  40,	0				},
};

Projectile::Projectile()
{
}

Projectile::~Projectile()
{
	AttachmentDie(mAttachmentID);
}

//0x46C730
void Projectile::ProjectileInitialize(int theX, int theY, int theRenderOrder, int theRow, ProjectileType theProjectileType)
{
	int aGridX = mBoard->PixelToGridXKeepOnBoard(theX, theY);
	mProjectileType = theProjectileType;
	mPosX = theX;
	mPosY = theY;
	mPosZ = 0.0f;
	mVelX = 3.33f;
	mVelY = 0.0f;
	mVelZ = 0.0f;
	mAccZ = 0.0f;
	mShadowY = mBoard->GridToPixelY(aGridX, theRow) + 67.0f;
	mHitTorchwoodGridX = -1;
	mMotionType = ProjectileMotion::MOTION_STRAIGHT;
	mFrame = 0;
	mNumFrames = 1;
	mRow = theRow;
	mCobTargetX = 0.0f;
	mDamageRangeFlags = 0;
	mDead = false;
	mAttachmentID = AttachmentID::ATTACHMENTID_NULL;
	mCobTargetRow = 0;
	mTargetZombieID = ZombieID::ZOMBIEID_NULL;
	mOnHighGround = mBoard->mGridSquareType[aGridX][theRow] == GridSquareType::GRIDSQUARE_HIGH_GROUND;
	if (mBoard->StageHasRoof())
	{
		mShadowY -= 12.0f;
	}
	mRenderOrder = theRenderOrder;
	mRotation = 0.0f;
	mRotationSpeed = 0.0f;
	mWidth = 40;
	mHeight = 40;
	mProjectileAge = 0;
	mClickBackoffCounter = 0;
	mAnimTicksPerFrame = 0;
	// Pierce
	mPierce = GetProjectileDef().mMaxPierce;
	mPersistentPierce = false;
	for (int i = 0; i < NUM_MAX_PIERCE; i++)
	{
		mPierceZombieIDs[i] = ZombieID::ZOMBIEID_NULL;
	}
	// Boomerang
	mTargetX = 700 + LAWN_XMIN;
	mOriginX = 40 + LAWN_XMIN;
	mTime = 200;

	if (mProjectileType == ProjectileType::PROJECTILE_PEPPER || mProjectileType == ProjectileType::PROJECTILE_CABBAGE || mProjectileType == ProjectileType::PROJECTILE_LETTUCE  || mProjectileType == ProjectileType::PROJECTILE_BUTTER)
	{
		mRotation = -7 * PI / 25;  // DEG_TO_RAD(-50.4f);
		mRotationSpeed = RandRangeFloat(-0.08f, -0.02f);
	}
	if (mProjectileType == ProjectileType::PROJECTILE_FIREBALL || mProjectileType == ProjectileType::PROJECTILE_PLASMABALL)
		mRotationSpeed = RandRangeFloat(-0.2f, -0.08f);
	else if (mProjectileType == ProjectileType::PROJECTILE_BOOMERANG)
	{
		mPersistentPierce = true;
	}
	else if (mProjectileType == ProjectileType::PROJECTILE_MELON || mProjectileType == ProjectileType::PROJECTILE_WINTERMELON)
	{
		mRotation = -2 * PI / 5;  // DEG_TO_RAD(-72.0f);
		mRotationSpeed = RandRangeFloat(-0.08f, -0.02f);
	}
	else if (mProjectileType == ProjectileType::PROJECTILE_KERNEL)
	{
		mRotation = 0.0f;
		mRotationSpeed = RandRangeFloat(-0.2f, -0.08f);
	}
	else if (mProjectileType == ProjectileType::PROJECTILE_SNOWPEA || mProjectileType == ProjectileType::PROJECTILE_LETTUCE || mProjectileType == ProjectileType::PROJECTILE_ICEPEA)
	{
		TodParticleSystem* aParticle = mApp->AddTodParticle(mPosX + 8.0f, mPosY + 13.0f, 400000, ParticleEffect::PARTICLE_SNOWPEA_TRAIL);
		AttachParticle(mAttachmentID, aParticle, 8.0f, 13.0f);
	}
	else if (mProjectileType == ProjectileType::PROJECTILE_COBBIG)
	{
		mWidth = IMAGE_REANIM_COBCANNON_COB->GetWidth();
		mHeight = IMAGE_REANIM_COBCANNON_COB->GetHeight();
		mRotation = PI / 2;
	}
	else if (mProjectileType == ProjectileType::PROJECTILE_SPECIALARROW)
		mRotation = PI / 2;
	else if (mProjectileType == ProjectileType::PROJECTILE_PUFF || mProjectileType == ProjectileType::PROJECTILE_MOLD || mProjectileType == ProjectileType::PROJECTILE_SPORE)
	{
		TodParticleSystem* aParticle = mApp->AddTodParticle(mPosX + 13.0f, mPosY + 13.0f, 400000, ParticleEffect::PARTICLE_PUFFSHROOM_TRAIL);
		AttachParticle(mAttachmentID, aParticle, 13.0f, 13.0f);
	}
	else if (mProjectileType == ProjectileType::PROJECTILE_BASKETBALL)
	{
		mRotation = RandRangeFloat(0.0f, 2 * PI);
		mRotationSpeed = RandRangeFloat(0.05f, 0.1f);
	}
	else if (mProjectileType == ProjectileType::PROJECTILE_STAR || mProjectileType == ProjectileType::PROJECTILE_FUTURESTAR)
	{
		mShadowY += 15.0f;
		mRotationSpeed = RandRangeFloat(0.05f, 0.1f);
		if (Rand(2) == 0)
		{
			mRotationSpeed = -mRotationSpeed;
		}
	}

	mAnimCounter = 0;
	mX = (int)mPosX;
	mY = (int)mPosY;
}

//0x46CAA0
Plant* Projectile::FindCollisionTargetPlant()
{
	Rect aProjectileRect = GetProjectileRect();

	Plant* aPlant = nullptr;
	while (mBoard->IteratePlants(aPlant))
	{
		if (aPlant->mRow != mRow)
			continue;

		if (mMotionType != ProjectileMotion::MOTION_LOBBED)
		{
			if (aPlant->mSeedType == SeedType::SEED_PUFFSHROOM ||
				aPlant->mSeedType == SeedType::SEED_SUNSHROOM ||
				aPlant->mSeedType == SeedType::SEED_POTATOMINE ||
				aPlant->mSeedType == SeedType::SEED_SPIKEWEED ||
				aPlant->mSeedType == SeedType::SEED_SPIKEROCK ||
				aPlant->mSeedType == SeedType::SEED_LILYPAD)  // 僵尸豌豆不能击中低矮植物
				continue;
		}

		Rect aPlantRect = aPlant->GetPlantRect();
		if (GetRectOverlap(aProjectileRect, aPlantRect) > 8)
		{
			if (mMotionType == ProjectileMotion::MOTION_LOBBED)
			{
				return mBoard->GetTopPlantAt(aPlant->mPlantCol, aPlant->mRow, PlantPriority::TOPPLANT_CATAPULT_ORDER);
			}
			else
			{
				return mBoard->GetTopPlantAt(aPlant->mPlantCol, aPlant->mRow, PlantPriority::TOPPLANT_EATING_ORDER);
			}
		}
	}

	return nullptr;
}

//0x46CC30
bool Projectile::PeaAboutToHitTorchwood()
{
	if (mMotionType != ProjectileMotion::MOTION_STRAIGHT)
		return false;

	if (mProjectileType != ProjectileType::PROJECTILE_PEA 
		&& mProjectileType != ProjectileType::PROJECTILE_SNOWPEA 
		&& mProjectileType != ProjectileType::PROJECTILE_FIREPEA 
		&& mProjectileType != ProjectileType::PROJECTILE_ICEPEA
		&& mProjectileType != ProjectileType::PROJECTILE_FIREBALL
		)
		return false;

	Plant* aPlant = nullptr;
	while (mBoard->IteratePlants(aPlant))
	{
		if (aPlant->mSeedType == SeedType::SEED_TORCHWOOD && aPlant->mRow == mRow && !aPlant->NotOnGround() && mHitTorchwoodGridX != aPlant->mPlantCol)
		{
			Rect aPlantAttackRect = aPlant->GetPlantAttackRect(PlantWeapon::WEAPON_PRIMARY);
			Rect aProjectileRect = GetProjectileRect();
			aProjectileRect.mX += 40;

			if (GetRectOverlap(aPlantAttackRect, aProjectileRect) > 10)
			{
				return true;
			}
		}
	}

	return false;
}

//0x46CD40
Zombie* Projectile::FindCollisionTarget()
{
	if (PeaAboutToHitTorchwood())  // “卡火炬”的原理，这段代码在两版内测版中均不存在
		return nullptr;

	Rect aProjectileRect = GetProjectileRect();
	Zombie* aBestZombie = nullptr;
	int aMinX = 0;

	Zombie* aZombie = nullptr;
	while (mBoard->IterateZombies(aZombie))
	{
		if ((aZombie->mZombieType == ZombieType::ZOMBIE_BOSS || aZombie->mRow == mRow) && !ZombieIsInList(aZombie) && aZombie->EffectedByDamage((unsigned int)mDamageRangeFlags))
		{
			if (aZombie->mZombiePhase == ZombiePhase::PHASE_SNORKEL_WALKING_IN_POOL && mPosZ >= 45.0f)
			{
				continue;
			}

			if ((mProjectileType == ProjectileType::PROJECTILE_STAR || mProjectileType == ProjectileType::PROJECTILE_FUTURESTAR) && mProjectileAge < 25 && mVelX >= 0.0f && Zombie::IsDigger(aZombie->mZombieType))
			{
				continue;
			}

			Rect aZombieRect = aZombie->GetZombieRect();
			if (GetRectOverlap(aProjectileRect, aZombieRect) > 0)
			{
				if (aBestZombie == nullptr || aZombie->mX < aMinX)
				{
					aBestZombie = aZombie;
					aMinX = aZombie->mX;
				}
			}
		}
	}

	return aBestZombie;
}

//0x46CE80
void Projectile::CheckForCollision()
{
	if (mMotionType == ProjectileMotion::MOTION_PUFF && mProjectileAge >= 75)
	{
		Die();
		return;
	}

	if (mPosX > WIDE_BOARD_WIDTH || mPosX + mWidth < LAWN_XMIN - 40)
	{
		if (mProjectileType == ProjectileType::PROJECTILE_FUTURESTAR) {
			mPierce--;
			if (mPierce < 0 && !mPersistentPierce) DoImpact(nullptr, nullptr);
			mVelX *= -1;
		}
		else if (mMotionType != ProjectileMotion::MOTION_BOOMERANG) {
			DoImpact(nullptr, nullptr);
		}
		return;
	}

	if (mMotionType == ProjectileMotion::MOTION_HOMING)
	{
		Zombie* aZombie = mBoard->ZombieTryToGet(mTargetZombieID);
		if (aZombie && aZombie->EffectedByDamage((unsigned int)mDamageRangeFlags))
		{
			Rect aProjectileRect = GetProjectileRect();
			Rect aZombieRect = aZombie->GetZombieRect();
			if (!ZombieIsInList(aZombie) && GetRectOverlap(aProjectileRect, aZombieRect) >= 0 && mPosY > aZombieRect.mY&& mPosY < aZombieRect.mY + aZombieRect.mHeight)
			{
				DoImpact(aZombie, nullptr);
			}
		}
		return;
	}

	if ((mPosY > BOARD_HEIGHT - LAWN_YMIN + (mBoard->StageHas6Rows() ? 85 : 0) || mPosY < LAWN_YMIN) && mMotionType == MOTION_STAR)
	{
		if (IsBounce())
		{
			mPierce--;
			if (mPierce < 0 && !mPersistentPierce) DoImpact(nullptr, nullptr);
			mVelY *= -1;
			return;
		}
		else
		{
			DoImpact(nullptr, nullptr);
			return;
		}
	}

	if ((mProjectileType == ProjectileType::PROJECTILE_PEA 
		|| mProjectileType == ProjectileType::PROJECTILE_STAR 
		|| mProjectileType == ProjectileType::PROJECTILE_FUTURESTAR) && mShadowY - mPosY > 90.0f)
	{
		return;
	}

	if (mMotionType == ProjectileMotion::MOTION_FLOAT_OVER)
	{
		return;
	}

	if (mHitPlant)
	{
		Plant* aPlant = FindCollisionTargetPlant();
		if (aPlant)
		{
			DoImpact(nullptr, aPlant);
		}
		return;
	}

	Zombie* aZombie = FindCollisionTarget();
	if (aZombie && !ZombieIsInList(aZombie))
	{
		if (aZombie->mOnHighGround && CantHitHighGround())
		{
			return;
		}

		DoImpact(aZombie, nullptr);
	}
}

bool Projectile::ZombieIsInList(Zombie* theZombie) 
{
	if (mPierce < 0) return true;
	for (int i = 0; i < NUM_MAX_PIERCE; i++)
	{
		if (theZombie == mBoard->ZombieTryToGet(mPierceZombieIDs[i])) return true;
	}
	return false;
}

//0x46D090
bool Projectile::CantHitHighGround()
{
	if (mMotionType == ProjectileMotion::MOTION_BACKWARDS || mMotionType == ProjectileMotion::MOTION_HOMING)
		return false;

	return (
		mProjectileType == ProjectileType::PROJECTILE_PEA 
		|| mProjectileType == ProjectileType::PROJECTILE_SNOWPEA 
		|| mProjectileType == ProjectileType::PROJECTILE_STAR 
		|| mProjectileType == ProjectileType::PROJECTILE_PUFF 
		|| mProjectileType == ProjectileType::PROJECTILE_FIREPEA 
		|| mProjectileType == ProjectileType::PROJECTILE_PLASMAPEA 
		|| mProjectileType == ProjectileType::PROJECTILE_BIGPLASMAPEA 
		|| mProjectileType == ProjectileType::PROJECTILE_ICEPEA 
		|| mProjectileType == ProjectileType::PROJECTILE_GOOPEA 
		|| mProjectileType == ProjectileType::PROJECTILE_BOMB 
		|| mProjectileType == ProjectileType::PROJECTILE_FUTURESTAR 
		|| mProjectileType == ProjectileType::PROJECTILE_ASPARAGUS 
		|| mProjectileType == ProjectileType::PROJECTILE_SUN 
		|| mProjectileType == ProjectileType::PROJECTILE_ROCKPEA
		) && !mOnHighGround;
}

//0x46D0D0
void Projectile::CheckForHighGround()
{
	float aShadowDelta = mShadowY - mPosY;

	if (mProjectileType == ProjectileType::PROJECTILE_PEA ||
		mProjectileType == ProjectileType::PROJECTILE_SNOWPEA ||
		mProjectileType == ProjectileType::PROJECTILE_FIREPEA ||
		mProjectileType == ProjectileType::PROJECTILE_PLASMAPEA ||
		mProjectileType == ProjectileType::PROJECTILE_BIGPLASMAPEA ||
		mProjectileType == ProjectileType::PROJECTILE_ICEPEA ||
		mProjectileType == ProjectileType::PROJECTILE_SPIKE ||
		mProjectileType == ProjectileType::PROJECTILE_BEE ||
		mProjectileType == ProjectileType::PROJECTILE_GOOPEA ||
		mProjectileType == ProjectileType::PROJECTILE_BOMB ||
		mProjectileType == ProjectileType::PROJECTILE_SUN ||
		mProjectileType == ProjectileType::PROJECTILE_ASPARAGUS ||
		mProjectileType == ProjectileType::PROJECTILE_COBBIG ||
		mProjectileType == ProjectileType::PROJECTILE_ROCKPEA)
	{
		if (aShadowDelta < 28.0f)
		{
			DoImpact(nullptr, nullptr);
			return;
		}
	}

	if (mProjectileType == ProjectileType::PROJECTILE_PUFF && aShadowDelta < 0.0f)
	{
		DoImpact(nullptr, nullptr);
		return;
	}

	if ((mProjectileType == ProjectileType::PROJECTILE_STAR || mProjectileType == ProjectileType::PROJECTILE_FUTURESTAR) && aShadowDelta < 23.0f)
	{
		DoImpact(nullptr, nullptr);
		return;
	}

	if (CantHitHighGround())
	{
		int aGridX = mBoard->PixelToGridXKeepOnBoard(mPosX + 30, mPosY);
		if (mBoard->mGridSquareType[aGridX][mRow] == GridSquareType::GRIDSQUARE_HIGH_GROUND)
		{
			DoImpact(nullptr, nullptr);
		}
	}
}

//0x46D1F0
bool Projectile::IsSplashDamage(Zombie* theZombie)
{
	if (mProjectileType && theZombie && theZombie->IsFireResistant())
		return false;

	return 
		mProjectileType == ProjectileType::PROJECTILE_MELON 
		|| mProjectileType == ProjectileType::PROJECTILE_WINTERMELON 
		|| IsFire();
}

bool Projectile::IsSplashDamage(Plant* thePlant)
{
	if (mProjectileType && thePlant && thePlant->IsNut(thePlant->mSeedType))
		return false;

	return
		mProjectileType == ProjectileType::PROJECTILE_MELON 
		|| mProjectileType == ProjectileType::PROJECTILE_WINTERMELON 
		|| IsFire();
}

bool Projectile::IsPierce()
{
	return
		mProjectileType == ProjectileType::PROJECTILE_MINT
		|| mProjectileType == ProjectileType::PROJECTILE_ASPARAGUS
		|| mProjectileType == ProjectileType::PROJECTILE_BOOMERANG
		|| mProjectileType == ProjectileType::PROJECTILE_FUTURESTAR
		|| mProjectileType == ProjectileType::PROJECTILE_SPIKE
		|| mProjectileType == ProjectileType::PROJECTILE_SEASTARFRUIT
		;
}

bool Projectile::IsBounce()
{
	return
		mProjectileType == ProjectileType::PROJECTILE_MINT 
		|| mProjectileType == ProjectileType::PROJECTILE_FUTURESTAR
		|| mProjectileType == ProjectileType::PROJECTILE_FIREBALL
		|| mProjectileType == ProjectileType::PROJECTILE_PLASMABALL
		;
}

bool Projectile::IsFire()
{
	return
		mProjectileType == ProjectileType::PROJECTILE_FIREPEA 
		|| mProjectileType == ProjectileType::PROJECTILE_PLASMAPEA 
		|| mProjectileType == ProjectileType::PROJECTILE_BIGPLASMAPEA 
		|| mProjectileType == ProjectileType::PROJECTILE_PEPPER
		|| mProjectileType == ProjectileType::PROJECTILE_FIREBALL
		|| mProjectileType == ProjectileType::PROJECTILE_PLASMABALL
		;
}

//0x46D230
unsigned int Projectile::GetDamageFlags(Zombie* theZombie)
{
	unsigned int aDamageFlags = 0U;

	if (IsSplashDamage(theZombie))
	{
		SetBit(aDamageFlags, (int)DamageFlags::DAMAGE_HITS_SHIELD_AND_BODY, true);
	}
	else if (mMotionType == ProjectileMotion::MOTION_BACKWARDS
		|| (mMotionType == ProjectileMotion::MOTION_LOBBED && mProjectileType != ProjectileType::PROJECTILE_FIREBALL && mProjectileType != ProjectileType::PROJECTILE_PLASMABALL)
		|| (mMotionType == ProjectileMotion::MOTION_STAR && mVelX < 0.0f)
		|| (mMotionType == ProjectileMotion::MOTION_BOOMERANG && mProjectileAge > mTime / 2))
	{
		SetBit(aDamageFlags, (int)DamageFlags::DAMAGE_BYPASSES_SHIELD, true);
	}
	if (mMotionType == ProjectileMotion::MOTION_LOBBED && mProjectileType != ProjectileType::PROJECTILE_FIREBALL && mProjectileType != ProjectileType::PROJECTILE_PLASMABALL)
	{
		SetBit(aDamageFlags, (int)DamageFlags::DAMAGE_LOBBED, true);
	}
	if (mProjectileType == ProjectileType::PROJECTILE_SNOWPEA || mProjectileType == ProjectileType::PROJECTILE_LETTUCE || mProjectileType == ProjectileType::PROJECTILE_WINTERMELON)
	{
		SetBit(aDamageFlags, (int)DamageFlags::DAMAGE_FREEZE, true);
	}
	else if (mProjectileType == ProjectileType::PROJECTILE_ICEPEA)
	{
		SetBit(aDamageFlags, (int)DamageFlags::DAMAGE_ICE, true);
	}
	else if (mProjectileType == ProjectileType::PROJECTILE_GOOPEA)
	{
		SetBit(aDamageFlags, (int)DamageFlags::DAMAGE_POISON, true);
	}
	else if (IsFire())
	{
		SetBit(aDamageFlags, (int)DamageFlags::DAMAGE_BURN, true);
	}
	else if (mProjectileType == ProjectileType::PROJECTILE_SPORE)
	{
		SetBit(aDamageFlags, (int)DamageFlags::DAMAGE_SPORE, true);
	}

	return aDamageFlags;
}

unsigned int Projectile::GetDamageFlags(Plant* thePlant)
{
	unsigned int aDamageFlags = 0U;

	if (IsSplashDamage(thePlant))
		SetBit(aDamageFlags, (int)DamageFlags::DAMAGE_HITS_SHIELD_AND_BODY, true);
	else if (mMotionType == ProjectileMotion::MOTION_LOBBED || mMotionType == ProjectileMotion::MOTION_BACKWARDS || (mMotionType == ProjectileMotion::MOTION_STAR && mVelX < 0.0f))
		SetBit(aDamageFlags, (int)DamageFlags::DAMAGE_BYPASSES_SHIELD, true);
	if (mProjectileType == ProjectileType::PROJECTILE_SNOWPEA || mProjectileType == ProjectileType::PROJECTILE_LETTUCE || mProjectileType == ProjectileType::PROJECTILE_WINTERMELON)
		SetBit(aDamageFlags, (int)DamageFlags::DAMAGE_FREEZE, true);
	else if (mProjectileType == ProjectileType::PROJECTILE_ICEPEA)
		SetBit(aDamageFlags, (int)DamageFlags::DAMAGE_ICE, true);
	else if (mProjectileType == ProjectileType::PROJECTILE_GOOPEA)
		SetBit(aDamageFlags, (int)DamageFlags::DAMAGE_POISON, true);

	return aDamageFlags;
}

//0x46D2B0
bool Projectile::IsZombieHitBySplash(Zombie* theZombie)
{
	Rect aProjectileRect = GetProjectileRect();
	if (IsFire())
	{
		aProjectileRect.mWidth = 100;
	}
	int aRowDeviation = theZombie->mRow - mRow;
	Rect aZombieRect = theZombie->GetZombieRect();
	if (IsFire() && theZombie->IsFireResistant())
		return false;

	if (theZombie->mZombieType == ZombieType::ZOMBIE_BOSS)
		aRowDeviation = 0;

	if (IsFire() && mMotionType != MOTION_LOBBED && aRowDeviation != 0)
		return false;
	else if (aRowDeviation > 1 || aRowDeviation < -1)
		return false;

	return theZombie->EffectedByDamage((unsigned int)mDamageRangeFlags) && GetRectOverlap(aProjectileRect, aZombieRect) >= 0;
}

bool Projectile::IsPlantHitBySplash(Plant* thePlant)
{
	Rect aProjectileRect = GetProjectileRect();
	if (IsFire())
	{
		aProjectileRect.mWidth = 100;
	}
	int aRowDeviation = thePlant->mRow - mRow;
	Rect aPlantRect = thePlant->GetPlantRect();
	if (IsFire() && thePlant->IsNut(thePlant->mSeedType))
	{
		return false;
	}
	if (IsFire() && mMotionType != MOTION_LOBBED && aRowDeviation != 0)
	{
		return false;
	}
	else if (aRowDeviation > 1 || aRowDeviation < -1)
	{
		return false;
	}

	return GetRectOverlap(aProjectileRect, aPlantRect) >= 0;
}

//0x46D390
void Projectile::DoSplashDamage(Zombie* theZombie)
{
	const ProjectileDefinition& aProjectileDef = GetProjectileDef();

	int aZombiesGetSplashed = 0;
	Zombie* aZombie = nullptr;
	while (mBoard->IterateZombies(aZombie))
	{
		if (aZombie != theZombie && IsZombieHitBySplash(aZombie))
		{
			aZombiesGetSplashed++;
		}
	}

	int aOriginalDamage = aProjectileDef.mDamage;
	int aSplashDamage = aProjectileDef.mDamage / 3;
	int aMaxSplashDamageAmount = aSplashDamage * 7;
	if (IsFire())
	{
		aMaxSplashDamageAmount = aOriginalDamage;
	}
	int aSplashDamageAmount = aSplashDamage * aZombiesGetSplashed;
	if (aSplashDamageAmount > aMaxSplashDamageAmount)
	{
		//aSplashDamage *= aMaxSplashDamageAmount / aSplashDamage;
		aSplashDamage = aOriginalDamage * aMaxSplashDamageAmount / (aSplashDamageAmount * 3);
		aSplashDamage = max(aSplashDamage, 1);
	}

	aZombie = nullptr;
	while (mBoard->IterateZombies(aZombie))
	{
		if (IsZombieHitBySplash(aZombie))
		{
			unsigned int aDamageFlags = GetDamageFlags(aZombie);
			if (aZombie == theZombie)
			{
				aZombie->TakeDamage(aOriginalDamage, aDamageFlags);
			}
			else
			{
				aZombie->TakeDamage(aSplashDamage, aDamageFlags);
			}
		}
	}
}

void Projectile::DoPlantSplashDamage(Plant* thePlant)
{
	const ProjectileDefinition& aProjectileDef = GetProjectileDef();

	int aPlantsGetSplashed = 0;
	Plant* aPlant = nullptr;
	while (mBoard->IteratePlants(aPlant))
	{
		if (aPlant != thePlant && IsPlantHitBySplash(aPlant))
		{
			aPlantsGetSplashed++;
		}
	}

	int aOriginalDamage = aProjectileDef.mDamage;
	int aSplashDamage = aProjectileDef.mDamage / 3;
	int aMaxSplashDamageAmount = aSplashDamage * 7;
	if (IsFire())
	{
		aMaxSplashDamageAmount = aOriginalDamage;
	}
	int aSplashDamageAmount = aSplashDamage * aPlantsGetSplashed;
	if (aSplashDamageAmount > aMaxSplashDamageAmount)
	{
		//aSplashDamage *= aMaxSplashDamageAmount / aSplashDamage;
		aSplashDamage = aOriginalDamage * aMaxSplashDamageAmount / (aSplashDamageAmount * 3);
		aSplashDamage = max(aSplashDamage, 1);
	}

	aPlant = nullptr;
	while (mBoard->IteratePlants(aPlant))
	{
		if (IsPlantHitBySplash(aPlant))
		{
			unsigned int aDamageFlags = GetDamageFlags(aPlant);
			if (aPlant == thePlant)
			{
				aPlant->TakeDamage(aOriginalDamage, aDamageFlags);
			}
			else
			{
				aPlant->TakeDamage(aSplashDamage, aDamageFlags);
			}
		}
	}
}

void Projectile::DoSplashWarm()
{
	Plant* aPlant = nullptr;
	while (mBoard->IteratePlants(aPlant))
	{
		Rect aProjectileRect = GetProjectileRect();
		aProjectileRect.mWidth = 100;
		int aRowDeviation = aPlant->mRow - mRow;
		Rect aPlantRect = aPlant->GetPlantRect();

		if (GetRectOverlap(aProjectileRect, aPlantRect) >= 0 && aRowDeviation <= 1 && aRowDeviation >= -1)
		{
			aPlant->mIceTrapCounter = 0;
			if (aPlant->mChilledCounter > 0)
				aPlant->mChilledCounter -= 10 * GetProjectileDef().mDamage;
			else
			{
				aPlant->mWarmCounter += 10 * GetProjectileDef().mDamage;
			}
		}
	}
}

//0x46D490
void Projectile::UpdateLobMotion()
{
	if ((mProjectileType == ProjectileType::PROJECTILE_COBBIG || mProjectileType == ProjectileType::PROJECTILE_SPECIALARROW) && mPosZ < -700.0f)
	{
		mVelZ = 8.0f;
		mRow = mCobTargetRow;
		mPosX = mCobTargetX;
		int aCobTargetCol = mBoard->PixelToGridXKeepOnBoard(mCobTargetX, 0);
		mPosY = mBoard->GridToPixelY(aCobTargetCol, mCobTargetRow);
		mShadowY = mPosY + 67.0f;
		mRotation = -PI / 2;
	}

	mVelZ += mAccZ;
	if (mApp->mGameMode == GameMode::GAMEMODE_CHALLENGE_HIGH_GRAVITY)
	{
		mVelZ += mAccZ;
	}
	mPosX += mVelX;
	mPosY += mVelY;
	mPosZ += mVelZ;

	bool isRising = mVelZ < 0.0f;
	if (isRising && (mProjectileType == ProjectileType::PROJECTILE_BASKETBALL || mProjectileType == ProjectileType::PROJECTILE_COBBIG || mProjectileType == ProjectileType::PROJECTILE_SPECIALARROW))
	{
		return;
	}
	if (mProjectileAge > 20)
	{
		if (isRising && mProjectileType != ProjectileType::PROJECTILE_FIREBALL && mProjectileType != ProjectileType::PROJECTILE_PLASMABALL)
		{
			return;
		}

		float aMinCollisionZ = 0.0f;
		if (mProjectileType == ProjectileType::PROJECTILE_BUTTER)
		{
			aMinCollisionZ = -32.0f;
		}
		else if (mProjectileType == ProjectileType::PROJECTILE_BASKETBALL)
			aMinCollisionZ = 60.0f;
		else if (mProjectileType == ProjectileType::PROJECTILE_MELON || mProjectileType == ProjectileType::PROJECTILE_WINTERMELON)
			aMinCollisionZ = -35.0f;
		else if (mProjectileType == ProjectileType::PROJECTILE_PEPPER 
			|| mProjectileType == ProjectileType::PROJECTILE_CABBAGE 
			|| mProjectileType == ProjectileType::PROJECTILE_LETTUCE 
			|| mProjectileType == ProjectileType::PROJECTILE_KERNEL 
			|| mProjectileType == ProjectileType::PROJECTILE_MOLD 
			|| mProjectileType == ProjectileType::PROJECTILE_SPORE
			|| mProjectileType == ProjectileType::PROJECTILE_SPECIALARROW 
			|| mProjectileType == ProjectileType::PROJECTILE_MCARROW
			|| mProjectileType == ProjectileType::PROJECTILE_FIREBALL
			|| mProjectileType == ProjectileType::PROJECTILE_PLASMABALL
			)
			aMinCollisionZ = -30.0f;
		else if (mProjectileType == ProjectileType::PROJECTILE_COBBIG
			)
			aMinCollisionZ = -60.0f;
		if (mBoard->mPlantRow[mRow] == PlantRowType::PLANTROW_POOL)
			aMinCollisionZ += 40.0f;

		if (mPosZ <= aMinCollisionZ)
			return;
	}

	Plant* aPlant = nullptr;
	Zombie* aZombie = nullptr;
	if (mHitPlant)
	{
		aPlant = FindCollisionTargetPlant();
	}
	else
	{
		aZombie = FindCollisionTarget();
	}

	float aGroundZ = 80.0f;
	if (mProjectileType == ProjectileType::PROJECTILE_COBBIG)
		aGroundZ = -40.0f;
	else if (mProjectileType == ProjectileType::PROJECTILE_FIREBALL || mProjectileType == ProjectileType::PROJECTILE_PLASMABALL)
		aGroundZ = 60.0f;

	bool hitGround = mPosZ > aGroundZ;
	if (aZombie == nullptr && aPlant == nullptr && !hitGround)
	{
		return;
	}

	if (aPlant)
	{
		Plant* aUmbrellaPlant = mBoard->FindUmbrellaPlant(aPlant->mPlantCol, aPlant->mRow);
		if (aUmbrellaPlant)
		{
			if (aUmbrellaPlant->mState == PlantState::STATE_UMBRELLA_REFLECTING)
			{
				mApp->PlayFoley(FoleyType::FOLEY_SPLAT);
				int aRenderPosition = Board::MakeRenderOrder(RenderLayer::RENDER_LAYER_TOP, 0, 1);
				mApp->AddTodParticle(mPosX + 20.0f, mPosY + 20.0f, aRenderPosition, ParticleEffect::PARTICLE_UMBRELLA_REFLECT);
				Die();
			}
			else if (aUmbrellaPlant->mState != PlantState::STATE_UMBRELLA_TRIGGERED)
			{
				mApp->PlayFoley(FoleyType::FOLEY_UMBRELLA);
				aUmbrellaPlant->DoSpecial();
			}
		}
		else
			DoImpact(nullptr, aPlant);
	}
	else if (mProjectileType == ProjectileType::PROJECTILE_COBBIG)
	{
		mBoard->KillAllZombiesInRadius(mRow, mPosX + 80, mPosY + 40, 115, 1, true, 1800, mDamageRangeFlags, 18U);
		DoImpact(nullptr, nullptr);
	}
	else if (aZombie && !ZombieIsInList(aZombie))
		DoImpact(aZombie, nullptr);
	else if (IsBounce() && mPosX < WIDE_BOARD_WIDTH && mPosX + mWidth > LAWN_XMIN - 40)
	{
		mVelZ = -3.5f;
		mPierce--;
		if (IsFire())
		{
			if (mBoard->IsPoolSquare(mBoard->PixelToGridX(mX, mY), mRow))
				DoImpact(nullptr, nullptr);
			else
			{
				Reanimation* aFireReanim = mApp->AddReanimation(mPosX + 38.0f, mPosY + 38.0f, mRenderOrder + 1, ReanimationType::REANIM_JALAPENO_FIRE);
				aFireReanim->mAnimTime = 0.25f;
				aFireReanim->mAnimRate = 24.0f;
				aFireReanim->OverrideScale(0.4f, 0.4f);
				DoSplashDamage(nullptr);
				DoSplashWarm();
			}
		}
		if (mPierce < 0 && !mPersistentPierce)
			DoImpact(nullptr, nullptr);
	}
	else
		DoImpact(nullptr, nullptr);
}

//0x46D890
void Projectile::UpdateNormalMotion()
{
	if (mMotionType == ProjectileMotion::MOTION_BACKWARDS)
	{
		mPosX -= mVelX;
	}
	else if (mMotionType == ProjectileMotion::MOTION_HOMING)
	{
		Zombie* aZombie = mBoard->ZombieTryToGet(mTargetZombieID);
		if (!aZombie || !aZombie->mHasHead)
		{
			aZombie = FindRetargetZombie();
			if (aZombie) mTargetZombieID = mBoard->ZombieGetID(aZombie);
		}
		if (aZombie && aZombie->EffectedByDamage((unsigned int)mDamageRangeFlags))
		{
			Rect aZombieRect = aZombie->GetZombieRect();
			SexyVector2 aTargetCenter(aZombie->ZombieTargetLeadX(0.0f), aZombieRect.mY + aZombieRect.mHeight / 2);
			SexyVector2 aProjectileCenter(mPosX + mWidth / 2, mPosY + mHeight / 2);
			SexyVector2 aToTarget = (aTargetCenter - aProjectileCenter).Normalize();
			SexyVector2 aMotion(mVelX, mVelY);

			aMotion += aToTarget * (0.001f * mProjectileAge);
			aMotion = aMotion.Normalize();
			aMotion *= 2.0f;

			mVelX = aMotion.x;
			mVelY = aMotion.y;
			mRotation = -atan2(mVelY, mVelX);
		}

		mPosY += mVelY;
		mPosX += mVelX;
		mShadowY += mVelY;
		mRow = mBoard->PixelToGridYKeepOnBoard(mPosX, mPosY);
	}
	else if (mMotionType == ProjectileMotion::MOTION_STAR)
	{
		mPosY += mVelY;
		mPosX += mVelX;
		mShadowY += mVelY;

		if (mVelY != 0.0f)
		{
			mRow = mBoard->PixelToGridYKeepOnBoard(mPosX, mPosY);
		}
	}
	else if (mMotionType == ProjectileMotion::MOTION_BEE)
	{
		if (mProjectileAge < 60)
		{
			mPosY -= 0.5f;
		}
		mPosX += mVelX;
	}
	else if (mMotionType == ProjectileMotion::MOTION_FLOAT_OVER)
	{
		if (mVelZ < 0.0f)
		{
			mVelZ += 0.002f;
			mVelZ = min(mVelZ, 0.0f);
			mPosY += mVelZ;
			mRotation = 0.3f - 0.7f * mVelZ * PI * 0.25f;
		}
		mPosX += 0.4f;
	}
	else if (mMotionType == ProjectileMotion::MOTION_BEE_BACKWARDS)
	{
		if (mProjectileAge < 60)
		{
			mPosY -= 0.5f;
		}
		mPosX -= mVelX;
	}
	else if (mMotionType == ProjectileMotion::MOTION_THREEPEATER)
	{
		mPosX += mVelX;
		mPosY += mVelY;
		mVelY *= 0.97f;
		mShadowY += mVelY;
	}
	else if (mMotionType == ProjectileMotion::MOTION_BOOMERANG)
	{
		mPosX = TodAnimateCurveFloat(0, mTime, mProjectileAge, mOriginX, mTargetX, TodCurves::CURVE_BOUNCE_SLOW_MIDDLE);
		if (mProjectileAge == mTime / 2)
		{
			mPierce = GetProjectileDef().mMaxPierce;
			for (int i = 0; i < NUM_MAX_PIERCE; i++)
			{
				mPierceZombieIDs[i] = ZombieID::ZOMBIEID_NULL;
			}
		}
		else if (mProjectileAge >= mTime) Die();
	}
	else
	{
		mPosX += mVelX;
	}

	if (mApp->mGameMode == GameMode::GAMEMODE_CHALLENGE_HIGH_GRAVITY)
	{
		if (mMotionType == ProjectileMotion::MOTION_FLOAT_OVER)
		{
			mVelZ += 0.004f;
		}
		else
		{
			mVelZ += 0.2f;
		}

		mPosY += mVelZ;
	}

	CheckForCollision();
	CheckForHighGround();
}

Zombie* Projectile::FindRetargetZombie() {
	int aHighestWeight = 0;
	Zombie* aBestZombie = nullptr;

	Zombie* aZombie = nullptr;
	while (mBoard->IterateZombies(aZombie)) {
		if (aZombie->EffectedByDamage(mDamageRangeFlags) && !ZombieIsInList(aZombie) && aZombie->mHasHead)
		{
			Rect aZombieRect = aZombie->GetZombieRect();
			int aDistance = Distance2D(mX + mWidth / 2, mY + mHeight / 2, aZombieRect.mX + aZombieRect.mWidth / 2, aZombieRect.mY + aZombieRect.mHeight / 2);
			int aWeight = -pow(aDistance, 2) + pow(aZombie->Health(), 2);
			if (aZombie->IsFloating())
			{
				aWeight += 10000;
			}

			if (aBestZombie == nullptr || aWeight > aHighestWeight)
			{
				aHighestWeight = aWeight;
				aBestZombie = aZombie;
			}
		}
	}
	return aBestZombie;
}

//0x46DC70
void Projectile::UpdateMotion()
{
	if (mAnimTicksPerFrame > 0)
	{
		mAnimCounter = (mAnimCounter + 1) % (mNumFrames * mAnimTicksPerFrame);
		mFrame = mAnimCounter / mAnimTicksPerFrame;
	}

	int aOldRow = mRow;
	float aOldY = mBoard->GetPosYBasedOnRow(mPosX, mRow);
	if (mMotionType == ProjectileMotion::MOTION_LOBBED)
	{
		UpdateLobMotion();
	}
	else
	{
		UpdateNormalMotion();
	}

	float aSlopeHeightChange = mBoard->GetPosYBasedOnRow(mPosX, aOldRow) - aOldY;
	// Do Fix Bugs
	if (mProjectileType == ProjectileType::PROJECTILE_COBBIG || mProjectileType == ProjectileType::PROJECTILE_SPECIALARROW)
	{
		aSlopeHeightChange = 0.0f;  // 修复“上界之风”
	}
	if (mMotionType == ProjectileMotion::MOTION_FLOAT_OVER)
	{
		mPosY += aSlopeHeightChange;
	}
	if (mMotionType == ProjectileMotion::MOTION_LOBBED)
	{
		mPosY += aSlopeHeightChange;
		mPosZ -= aSlopeHeightChange;
	}
	mShadowY += aSlopeHeightChange;
	mX = (int)mPosX;
	mY = (int)(mPosY + mPosZ);
}

//0x46DD30
void Projectile::PlayImpactSound(Zombie* theZombie)
{
	bool aPlayHelmSound = true;
	bool aPlaySplatSound = true;
	if (mProjectileType == ProjectileType::PROJECTILE_KERNEL)
	{
		mApp->PlayFoley(FoleyType::FOLEY_KERNEL_SPLAT);
		aPlayHelmSound = false;
		aPlaySplatSound = false;
	}
	else if (mProjectileType == ProjectileType::PROJECTILE_BUTTER)
	{
		mApp->PlayFoley(FoleyType::FOLEY_BUTTER);
		aPlaySplatSound = false;
	}
	else if (IsFire() && IsSplashDamage(theZombie))
	{
		mApp->PlayFoley(FoleyType::FOLEY_IGNITE);
		aPlayHelmSound = false;
		aPlaySplatSound = false;
	}
	else if (mProjectileType == ProjectileType::PROJECTILE_MELON || mProjectileType == ProjectileType::PROJECTILE_WINTERMELON)
	{
		mApp->PlayFoley(FoleyType::FOLEY_MELONIMPACT);
		aPlaySplatSound = false;
	}

	if (aPlayHelmSound && theZombie)
	{
		if (theZombie->mHelmType == HELMTYPE_PAIL || theZombie->mHelmType == HELMTYPE_POT || theZombie->mHelmType == HELMTYPE_HELMET)
		{
			mApp->PlayFoley(FoleyType::FOLEY_SHIELD_HIT);
			aPlaySplatSound = false;
		}
		else if (theZombie->mHelmType == HELMTYPE_TRAFFIC_CONE || theZombie->mHelmType == HELMTYPE_DIGGER || theZombie->mHelmType == HELMTYPE_FOOTBALL)
			mApp->PlayFoley(FoleyType::FOLEY_PLASTIC_HIT);
	}

	if (aPlaySplatSound)
		mApp->PlayFoley(FoleyType::FOLEY_SPLAT);
}

//0x46E000
void Projectile::DoImpact(Zombie* theZombie, Plant* thePlant)
{
	if (theZombie) {
		int aPierceZombie = GetProjectileDef().mMaxPierce - mPierce;
		mPierceZombieIDs[aPierceZombie] = mBoard->ZombieGetID(theZombie);
		mTargetZombieID = ZOMBIEID_NULL;
		mPierce--;
		if (IsSplashDamage(theZombie))
		{
			if (IsFire())
				theZombie->RemoveColdEffects();

			DoSplashDamage(theZombie);
		}
		else
		{
			unsigned int aDamageFlags = GetDamageFlags(theZombie);
			theZombie->TakeDamage(GetProjectileDef().mDamage, aDamageFlags);
		}
		if (mProjectileType == ProjectileType::PROJECTILE_MINT && mMotionType == ProjectileMotion::MOTION_STRAIGHT)
		{
			mMotionType = ProjectileMotion::MOTION_STAR;
			mVelX = 3;
			mVelY = Sexy::Rand(2) ? 3 : -3;
		}
		if (mProjectileType == ProjectileType::PROJECTILE_ROCKPEA && Rand(2))
		{
			theZombie->mPosX = min(theZombie->mPosX + 80, WIDE_BOARD_WIDTH);
		}
		if (IsBounce() && mMotionType == ProjectileMotion::MOTION_STAR)
		{
			mVelY *= -1;
		}
	}
	else if (thePlant) {
		if (IsSplashDamage(thePlant))
		{
			if (IsFire()) {
				thePlant->RemoveColdEffects();
			}

			DoPlantSplashDamage(thePlant);
		}
		/*else if (IsPierce() && theZombie != mBoard->ZombieTryToGet(mStoredZombieID))
		{
			if (mProjectileType == ProjectileType::PROJECTILE_MINT) {

				mMotionType = ProjectileMotion::MOTION_STAR;
				mVelX = 3;
				mVelY = Sexy::Rand(2) ? 3 : -3;
			}
			unsigned int aDamageFlags = GetDamageFlags(theZombie);
			theZombie->TakeDamage(GetProjectileDef().mDamage, aDamageFlags);
			mStoredZombieID = mBoard->ZombieGetID(theZombie);
		}*/
		else //if (!IsPierce())
		{
			unsigned int aDamageFlags = GetDamageFlags(thePlant);
			thePlant->TakeDamage(GetProjectileDef().mDamage, aDamageFlags);
		}
		//else return;
	}

	PlayImpactSound(theZombie);

	float aLastPosX = mPosX - mVelX;
	float aLastPosY = mPosY + mPosZ - mVelY - mVelZ;
	ParticleEffect aEffect = ParticleEffect::PARTICLE_NONE;
	float aSplatPosX = mPosX + 12.0f;
	float aSplatPosY = mPosY + 12.0f;
	if (mProjectileType == ProjectileType::PROJECTILE_MELON)
	{
		mApp->AddTodParticle(aLastPosX + 30.0f, aLastPosY + 30.0f, mRenderOrder + 1, ParticleEffect::PARTICLE_MELONSPLASH);
	}
	else if (mProjectileType == ProjectileType::PROJECTILE_WINTERMELON)
	{
		mApp->AddTodParticle(aLastPosX + 30.0f, aLastPosY + 30.0f, mRenderOrder + 1, ParticleEffect::PARTICLE_WINTERMELON);
	}
	else if (mProjectileType == ProjectileType::PROJECTILE_COBBIG)
	{
		int aRenderOrder = Board::MakeRenderOrder(RenderLayer::RENDER_LAYER_GROUND, mCobTargetRow, 2);
		mApp->AddTodParticle(mPosX + 80.0f, mPosY + 40.0f, aRenderOrder, ParticleEffect::PARTICLE_BLASTMARK);
		mApp->AddTodParticle(mPosX + 80.0f, mPosY + 40.0f, mRenderOrder + 1, ParticleEffect::PARTICLE_POPCORNSPLASH);
		mApp->PlaySample(SOUND_DOOMSHROOM);
		mBoard->ShakeBoard(3, -4);
	}
	else if (mProjectileType == ProjectileType::PROJECTILE_SUN) {
		mBoard->AddCoin(mX, mY, CoinType::COIN_SMALLSUN, CoinMotion::COIN_MOTION_FROM_PLANT);
	}
	else if (mProjectileType == ProjectileType::PROJECTILE_PEA || mProjectileType == ProjectileType::PROJECTILE_ROCKPEA)
	{
		aSplatPosX -= 15.0f;
		aEffect = ParticleEffect::PARTICLE_PEA_SPLAT;
	}
	else if (mProjectileType == PROJECTILE_BOMB) {
		mApp->PlayFoley(FoleyType::FOLEY_CHERRYBOMB);
		mApp->PlayFoley(FoleyType::FOLEY_JUICY);

		mBoard->KillAllZombiesInRadius(mRow, mPosX, mPosY, 115, 1, true, 1800, mDamageRangeFlags, 18U);

		mApp->AddTodParticle(mPosX, mPosY, (int)RenderLayer::RENDER_LAYER_TOP, ParticleEffect::PARTICLE_POWIE);
		mBoard->ShakeBoard(3, -4);
	}
	else if (mProjectileType == PROJECTILE_BIGPLASMAPEA) {
		mApp->PlayFoley(FoleyType::FOLEY_CHERRYBOMB);

		mBoard->KillAllZombiesInRadius(mRow, mPosX, mPosY, 115, 1, false, 240, mDamageRangeFlags, 18U);

		mApp->AddTodParticle(mPosX, mPosY, (int)RenderLayer::RENDER_LAYER_TOP, ParticleEffect::PARTICLE_POWIE);
		mBoard->ShakeBoard(3, -4);
	}
	else if (mProjectileType == ProjectileType::PROJECTILE_SNOWPEA || mProjectileType == ProjectileType::PROJECTILE_ICEPEA)
	{
		aSplatPosX -= 15.0f;
		aEffect = ParticleEffect::PARTICLE_SNOWPEA_SPLAT;
	}
	else if (IsFire() && IsSplashDamage(theZombie))
	{
		Reanimation* aFireReanim = mApp->AddReanimation(mPosX + 38.0f, mPosY - 20.0f, mRenderOrder + 1, ReanimationType::REANIM_JALAPENO_FIRE);
		aFireReanim->mAnimTime = 0.25f;
		aFireReanim->mAnimRate = 24.0f;
		aFireReanim->OverrideScale(0.7f, 0.4f);
	}
	else if (mProjectileType == ProjectileType::PROJECTILE_STAR || mProjectileType == ProjectileType::PROJECTILE_FUTURESTAR)
	{
		aEffect = ParticleEffect::PARTICLE_STAR_SPLAT;
	}
	else if (mProjectileType == ProjectileType::PROJECTILE_PUFF || mProjectileType == ProjectileType::PROJECTILE_MOLD || mProjectileType == ProjectileType::PROJECTILE_SPORE)
	{
		aSplatPosX -= 20.0f;
		aEffect = ParticleEffect::PARTICLE_PUFF_SPLAT;
	}
	else if (mProjectileType == ProjectileType::PROJECTILE_PEPPER || mProjectileType == ProjectileType::PROJECTILE_CABBAGE || mProjectileType == ProjectileType::PROJECTILE_LETTUCE)
	{
		aSplatPosX = aLastPosX - 38.0f;
		aSplatPosY = aLastPosY + 23.0f;
		aEffect = ParticleEffect::PARTICLE_CABBAGE_SPLAT;
	}
	else if (mProjectileType == ProjectileType::PROJECTILE_BUTTER)
	{
		aSplatPosX = aLastPosX - 20.0f;
		aSplatPosY = aLastPosY + 63.0f;
		aEffect = ParticleEffect::PARTICLE_BUTTER_SPLAT;

		if (theZombie)
		{
			theZombie->ApplyButter();
		}
	}

	if (aEffect != ParticleEffect::PARTICLE_NONE)
	{
		if (theZombie)
		{
			float aPosX = aSplatPosX + 52.0f - theZombie->mX;
			float aPosY = aSplatPosY - theZombie->mY;
			if (theZombie->mZombiePhase == ZombiePhase::PHASE_SNORKEL_WALKING_IN_POOL || theZombie->mZombiePhase == ZombiePhase::PHASE_DOLPHIN_WALKING_IN_POOL)
			{
				aPosY += 60.0f;
			}
			if (mMotionType == ProjectileMotion::MOTION_BACKWARDS)
			{
				aPosX -= 80.0f;
			}
			else if (mPosX > theZombie->mX + 40 && mMotionType != ProjectileMotion::MOTION_LOBBED)
			{
				aPosX -= 60.0f;
			}

			aPosY = ClampFloat(aPosY, 20.0f, 100.0f);
			theZombie->AddAttachedParticle(aPosX, aPosY, aEffect);
		}
		else
		{
			mApp->AddTodParticle(aSplatPosX, aSplatPosY, mRenderOrder + 1, aEffect);
		}
	}

	if (!IsPierce() || (!theZombie && !thePlant) || (mPierce < 0 && !mPersistentPierce)) {
		Die();
	}
}

//0x46E460
void Projectile::Update()
{
	mProjectileAge++;
	if (mApp->mGameScene != GameScenes::SCENE_PLAYING && !mBoard->mCutScene->ShouldRunUpsellBoard())
		return;

	int aTime = 20;
	if (mProjectileType == ProjectileType::PROJECTILE_PEA || 
		mProjectileType == ProjectileType::PROJECTILE_SNOWPEA || 
		mProjectileType == ProjectileType::PROJECTILE_ICEPEA ||
		mProjectileType == ProjectileType::PROJECTILE_MINT ||
		mProjectileType == ProjectileType::PROJECTILE_CABBAGE || 
		mProjectileType == ProjectileType::PROJECTILE_PEPPER ||
		mProjectileType == ProjectileType::PROJECTILE_LETTUCE ||
		mProjectileType == ProjectileType::PROJECTILE_MELON || 
		mProjectileType == ProjectileType::PROJECTILE_WINTERMELON || 
		mProjectileType == ProjectileType::PROJECTILE_KERNEL || 
		mProjectileType == ProjectileType::PROJECTILE_BUTTER || 
		mProjectileType == ProjectileType::PROJECTILE_COBBIG || 
		mProjectileType == ProjectileType::PROJECTILE_SPIKE ||
		mProjectileType == ProjectileType::PROJECTILE_BEE ||
		mProjectileType == ProjectileType::PROJECTILE_GOOPEA ||
		mProjectileType == ProjectileType::PROJECTILE_SUN ||
		mProjectileType == ProjectileType::PROJECTILE_ASPARAGUS ||
		mProjectileType == ProjectileType::PROJECTILE_BOMB ||
		mProjectileType == ProjectileType::PROJECTILE_ROCKPEA
		)
		aTime = 0;

	if (mProjectileAge > aTime)
		mRenderOrder = Board::MakeRenderOrder(RenderLayer::RENDER_LAYER_PROJECTILE, mRow, 0);

	if (mClickBackoffCounter > 0)
		mClickBackoffCounter--;

	mRotation += mRotationSpeed;

	UpdateMotion();
	AttachmentUpdateAndMove(mAttachmentID, mPosX, mPosY + mPosZ);
}

//0x46E540
void Projectile::Draw(Graphics* g)
{
	const ProjectileDefinition& aProjectileDef = GetProjectileDef();

	Image* aImage;
	float aScale = 1.0f;
	if (mProjectileType == ProjectileType::PROJECTILE_COBBIG)
	{
		aImage = IMAGE_REANIM_COBCANNON_COB;
		aScale = 0.9f;
	}
	else if (mProjectileType == ProjectileType::PROJECTILE_PEA || mProjectileType == ProjectileType::PROJECTILE_ROCKPEA)
		aImage = IMAGE_PROJECTILEPEA;

	else if (mProjectileType == ProjectileType::PROJECTILE_BOMB)
		aImage = IMAGE_PROJECTILEPEA;

	else if (mProjectileType == ProjectileType::PROJECTILE_GOOPEA)
		aImage = IMAGE_PROJECTILEPEA;

	else if (mProjectileType == ProjectileType::PROJECTILE_SNOWPEA)
		aImage = IMAGE_PROJECTILESNOWPEA;

	else if (mProjectileType == ProjectileType::PROJECTILE_ICEPEA)
		aImage = IMAGE_PROJECTILESNOWPEA;

	else if (mProjectileType == ProjectileType::PROJECTILE_MINT)
		aImage = IMAGE_PROJECTILEMINT;

	else if (mProjectileType == ProjectileType::PROJECTILE_FIREPEA || mProjectileType == ProjectileType::PROJECTILE_PLASMAPEA || mProjectileType == ProjectileType::PROJECTILE_BIGPLASMAPEA || mProjectileType == ProjectileType::PROJECTILE_SUN)
		aImage = nullptr;

	else if (mProjectileType == ProjectileType::PROJECTILE_SPIKE)
		aImage = IMAGE_PROJECTILECACTUS;

	else if (mProjectileType == ProjectileType::PROJECTILE_ARROW || mProjectileType == ProjectileType::PROJECTILE_SPECIALARROW || mProjectileType == ProjectileType::PROJECTILE_MCARROW)
		aImage = IMAGE_PROJECTILECACTUS;

	else if (mProjectileType == ProjectileType::PROJECTILE_BEE)
		aImage = IMAGE_PROJECTILECACTUS;

	else if (mProjectileType == ProjectileType::PROJECTILE_ASPARAGUS)
		aImage = IMAGE_PROJECTILECACTUS;

	else if (mProjectileType == ProjectileType::PROJECTILE_STAR)
		aImage = IMAGE_PROJECTILE_STAR;

	else if (mProjectileType == ProjectileType::PROJECTILE_FUTURESTAR)
		aImage = IMAGE_PROJECTILE_FUTURESTAR;

	else if (mProjectileType == ProjectileType::PROJECTILE_SEASTARFRUIT)
		aImage = IMAGE_PROJECTILE_FUTURESTAR;

	else if (mProjectileType == ProjectileType::PROJECTILE_BOOMERANG)
		aImage = IMAGE_PROJECTILEPEA;

	else if (mProjectileType == ProjectileType::PROJECTILE_PUFF)
	{
		aImage = IMAGE_PUFFSHROOM_PUFF1;
		aScale = TodAnimateCurveFloat(0, 30, mProjectileAge, 0.3f, 1.0f, TodCurves::CURVE_LINEAR);
	}
	else if (mProjectileType == ProjectileType::PROJECTILE_MOLD)
	{
		aImage = IMAGE_PUFFSHROOM_PUFF1;
		aScale = TodAnimateCurveFloat(0, 30, mProjectileAge, 0.3f, 1.0f, TodCurves::CURVE_LINEAR);
	}
	else if (mProjectileType == ProjectileType::PROJECTILE_SPORE)
	{
		aImage = IMAGE_PUFFSHROOM_PUFF1;
		aScale = TodAnimateCurveFloat(0, 30, mProjectileAge, 0.3f, 1.0f, TodCurves::CURVE_LINEAR);
	}
	else if (mProjectileType == ProjectileType::PROJECTILE_BASKETBALL)
	{
		aImage = IMAGE_REANIM_ZOMBIE_CATAPULT_BASKETBALL;
		aScale = 1.1f;
	}
	else if (mProjectileType == ProjectileType::PROJECTILE_CABBAGE)
		aImage = IMAGE_REANIM_CABBAGEPULT_CABBAGE;

	else if (mProjectileType == ProjectileType::PROJECTILE_LETTUCE)
		aImage = IMAGE_REANIM_ICEBERGLETTUCE_LETTUCE;

	else if (mProjectileType == ProjectileType::PROJECTILE_PEPPER)
		aImage = IMAGE_REANIM_PROJECTILEPEPPER;

	else if (mProjectileType == ProjectileType::PROJECTILE_FIREBALL)
		aImage = IMAGE_PROJECTILEFIREBALL;

	else if (mProjectileType == ProjectileType::PROJECTILE_PLASMABALL)
		aImage = IMAGE_PROJECTILEPLASMABALL;

	else if (mProjectileType == ProjectileType::PROJECTILE_KERNEL)
	{
		aImage = IMAGE_REANIM_CORNPULT_KERNAL;
		aScale = 0.95f;
	}
	else if (mProjectileType == ProjectileType::PROJECTILE_BUTTER)
	{
		aImage = IMAGE_REANIM_CORNPULT_BUTTER;
		aScale = 0.8f;
	}
	else if (mProjectileType == ProjectileType::PROJECTILE_MELON)
	{
		aImage = IMAGE_REANIM_MELONPULT_MELON;
		aScale = 1.0f;
	}
	else if (mProjectileType == ProjectileType::PROJECTILE_WINTERMELON)
	{
		aImage = IMAGE_REANIM_WINTERMELON_PROJECTILE;
		aScale = 1.0f;
	}
	else
		TOD_ASSERT();

	bool aMirror = false;
	if (mMotionType == ProjectileMotion::MOTION_BEE_BACKWARDS)
	{
		aMirror = true;
	}

	if (aImage)
	{
		TOD_ASSERT(aProjectileDef.mImageRow < aImage->mNumRows);
		TOD_ASSERT(mFrame < aImage->mNumCols);

		int aCelWidth = aImage->GetCelWidth();
		int aCelHeight = aImage->GetCelHeight();
		Rect aSrcRect(aCelWidth * mFrame, aCelHeight * aProjectileDef.mImageRow, aCelWidth, aCelHeight);
		if (FloatApproxEqual(mRotation, 0.0f) && FloatApproxEqual(aScale, 1.0f))
		{
			Rect aDestRect(0, 0, aCelWidth, aCelHeight);
			g->DrawImageMirror(aImage, aDestRect, aSrcRect, aMirror);
		}
		else
		{
			float aOffsetX = mPosX + aCelWidth * 0.5f;
			float aOffsetY = mPosZ + mPosY + aCelHeight * 0.5f;
			SexyTransform2D aTransform;
			TodScaleRotateTransformMatrix(aTransform, aOffsetX + mBoard->mX, aOffsetY + mBoard->mY, mRotation, aScale, aScale);
			TodBltMatrix(g, aImage, aTransform, g->mClipRect, Color::White, g->mDrawMode, aSrcRect);
		}
	}

	if (mAttachmentID != AttachmentID::ATTACHMENTID_NULL)
	{
		Graphics theParticleGraphics(*g);
		MakeParentGraphicsFrame(&theParticleGraphics);
		AttachmentDraw(mAttachmentID, &theParticleGraphics, false);
	}
}

//0x46E8C0
void Projectile::DrawShadow(Graphics* g)
{
	int aCelCol = 0;
	float aScale = 1.0f;
	float aStretch = 1.0f;
	float aOffsetX = mPosX - mX;
	float aOffsetY = mPosY - mY;

	int aGridX = mBoard->PixelToGridXKeepOnBoard(mX, mY);
	bool isHighGround = false;
	if (mBoard->mGridSquareType[aGridX][mRow] == GridSquareType::GRIDSQUARE_HIGH_GROUND)
	{
		isHighGround = true;
	}
	if (mOnHighGround && !isHighGround)
	{
		aOffsetY += HIGH_GROUND_HEIGHT;
	}
	else if (!mOnHighGround && isHighGround)
	{
		aOffsetY -= HIGH_GROUND_HEIGHT;
	}

	if (mBoard->StageIsNight())
	{
		aCelCol = 1;
	}

	switch (mProjectileType)
	{
	case ProjectileType::PROJECTILE_BOMB:
	case ProjectileType::PROJECTILE_PEA:
	case ProjectileType::PROJECTILE_ROCKPEA:
	case ProjectileType::PROJECTILE_GOOPEA:
		aOffsetX += 3.0f;
		break;

	case ProjectileType::PROJECTILE_FIREBALL:
	case ProjectileType::PROJECTILE_PLASMABALL:
		aOffsetX += 3.0f;
		aOffsetY += 10.0f;
		break;

	case ProjectileType::PROJECTILE_ICEPEA:
	case ProjectileType::PROJECTILE_SNOWPEA:
		aOffsetX += -1.0f;
		aScale = 1.3f;
		break;

	case ProjectileType::PROJECTILE_MINT:
		aOffsetX += -1.0f;
		aScale = 1.3f;
		break;

	case ProjectileType::PROJECTILE_STAR:
	case ProjectileType::PROJECTILE_FUTURESTAR:
		aOffsetX += 7.0f;
		break;

	case ProjectileType::PROJECTILE_CABBAGE:
	case ProjectileType::PROJECTILE_PEPPER:
	case ProjectileType::PROJECTILE_LETTUCE:
	case ProjectileType::PROJECTILE_KERNEL:
	case ProjectileType::PROJECTILE_BUTTER:
	case ProjectileType::PROJECTILE_MELON:
	case ProjectileType::PROJECTILE_WINTERMELON:
	case ProjectileType::PROJECTILE_MOLD:
	case ProjectileType::PROJECTILE_SPORE:
		aOffsetX += 3.0f;
		aOffsetY += 10.0f;
		aScale = 1.6f;
		break;

	case ProjectileType::PROJECTILE_PUFF:
		return;
		
	case ProjectileType::PROJECTILE_COBBIG:
		aScale = 1.0f;
		aStretch = 3.0f;
		aOffsetX += 57.0f;
		break;

	case ProjectileType::PROJECTILE_FIREPEA:
	case ProjectileType::PROJECTILE_PLASMAPEA:
	case ProjectileType::PROJECTILE_BIGPLASMAPEA:
		aScale = 1.4f;
		break;
	}

	if (mMotionType == ProjectileMotion::MOTION_LOBBED)
	{
		float aHeight = ClampFloat(-mPosZ, 0.0f, 200.0f);
		aScale *= 200.0f / (aHeight + 200.0f);
	}

	TodDrawImageCelScaledF(g, IMAGE_PEA_SHADOWS, aOffsetX, (mShadowY - mPosY + aOffsetY), aCelCol, 0, aScale * aStretch, aScale);
}

//0x46EB20
void Projectile::Die()
{
	mDead = true;

	if (mProjectileType == ProjectileType::PROJECTILE_PUFF || mProjectileType == ProjectileType::PROJECTILE_MOLD || mProjectileType == ProjectileType::PROJECTILE_SPORE || mProjectileType == ProjectileType::PROJECTILE_ICEPEA || mProjectileType == ProjectileType::PROJECTILE_SNOWPEA || mProjectileType == ProjectileType::PROJECTILE_MINT)
	{
		AttachmentCrossFade(mAttachmentID, "FadeOut");
		AttachmentDetach(mAttachmentID);
	}
	else
	{
		AttachmentDie(mAttachmentID);
	}
}

//0x46EBC0
Rect Projectile::GetProjectileRect()
{
	if (mProjectileType == ProjectileType::PROJECTILE_PEA 
		|| mProjectileType == ProjectileType::PROJECTILE_SNOWPEA 
		|| mProjectileType == ProjectileType::PROJECTILE_ICEPEA 
		|| mProjectileType == ProjectileType::PROJECTILE_MINT 
		|| mProjectileType == ProjectileType::PROJECTILE_GOOPEA 
		|| mProjectileType == ProjectileType::PROJECTILE_BOMB
		|| mProjectileType == ProjectileType::PROJECTILE_FIREBALL
		|| mProjectileType == ProjectileType::PROJECTILE_PLASMABALL
		|| mProjectileType == ProjectileType::PROJECTILE_ROCKPEA
		)
	{
		return Rect(mX - 15, mY, mWidth + 15, mHeight);
	}
	else if (mProjectileType == ProjectileType::PROJECTILE_COBBIG)
	{
		return Rect(mX + mWidth / 2 - 115, mY + mHeight / 2 - 115, 230, 230);
	}
	else if (mProjectileType == ProjectileType::PROJECTILE_MELON || mProjectileType == ProjectileType::PROJECTILE_WINTERMELON)
	{
		return Rect(mX + 20, mY, 60, mHeight);
	}
	else if (mProjectileType == ProjectileType::PROJECTILE_FIREPEA || mProjectileType == ProjectileType::PROJECTILE_PLASMAPEA || mProjectileType == ProjectileType::PROJECTILE_BIGPLASMAPEA)
	{
		return Rect(mX, mY, mWidth - 10, mHeight);
	}
	else if (mProjectileType == ProjectileType::PROJECTILE_SPIKE || mProjectileType == ProjectileType::PROJECTILE_BEE || mProjectileType == ProjectileType::PROJECTILE_ASPARAGUS)
	{
		return Rect(mX - 25, mY, mWidth + 25, mHeight);
	}
	else
	{
		return Rect(mX, mY, mWidth, mHeight);
	}
}

void Projectile::ConvertToPlasmapea(int theGridX)
{
	if (mHitTorchwoodGridX == theGridX)
		return;

	AttachmentDie(mAttachmentID);
	mProjectileType = ProjectileType::PROJECTILE_PLASMAPEA;
	mHitTorchwoodGridX = theGridX;
	mApp->PlayFoley(FoleyType::FOLEY_FIREPEA);

	float aOffsetX = -25.0f;
	float aOffsetY = -25.0f;
	Reanimation* aPlasmaPeaReanim = mApp->AddReanimation(0.0f, 0.0f, 0, ReanimationType::REANIM_PLASMA_PEA);
	if (mMotionType == ProjectileMotion::MOTION_BACKWARDS)
	{
		aPlasmaPeaReanim->OverrideScale(-1.0f, 1.0f);
		aOffsetX += 80.0f;
	}

	aPlasmaPeaReanim->SetPosition(mPosX + aOffsetX, mPosY + aOffsetY);
	aPlasmaPeaReanim->mLoopType = ReanimLoopType::REANIM_LOOP;
	aPlasmaPeaReanim->mAnimRate = RandRangeFloat(50.0f, 80.0f);
	AttachReanim(mAttachmentID, aPlasmaPeaReanim, aOffsetX, aOffsetY);
}

//0x46ECB0
void Projectile::ConvertToFirepea(int theGridX)
{
	if (mHitTorchwoodGridX == theGridX)
		return;

	mProjectileType = ProjectileType::PROJECTILE_FIREPEA;
	mHitTorchwoodGridX = theGridX;
	mApp->PlayFoley(FoleyType::FOLEY_FIREPEA);

	float aOffsetX = -25.0f;
	float aOffsetY = -25.0f;
	Reanimation* aFirePeaReanim = mApp->AddReanimation(0.0f, 0.0f, 0, ReanimationType::REANIM_FIRE_PEA);
	if (mMotionType == ProjectileMotion::MOTION_BACKWARDS)
	{
		aFirePeaReanim->OverrideScale(-1.0f, 1.0f);
		aOffsetX += 80.0f;
	}

	aFirePeaReanim->SetPosition(mPosX + aOffsetX, mPosY + aOffsetY);
	aFirePeaReanim->mLoopType = ReanimLoopType::REANIM_LOOP;
	aFirePeaReanim->mAnimRate = RandRangeFloat(50.0f, 80.0f);
	AttachReanim(mAttachmentID, aFirePeaReanim, aOffsetX, aOffsetY);
}

//0x46EE00
void Projectile::ConvertToPea(int theGridX)
{
	if (mHitTorchwoodGridX == theGridX)
		return;

	AttachmentDie(mAttachmentID);
	mProjectileType = ProjectileType::PROJECTILE_PEA;
	mHitTorchwoodGridX = theGridX;
	mApp->PlayFoley(FoleyType::FOLEY_THROW);
}

//0x46EE00
void Projectile::ConvertToSnowPea(int theGridX)
{
	if (mHitTorchwoodGridX == theGridX)
		return;

	AttachmentDie(mAttachmentID);
	mProjectileType = ProjectileType::PROJECTILE_SNOWPEA;
	mHitTorchwoodGridX = theGridX;
	TodParticleSystem* aParticle = mApp->AddTodParticle(mPosX + 8.0f, mPosY + 13.0f, 400000, ParticleEffect::PARTICLE_SNOWPEA_TRAIL);
	AttachParticle(mAttachmentID, aParticle, 8.0f, 13.0f);
	mApp->PlayFoley(FoleyType::FOLEY_THROW);
}

void Projectile::ConvertToPlasmaball(int theGridX)
{
	if (mHitTorchwoodGridX == theGridX)
		return;

	AttachmentDie(mAttachmentID);
	mProjectileType = ProjectileType::PROJECTILE_PLASMABALL;
	mHitTorchwoodGridX = theGridX;
	mApp->PlayFoley(FoleyType::FOLEY_THROW);
}

ProjectileDefinition& Projectile::GetProjectileDef()
{
	ProjectileDefinition& aProjectileDef = gProjectileDefinition[(int)mProjectileType];
	TOD_ASSERT(aProjectileDef.mProjectileType == mProjectileType);

	return aProjectileDef;
}
