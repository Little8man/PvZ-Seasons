#pragma once

#include <string>
#include "GameObject.h"

#define MAX_MAGNET_ITEMS 10

#define RECHARGE_FAST 750
#define RECHARGE_SLOW 3000
#define RECHARGE_VERY_SLOW 5000
#define RECHARGE_PREMIUM 5000
#define RECHARGE_NONE 0
#define HEALTH_WEAK 150
#define HEALTH_NORMAL 300
#define HEALTH_MEDIUM 400
#define SPIKEROCK_HITS 9
#define HEALTH_SEMI_STRONG 2000
#define HEALTH_STRONG 4000
#define HEALTH_VERY_STRONG 8000
#define HEALTH_INSANE 12000
#define FIRE_RATE_VERY_FAST 75
#define FIRE_RATE_FAST 100
#define FIRE_RATE_NORMAL 150
#define FIRE_RATE_SLOW 300
#define FIRE_RATE_VERY_SLOW 600
#define FIRE_RATE_GLOOM 200
#define FIRE_RATE_BONK 40
#define FIRE_RATE_OAK 430
#define SUN_RATE_NORMAL 2500
#define SUN_RATE_FAST 1250

enum PlantSubClass
{
    SUBCLASS_NORMAL = 0,
    SUBCLASS_SHOOTER = 1
};

enum PlantWeapon
{
    WEAPON_PRIMARY,
    WEAPON_SECONDARY
};

enum PlantOnBungeeState
{
    NOT_ON_BUNGEE,
    GETTING_GRABBED_BY_BUNGEE,
    RISING_WITH_BUNGEE
};

enum PlantState
{
    STATE_NOTREADY,
    STATE_READY,
    STATE_DOINGSPECIAL,
    STATE_SQUASH_LOOK,
    STATE_SQUASH_PRE_LAUNCH,
    STATE_SQUASH_RISING,
    STATE_SQUASH_FALLING,
    STATE_SQUASH_DONE_FALLING,
    STATE_GRAVEBUSTER_LANDING,
    STATE_GRAVEBUSTER_EATING,
    STATE_CHOMPER_BITING,
    STATE_CHOMPER_BITING_GOT_ONE,
    STATE_CHOMPER_BITING_MISSED,
    STATE_CHOMPER_DIGESTING,
    STATE_CHOMPER_SWALLOWING,
    STATE_POTATO_RISING,
    STATE_POTATO_ARMED,
    STATE_POTATO_MASHED,
    STATE_SPIKEWEED_ATTACKING,
    STATE_SPIKEWEED_ATTACKING_2,
    STATE_SCAREDYSHROOM_LOWERING,
    STATE_SCAREDYSHROOM_SCARED,
    STATE_SCAREDYSHROOM_RAISING,
    STATE_SUNSHROOM_SMALL,
    STATE_SUNSHROOM_GROWING,
    STATE_SUNSHROOM_BIG,
    STATE_MAGNETSHROOM_SUCKING,
    STATE_MAGNETSHROOM_CHARGING,
    STATE_BOWLING_UP,
    STATE_BOWLING_DOWN,
    STATE_CACTUS_LOW,
    STATE_CACTUS_RISING,
    STATE_CACTUS_HIGH,
    STATE_CACTUS_LOWERING,
    STATE_TANGLEKELP_GRABBING,
    STATE_COBCANNON_ARMING,
    STATE_COBCANNON_LOADING,
    STATE_COBCANNON_READY,
    STATE_COBCANNON_FIRING,
    STATE_KERNELPULT_BUTTER,
    STATE_UMBRELLA_TRIGGERED,
    STATE_UMBRELLA_REFLECTING,
    STATE_IMITATER_MORPHING,
    STATE_ZEN_GARDEN_WATERED,
    STATE_ZEN_GARDEN_NEEDY,
    STATE_ZEN_GARDEN_HAPPY,
    STATE_MARIGOLD_ENDING,
    STATE_INVULNERABLE,
    STATE_DOING_PLANT_FOOD,
};

enum PLANT_LAYER
{
    PLANT_LAYER_BELOW = -1,
    PLANT_LAYER_MAIN,
    PLANT_LAYER_REANIM,
    PLANT_LAYER_REANIM_HEAD,
    PLANT_LAYER_REANIM_BLINK,
    PLANT_LAYER_ON_TOP,
    NUM_PLANT_LAYERS
};

enum PLANT_ORDER
{
    PLANT_ORDER_LILYPAD,
    PLANT_ORDER_NORMAL,
    PLANT_ORDER_PUMPKIN,
    PLANT_ORDER_FLYER,
    PLANT_ORDER_CHERRYBOMB
};

enum MagnetItemType
{
    MAGNET_ITEM_NONE,
    MAGNET_ITEM_PAIL_1,
    MAGNET_ITEM_PAIL_2,
    MAGNET_ITEM_PAIL_3,
    MAGNET_ITEM_FOOTBALL_HELMET_1,
    MAGNET_ITEM_FOOTBALL_HELMET_2,
    MAGNET_ITEM_FOOTBALL_HELMET_3,
    MAGNET_ITEM_DOOR_1,
    MAGNET_ITEM_DOOR_2,
    MAGNET_ITEM_DOOR_3,
    //MAGNET_ITEM_PROPELLER,
    MAGNET_ITEM_POGO_1,
    MAGNET_ITEM_POGO_2,
    MAGNET_ITEM_POGO_3,
    MAGNET_ITEM_JACK_IN_THE_BOX,
    MAGNET_ITEM_LADDER_1,
    MAGNET_ITEM_LADDER_2,
    MAGNET_ITEM_LADDER_3,
    MAGNET_ITEM_LADDER_PLACED,
    MAGNET_ITEM_SILVER_COIN,
    MAGNET_ITEM_GOLD_COIN,
    MAGNET_ITEM_DIAMOND,
    MAGNET_ITEM_PICK_AXE,
    MAGNET_ITEM_SUN,
    MAGNET_ITEM_SMALLSUN,
    MAGNET_ITEM_LARGESUN,
    MAGNET_ITEM_HELMET_1,
    MAGNET_ITEM_HELMET_2,
    MAGNET_ITEM_HELMET_3,
    MAGNET_ITEM_POT_1,
    MAGNET_ITEM_POT_2,
    MAGNET_ITEM_POT_3,
};

enum PlantSoundType {
    PLANT_SOUND_SUNFLOWER,
    PLANT_SOUND_PEASHOOTER,
    PLANT_SOUND_NONE
};

enum DeathSoundType {
    DEATH_SOUND_SUNFLOWER,
    DEATH_SOUND_PEASHOOTER,
    DEATH_SOUND_NONE
};

class MagnetItem
{
public:
    float                   mPosX;                          //+0x0
    float                   mPosY;                          //+0x4
    float                   mDestOffsetX;                   //+0x8
    float                   mDestOffsetY;                   //+0xC
    MagnetItemType          mItemType;                      //+0x10
};

class Coin;
class Zombie;
class Reanimation;
class TodParticleSystem;

class Plant : public GameObject
{
public:
    SeedType                mSeedType;                      //+0x24
    int                     mPlantCol;                      //+0x28
    int                     mAnimCounter;                   //+0x2C
    int                     mFrame;                         //+0x30
    int                     mFrameLength;                   //+0x34
    int                     mNumFrames;                     //+0x38
    PlantState              mState;                         //+0x3C
    int                     mPlantHealth;                   //+0x40
    int                     mPlantMaxHealth;                //+0x44
    int                     mSubclass;                      //+0x48
    int                     mDisappearCountdown;            //+0x4C
    int                     mDoSpecialCountdown;            //+0x50
    int                     mStateCountdown;                //+0x54
    int                     mLaunchCounter;                 //+0x58
    int                     mLaunchRate;                    //+0x5C
    Rect                    mPlantRect;                     //+0x60
    Rect                    mPlantAttackRect;               //+0x70
    int                     mTargetX;                       //+0x80
    int                     mTargetY;                       //+0x84
    int                     mStartRow;                      //+0x88
    ParticleSystemID        mParticleID;                    //+0x8C
    int                     mShootingCounter;               //+0x90
    ReanimationID           mBodyReanimID;                  //+0x94
    ReanimationID           mHeadReanimID;                  //+0x98
    ReanimationID           mHeadReanimID2;                 //+0x9C
    ReanimationID           mHeadReanimID3;                 //+0xA0
    ReanimationID           mBlinkReanimID;                 //+0xA4
    ReanimationID           mLightReanimID;                 //+0xA8
    ReanimationID           mSleepingReanimID;              //+0xAC
    int                     mBlinkCountdown;                //+0xB0
    int                     mRecentlyEatenCountdown;        //+0xB4
    int                     mEatenFlashCountdown;           //+0xB8
    int                     mBeghouledFlashCountdown;       //+0xBC
    float                   mShakeOffsetX;                  //+0xC0
    float                   mShakeOffsetY;                  //+0xC4
    MagnetItem              mMagnetItems[MAX_MAGNET_ITEMS]; //+0xC8
    ZombieID                mTargetZombieID;                //+0x12C
    int                     mWakeUpCounter;                 //+0x130
    PlantOnBungeeState      mOnBungeeState;                 //+0x134
    SeedType                mImitaterType;                  //+0x138
    int                     mPottedPlantIndex;              //+0x13C
    bool                    mAnimPing;                      //+0x140
    bool                    mDead;                          //+0x141
    bool                    mSquished;                      //+0x142
    bool                    mIsAsleep;                      //+0x143
    bool                    mIsOnBoard;                     //+0x144
    bool                    mHighlighted;                   //+0x145
    // Coffee Boost
    int                     mBoostCounter;
    bool                    mIsBoosted;
    // Repeater Fix
    int                     mStoredLaunchRate;
    // Sounds
    DeathSoundType          mDeathSound;
    // Zombotany
    int                     mChilledCounter;
    int                     mIceTrapCounter;
    int                     mWarmCounter;
    int                     mPoisonCounter;                             // How long the plant is poisoned
    int                     mPoisonRate;                                // Poison damage timer
    int                     mPoisonDamage;                              // Poison damage amount
    int                     mPlantAge;
    // Plant Food
    bool                    mPlantFoodReady;

public:
    Plant();

    void                    PlantInitialize(int theGridX, int theGridY, SeedType theSeedType, SeedType theImitaterType);
    void                    Update();
    void                    Animate();
    void                    Draw(Graphics* g);
    void                    MouseDown(int x, int y, int theClickCount);
    void                    DoSpecial();
    void                    Fire(Zombie* theTargetZombie, int theRow, PlantWeapon thePlantWeapon = PlantWeapon::WEAPON_PRIMARY);
    Zombie*                 FindTargetZombie(int theRow, PlantWeapon thePlantWeapon = PlantWeapon::WEAPON_PRIMARY);
    void                    Die();
    void                    UpdateProductionPlant();
    void                    UpdateShooter();
    bool                    FindTargetAndFire(int theRow, PlantWeapon thePlantWeapon = PlantWeapon::WEAPON_PRIMARY);
    void                    LaunchThreepeater();
    static Image*           GetImage(SeedType theSeedType);
    static int              GetCost(SeedType theSeedType, SeedType theImitaterType = SeedType::SEED_NONE);
    static SexyString       GetNameString(SeedType theSeedType, SeedType theImitaterType = SeedType::SEED_NONE);
    static SexyString       GetToolTip(SeedType theSeedType);
    static int              GetRefreshTime(SeedType theSeedType, SeedType theImitaterType = SeedType::SEED_NONE);
    const static bool       IsNocturnal(SeedType theSeedtype);
    const static bool       IsAquatic(SeedType theSeedType);
    const static bool       IsFlying(SeedType theSeedtype);
    const static bool       IsUpgrade(SeedType theSeedtype);
    const static bool       IsPremium(SeedType theSeedtype);
    void                    UpdateAbilities();
    void                    Squish();
    void                    DoRowAreaDamage(int theDamage, unsigned int theDamageFlags, PlantWeapon thePlantWeapon = PlantWeapon::WEAPON_PRIMARY);
    int                     GetDamageRangeFlags(PlantWeapon thePlantWeapon = PlantWeapon::WEAPON_PRIMARY);
    Rect                    GetPlantRect();
    Rect                    GetPlantAttackRect(PlantWeapon thePlantWeapon = PlantWeapon::WEAPON_PRIMARY);
    Zombie*                 FindSquashTarget();
    void                    UpdateSquash();
    /*inline*/ bool         NotOnGround();
    void                    DoSquashDamage();
    void                    IceZombies();
    void                    BlowAwayFliers(int theX, int theRow);
    void                    UpdateGraveBuster();
    TodParticleSystem*      AddAttachedParticle(int thePosX, int thePosY, int theRenderPosition, ParticleEffect theEffect);
    void                    GetPeaHeadOffset(int& theOffsetX, int& theOffsetY);
    static void             DrawSeedType(Graphics* g, SeedType theSeedType, SeedType theImitaterType, DrawVariation theDrawVariation, float thePosX, float thePosY);
    void                    KillAllPlantsNearDoom();
    bool                    IsOnHighGround();
    void                    UpdateTorchwood();
    void                    UpdateFlamewood();
    void                    LaunchStarFruit();
    bool                    FindStarFruitTarget();
    void                    UpdateChomper();
    void                    DoBlink();
    void                    UpdateBlink();
    void                    PlayBodyReanim(const char* theTrackName, ReanimLoopType theLoopType, int theBlendTime, float theAnimRate);
    void                    UpdateMagnetShroom();
    MagnetItem*             GetFreeMagnetItem();
    void                    DrawMagnetItems(Graphics* g);
    void                    UpdateDoomShroom();
    void                    UpdateIceShroom();
    void                    UpdatePotato();
    int                     CalcRenderOrder();
    void                    AnimateNuts();
    void                    SetSleeping(bool theIsAsleep);
    void                    UpdateShooting();
    void                    DrawShadow(Graphics* g, float theOffsetX, float theOffsetY);
    void                    UpdateScaredyShroom();
    int                     DistanceToClosestZombie();
    void                    UpdateSpikeweed();
    void                    MagnetShroomAttactItem(Zombie* theZombie);
    void                    UpdateSunShroom();
    void                    UpdateBowling();
    void                    AnimatePumpkin();
    void                    UpdateBlover();
    void                    UpdateCactus();
    void                    StarFruitFire();
    void                    UpdateTanglekelp();
    Reanimation*            AttachBlinkAnim(Reanimation* theReanimBody);
    void                    UpdateReanimColor();
    bool                    IsUpgradableTo(SeedType theUpgradedType);
    bool                    IsPartOfUpgradableTo(SeedType theUpgradedType);
    void                    UpdateCobCannon();
    void                    CobCannonFire(int theTargetX, int theTargetY);
    void                    UpdateGoldMagnetShroom();
    /*inline*/ bool         IsOnBoard();
    void                    RemoveEffects();
    void                    UpdateCoffeeBean();
    void                    UpdateUmbrella();
    void                    EndBlink();
    void                    AnimateGarlic();
    Coin*                   FindGoldMagnetTarget();
    void                    SpikeweedAttack();
    void                    ImitaterMorph();
    void                    UpdateImitater();
    void                    UpdateReanim();
    void                    SpikeRockTakeDamage();
    bool                    IsSpiky();
    static /*inline*/ void  PreloadPlantResources(SeedType theSeedType);
    /*inline*/ bool         IsInPlay();
    void                    UpdateNeedsFood() { ; }
    void                    PlayIdleAnim(float theRate);
    void                    UpdateUnderPlant();
    void                    GoldMagnetFindTargets();
    bool                    IsAGoldMagnetAboutToSuck();
    bool                    DrawMagnetItemsOnTop();
    // Counters
    void                    UpdateCounters();
    // Declutter
    const static bool       IsNut(SeedType theSeedtype);
    const static bool       IsWallNut(SeedType theSeedtype);
    const static bool       IsWall(SeedType theSeedtype);
    const static bool       MakesSun(SeedType theSeedtype);
    const static bool       IsPeashooter(SeedType theSeedType);
    const static bool       IsFakePlantFood(SeedType theSeedtype);
    const static bool       IsWarm(SeedType theSeedType);
    const static bool       IsCold(SeedType theSeedType);
    const static bool       IsToxic(SeedType theSeedType);
    const bool              IsInsomniac();
    // Future Star
    void                    FutureStarFire(int aProjectileCount, Zombie* theZombie);
    // Vine Nut
    void                    UpdateVineNut();
    // Iceberg Lettuce
    void                    UpdateIceberg();
    // GoldBloom
    void                    UpdateGoldbloom();
    // Bloomerang
    Zombie*                 FindBloomerangTarget();
    void                    LaunchBloomerang();
    // Infinut
    void                    UpdateInfiNut();
    // Melee
    void                    UpdateMelee();
    // Charging
    void                    UpdateChargingShooter();
    // Fake Plant Food
    void                    UpdateFakePlantFood();
    // Sounds
    static PlantSoundType   GetPlantSoundType(SeedType theSeedType);
    void                    Eaten();
    // Roof
    const static bool       IsRoof(SeedType theSeedType);
    // Zombotany
    void                    TakeDamage(int theDamage, unsigned int theDamageFlags);
    void                    ApplyPoison(int aPoisonTime, int aPoisonDamage, bool theDoesStack);
    void                    HitIceTrap();
    void                    HitIceTrapCustom(int theTime);
    void                    ApplyChill(bool theIsIceTrap);
    const static bool       CanBeChilled(SeedType theSeedType);
    void                    RemoveColdEffects();
};

float                       PlantDrawHeightOffset(Board* theBoard, Plant* thePlant, SeedType theSeedType, int theCol, int theRow);
float                       PlantFlowerPotHeightOffset(SeedType theSeedType, float theFlowerPotScale);

//PLANT.h (REPLACE PLANTDEFNITION CLASS)
class PlantDefinition
{
public:
    SeedType                mSeedType;          //+0x0
    Image**                 mPlantImage;        //+0x4
    ReanimationType         mReanimationType;   //+0x8
    int                     mPacketIndex;       //+0xC
    int                     mPlantMaxHealth;         //+0x10
    int                     mSeedCost;          //+0x10
    int                     mRefreshTime;       //+0x14
    PlantSubClass           mSubClass;          //+0x18
    int                     mLaunchRate;        //+0x1C
    const SexyChar*         mPlantName;         //+0x20
    PlantSoundType          mPlantSound;
    DeathSoundType          mDeathSound;
    int                     mPage;
};
extern PlantDefinition gPlantDefs[SeedType::NUM_SEED_TYPES];

/*inline*/ PlantDefinition& GetPlantDefinition(SeedType theSeedType);
