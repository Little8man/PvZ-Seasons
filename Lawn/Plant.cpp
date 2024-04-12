#include "Coin.h"
#include "Plant.h"
#include "Board.h"
#include "Zombie.h"
#include "Cutscene.h"
#include "GridItem.h"
#include "ZenGarden.h"
#include "Challenge.h"
#include "Projectile.h"
#include "SeedPacket.h"
#include "../LawnApp.h"
#include "CursorObject.h"
#include "../GameConstants.h"
#include "System/PlayerInfo.h"
#include "System/ReanimationLawn.h"
#include "../Sexy.TodLib/TodFoley.h"
#include "../Sexy.TodLib/TodDebug.h"
#include "../Sexy.TodLib/Attachment.h"
#include "../Sexy.TodLib/Reanimator.h"
#include "../Sexy.TodLib/TodParticle.h"
#include "../Sexy.TodLib/EffectSystem.h"
#include "../Sexy.TodLib/TodStringFile.h"

PlantDefinition gPlantDefs[SeedType::NUM_SEED_TYPES] = {  //0x69F2B0
    { SeedType::SEED_PEASHOOTER,        nullptr, ReanimationType::REANIM_PEASHOOTER,        0,  HEALTH_NORMAL,      8,      RECHARGE_FAST,      PlantSubClass::SUBCLASS_SHOOTER,    FIRE_RATE_NORMAL,   _S("PEASHOOTER"),           PlantSoundType::PLANT_SOUND_PEASHOOTER,     DeathSoundType::DEATH_SOUND_PEASHOOTER, SeedChooserPage::PAGE_NORMAL    },
    { SeedType::SEED_SUNFLOWER,         nullptr, ReanimationType::REANIM_SUNFLOWER,         1,  HEALTH_WEAK,        4,      RECHARGE_FAST,      PlantSubClass::SUBCLASS_NORMAL,     SUN_RATE_NORMAL,    _S("SUNFLOWER"),            PlantSoundType::PLANT_SOUND_SUNFLOWER,      DeathSoundType::DEATH_SOUND_SUNFLOWER,  SeedChooserPage::PAGE_NORMAL    },
    { SeedType::SEED_CHERRYBOMB,        nullptr, ReanimationType::REANIM_CHERRYBOMB,        3,  HEALTH_NORMAL,      12,     RECHARGE_VERY_SLOW, PlantSubClass::SUBCLASS_NORMAL,     0,                  _S("CHERRY_BOMB"),          PlantSoundType::PLANT_SOUND_NONE,           DeathSoundType::DEATH_SOUND_NONE,       SeedChooserPage::PAGE_NORMAL    },
    { SeedType::SEED_WALLNUT,           nullptr, ReanimationType::REANIM_WALLNUT,           2,  HEALTH_STRONG,      4,      RECHARGE_SLOW,      PlantSubClass::SUBCLASS_NORMAL,     0,                  _S("WALL_NUT"),             PlantSoundType::PLANT_SOUND_NONE,           DeathSoundType::DEATH_SOUND_NONE,       SeedChooserPage::PAGE_NORMAL    },
    { SeedType::SEED_POTATOMINE,        nullptr, ReanimationType::REANIM_POTATOMINE,        37, HEALTH_NORMAL,      2,      RECHARGE_SLOW,      PlantSubClass::SUBCLASS_NORMAL,     0,                  _S("POTATO_MINE"),          PlantSoundType::PLANT_SOUND_NONE,           DeathSoundType::DEATH_SOUND_NONE,       SeedChooserPage::PAGE_NORMAL    },
    { SeedType::SEED_SNOWPEA,           nullptr, ReanimationType::REANIM_SNOWPEA,           4,  HEALTH_NORMAL,      14,     RECHARGE_FAST,      PlantSubClass::SUBCLASS_SHOOTER,    FIRE_RATE_NORMAL,   _S("SNOW_PEA"),             PlantSoundType::PLANT_SOUND_PEASHOOTER,     DeathSoundType::DEATH_SOUND_PEASHOOTER, SeedChooserPage::PAGE_NORMAL    },
    { SeedType::SEED_CHOMPER,           nullptr, ReanimationType::REANIM_CHOMPER,           31, HEALTH_NORMAL,      12,     RECHARGE_FAST,      PlantSubClass::SUBCLASS_NORMAL,     0,                  _S("CHOMPER"),              PlantSoundType::PLANT_SOUND_NONE,           DeathSoundType::DEATH_SOUND_NONE,       SeedChooserPage::PAGE_NORMAL    },
    { SeedType::SEED_REPEATER,          nullptr, ReanimationType::REANIM_REPEATER,          5,  HEALTH_NORMAL,      16,     RECHARGE_FAST,      PlantSubClass::SUBCLASS_SHOOTER,    FIRE_RATE_NORMAL,   _S("REPEATER"),             PlantSoundType::PLANT_SOUND_PEASHOOTER,     DeathSoundType::DEATH_SOUND_PEASHOOTER, SeedChooserPage::PAGE_NORMAL    },
    { SeedType::SEED_PUFFSHROOM,        nullptr, ReanimationType::REANIM_PUFFSHROOM,        6,  HEALTH_WEAK,        0,      RECHARGE_FAST,      PlantSubClass::SUBCLASS_SHOOTER,    FIRE_RATE_NORMAL,   _S("PUFF_SHROOM"),          PlantSoundType::PLANT_SOUND_NONE,           DeathSoundType::DEATH_SOUND_NONE,       SeedChooserPage::PAGE_NORMAL    },
    { SeedType::SEED_SUNSHROOM,         nullptr, ReanimationType::REANIM_SUNSHROOM,         7,  HEALTH_WEAK,        2,      RECHARGE_FAST,      PlantSubClass::SUBCLASS_NORMAL,     SUN_RATE_NORMAL,    _S("SUN_SHROOM"),           PlantSoundType::PLANT_SOUND_NONE,           DeathSoundType::DEATH_SOUND_NONE,       SeedChooserPage::PAGE_NORMAL    },
    { SeedType::SEED_FUMESHROOM,        nullptr, ReanimationType::REANIM_FUMESHROOM,        9,  HEALTH_NORMAL,      6,      RECHARGE_FAST,      PlantSubClass::SUBCLASS_SHOOTER,    FIRE_RATE_NORMAL,   _S("FUME_SHROOM"),          PlantSoundType::PLANT_SOUND_NONE,           DeathSoundType::DEATH_SOUND_NONE,       SeedChooserPage::PAGE_NORMAL    },
    { SeedType::SEED_GRAVEBUSTER,       nullptr, ReanimationType::REANIM_GRAVE_BUSTER,      40, HEALTH_NORMAL,      6,      RECHARGE_FAST,      PlantSubClass::SUBCLASS_NORMAL,     0,                  _S("GRAVE_BUSTER"),         PlantSoundType::PLANT_SOUND_NONE,           DeathSoundType::DEATH_SOUND_NONE,       SeedChooserPage::PAGE_NORMAL    },
    { SeedType::SEED_HYPNOSHROOM,       nullptr, ReanimationType::REANIM_HYPNOSHROOM,       10, HEALTH_NORMAL,      6,      RECHARGE_SLOW,      PlantSubClass::SUBCLASS_NORMAL,     0,                  _S("HYPNO_SHROOM"),         PlantSoundType::PLANT_SOUND_NONE,           DeathSoundType::DEATH_SOUND_NONE,       SeedChooserPage::PAGE_NORMAL    },
    { SeedType::SEED_SCAREDYSHROOM,     nullptr, ReanimationType::REANIM_SCRAREYSHROOM,     33, HEALTH_WEAK,        2,      RECHARGE_FAST,      PlantSubClass::SUBCLASS_SHOOTER,    FIRE_RATE_NORMAL,   _S("SCAREDY_SHROOM"),       PlantSoundType::PLANT_SOUND_NONE,           DeathSoundType::DEATH_SOUND_NONE,       SeedChooserPage::PAGE_NORMAL    },
    { SeedType::SEED_ICESHROOM,         nullptr, ReanimationType::REANIM_ICESHROOM,         36, HEALTH_NORMAL,      6,      RECHARGE_VERY_SLOW, PlantSubClass::SUBCLASS_NORMAL,     0,                  _S("ICE_SHROOM"),           PlantSoundType::PLANT_SOUND_NONE,           DeathSoundType::DEATH_SOUND_NONE,       SeedChooserPage::PAGE_NORMAL    },
    { SeedType::SEED_DOOMSHROOM,        nullptr, ReanimationType::REANIM_DOOMSHROOM,        20, HEALTH_NORMAL,      10,     RECHARGE_VERY_SLOW, PlantSubClass::SUBCLASS_NORMAL,     0,                  _S("DOOM_SHROOM"),          PlantSoundType::PLANT_SOUND_NONE,           DeathSoundType::DEATH_SOUND_NONE,       SeedChooserPage::PAGE_NORMAL    },
    { SeedType::SEED_LILYPAD,           nullptr, ReanimationType::REANIM_LILYPAD,           19, HEALTH_WEAK,        2,      RECHARGE_FAST,      PlantSubClass::SUBCLASS_NORMAL,     0,                  _S("LILY_PAD"),             PlantSoundType::PLANT_SOUND_NONE,           DeathSoundType::DEATH_SOUND_NONE,       SeedChooserPage::PAGE_NORMAL    },
    { SeedType::SEED_SQUASH,            nullptr, ReanimationType::REANIM_SQUASH,            21, HEALTH_NORMAL,      4,      RECHARGE_SLOW,      PlantSubClass::SUBCLASS_NORMAL,     0,                  _S("SQUASH"),               PlantSoundType::PLANT_SOUND_NONE,           DeathSoundType::DEATH_SOUND_NONE,       SeedChooserPage::PAGE_NORMAL    },
    { SeedType::SEED_THREEPEATER,       nullptr, ReanimationType::REANIM_THREEPEATER,       12, HEALTH_NORMAL,      26,     RECHARGE_FAST,      PlantSubClass::SUBCLASS_SHOOTER,    FIRE_RATE_NORMAL,   _S("THREEPEATER"),          PlantSoundType::PLANT_SOUND_PEASHOOTER,     DeathSoundType::DEATH_SOUND_PEASHOOTER, SeedChooserPage::PAGE_NORMAL    },
    { SeedType::SEED_TANGLEKELP,        nullptr, ReanimationType::REANIM_TANGLEKELP,        17, HEALTH_NORMAL,      2,      RECHARGE_SLOW,      PlantSubClass::SUBCLASS_NORMAL,     0,                  _S("TANGLE_KELP"),          PlantSoundType::PLANT_SOUND_NONE,           DeathSoundType::DEATH_SOUND_NONE,       SeedChooserPage::PAGE_NORMAL    },
    { SeedType::SEED_JALAPENO,          nullptr, ReanimationType::REANIM_JALAPENO,          11, HEALTH_NORMAL,      10,     RECHARGE_VERY_SLOW, PlantSubClass::SUBCLASS_NORMAL,     0,                  _S("JALAPENO"),             PlantSoundType::PLANT_SOUND_NONE,           DeathSoundType::DEATH_SOUND_NONE,       SeedChooserPage::PAGE_NORMAL    },
    { SeedType::SEED_SPIKEWEED,         nullptr, ReanimationType::REANIM_SPIKEWEED,         22, HEALTH_NORMAL,      8,      RECHARGE_FAST,      PlantSubClass::SUBCLASS_NORMAL,     FIRE_RATE_FAST,     _S("SPIKEWEED"),            PlantSoundType::PLANT_SOUND_NONE,           DeathSoundType::DEATH_SOUND_NONE,       SeedChooserPage::PAGE_NORMAL    },
    { SeedType::SEED_TORCHWOOD,         nullptr, ReanimationType::REANIM_TORCHWOOD,         29, HEALTH_NORMAL,      14,     RECHARGE_FAST,      PlantSubClass::SUBCLASS_NORMAL,     0,                  _S("TORCHWOOD"),            PlantSoundType::PLANT_SOUND_NONE,           DeathSoundType::DEATH_SOUND_NONE,       SeedChooserPage::PAGE_NORMAL    },
    { SeedType::SEED_TALLNUT,           nullptr, ReanimationType::REANIM_TALLNUT,           28, HEALTH_VERY_STRONG, 10,     RECHARGE_SLOW,      PlantSubClass::SUBCLASS_NORMAL,     0,                  _S("TALL_NUT"),             PlantSoundType::PLANT_SOUND_NONE,           DeathSoundType::DEATH_SOUND_NONE,       SeedChooserPage::PAGE_NORMAL    },
    { SeedType::SEED_SEASHROOM,         nullptr, ReanimationType::REANIM_SEASHROOM,         39, HEALTH_NORMAL,      0,      RECHARGE_SLOW,      PlantSubClass::SUBCLASS_SHOOTER,    FIRE_RATE_NORMAL,   _S("SEA_SHROOM"),           PlantSoundType::PLANT_SOUND_NONE,           DeathSoundType::DEATH_SOUND_NONE,       SeedChooserPage::PAGE_NORMAL    },
    { SeedType::SEED_PLANTERN,          nullptr, ReanimationType::REANIM_PLANTERN,          38, HEALTH_NORMAL,      2,      RECHARGE_SLOW,      PlantSubClass::SUBCLASS_NORMAL,     SUN_RATE_NORMAL,    _S("PLANTERN"),             PlantSoundType::PLANT_SOUND_NONE,           DeathSoundType::DEATH_SOUND_NONE,       SeedChooserPage::PAGE_NORMAL    },
    { SeedType::SEED_CACTUS,            nullptr, ReanimationType::REANIM_CACTUS,            15, HEALTH_NORMAL,      10,     RECHARGE_FAST,      PlantSubClass::SUBCLASS_SHOOTER,    FIRE_RATE_NORMAL,   _S("CACTUS"),               PlantSoundType::PLANT_SOUND_NONE,           DeathSoundType::DEATH_SOUND_NONE,       SeedChooserPage::PAGE_NORMAL    },
    { SeedType::SEED_BLOVER,            nullptr, ReanimationType::REANIM_BLOVER,            18, HEALTH_NORMAL,      8,      RECHARGE_FAST,      PlantSubClass::SUBCLASS_NORMAL,     0,                  _S("BLOVER"),               PlantSoundType::PLANT_SOUND_NONE,           DeathSoundType::DEATH_SOUND_NONE,       SeedChooserPage::PAGE_NORMAL    },
    { SeedType::SEED_SPLITPEA,          nullptr, ReanimationType::REANIM_SPLITPEA,          32, HEALTH_NORMAL,      10,     RECHARGE_FAST,      PlantSubClass::SUBCLASS_SHOOTER,    FIRE_RATE_NORMAL,   _S("SPLIT_PEA"),            PlantSoundType::PLANT_SOUND_PEASHOOTER,     DeathSoundType::DEATH_SOUND_PEASHOOTER, SeedChooserPage::PAGE_NORMAL    },
    { SeedType::SEED_STARFRUIT,         nullptr, ReanimationType::REANIM_STARFRUIT,         30, HEALTH_NORMAL,      10,     RECHARGE_FAST,      PlantSubClass::SUBCLASS_SHOOTER,    FIRE_RATE_NORMAL,   _S("STARFRUIT"),            PlantSoundType::PLANT_SOUND_NONE,           DeathSoundType::DEATH_SOUND_NONE,       SeedChooserPage::PAGE_NORMAL    },
    { SeedType::SEED_PUMPKINSHELL,      nullptr, ReanimationType::REANIM_PUMPKIN,           25, HEALTH_STRONG,      10,     RECHARGE_SLOW,      PlantSubClass::SUBCLASS_NORMAL,     0,                  _S("PUMPKIN"),              PlantSoundType::PLANT_SOUND_NONE,           DeathSoundType::DEATH_SOUND_NONE,       SeedChooserPage::PAGE_NORMAL    },
    { SeedType::SEED_MAGNETSHROOM,      nullptr, ReanimationType::REANIM_MAGNETSHROOM,      35, HEALTH_NORMAL,      8,      RECHARGE_FAST,      PlantSubClass::SUBCLASS_NORMAL,     0,                  _S("MAGNET_SHROOM"),        PlantSoundType::PLANT_SOUND_NONE,           DeathSoundType::DEATH_SOUND_NONE,       SeedChooserPage::PAGE_NORMAL    },
    { SeedType::SEED_CABBAGEPULT,       nullptr, ReanimationType::REANIM_CABBAGEPULT,       13, HEALTH_NORMAL,      8,      RECHARGE_FAST,      PlantSubClass::SUBCLASS_SHOOTER,    FIRE_RATE_SLOW,     _S("CABBAGE_PULT"),         PlantSoundType::PLANT_SOUND_NONE,           DeathSoundType::DEATH_SOUND_NONE,       SeedChooserPage::PAGE_NORMAL    },
    { SeedType::SEED_FLOWERPOT,         nullptr, ReanimationType::REANIM_FLOWER_POT,        33, HEALTH_WEAK,        2,      RECHARGE_FAST,      PlantSubClass::SUBCLASS_NORMAL,     0,                  _S("FLOWER_POT"),           PlantSoundType::PLANT_SOUND_NONE,           DeathSoundType::DEATH_SOUND_NONE,       SeedChooserPage::PAGE_NORMAL    },
    { SeedType::SEED_KERNELPULT,        nullptr, ReanimationType::REANIM_KERNELPULT,        13, HEALTH_NORMAL,      8,      RECHARGE_FAST,      PlantSubClass::SUBCLASS_SHOOTER,    FIRE_RATE_SLOW,     _S("KERNEL_PULT"),          PlantSoundType::PLANT_SOUND_NONE,           DeathSoundType::DEATH_SOUND_NONE,       SeedChooserPage::PAGE_NORMAL    },
    { SeedType::SEED_INSTANT_COFFEE,    nullptr, ReanimationType::REANIM_COFFEEBEAN,        33, HEALTH_NORMAL,      6,      RECHARGE_FAST,      PlantSubClass::SUBCLASS_NORMAL,     0,                  _S("COFFEE_BEAN"),          PlantSoundType::PLANT_SOUND_NONE,           DeathSoundType::DEATH_SOUND_NONE,       SeedChooserPage::PAGE_NORMAL    },
    { SeedType::SEED_GARLIC,            nullptr, ReanimationType::REANIM_GARLIC,            8,  HEALTH_MEDIUM,      4,      RECHARGE_FAST,      PlantSubClass::SUBCLASS_NORMAL,     0,                  _S("GARLIC"),               PlantSoundType::PLANT_SOUND_NONE,           DeathSoundType::DEATH_SOUND_NONE,       SeedChooserPage::PAGE_NORMAL    },
    { SeedType::SEED_UMBRELLA,          nullptr, ReanimationType::REANIM_UMBRELLALEAF,      23, HEALTH_NORMAL,      8,      RECHARGE_FAST,      PlantSubClass::SUBCLASS_NORMAL,     0,                  _S("UMBRELLA_LEAF"),        PlantSoundType::PLANT_SOUND_NONE,           DeathSoundType::DEATH_SOUND_NONE,       SeedChooserPage::PAGE_NORMAL    },
    { SeedType::SEED_MARIGOLD,          nullptr, ReanimationType::REANIM_MARIGOLD,          24, HEALTH_NORMAL,      4,      RECHARGE_SLOW,      PlantSubClass::SUBCLASS_NORMAL,     SUN_RATE_NORMAL,    _S("MARIGOLD"),             PlantSoundType::PLANT_SOUND_NONE,           DeathSoundType::DEATH_SOUND_NONE,       SeedChooserPage::PAGE_NORMAL    },
    { SeedType::SEED_MELONPULT,         nullptr, ReanimationType::REANIM_MELONPULT,         14, HEALTH_NORMAL,      24,     RECHARGE_FAST,      PlantSubClass::SUBCLASS_SHOOTER,    FIRE_RATE_SLOW,     _S("MELON_PULT"),           PlantSoundType::PLANT_SOUND_NONE,           DeathSoundType::DEATH_SOUND_NONE,       SeedChooserPage::PAGE_NORMAL    },
    { SeedType::SEED_MOLDSHROOM,        nullptr, ReanimationType::REANIM_PUFFSHROOM,        6,  HEALTH_NORMAL,      0,      RECHARGE_SLOW,      PlantSubClass::SUBCLASS_SHOOTER,    FIRE_RATE_SLOW,     _S("MOLD_SHROOM"),          PlantSoundType::PLANT_SOUND_NONE,           DeathSoundType::DEATH_SOUND_NONE,       SeedChooserPage::PAGE_NORMAL    },
    { SeedType::SEED_WATERPOT,          nullptr, ReanimationType::REANIM_FLOWER_POT,        33, HEALTH_NORMAL,      4,      RECHARGE_SLOW,      PlantSubClass::SUBCLASS_NORMAL,     0,                  _S("WATER_POT"),            PlantSoundType::PLANT_SOUND_NONE,           DeathSoundType::DEATH_SOUND_NONE,       SeedChooserPage::PAGE_NORMAL    },
    { SeedType::SEED_COFFEEPOT,         nullptr, ReanimationType::REANIM_FLOWER_POT,        33, HEALTH_NORMAL,      6,      RECHARGE_SLOW,      PlantSubClass::SUBCLASS_NORMAL,     0,                  _S("COFFEE_POT"),           PlantSoundType::PLANT_SOUND_NONE,           DeathSoundType::DEATH_SOUND_NONE,       SeedChooserPage::PAGE_NORMAL    },
    { SeedType::SEED_ICEBERGLETTUCEBETA,nullptr, ReanimationType::REANIM_ICEBERG_LETTUCE,   40, HEALTH_NORMAL,      14,     RECHARGE_FAST,      PlantSubClass::SUBCLASS_SHOOTER,    FIRE_RATE_SLOW,     _S("ICEBERG_LETTUCE_BETA"), PlantSoundType::PLANT_SOUND_NONE,           DeathSoundType::DEATH_SOUND_NONE,       SeedChooserPage::PAGE_NORMAL    },
    { SeedType::SEED_GATLINGPEA,        nullptr, ReanimationType::REANIM_GATLINGPEA,        5,  HEALTH_NORMAL,      36,     RECHARGE_PREMIUM,   PlantSubClass::SUBCLASS_SHOOTER,    FIRE_RATE_NORMAL,   _S("GATLING_PEA"),          PlantSoundType::PLANT_SOUND_PEASHOOTER,     DeathSoundType::DEATH_SOUND_PEASHOOTER, SeedChooserPage::PAGE_NORMAL    },
    { SeedType::SEED_TWINSUNFLOWER,     nullptr, ReanimationType::REANIM_TWIN_SUNFLOWER,    1,  HEALTH_NORMAL,      16,     RECHARGE_PREMIUM,   PlantSubClass::SUBCLASS_NORMAL,     SUN_RATE_NORMAL,    _S("TWIN_SUNFLOWER"),       PlantSoundType::PLANT_SOUND_SUNFLOWER,      DeathSoundType::DEATH_SOUND_SUNFLOWER,  SeedChooserPage::PAGE_NORMAL    },
    { SeedType::SEED_GLOOMSHROOM,       nullptr, ReanimationType::REANIM_GLOOMSHROOM,       27, HEALTH_NORMAL,      18,     RECHARGE_PREMIUM,   PlantSubClass::SUBCLASS_SHOOTER,    FIRE_RATE_GLOOM,    _S("GLOOM_SHROOM"),         PlantSoundType::PLANT_SOUND_NONE,           DeathSoundType::DEATH_SOUND_NONE,       SeedChooserPage::PAGE_NORMAL    },
    { SeedType::SEED_CATTAIL,           nullptr, ReanimationType::REANIM_CATTAIL,           27, HEALTH_NORMAL,      20,     RECHARGE_PREMIUM,   PlantSubClass::SUBCLASS_SHOOTER,    FIRE_RATE_NORMAL,   _S("CATTAIL"),              PlantSoundType::PLANT_SOUND_NONE,           DeathSoundType::DEATH_SOUND_NONE,       SeedChooserPage::PAGE_NORMAL    },
    { SeedType::SEED_WINTERMELON,       nullptr, ReanimationType::REANIM_WINTER_MELON,      27, HEALTH_NORMAL,      40,     RECHARGE_PREMIUM,   PlantSubClass::SUBCLASS_SHOOTER,    FIRE_RATE_SLOW,     _S("WINTER_MELON"),         PlantSoundType::PLANT_SOUND_NONE,           DeathSoundType::DEATH_SOUND_NONE,       SeedChooserPage::PAGE_NORMAL    },
    { SeedType::SEED_GOLD_MAGNET,       nullptr, ReanimationType::REANIM_GOLD_MAGNET,       27, HEALTH_NORMAL,      12,     RECHARGE_PREMIUM,   PlantSubClass::SUBCLASS_NORMAL,     0,                  _S("GOLD_MAGNET"),          PlantSoundType::PLANT_SOUND_NONE,           DeathSoundType::DEATH_SOUND_NONE,       SeedChooserPage::PAGE_NORMAL    },
    { SeedType::SEED_SPIKEROCK,         nullptr, ReanimationType::REANIM_SPIKEROCK,         27, SPIKEROCK_HITS * 50,18,     RECHARGE_PREMIUM,   PlantSubClass::SUBCLASS_NORMAL,     FIRE_RATE_FAST,     _S("SPIKEROCK"),            PlantSoundType::PLANT_SOUND_NONE,           DeathSoundType::DEATH_SOUND_NONE,       SeedChooserPage::PAGE_NORMAL    },
    { SeedType::SEED_COBCANNON,         nullptr, ReanimationType::REANIM_COBCANNON,         16, HEALTH_NORMAL,      40,     RECHARGE_PREMIUM,   PlantSubClass::SUBCLASS_NORMAL,     FIRE_RATE_VERY_SLOW,_S("COB_CANNON"),           PlantSoundType::PLANT_SOUND_NONE,           DeathSoundType::DEATH_SOUND_NONE,       SeedChooserPage::PAGE_NORMAL    },
    { SeedType::SEED_IMITATER,          nullptr, ReanimationType::REANIM_IMITATER,          33, HEALTH_NORMAL,      0,      RECHARGE_FAST,      PlantSubClass::SUBCLASS_NORMAL,     0,                  _S("IMITATER"),             PlantSoundType::PLANT_SOUND_NONE,           DeathSoundType::DEATH_SOUND_NONE,       SeedChooserPage::PAGE_NORMAL    },
    { SeedType::SEED_ASPEARAGUS,        nullptr, ReanimationType::REANIM_PEASHOOTER,        0,  HEALTH_NORMAL,      10,     RECHARGE_FAST,      PlantSubClass::SUBCLASS_SHOOTER,    FIRE_RATE_SLOW,     _S("ASPEARAGUS"),           PlantSoundType::PLANT_SOUND_NONE,           DeathSoundType::DEATH_SOUND_NONE,       SeedChooserPage::PAGE_ADVENTURES},
    { SeedType::SEED_POPCORN,           nullptr, ReanimationType::REANIM_POTATOMINE,        40, HEALTH_NORMAL,      4,      RECHARGE_VERY_SLOW, PlantSubClass::SUBCLASS_NORMAL,     0,                  _S("POPCORN"),              PlantSoundType::PLANT_SOUND_NONE,           DeathSoundType::DEATH_SOUND_NONE,       SeedChooserPage::PAGE_ADVENTURES},
    { SeedType::SEED_BEET,              nullptr, ReanimationType::REANIM_CHOMPER,           31, HEALTH_NORMAL,      8,      RECHARGE_FAST,      PlantSubClass::SUBCLASS_NORMAL,     FIRE_RATE_VERY_FAST,_S("BEET"),                 PlantSoundType::PLANT_SOUND_NONE,           DeathSoundType::DEATH_SOUND_NONE,       SeedChooserPage::PAGE_ADVENTURES},
    { SeedType::SEED_SHAMROCK,          nullptr, ReanimationType::REANIM_SCRAREYSHROOM,     13, HEALTH_SEMI_STRONG, 10,     RECHARGE_SLOW,      PlantSubClass::SUBCLASS_SHOOTER,    FIRE_RATE_SLOW,     _S("SHAMROCK"),             PlantSoundType::PLANT_SOUND_NONE,           DeathSoundType::DEATH_SOUND_NONE,       SeedChooserPage::PAGE_ADVENTURES},
    { SeedType::SEED_BAMBOOSHOOT,       nullptr, ReanimationType::REANIM_CABBAGEPULT,       13, HEALTH_NORMAL,      26,     RECHARGE_FAST,      PlantSubClass::SUBCLASS_SHOOTER,    FIRE_RATE_SLOW,     _S("BAMBOO_SHOOT"),         PlantSoundType::PLANT_SOUND_NONE,           DeathSoundType::DEATH_SOUND_NONE,       SeedChooserPage::PAGE_ADVENTURES},
    { SeedType::SEED_FIREPEA,           nullptr, ReanimationType::REANIM_SNOWPEA,           40, HEALTH_NORMAL,      14,     RECHARGE_FAST,      PlantSubClass::SUBCLASS_SHOOTER,    FIRE_RATE_NORMAL,   _S("FIRE_PEA"),             PlantSoundType::PLANT_SOUND_PEASHOOTER,     DeathSoundType::DEATH_SOUND_PEASHOOTER, SeedChooserPage::PAGE_ADVENTURES},
    { SeedType::SEED_CHILLYPEPPER,      nullptr, ReanimationType::REANIM_CHILLYPEPPER,      11, HEALTH_NORMAL,      10,     RECHARGE_VERY_SLOW, PlantSubClass::SUBCLASS_NORMAL,     0,                  _S("CHILLY_PEPPER"),        PlantSoundType::PLANT_SOUND_NONE,           DeathSoundType::DEATH_SOUND_NONE,       SeedChooserPage::PAGE_ADVENTURES},
    { SeedType::SEED_SWEETPEA,          nullptr, ReanimationType::REANIM_SNOWPEA,           27, HEALTH_NORMAL,      32,     RECHARGE_PREMIUM,   PlantSubClass::SUBCLASS_SHOOTER,    FIRE_RATE_NORMAL,   _S("SWEET_PEA"),            PlantSoundType::PLANT_SOUND_PEASHOOTER,     DeathSoundType::DEATH_SOUND_PEASHOOTER, SeedChooserPage::PAGE_ADVENTURES},
    { SeedType::SEED_BEESHOOTER,        nullptr, ReanimationType::REANIM_SNOWPEA,           27, HEALTH_NORMAL,      36,     RECHARGE_PREMIUM,   PlantSubClass::SUBCLASS_SHOOTER,    FIRE_RATE_NORMAL,   _S("BEESHOOTER"),           PlantSoundType::PLANT_SOUND_PEASHOOTER,     DeathSoundType::DEATH_SOUND_PEASHOOTER, SeedChooserPage::PAGE_ADVENTURES},
    { SeedType::SEED_HARDNUT,           nullptr, ReanimationType::REANIM_WALLNUT,           2,  HEALTH_INSANE,      22,     RECHARGE_PREMIUM,   PlantSubClass::SUBCLASS_NORMAL,     0,                  _S("HARD_NUT"),             PlantSoundType::PLANT_SOUND_NONE,           DeathSoundType::DEATH_SOUND_NONE,       SeedChooserPage::PAGE_ADVENTURES},
    { SeedType::SEED_ACEPEARAGUS,       nullptr, ReanimationType::REANIM_PEASHOOTER,        0,  HEALTH_NORMAL,      24,     RECHARGE_PREMIUM,   PlantSubClass::SUBCLASS_SHOOTER,    FIRE_RATE_SLOW,     _S("ACEPEARAGUS"),          PlantSoundType::PLANT_SOUND_NONE,           DeathSoundType::DEATH_SOUND_NONE,       SeedChooserPage::PAGE_ADVENTURES},
    { SeedType::SEED_POWERFLOWER,       nullptr, ReanimationType::REANIM_POWERFLOWER,       27, HEALTH_NORMAL,      36,     RECHARGE_PREMIUM,   PlantSubClass::SUBCLASS_NORMAL,     SUN_RATE_NORMAL,    _S("POWER_FLOWER"),         PlantSoundType::PLANT_SOUND_SUNFLOWER,      DeathSoundType::DEATH_SOUND_SUNFLOWER,  SeedChooserPage::PAGE_ADVENTURES},
    { SeedType::SEED_BEETBOXER,         nullptr, ReanimationType::REANIM_CHOMPER,           31, HEALTH_NORMAL,      20,     RECHARGE_PREMIUM,   PlantSubClass::SUBCLASS_NORMAL,     FIRE_RATE_VERY_FAST,_S("BEETBOXER"),            PlantSoundType::PLANT_SOUND_NONE,           DeathSoundType::DEATH_SOUND_NONE,       SeedChooserPage::PAGE_ADVENTURES},
    { SeedType::SEED_ICEQUEENPEA,       nullptr, ReanimationType::REANIM_SNOWPEA,           27, HEALTH_NORMAL,      38,     RECHARGE_PREMIUM,   PlantSubClass::SUBCLASS_SHOOTER,    FIRE_RATE_NORMAL,   _S("ICE_QUEEN_PEA"),        PlantSoundType::PLANT_SOUND_PEASHOOTER,     DeathSoundType::DEATH_SOUND_PEASHOOTER, SeedChooserPage::PAGE_ADVENTURES},
    { SeedType::SEED_SHAMROCKSTAR,      nullptr, ReanimationType::REANIM_SCRAREYSHROOM,     13, HEALTH_STRONG,      10,     RECHARGE_PREMIUM,   PlantSubClass::SUBCLASS_SHOOTER,    FIRE_RATE_SLOW,     _S("SHAMROCKSTAR"),         PlantSoundType::PLANT_SOUND_NONE,           DeathSoundType::DEATH_SOUND_NONE,       SeedChooserPage::PAGE_ADVENTURES},
    { SeedType::SEED_BAMBOOM,           nullptr, ReanimationType::REANIM_CABBAGEPULT,       13, HEALTH_NORMAL,      40,     RECHARGE_PREMIUM,   PlantSubClass::SUBCLASS_SHOOTER,    FIRE_RATE_SLOW,     _S("BAMBOOM"),              PlantSoundType::PLANT_SOUND_NONE,           DeathSoundType::DEATH_SOUND_NONE,       SeedChooserPage::PAGE_ADVENTURES},
    { SeedType::SEED_BLOOMERANG,        nullptr, ReanimationType::REANIM_CACTUS,            0,  HEALTH_NORMAL,      14,     RECHARGE_FAST,      PlantSubClass::SUBCLASS_SHOOTER,    FIRE_RATE_SLOW,     _S("BLOOMERANG"),           PlantSoundType::PLANT_SOUND_NONE,           DeathSoundType::DEATH_SOUND_NONE,       SeedChooserPage::PAGE_SEQUAL    },
    { SeedType::SEED_ICEBERGLETTUCE,    nullptr, ReanimationType::REANIM_POTATOMINE,        37, HEALTH_NORMAL,      0,      RECHARGE_SLOW,      PlantSubClass::SUBCLASS_NORMAL,     0,                  _S("ICEBERG_LETTUCE"),      PlantSoundType::PLANT_SOUND_NONE,           DeathSoundType::DEATH_SOUND_NONE,       SeedChooserPage::PAGE_SEQUAL    },
    { SeedType::SEED_HOTPOTATO,         nullptr, ReanimationType::REANIM_COFFEEBEAN,        33, HEALTH_NORMAL,      0,      RECHARGE_FAST,      PlantSubClass::SUBCLASS_NORMAL,     0,                  _S("HOT_POTATO"),           PlantSoundType::PLANT_SOUND_NONE,           DeathSoundType::DEATH_SOUND_NONE,       SeedChooserPage::PAGE_SEQUAL    },
    { SeedType::SEED_PEPPERPULT,        nullptr, ReanimationType::REANIM_PEPPERPULT,        40, HEALTH_NORMAL,      14,     RECHARGE_FAST,      PlantSubClass::SUBCLASS_SHOOTER,    FIRE_RATE_SLOW,     _S("PEPPER_PULT"),          PlantSoundType::PLANT_SOUND_NONE,           DeathSoundType::DEATH_SOUND_NONE,       SeedChooserPage::PAGE_SEQUAL    },
    { SeedType::SEED_INFINUT,           nullptr, ReanimationType::REANIM_INFINUT,           2,  HEALTH_SEMI_STRONG, 6,      RECHARGE_SLOW,      PlantSubClass::SUBCLASS_NORMAL,     0,                  _S("INFI_NUT"),             PlantSoundType::PLANT_SOUND_NONE,           DeathSoundType::DEATH_SOUND_NONE,       SeedChooserPage::PAGE_SEQUAL    },
    { SeedType::SEED_SPORESHROOM,       nullptr, ReanimationType::REANIM_SPORESHROOM,       6,  HEALTH_NORMAL,      10,     RECHARGE_FAST,      PlantSubClass::SUBCLASS_SHOOTER,    FIRE_RATE_SLOW,     _S("SPORE_SHROOM"),         PlantSoundType::PLANT_SOUND_NONE,           DeathSoundType::DEATH_SOUND_NONE,       SeedChooserPage::PAGE_SEQUAL    },
    { SeedType::SEED_PRIMALSUNFLOWER,   nullptr, ReanimationType::REANIM_SUNFLOWER,         1,  HEALTH_NORMAL,      10,     RECHARGE_FAST,      PlantSubClass::SUBCLASS_NORMAL,     SUN_RATE_NORMAL,    _S("PRIMAL_SUNFLOWER"),     PlantSoundType::PLANT_SOUND_SUNFLOWER,      DeathSoundType::DEATH_SOUND_SUNFLOWER,  SeedChooserPage::PAGE_SEQUAL    },
    { SeedType::SEED_PRIMALPEASHOOTER,  nullptr, ReanimationType::REANIM_PEASHOOTER,        0,  HEALTH_NORMAL,      14,     RECHARGE_FAST,      PlantSubClass::SUBCLASS_SHOOTER,    FIRE_RATE_SLOW,     _S("PRIMAL_PEASHOOTER"),    PlantSoundType::PLANT_SOUND_PEASHOOTER,     DeathSoundType::DEATH_SOUND_PEASHOOTER, SeedChooserPage::PAGE_SEQUAL    },
    { SeedType::SEED_GOLDBLOOM,         nullptr, ReanimationType::REANIM_BLOVER,            18, HEALTH_NORMAL,      4,      RECHARGE_VERY_SLOW, PlantSubClass::SUBCLASS_NORMAL,     0,                  _S("GOLDBLOOM"),            PlantSoundType::PLANT_SOUND_NONE,           DeathSoundType::DEATH_SOUND_NONE,       SeedChooserPage::PAGE_SEQUAL    },
    { SeedType::SEED_GOOPEASHOOTER,     nullptr, ReanimationType::REANIM_PEASHOOTER,        0,  HEALTH_NORMAL,      10,     RECHARGE_FAST,      PlantSubClass::SUBCLASS_SHOOTER,    FIRE_RATE_NORMAL,   _S("GOO_PEASHOOTER"),       PlantSoundType::PLANT_SOUND_PEASHOOTER,     DeathSoundType::DEATH_SOUND_PEASHOOTER, SeedChooserPage::PAGE_SEQUAL    },
    { SeedType::SEED_SEASTARFRUIT,      nullptr, ReanimationType::REANIM_CHERRYBOMB,        3,  HEALTH_NORMAL,      12,     RECHARGE_VERY_SLOW, PlantSubClass::SUBCLASS_NORMAL,     0,                  _S("SEA_STARFRUIT"),        PlantSoundType::PLANT_SOUND_NONE,           DeathSoundType::DEATH_SOUND_NONE,       SeedChooserPage::PAGE_CHINA     },
    { SeedType::SEED_PICKLEDPEPPER,     nullptr, ReanimationType::REANIM_PICKLEDPEPPER,     27, HEALTH_NORMAL,      20,     RECHARGE_PREMIUM,   PlantSubClass::SUBCLASS_NORMAL,     0,                  _S("PICKLED_PEPPER"),       PlantSoundType::PLANT_SOUND_NONE,           DeathSoundType::DEATH_SOUND_NONE,       SeedChooserPage::PAGE_CHINA     },
    { SeedType::SEED_ICYFUMESHROOM,     nullptr, ReanimationType::REANIM_ICYFUMESHROOM,     27, HEALTH_NORMAL,      24,     RECHARGE_PREMIUM,   PlantSubClass::SUBCLASS_SHOOTER,    FIRE_RATE_NORMAL,   _S("ICY_FUME_SHROOM"),      PlantSoundType::PLANT_SOUND_NONE,           DeathSoundType::DEATH_SOUND_NONE,       SeedChooserPage::PAGE_CHINA     },
    { SeedType::SEED_FIRESHROOM,        nullptr, ReanimationType::REANIM_WALLNUT,           27, HEALTH_MEDIUM,      6,      RECHARGE_PREMIUM,   PlantSubClass::SUBCLASS_NORMAL,     FIRE_RATE_FAST / 4, _S("FIRE_SHROOM"),          PlantSoundType::PLANT_SOUND_NONE,           DeathSoundType::DEATH_SOUND_NONE,       SeedChooserPage::PAGE_CHINA     },
    { SeedType::SEED_FLAMEWOOD,         nullptr, ReanimationType::REANIM_FLAMEWOOD,         27, HEALTH_NORMAL,      24,     RECHARGE_PREMIUM,   PlantSubClass::SUBCLASS_NORMAL,     0,                  _S("FLAMEWOOD"),            PlantSoundType::PLANT_SOUND_NONE,           DeathSoundType::DEATH_SOUND_NONE,       SeedChooserPage::PAGE_CHINA     },
    { SeedType::SEED_VINENUT,           nullptr, ReanimationType::REANIM_TALLNUT,           27, HEALTH_VERY_STRONG, 22,     RECHARGE_PREMIUM,   PlantSubClass::SUBCLASS_NORMAL,     FIRE_RATE_FAST / 4, _S("VINE_NUT"),             PlantSoundType::PLANT_SOUND_NONE,           DeathSoundType::DEATH_SOUND_NONE,       SeedChooserPage::PAGE_CHINA     },
    { SeedType::SEED_SUPERCHOMPER,      nullptr, ReanimationType::REANIM_SUPERCHOMPER,      27, HEALTH_NORMAL,      12,     RECHARGE_PREMIUM,   PlantSubClass::SUBCLASS_NORMAL,     0,                  _S("SUPER_CHOMPER"),        PlantSoundType::PLANT_SOUND_NONE,           DeathSoundType::DEATH_SOUND_NONE,       SeedChooserPage::PAGE_CHINA     },
    { SeedType::SEED_FUTURESTAR,        nullptr, ReanimationType::REANIM_STARFRUIT,         27, HEALTH_NORMAL,      22,     RECHARGE_PREMIUM,   PlantSubClass::SUBCLASS_SHOOTER,    FIRE_RATE_SLOW,     _S("FUTURESTAR"),           PlantSoundType::PLANT_SOUND_NONE,           DeathSoundType::DEATH_SOUND_NONE,       SeedChooserPage::PAGE_CHINA     },
    { SeedType::SEED_OAKARCHER,         nullptr, ReanimationType::REANIM_CACTUS,            15, HEALTH_NORMAL,      34,     RECHARGE_PREMIUM,   PlantSubClass::SUBCLASS_SHOOTER,    FIRE_RATE_OAK,      _S("OAK_ARCHER"),           PlantSoundType::PLANT_SOUND_NONE,           DeathSoundType::DEATH_SOUND_NONE,       SeedChooserPage::PAGE_CHINA     },
    { SeedType::SEED_LASERBEAN,         nullptr, ReanimationType::REANIM_FUMESHROOM,        9,  HEALTH_NORMAL,      28,     RECHARGE_PREMIUM,   PlantSubClass::SUBCLASS_SHOOTER,    FIRE_RATE_SLOW,     _S("LASER_BEAN"),           PlantSoundType::PLANT_SOUND_NONE,           DeathSoundType::DEATH_SOUND_NONE,       SeedChooserPage::PAGE_CHINA     },
    { SeedType::SEED_BONKCHOY,          nullptr, ReanimationType::REANIM_CHOMPER,           31, HEALTH_NORMAL,      24,     RECHARGE_PREMIUM,   PlantSubClass::SUBCLASS_NORMAL,     FIRE_RATE_BONK,     _S("BONK_CHOY"),            PlantSoundType::PLANT_SOUND_NONE,           DeathSoundType::DEATH_SOUND_NONE,       SeedChooserPage::PAGE_CHINA     },
    { SeedType::SEED_STUFFYSUNFLOWER,   nullptr, ReanimationType::REANIM_SUNFLOWER,         1,  HEALTH_NORMAL,      20,     RECHARGE_PREMIUM,   PlantSubClass::SUBCLASS_SHOOTER,    SUN_RATE_FAST,      _S("STUFFY_SUNFLOWER"),     PlantSoundType::PLANT_SOUND_SUNFLOWER,      DeathSoundType::DEATH_SOUND_SUNFLOWER,  SeedChooserPage::PAGE_DEBUG     },
    { SeedType::SEED_SHADOWFLOWER,      nullptr, ReanimationType::REANIM_SUNFLOWER,         1,  HEALTH_NORMAL,      16,     RECHARGE_PREMIUM,   PlantSubClass::SUBCLASS_NORMAL,     SUN_RATE_NORMAL,    _S("SHADOW_FLOWER"),        PlantSoundType::PLANT_SOUND_SUNFLOWER,      DeathSoundType::DEATH_SOUND_SUNFLOWER,  SeedChooserPage::PAGE_DEBUG     },
    { SeedType::SEED_FIREFLOWER,        nullptr, ReanimationType::REANIM_SUNFLOWER,         1,  HEALTH_NORMAL,      18,     RECHARGE_FAST,      PlantSubClass::SUBCLASS_SHOOTER,    FIRE_RATE_NORMAL,   _S("FIRE_FLOWER"),          PlantSoundType::PLANT_SOUND_SUNFLOWER,      DeathSoundType::DEATH_SOUND_SUNFLOWER,  SeedChooserPage::PAGE_DEBUG     },
    { SeedType::SEED_PLASMAPEA,         nullptr, ReanimationType::REANIM_SNOWPEA,           5,  HEALTH_NORMAL,      36,     RECHARGE_PREMIUM,   PlantSubClass::SUBCLASS_SHOOTER,    FIRE_RATE_NORMAL,   _S("PLASMA_PEA"),           PlantSoundType::PLANT_SOUND_PEASHOOTER,     DeathSoundType::DEATH_SOUND_PEASHOOTER, SeedChooserPage::PAGE_DEBUG     },
    { SeedType::SEED_SPOOKYSQUASH,      nullptr, ReanimationType::REANIM_SQUASH,            21, HEALTH_NORMAL,      16,     RECHARGE_PREMIUM,   PlantSubClass::SUBCLASS_NORMAL,     0,                  _S("SPOOKY_SQUASH"),        PlantSoundType::PLANT_SOUND_NONE,           DeathSoundType::DEATH_SOUND_NONE,       SeedChooserPage::PAGE_DEBUG     },
    { SeedType::SEED_EXPLODE_O_NUT,     nullptr, ReanimationType::REANIM_EXPLODEONUT,       2,  HEALTH_SEMI_STRONG, 6,      RECHARGE_VERY_SLOW, PlantSubClass::SUBCLASS_NORMAL,     0,                  _S("EXPLODE_O_NUT"),        PlantSoundType::PLANT_SOUND_NONE,           DeathSoundType::DEATH_SOUND_NONE,       SeedChooserPage::PAGE_DEBUG     },
    { SeedType::SEED_GIANT_WALLNUT,     nullptr, ReanimationType::REANIM_WALLNUT,           2,  HEALTH_VERY_STRONG, 8,      RECHARGE_SLOW,      PlantSubClass::SUBCLASS_NORMAL,     0,                  _S("GIANT_WALLNUT"),        PlantSoundType::PLANT_SOUND_NONE,           DeathSoundType::DEATH_SOUND_NONE,       SeedChooserPage::PAGE_DEBUG     },
    { SeedType::SEED_BOWLNUT,           nullptr, ReanimationType::REANIM_WALLNUT,           2,  HEALTH_NORMAL,      4,      RECHARGE_SLOW,      PlantSubClass::SUBCLASS_NORMAL,     0,                  _S("BOWL_NUT"),             PlantSoundType::PLANT_SOUND_NONE,           DeathSoundType::DEATH_SOUND_NONE,       SeedChooserPage::PAGE_DEBUG     },
    { SeedType::SEED_HAZYNUT,           nullptr, ReanimationType::REANIM_WALLNUT,           2,  HEALTH_NORMAL,      4,      RECHARGE_SLOW,      PlantSubClass::SUBCLASS_NORMAL,     0,                  _S("HAZY_NUT"),             PlantSoundType::PLANT_SOUND_NONE,           DeathSoundType::DEATH_SOUND_NONE,       SeedChooserPage::PAGE_DEBUG     },
    { SeedType::SEED_FROSTACHIO,        nullptr, ReanimationType::REANIM_WALLNUT,           2,  HEALTH_NORMAL,      4,      RECHARGE_SLOW,      PlantSubClass::SUBCLASS_NORMAL,     0,                  _S("FROSTACHIO"),           PlantSoundType::PLANT_SOUND_NONE,           DeathSoundType::DEATH_SOUND_NONE,       SeedChooserPage::PAGE_DEBUG     },
    { SeedType::SEED_SPROUT,            nullptr, ReanimationType::REANIM_ZENGARDEN_SPROUT,  33, HEALTH_NORMAL,      0,      RECHARGE_SLOW,      PlantSubClass::SUBCLASS_NORMAL,     0,                  _S("SPROUT"),               PlantSoundType::PLANT_SOUND_NONE,           DeathSoundType::DEATH_SOUND_NONE,       SeedChooserPage::PAGE_DEBUG     },
    { SeedType::SEED_LEFTPEATER,        nullptr, ReanimationType::REANIM_REPEATER,          5,  HEALTH_NORMAL,      16,     RECHARGE_FAST,      PlantSubClass::SUBCLASS_SHOOTER,    FIRE_RATE_NORMAL,   _S("REPEATER"),             PlantSoundType::PLANT_SOUND_PEASHOOTER,     DeathSoundType::DEATH_SOUND_PEASHOOTER, SeedChooserPage::PAGE_DEBUG     },
    { SeedType::SEED_PEANIS,            nullptr, ReanimationType::REANIM_PEASHOOTER,        0,  HEALTH_NORMAL,      0,      RECHARGE_NONE,      PlantSubClass::SUBCLASS_SHOOTER,    FIRE_RATE_NORMAL,   _S("PEANIS"),               PlantSoundType::PLANT_SOUND_PEASHOOTER,     DeathSoundType::DEATH_SOUND_PEASHOOTER, SeedChooserPage::PAGE_DEBUG     },
};

//0x401B20
Plant::Plant()
{
}

//0x45DB60
void Plant::PlantInitialize(int theGridX, int theGridY, SeedType theSeedType, SeedType theImitaterType)
{
    mPlantCol = theGridX;
    mRow = theGridY;
    if (mBoard)
    {
        mX = mBoard->GridToPixelX(theGridX, theGridY);
        mY = mBoard->GridToPixelY(theGridX, theGridY);
    }
    mAnimCounter = 0;
    mAnimPing = true;
    mFrame = 0;
    mShootingCounter = 0;
    mShakeOffsetX = 0.0f;
    mShakeOffsetY = 0.0f;
    mFrameLength = RandRangeInt(12, 18);
    mTargetX = -1;
    mTargetY = -1;
    mStartRow = mRow;
    mNumFrames = 5;
    mState = PlantState::STATE_NOTREADY;
    mDead = false;
    mSquished = false;
    mSeedType = theSeedType;
    mImitaterType = theImitaterType;
    mDoSpecialCountdown = 0;
    mDisappearCountdown = 200;
    mStateCountdown = 0;
    mParticleID = ParticleSystemID::PARTICLESYSTEMID_NULL;
    mBodyReanimID = ReanimationID::REANIMATIONID_NULL;
    mHeadReanimID = ReanimationID::REANIMATIONID_NULL;
    mHeadReanimID2 = ReanimationID::REANIMATIONID_NULL;
    mHeadReanimID3 = ReanimationID::REANIMATIONID_NULL;
    mBlinkReanimID = ReanimationID::REANIMATIONID_NULL;
    mLightReanimID = ReanimationID::REANIMATIONID_NULL;
    mSleepingReanimID = ReanimationID::REANIMATIONID_NULL;
    mBlinkCountdown = 0;
    mRecentlyEatenCountdown = 0;
    mEatenFlashCountdown = 0;
    mBeghouledFlashCountdown = 0;
    mWidth = 80;
    mHeight = 80;
    memset(mMagnetItems, 0, sizeof(mMagnetItems));
    const PlantDefinition& aPlantDef = GetPlantDefinition(theSeedType);
    mIsAsleep = false;
    mWakeUpCounter = 0;
    mOnBungeeState = PlantOnBungeeState::NOT_ON_BUNGEE;
    mPottedPlantIndex = -1;
    mLaunchRate = aPlantDef.mLaunchRate;
    mSubclass = aPlantDef.mSubClass;
    mRenderOrder = CalcRenderOrder();
    mWarmCounter = 2000;

    // Health
    mPlantMaxHealth = aPlantDef.mPlantMaxHealth;

    // Boost
    mBoostCounter = 0;
    mIsBoosted = false;
    // Sounds
    mDeathSound = aPlantDef.mDeathSound;
    // Repeater Fix
    mStoredLaunchRate = mLaunchRate;
    // Zombotany
    mChilledCounter = 0;
    mPoisonCounter = 0;
    mPoisonDamage = BASE_POISON_DAMAGE;
    mPoisonRate = BASE_POISON_TICKS;
    mPlantAge = 0;
    // Fake Plant Food
    mPlantFoodReady = false;

    Reanimation* aBodyReanim = nullptr;
    if (aPlantDef.mReanimationType != ReanimationType::REANIM_NONE)
    {
        float aOffsetY = PlantDrawHeightOffset(mBoard, this, mSeedType, mPlantCol, mRow);
        aBodyReanim = mApp->AddReanimation(0.0f, aOffsetY, mRenderOrder + 1, aPlantDef.mReanimationType);
        aBodyReanim->mLoopType = ReanimLoopType::REANIM_LOOP;
        aBodyReanim->mAnimRate = RandRangeFloat(10.0f, 15.0f);
        if (aBodyReanim->TrackExists("anim_idle"))
            aBodyReanim->SetFramesForLayer("anim_idle");

        if (mApp->IsWallnutBowlingLevel() && aBodyReanim->TrackExists("_ground"))
        {
            aBodyReanim->SetFramesForLayer("_ground");
            if (mSeedType == SeedType::SEED_GIANT_WALLNUT)
                aBodyReanim->mAnimRate = RandRangeFloat(6.0f, 10.0f);
            else if (IsWallNut(mSeedType))
                aBodyReanim->mAnimRate = RandRangeFloat(12.0f, 18.0f);
        }
        // Bowling Nut
        if (mSeedType == SeedType::SEED_BOWLNUT && aBodyReanim->TrackExists("_ground"))
        {
            aBodyReanim->SetFramesForLayer("_ground");
            aBodyReanim->mAnimRate = RandRangeFloat(12.0f, 18.0f);
        }

        aBodyReanim->mIsAttachment = true;
        mBodyReanimID = mApp->ReanimationGetID(aBodyReanim);
        mBlinkCountdown = 400 + Sexy::Rand(400);
    }

    if (IsNocturnal(mSeedType) && mBoard && !mBoard->StageIsNight() && !mBoard->GetCoffeePotAt(mPlantCol, mRow))
        SetSleeping(true);

    if (mLaunchRate > 0)
    {
        if (MakesSun(mSeedType) || mSeedType == SeedType::SEED_SHADOWFLOWER || mSeedType == SeedType::SEED_MARIGOLD)
            mLaunchCounter = RandRangeInt(300, mLaunchRate / 2);
        else
            mLaunchCounter = RandRangeInt(0, mLaunchRate);
    }
    else
        mLaunchCounter = 0;

    switch (theSeedType)
    {
    case SeedType::SEED_BLOVER:
    {
        mDoSpecialCountdown = 50;

        if (IsInPlay())
        {
            aBodyReanim->SetFramesForLayer("anim_blow");
            aBodyReanim->mLoopType = ReanimLoopType::REANIM_PLAY_ONCE_AND_HOLD;
            aBodyReanim->mAnimRate = 20.0f;
        }
        else
        {
            aBodyReanim->SetFramesForLayer("anim_idle");
            aBodyReanim->mAnimRate = 10.0f;
        }

        break;
    }
    case SeedType::SEED_SPLITPEA:
    {
        TOD_ASSERT(aBodyReanim);

        aBodyReanim->mAnimRate = RandRangeFloat(15.0f, 20.0f);
        Reanimation* aHeadReanim1 = mApp->AddReanimation(0.0f, 0.0f, mRenderOrder + 2, aPlantDef.mReanimationType);
        aHeadReanim1->mLoopType = ReanimLoopType::REANIM_LOOP;
        aHeadReanim1->mAnimRate = aBodyReanim->mAnimRate;
        aHeadReanim1->SetFramesForLayer("anim_head_idle");
        aHeadReanim1->AttachToAnotherReanimation(aBodyReanim, "anim_idle");
        mHeadReanimID = mApp->ReanimationGetID(aHeadReanim1);

        Reanimation* aHeadReanim2 = mApp->AddReanimation(0.0f, 0.0f, mRenderOrder + 2, aPlantDef.mReanimationType);
        aHeadReanim2->mLoopType = ReanimLoopType::REANIM_LOOP;
        aHeadReanim2->mAnimRate = aBodyReanim->mAnimRate;
        aHeadReanim2->SetFramesForLayer("anim_splitpea_idle");
        aHeadReanim2->AttachToAnotherReanimation(aBodyReanim, "anim_idle");
        mHeadReanimID2 = mApp->ReanimationGetID(aHeadReanim2);

        break;
    }
    case SeedType::SEED_THREEPEATER:
    {
        TOD_ASSERT(aBodyReanim);

        aBodyReanim->mAnimRate = RandRangeFloat(15.0f, 20.0f);
        Reanimation* aHeadReanim1 = mApp->AddReanimation(0.0f, 0.0f, mRenderOrder + 2, aPlantDef.mReanimationType);
        aHeadReanim1->mLoopType = ReanimLoopType::REANIM_LOOP;
        aHeadReanim1->mAnimRate = aBodyReanim->mAnimRate;
        aHeadReanim1->SetFramesForLayer("anim_head_idle1");
        aHeadReanim1->AttachToAnotherReanimation(aBodyReanim, "anim_head1");
        mHeadReanimID = mApp->ReanimationGetID(aHeadReanim1);

        Reanimation* aHeadReanim2 = mApp->AddReanimation(0.0f, 0.0f, mRenderOrder + 2, aPlantDef.mReanimationType);
        aHeadReanim2->mLoopType = ReanimLoopType::REANIM_LOOP;
        aHeadReanim2->mAnimRate = aBodyReanim->mAnimRate;
        aHeadReanim2->SetFramesForLayer("anim_head_idle2");
        aHeadReanim2->AttachToAnotherReanimation(aBodyReanim, "anim_head2");
        mHeadReanimID2 = mApp->ReanimationGetID(aHeadReanim2);

        Reanimation* aHeadReanim3 = mApp->AddReanimation(0.0f, 0.0f, mRenderOrder + 2, aPlantDef.mReanimationType);
        aHeadReanim3->mLoopType = ReanimLoopType::REANIM_LOOP;
        aHeadReanim3->mAnimRate = aBodyReanim->mAnimRate;
        aHeadReanim3->SetFramesForLayer("anim_head_idle3");
        aHeadReanim3->AttachToAnotherReanimation(aBodyReanim, "anim_head3");
        mHeadReanimID3 = mApp->ReanimationGetID(aHeadReanim3);

        break;
    }
    case SeedType::SEED_WALLNUT:
    case SeedType::SEED_EXPLODE_O_NUT:
    case SeedType::SEED_BOWLNUT:
    case SeedType::SEED_HARDNUT:
        mBlinkCountdown = 1000 + Sexy::Rand(1000);
        break;
    case SeedType::SEED_INFINUT:
        mState = PlantState::STATE_READY;
        mBlinkCountdown = 1000 + Sexy::Rand(1000);
        aBodyReanim->mColorOverride = Color(255, 64, 64);
        break;
    case SeedType::SEED_HAZYNUT:
        mBlinkCountdown = 1000 + Sexy::Rand(1000);
        aBodyReanim->mColorOverride = Color(255, 64, 255);
        break;
    case SeedType::SEED_FROSTACHIO:
        mBlinkCountdown = 1000 + Sexy::Rand(1000);
        aBodyReanim->mColorOverride = Color(64, 255, 255);
        break;
    case SeedType::SEED_GIANT_WALLNUT:
        mBlinkCountdown = 1000 + Sexy::Rand(1000);
        break;
    case SeedType::SEED_VINENUT:
    case SeedType::SEED_TALLNUT:
        mHeight = 80;
        mBlinkCountdown = 1000 + Sexy::Rand(1000);
        break;
    case SeedType::SEED_GARLIC:
        TOD_ASSERT(aBodyReanim);
        aBodyReanim->SetTruncateDisappearingFrames();
        break;
    case SeedType::SEED_GOLD_MAGNET:
        TOD_ASSERT(aBodyReanim);
        aBodyReanim->SetTruncateDisappearingFrames();
        break;
    case SeedType::SEED_IMITATER:
        TOD_ASSERT(aBodyReanim);
        aBodyReanim->mAnimRate = RandRangeFloat(25.0f, 30.0f);
        mStateCountdown = 200;
        break;
    case SeedType::SEED_CHERRYBOMB:
    case SeedType::SEED_JALAPENO:
    case SeedType::SEED_PICKLEDPEPPER:
    case SeedType::SEED_CHILLYPEPPER:
    case SeedType::SEED_SEASTARFRUIT:
    {
        TOD_ASSERT(aBodyReanim);

        if (IsInPlay())
        {
            mDoSpecialCountdown = 100;

            aBodyReanim->SetFramesForLayer("anim_explode");
            aBodyReanim->mLoopType = ReanimLoopType::REANIM_PLAY_ONCE_AND_HOLD;

            mApp->PlayFoley(FoleyType::FOLEY_REVERSE_EXPLOSION);
        }

        break;
    }
    case SeedType::SEED_POPCORN:
    case SeedType::SEED_POTATOMINE:
    {
        TOD_ASSERT(aBodyReanim);

        aBodyReanim->mAnimRate = 12.0f;

        if (IsInPlay())
        {
            aBodyReanim->AssignRenderGroupToTrack("anim_glow", RENDER_GROUP_HIDDEN);
            mStateCountdown = 1500;
        }
        else
        {
            aBodyReanim->SetFramesForLayer("anim_armed");
            mState = PlantState::STATE_POTATO_ARMED;
        }

        break;
    }
    case SeedType::SEED_GOLDBLOOM:
    {
        TOD_ASSERT(aBodyReanim);

        aBodyReanim->SetFramesForLayer("anim_idle");
        aBodyReanim->mAnimRate = 10.0f;

        if (IsInPlay())
        {
            mStateCountdown = 2500;
        }

        break;
    }
    case SeedType::SEED_GRAVEBUSTER:
    {
        TOD_ASSERT(aBodyReanim);

        if (IsInPlay())
        {
            aBodyReanim->SetFramesForLayer("anim_land");
            aBodyReanim->mLoopType = ReanimLoopType::REANIM_PLAY_ONCE_AND_HOLD;

            mState = PlantState::STATE_GRAVEBUSTER_LANDING;
            mApp->PlayFoley(FoleyType::FOLEY_GRAVEBUSTERCHOMP);
        }

        break;
    }
    case SeedType::SEED_SUNSHROOM:
    {
        TOD_ASSERT(aBodyReanim);

        aBodyReanim->mFrameBasePose = 6;

        if (IsInPlay())
        {
            mX += Sexy::Rand(10) - 5;
            mY += Sexy::Rand(10) - 5;
        }
        else if (mIsAsleep)
            aBodyReanim->SetFramesForLayer("anim_bigsleep");
        else
            aBodyReanim->SetFramesForLayer("anim_bigidle");

        mState = PlantState::STATE_SUNSHROOM_SMALL;
        mStateCountdown = 12000;

        break;
    }
    case SeedType::SEED_PUFFSHROOM:
    case SeedType::SEED_MOLDSHROOM:
    case SeedType::SEED_SEASHROOM:
        if (IsInPlay())
        {
            mX += Sexy::Rand(10) - 5;
            mY += Sexy::Rand(6) - 3;
        }
        break;
    case SeedType::SEED_PUMPKINSHELL:
    {
        mWidth = 120;

        TOD_ASSERT(aBodyReanim);
        aBodyReanim->AssignRenderGroupToTrack("Pumpkin_back", 1);
        break;
    }
    case SeedType::SEED_SUPERCHOMPER:
    case SeedType::SEED_CHOMPER:
        mState = PlantState::STATE_READY;
        break;
    case SeedType::SEED_PLANTERN:
    {
        mStateCountdown = 50;

        if (!IsOnBoard() || mApp->mGameMode != GameMode::GAMEMODE_CHALLENGE_ZEN_GARDEN)
        {
            AddAttachedParticle(mX + 40, mY + 40, (int)RenderLayer::RENDER_LAYER_FOG + 1, ParticleEffect::PARTICLE_LANTERN_SHINE);
        }
        if (IsInPlay())
        {
            mApp->PlaySample(Sexy::SOUND_PLANTERN);
        }
        
        break;
    }
    case SeedType::SEED_TORCHWOOD:
    case SeedType::SEED_FLAMEWOOD:
        break;
    case SeedType::SEED_MARIGOLD:
        TOD_ASSERT(aBodyReanim);
        aBodyReanim->mAnimRate = RandRangeFloat(15.0f, 20.0f);
        break;
    case SeedType::SEED_CACTUS:
        mState = PlantState::STATE_CACTUS_LOW;
        break;
    case SeedType::SEED_OAKARCHER:
    case SeedType::SEED_BONKCHOY:
    case SeedType::SEED_LASERBEAN:
        if (IsInPlay()) {
            mStateCountdown = 4000;
        }
        break;
    case SeedType::SEED_INSTANT_COFFEE:
    case SeedType::SEED_HOTPOTATO:
        mDoSpecialCountdown = 100;
        break;
    case SeedType::SEED_SCAREDYSHROOM:
    case SeedType::SEED_SHAMROCK:
    case SeedType::SEED_SHAMROCKSTAR:
        mState = PlantState::STATE_READY;
        break;
    case SeedType::SEED_COBCANNON:
        if (IsInPlay())
        {
            mState = PlantState::STATE_COBCANNON_ARMING;
            mStateCountdown = 500;

            TOD_ASSERT(aBodyReanim);
            aBodyReanim->SetFramesForLayer("anim_unarmed_idle");
        }
        break;
    case SeedType::SEED_KERNELPULT:
        TOD_ASSERT(aBodyReanim);
        aBodyReanim->AssignRenderGroupToPrefix("Cornpult_butter", RENDER_GROUP_HIDDEN);
        break;
    case SeedType::SEED_MAGNETSHROOM:
        TOD_ASSERT(aBodyReanim);
        aBodyReanim->SetTruncateDisappearingFrames();
        break;
    case SeedType::SEED_SPIKEROCK:
        TOD_ASSERT(aBodyReanim);
        break;
    case SeedType::SEED_SPROUT:
        break;
    case SeedType::SEED_FLOWERPOT:
    case SeedType::SEED_WATERPOT:
    case SeedType::SEED_COFFEEPOT:
    case SeedType::SEED_LILYPAD:
        if (IsInPlay())
        {
            mState = PlantState::STATE_INVULNERABLE;
            mStateCountdown = 100;
        }
        break;
    case SeedType::SEED_TANGLEKELP:
        TOD_ASSERT(aBodyReanim);
        aBodyReanim->SetTruncateDisappearingFrames();
    }

    if (IsPeashooter(theSeedType) || theSeedType == SeedType::SEED_GATLINGPEA || theSeedType == SeedType::SEED_LEFTPEATER)
    {
        if (aBodyReanim)
        {
            aBodyReanim->mAnimRate = RandRangeFloat(15.0f, 20.0f);
            Reanimation* aHeadReanim = mApp->AddReanimation(0.0f, 0.0f, mRenderOrder + 2, aPlantDef.mReanimationType);
            aHeadReanim->mLoopType = ReanimLoopType::REANIM_LOOP;
            aHeadReanim->mAnimRate = aBodyReanim->mAnimRate;
            aHeadReanim->SetFramesForLayer("anim_head_idle");
            mHeadReanimID = mApp->ReanimationGetID(aHeadReanim);

            if (aBodyReanim->TrackExists("anim_stem"))
                aHeadReanim->AttachToAnotherReanimation(aBodyReanim, "anim_stem");
            else if (aBodyReanim->TrackExists("anim_idle"))
                aHeadReanim->AttachToAnotherReanimation(aBodyReanim, "anim_idle");
        }
    }
    
    if ((mApp->mGameMode == GameMode::GAMEMODE_CHALLENGE_BIG_TIME) &&
        (theSeedType == SeedType::SEED_WALLNUT || theSeedType == SeedType::SEED_SUNFLOWER || theSeedType == SeedType::SEED_MARIGOLD))
    {
        mPlantMaxHealth *= 2;
    }

    mPlantHealth = mPlantMaxHealth;

    if (IsOnBoard())
    {
        TOD_ASSERT(mBoard);
        if (mSeedType != SeedType::SEED_FLOWERPOT)
        {
            Plant* aFlowerPot = mBoard->GetFlowerPotAt(mPlantCol, mRow);
            if (aFlowerPot)
                mApp->ReanimationGet(aFlowerPot->mBodyReanimID)->mAnimRate = 0.0f;
        }
        else if (mSeedType != SeedType::SEED_WATERPOT)
        {
            Plant* aWaterPot = mBoard->GetWaterPotAt(mPlantCol, mRow);
            if (aWaterPot)
                mApp->ReanimationGet(aWaterPot->mBodyReanimID)->mAnimRate = 0.0f;
        }
    }
}

//0x45E7C0
int Plant::CalcRenderOrder()
{
    PLANT_ORDER anOrder = PLANT_ORDER::PLANT_ORDER_NORMAL;
    RenderLayer aLayer = RenderLayer::RENDER_LAYER_PLANT;

    SeedType aSeedType = mSeedType;
    if (mSeedType == SeedType::SEED_IMITATER && mImitaterType != SeedType::SEED_NONE)
        aSeedType = mImitaterType;

    if ((mApp->IsWallnutBowlingLevel() && IsWallNut(aSeedType)) || aSeedType == SeedType::SEED_BOWLNUT)
    {
        aLayer = RenderLayer::RENDER_LAYER_PROJECTILE;
    }
    else if (aSeedType == SeedType::SEED_PUMPKINSHELL)
    {
        anOrder = PLANT_ORDER::PLANT_ORDER_PUMPKIN;
    }
    else if (IsFlying(aSeedType))
    {
        anOrder = PLANT_ORDER::PLANT_ORDER_FLYER;
    }
    else if (aSeedType == SeedType::SEED_FLOWERPOT || aSeedType == SeedType::SEED_WATERPOT || aSeedType == SeedType::SEED_COFFEEPOT || (aSeedType == SeedType::SEED_LILYPAD && mApp->mGameMode != GameMode::GAMEMODE_CHALLENGE_ZEN_GARDEN))
    {
        anOrder = PLANT_ORDER::PLANT_ORDER_LILYPAD;
    }

    return Board::MakeRenderOrder(aLayer, mRow, anOrder * 5 - mX + 800);
}

//0x45E860
void Plant::SetSleeping(bool theIsAsleep)
{
    if (mIsAsleep == theIsAsleep || NotOnGround())
        return;

    mIsAsleep = theIsAsleep;
    if (theIsAsleep)
    {
        float aPosX = mX + 50.0f;
        float aPosY = mY + 40.0f;
        if (mSeedType == SeedType::SEED_FUMESHROOM)
            aPosX += 12.0f;
        else if (mSeedType == SeedType::SEED_SCAREDYSHROOM || mSeedType == SeedType::SEED_SHAMROCK || mSeedType == SeedType::SEED_SHAMROCKSTAR)
            aPosY -= 20.0f;
        else if (mSeedType == SeedType::SEED_GLOOMSHROOM)
            aPosY -= 12.0f;

        Reanimation* aSleepReanim = mApp->AddReanimation(aPosX, aPosY, mRenderOrder + 2, ReanimationType::REANIM_SLEEPING);
        aSleepReanim->mLoopType = ReanimLoopType::REANIM_LOOP;
        aSleepReanim->mAnimRate = RandRangeFloat(6.0f, 8.0f);
        aSleepReanim->mAnimTime = RandRangeFloat(0.0f, 0.9f);
        mSleepingReanimID = mApp->ReanimationGetID(aSleepReanim);
    }
    else
    {
        mApp->RemoveReanimation(mSleepingReanimID);
        mSleepingReanimID = ReanimationID::REANIMATIONID_NULL;
    }

    Reanimation* aBodyReanim = mApp->ReanimationTryToGet(mBodyReanimID);
    if (aBodyReanim == nullptr)
        return;

    if (theIsAsleep)
    {
        if (!IsInPlay() && mSeedType == SeedType::SEED_SUNSHROOM)
        {
            aBodyReanim->SetFramesForLayer("anim_bigsleep");
        }
        else if (aBodyReanim->TrackExists("anim_sleep"))
        {
            float aAnimTime = aBodyReanim->mAnimTime;
            aBodyReanim->StartBlend(20);
            aBodyReanim->SetFramesForLayer("anim_sleep");
            aBodyReanim->mAnimTime = aAnimTime;
        }
        else
        {
            aBodyReanim->mAnimRate = 1.0f;
        }

        EndBlink();
    }
    else
    {
        if (!IsInPlay() && mSeedType == SeedType::SEED_SUNSHROOM)
        {
            aBodyReanim->SetFramesForLayer("anim_bigidle");
        }
        else if (aBodyReanim->TrackExists("anim_idle"))
        {
            float aAnimTime = aBodyReanim->mAnimTime;
            aBodyReanim->StartBlend(20);
            aBodyReanim->SetFramesForLayer("anim_idle");
            aBodyReanim->mAnimTime = aAnimTime;
        }

        if (aBodyReanim->mAnimRate < 2.0f && IsInPlay())
            aBodyReanim->mAnimRate = RandRangeFloat(10.0f, 15.0f);
    }
}

//0x45EB10
int Plant::GetDamageRangeFlags(PlantWeapon thePlantWeapon)
{
    // Convert to binary, look at the damage range flag list backwards, convert back.
    switch (mSeedType)
    {
    case SeedType::SEED_CACTUS:
        return thePlantWeapon == PlantWeapon::WEAPON_SECONDARY ? 1 : 2;
    case SeedType::SEED_OAKARCHER:
        return thePlantWeapon == PlantWeapon::WEAPON_SECONDARY ? 15 : 1;
    case SeedType::SEED_CHERRYBOMB:
    case SeedType::SEED_JALAPENO:
    case SeedType::SEED_CHILLYPEPPER:
    case SeedType::SEED_PICKLEDPEPPER:
    case SeedType::SEED_COBCANNON:
    case SeedType::SEED_DOOMSHROOM:
    case SeedType::SEED_POPCORN:
        return 127;
    case SeedType::SEED_ICEBERGLETTUCE:
        return 383;
    case SeedType::SEED_MELONPULT:
    case SeedType::SEED_CABBAGEPULT:
    case SeedType::SEED_PEPPERPULT:
    case SeedType::SEED_ICEBERGLETTUCEBETA:
    case SeedType::SEED_KERNELPULT:
    case SeedType::SEED_WINTERMELON:
    case SeedType::SEED_MOLDSHROOM:
    case SeedType::SEED_BAMBOOSHOOT:
    case SeedType::SEED_BAMBOOM:
    case SeedType::SEED_SPORESHROOM:
        return 13;
    case SeedType::SEED_POTATOMINE:
        return 333;
    case SeedType::SEED_SQUASH:
    case SeedType::SEED_SPOOKYSQUASH:
        return 13;
    case SeedType::SEED_PUFFSHROOM:
    case SeedType::SEED_SEASHROOM:
    case SeedType::SEED_FUMESHROOM:
    case SeedType::SEED_ICYFUMESHROOM:
    case SeedType::SEED_GLOOMSHROOM:
    case SeedType::SEED_SUPERCHOMPER:
    case SeedType::SEED_CHOMPER:
    case SeedType::SEED_BEET:
    case SeedType::SEED_BEETBOXER:
    case SeedType::SEED_BONKCHOY:
    case SeedType::SEED_SEASTARFRUIT:
        return 9;
    case SeedType::SEED_CATTAIL:
        return 11;
    case SeedType::SEED_TANGLEKELP:
        return 5;
    case SeedType::SEED_GIANT_WALLNUT:
        return 27;
    case SeedType::SEED_SPIKEWEED:
    case SeedType::SEED_SPIKEROCK:
        return 265;
    default:
        return 1;
    }
}

//0x45EBD0
bool Plant::IsOnHighGround()
{
    return mBoard && mBoard->mGridSquareType[mPlantCol][mRow] == GridSquareType::GRIDSQUARE_HIGH_GROUND;
}

//0x45EC00
void Plant::SpikeRockTakeDamage()
{
    Reanimation* aBodyReanim = mApp->ReanimationGet(mBodyReanimID);

    SpikeweedAttack();

    mPlantHealth -= 50;
    if (mPlantHealth <= 300)
    {
        aBodyReanim->AssignRenderGroupToTrack("bigspike3", RENDER_GROUP_HIDDEN);
    }
    if (mPlantHealth <= 150)
    {
        aBodyReanim->AssignRenderGroupToTrack("bigspike2", RENDER_GROUP_HIDDEN);
    }
    if (mPlantHealth <= 0)
    {
        mApp->PlayFoley(FoleyType::FOLEY_SQUISH);
        Die();
    }
}

bool Plant::IsSpiky()
{
    return mSeedType == SeedType::SEED_SPIKEWEED || mSeedType == SeedType::SEED_SPIKEROCK;
}

//0x45ED00
void Plant::DoRowAreaDamage(int theDamage, unsigned int theDamageFlags, PlantWeapon theWeapon)
{
    int aDamageRangeFlags = GetDamageRangeFlags(theWeapon);
    Rect aAttackRect = GetPlantAttackRect(theWeapon);

    Zombie* aZombie = nullptr;
    while (mBoard->IterateZombies(aZombie))
    {
        int aDiffY = (aZombie->mZombieType == ZombieType::ZOMBIE_BOSS) ? 0 : (aZombie->mRow - mRow);
        if (mSeedType == SeedType::SEED_GLOOMSHROOM || (mSeedType == SeedType::SEED_BONKCHOY && theWeapon == PlantWeapon::WEAPON_SECONDARY))
        {
            if (aDiffY < -1 || aDiffY > 1)
                continue;
        }
        else if (aDiffY)
            continue;

        if (aZombie->mOnHighGround == IsOnHighGround() && aZombie->EffectedByDamage(aDamageRangeFlags))
        {
            Rect aZombieRect = aZombie->GetZombieRect();
            int aExtraRange = 0;
            if ((mSeedType == SeedType::SEED_BEET || mSeedType == SeedType::SEED_BEETBOXER) && aZombie->mIsEating) aExtraRange = 60;
            if (GetRectOverlap(aAttackRect, aZombieRect) > -aExtraRange)
            {
                int aDamage = theDamage;
                if (Zombie::IsMachine(aZombie->mZombieType) &&
                    (TestBit(theDamageFlags, DamageFlags::DAMAGE_SPIKE)))
                {
                    aDamage = 1800;

                    if (mSeedType == SeedType::SEED_SPIKEROCK)
                    {
                        SpikeRockTakeDamage();
                    }
                    else
                    {
                        Die();
                    }
                }

                aZombie->TakeDamage(aDamage, theDamageFlags);
                mApp->PlayFoley(FoleyType::FOLEY_SPLAT);
            }
        }
    }
}

//0x45EEA0
TodParticleSystem* Plant::AddAttachedParticle(int thePosX, int thePosY, int theRenderPosition, ParticleEffect theEffect)
{
    TodParticleSystem* aParticle = mApp->ParticleTryToGet(mParticleID);
    if (aParticle)
        aParticle->ParticleSystemDie();

    TodParticleSystem* aNewParticle = mApp->AddTodParticle(thePosX, thePosY, theRenderPosition, theEffect);
    if (aNewParticle)
        mParticleID = mApp->ParticleGetID(aNewParticle);

    return aNewParticle;
}

//0x45EF10
bool Plant::FindTargetAndFire(int theRow, PlantWeapon thePlantWeapon)
{
    Zombie* aZombie = FindTargetZombie(theRow, thePlantWeapon);
    if (aZombie == nullptr)
        return false;

    EndBlink();
    Reanimation* aBodyReanim = mApp->ReanimationTryToGet(mBodyReanimID);
    Reanimation* aHeadReanim = mApp->ReanimationTryToGet(mHeadReanimID);

    if (mSeedType == SeedType::SEED_SPLITPEA && thePlantWeapon == PlantWeapon::WEAPON_SECONDARY)
    {
        Reanimation* aHeadReanim2 = mApp->ReanimationGet(mHeadReanimID2);
        aHeadReanim2->StartBlend(20);
        aHeadReanim2->mLoopType = ReanimLoopType::REANIM_PLAY_ONCE_AND_HOLD;
        aHeadReanim2->mAnimRate = 35.0f;
        aHeadReanim2->SetFramesForLayer("anim_splitpea_shooting");
        mShootingCounter = 26;
    }
    else if (aHeadReanim && aHeadReanim->TrackExists("anim_shooting"))
    {
        aHeadReanim->StartBlend(20);
        aHeadReanim->mLoopType = ReanimLoopType::REANIM_PLAY_ONCE_AND_HOLD;
        aHeadReanim->mAnimRate = 35.0f;
        aHeadReanim->SetFramesForLayer("anim_shooting");

        mShootingCounter = 33;
        if (mSeedType == SeedType::SEED_REPEATER || mSeedType == SeedType::SEED_SPLITPEA || mSeedType == SeedType::SEED_LEFTPEATER || mSeedType == SeedType::SEED_ICEQUEENPEA || mSeedType == SeedType::SEED_ACEPEARAGUS)
        {
            aHeadReanim->mAnimRate = 45.0f;
            mShootingCounter = 26;
        }
        else if (mSeedType == SeedType::SEED_GATLINGPEA)
        {
            aHeadReanim->mAnimRate = 38.0f;
            mShootingCounter = 100;
        }
    }
    else if (mState == PlantState::STATE_CACTUS_HIGH)
    {
        PlayBodyReanim("anim_shootinghigh", ReanimLoopType::REANIM_PLAY_ONCE_AND_HOLD, 20, 35.0f);
        mShootingCounter = 23;
    }
    else if (mSeedType == SeedType::SEED_GLOOMSHROOM)
    {
        PlayBodyReanim("anim_shooting", ReanimLoopType::REANIM_PLAY_ONCE_AND_HOLD, 20, 14.0f);
        mShootingCounter = 200;
    }
    else if (mSeedType == SeedType::SEED_CATTAIL)
    {
        PlayBodyReanim("anim_shooting", ReanimLoopType::REANIM_PLAY_ONCE_AND_HOLD, 20, 30.0f);
        mShootingCounter = 50;
    }
    else if (mSeedType == SeedType::SEED_FUTURESTAR)
    {
        PlayBodyReanim("anim_shoot", ReanimLoopType::REANIM_PLAY_ONCE_AND_HOLD, 20, 28.0f);
        mShootingCounter = 40;
    }
    else if (aBodyReanim && aBodyReanim->TrackExists("anim_shooting"))
    {
        PlayBodyReanim("anim_shooting", ReanimLoopType::REANIM_PLAY_ONCE_AND_HOLD, 20, 35.0f);

        switch (mSeedType)
        {
        case SeedType::SEED_FUMESHROOM: 
        case SeedType::SEED_LASERBEAN:
        case SeedType::SEED_ICYFUMESHROOM:  mShootingCounter = 50;  break;
        case SeedType::SEED_PUFFSHROOM:     mShootingCounter = 29;  break;
        case SeedType::SEED_SHAMROCK:
        case SeedType::SEED_SHAMROCKSTAR:
        case SeedType::SEED_SCAREDYSHROOM:  mShootingCounter = 25;  break;
        case SeedType::SEED_ICEBERGLETTUCEBETA:
        case SeedType::SEED_MOLDSHROOM:
        case SeedType::SEED_PEPPERPULT:
        case SeedType::SEED_CABBAGEPULT:
        case SeedType::SEED_SPORESHROOM:
        case SeedType::SEED_BAMBOOSHOOT:
        case SeedType::SEED_BAMBOOM:        mShootingCounter = 32;  break;
        case SeedType::SEED_MELONPULT:
        case SeedType::SEED_WINTERMELON:    mShootingCounter = 36;  break;
        case SeedType::SEED_KERNELPULT:

        {
            if (Sexy::Rand(4) == 0 || mIsBoosted)
            {
                aBodyReanim = mApp->ReanimationGet(mBodyReanimID);
                aBodyReanim->AssignRenderGroupToPrefix("Cornpult_butter", RENDER_GROUP_NORMAL);
                aBodyReanim->AssignRenderGroupToPrefix("Cornpult_kernal", RENDER_GROUP_HIDDEN);
                mState = PlantState::STATE_KERNELPULT_BUTTER;
            }

            mShootingCounter = 30;
            break;
        }
        case SeedType::SEED_CACTUS:         mShootingCounter = 35;  break;
        default:                            mShootingCounter = 29;  break;
        }
    }
    else
        Fire(aZombie, theRow, thePlantWeapon);

    return true;
}

//0x45F2A0
void Plant::LaunchThreepeater()
{
    int rowAbove = mRow - 1;
    int rowBelow = mRow + 1;

    if ((FindTargetZombie(mRow, PlantWeapon::WEAPON_PRIMARY)) ||
        (mBoard->RowCanHaveZombies(rowAbove) && FindTargetZombie(rowAbove, PlantWeapon::WEAPON_PRIMARY)) ||
        (mBoard->RowCanHaveZombies(rowBelow) && FindTargetZombie(rowBelow, PlantWeapon::WEAPON_PRIMARY)))
    {
        Reanimation* aHeadReanim1 = mApp->ReanimationGet(mHeadReanimID);
        Reanimation* aHeadReanim2 = mApp->ReanimationGet(mHeadReanimID2);
        Reanimation* aHeadReanim3 = mApp->ReanimationGet(mHeadReanimID3);

        if (mBoard->RowCanHaveZombies(rowBelow) && FindTargetZombie(rowBelow, PlantWeapon::WEAPON_PRIMARY))
        {
            aHeadReanim1->StartBlend(10);
            aHeadReanim1->mLoopType = ReanimLoopType::REANIM_PLAY_ONCE_AND_HOLD;
            aHeadReanim1->mAnimRate = 20.0f;
            aHeadReanim1->SetFramesForLayer("anim_shooting1");
        }

        if (FindTargetZombie(mRow, PlantWeapon::WEAPON_PRIMARY))
        {
            aHeadReanim2->StartBlend(10);
            aHeadReanim2->mLoopType = ReanimLoopType::REANIM_PLAY_ONCE_AND_HOLD;
            aHeadReanim2->mAnimRate = 20.0f;
            aHeadReanim2->SetFramesForLayer("anim_shooting2");
        }
        

        if (mBoard->RowCanHaveZombies(rowAbove) && FindTargetZombie(rowAbove, PlantWeapon::WEAPON_PRIMARY))
        {
            aHeadReanim3->StartBlend(10);
            aHeadReanim3->mLoopType = ReanimLoopType::REANIM_PLAY_ONCE_AND_HOLD;
            aHeadReanim3->mAnimRate = 20.0f;
            aHeadReanim3->SetFramesForLayer("anim_shooting3");
        }

        mShootingCounter = 35;
    }
}

//0x45F470
bool Plant::FindStarFruitTarget()
{
    if (mRecentlyEatenCountdown > 0)
        return true;

    int aDamageRangeFlags = GetDamageRangeFlags(PlantWeapon::WEAPON_PRIMARY);
    int aCenterStarX = mX + 40;
    int aCenterStarY = mY + 40;


    Zombie* aZombie = nullptr;
    while (mBoard->IterateZombies(aZombie))
    {
        Rect aZombieRect = aZombie->GetZombieRect();
        if (aZombie->EffectedByDamage(aDamageRangeFlags))
        {
            if (aZombie->mZombieType == ZombieType::ZOMBIE_BOSS && mPlantCol >= 5)
                return true;

            if (aZombie->mRow == mRow)
            {
                if (aZombieRect.mX + aZombieRect.mWidth < aCenterStarX)
                    return true;
            }
            else
            {
                if (Zombie::IsDigger(aZombie->mZombieType))
                    aZombieRect.mX += 10;

                float aProjectileTime = Distance2D(aCenterStarX, aCenterStarY, aZombieRect.mX + aZombieRect.mWidth / 2, aZombieRect.mY + aZombieRect.mHeight / 2) / 3.33f;
                int aZombieHitX = aZombie->ZombieTargetLeadX(aProjectileTime) - aZombieRect.mWidth / 2;
                if ((aZombieHitX + aZombieRect.mWidth > aCenterStarX) && (aZombieHitX < aCenterStarX))
                    return true;

                int aCenterZombieX = aZombieHitX + aZombieRect.mWidth / 2;
                int aCenterZombieY = aZombieRect.mY + aZombieRect.mHeight / 2;
                float angle = RAD_TO_DEG(atan2(aCenterZombieY - aCenterStarY, aCenterZombieX - aCenterStarX));
                if (abs(aZombie->mRow - mRow) < 2)
                {
                    if ((angle > 20.0f && angle < 40.0f) || (angle < -25.0f && angle > -45.0f))
                        return true;
                }
                else
                {
                    if ((angle > 25.0f && angle < 35.0f) || (angle < -28.0f && angle > -38.0f))
                        return true;
                }
            }
        }
    }

    return false;
}

//0x45F6E0
void Plant::LaunchStarFruit()
{
    if (FindStarFruitTarget())
    {
        PlayBodyReanim("anim_shoot", ReanimLoopType::REANIM_PLAY_ONCE_AND_HOLD, 20, 28.0f);
        mShootingCounter = 40;
    }
}

//0x45F720
void Plant::StarFruitFire()
{
    mApp->PlayFoley(FoleyType::FOLEY_THROW);

    float aShootAngleX = cos(DEG_TO_RAD(30.0f)) * 3.33f;
    float aShootAngleY = sin(DEG_TO_RAD(30.0f)) * 3.33f;
    for (int i = 0; i < 5; i++)
    {
        Projectile* aProjectile = mBoard->AddProjectile(mX + 25, mY + 25, mRenderOrder - 1, mRow, ProjectileType::PROJECTILE_STAR);
        aProjectile->mDamageRangeFlags = GetDamageRangeFlags(PlantWeapon::WEAPON_PRIMARY);
        aProjectile->mMotionType = ProjectileMotion::MOTION_STAR;

        switch (i)
        {
        case 0:     aProjectile->mVelX = -3.33f;         aProjectile->mVelY = 0.0f;             break;
        case 1:     aProjectile->mVelX = 0.0f;          aProjectile->mVelY = 3.33f;             break;
        case 2:     aProjectile->mVelX = 0.0f;          aProjectile->mVelY = -3.33f;            break;
        case 3:     aProjectile->mVelX = aShootAngleX;  aProjectile->mVelY = aShootAngleY;      break;
        case 4:     aProjectile->mVelX = aShootAngleX;  aProjectile->mVelY = -aShootAngleY;     break;
        default:    TOD_ASSERT();                                                               break;
        }
    }
}

void Plant::FutureStarFire(int aProjectileCount, Zombie* theZombie)
{
    mApp->PlayFoley(FoleyType::FOLEY_THROW);
    int aCenterStarX = mX + 40;
    int aCenterStarY = mY + 40;
    Rect aZombieRect = theZombie->GetZombieRect();
    float aProjectileTime = Distance2D(aCenterStarX, aCenterStarY, aZombieRect.mX + aZombieRect.mWidth / 2, aZombieRect.mY + aZombieRect.mHeight / 2) / 3.33f;
    int aCenterZombieX = theZombie->ZombieTargetLeadX(aProjectileTime);
    int aCenterZombieY = aZombieRect.mY + aZombieRect.mHeight / 2;
    float angle = RAD_TO_DEG(atan2(aCenterZombieY - aCenterStarY, aCenterZombieX - aCenterStarX));

    float aProjectileDegrees = 360 / aProjectileCount;
    for (int i = 0; i < aProjectileCount; i++)
    {
        Projectile* aProjectile = mBoard->AddProjectile(mX + 25, mY + 25, mRenderOrder - 1, mRow, ProjectileType::PROJECTILE_FUTURESTAR);
        aProjectile->mDamageRangeFlags = GetDamageRangeFlags(PlantWeapon::WEAPON_PRIMARY);
        aProjectile->mMotionType = ProjectileMotion::MOTION_STAR;
        aProjectile->mVelX = cos(DEG_TO_RAD(aProjectileDegrees * i + angle)) * 3.33f;
        aProjectile->mVelY = sin(DEG_TO_RAD(aProjectileDegrees * i + angle)) * 3.33f;
    }
}

//0x45F8A0
void Plant::UpdateShooter()
{
    
    mLaunchCounter--;
    if (mLaunchCounter <= 0)
    {
        mStoredLaunchRate = mLaunchRate - Sexy::Rand(15);
        mLaunchCounter = mStoredLaunchRate;

        if (mSeedType == SeedType::SEED_THREEPEATER)
            LaunchThreepeater();
        else if (mSeedType == SeedType::SEED_STARFRUIT)
            LaunchStarFruit();
            
        else if (mSeedType == SeedType::SEED_SPLITPEA)
        {
            FindTargetAndFire(mRow, PlantWeapon::WEAPON_PRIMARY);
            FindTargetAndFire(mRow, PlantWeapon::WEAPON_SECONDARY);
        }
        else if (mSeedType == SeedType::SEED_CACTUS)
        {
            if (mState == PlantState::STATE_CACTUS_HIGH)
                FindTargetAndFire(mRow, PlantWeapon::WEAPON_PRIMARY);
            else if (mState == PlantState::STATE_CACTUS_LOW)
                FindTargetAndFire(mRow, PlantWeapon::WEAPON_SECONDARY);
        }
        else if (mSeedType == SeedType::SEED_PLASMAPEA && mState == PlantState::STATE_READY) {
            FindTargetAndFire(mRow, PlantWeapon::WEAPON_SECONDARY);
        }
        else
            FindTargetAndFire(mRow, PlantWeapon::WEAPON_PRIMARY);
    }

    if (mLaunchCounter == mStoredLaunchRate - 50)
    {
        //if (mSeedType == SeedType::SEED_CATTAIL) FindTargetAndFire(mRow, PlantWeapon::WEAPON_PRIMARY);
        if (mSeedType == SeedType::SEED_BAMBOOSHOOT || mSeedType == SeedType::SEED_BAMBOOM) FindTargetAndFire((mRow - 1), PlantWeapon::WEAPON_PRIMARY);
    }
    if (mLaunchCounter == mStoredLaunchRate - 25)
    {
        if (mSeedType == SeedType::SEED_REPEATER || mSeedType == SeedType::SEED_LEFTPEATER || mSeedType == SeedType::SEED_ICEQUEENPEA || mSeedType == SeedType::SEED_ACEPEARAGUS)
        {
            FindTargetAndFire(mRow, PlantWeapon::WEAPON_PRIMARY);
        }
        else if (mSeedType == SeedType::SEED_SPLITPEA)
        {
            FindTargetAndFire(mRow, PlantWeapon::WEAPON_SECONDARY);
        }
    }
    if (mLaunchCounter == mStoredLaunchRate - 100)
    {
        if (mSeedType == SeedType::SEED_BAMBOOSHOOT || mSeedType == SeedType::SEED_BAMBOOM) FindTargetAndFire((mRow + 1), PlantWeapon::WEAPON_PRIMARY);
    }
}

//0x45F9A0
void Plant::UpdateProductionPlant()
{
    if (!IsInPlay() || mApp->IsIZombieLevel() || mApp->mGameMode == GameMode::GAMEMODE_UPSELL || mApp->mGameMode == GameMode::GAMEMODE_INTRO)
        return;

    if (mBoard->HasLevelAwardDropped())
        return;

    if (mSeedType == SeedType::SEED_MARIGOLD && mBoard->mCurrentWave == mBoard->mNumWaves)
    {
        if (mState != PlantState::STATE_MARIGOLD_ENDING)
        {
            mState = PlantState::STATE_MARIGOLD_ENDING;
            mStateCountdown = 6000;
        }
        else if (mStateCountdown <= 0)
            return;
    }

    if (mApp->IsLastStandLevel() && mBoard->mChallenge->mChallengeState != ChallengeState::STATECHALLENGE_LAST_STAND_ONSLAUGHT)
        return;

    mLaunchCounter--;
    if (mLaunchCounter <= 100)
    {
        int aFlashCountdown = TodAnimateCurve(100, 0, mLaunchCounter, 0, 100, TodCurves::CURVE_LINEAR);
        mEatenFlashCountdown = max(mEatenFlashCountdown, aFlashCountdown);
    }
    if (mLaunchCounter <= 0)
    {
        mLaunchCounter = RandRangeInt(mLaunchRate - 150, mLaunchRate);
        mApp->PlayFoley(FoleyType::FOLEY_SPAWN_SUN);
        CoinType aSunType = mState == PlantState::STATE_SUNSHROOM_SMALL ?
            mApp->mGameMode == GameMode::GAMEMODE_CHALLENGE_SUNNY_DAY ? CoinType::COIN_SUN : CoinType::COIN_SMALLSUN :
            mApp->mGameMode == GameMode::GAMEMODE_CHALLENGE_SUNNY_DAY ? CoinType::COIN_LARGESUN : CoinType::COIN_SUN;
        if (mSeedType == SeedType::SEED_SUNSHROOM) 
        {
            mBoard->AddCoin(mX, mY, aSunType, CoinMotion::COIN_MOTION_FROM_PLANT);
        }
        else if (mSeedType == SeedType::SEED_SUNFLOWER)
        {
            if (mApp->mGameMode == GameMode::GAMEMODE_CHALLENGE_BIG_TIME)
                mBoard->AddCoin(mX, mY, CoinType::COIN_LARGESUN, CoinMotion::COIN_MOTION_FROM_PLANT);
            else
                mBoard->AddCoin(mX, mY, aSunType, CoinMotion::COIN_MOTION_FROM_PLANT);
        }
        else if (mSeedType == SeedType::SEED_PRIMALSUNFLOWER)
        {
            Coin* aSun = mBoard->AddCoin(mX, mY, aSunType, CoinMotion::COIN_MOTION_FROM_PLANT);
            aSun->mValue = mApp->mGameMode == GameMode::GAMEMODE_CHALLENGE_SUNNY_DAY ? 6 : 3;
        }
        else if (mSeedType == SeedType::SEED_SHADOWFLOWER)
        {
            mBoard->AddCoin(mX, mY, CoinType::COIN_SHADOW_SUN, CoinMotion::COIN_MOTION_FROM_PLANT);
        }
        else if (mSeedType == SeedType::SEED_TWINSUNFLOWER)
        {
            mBoard->AddCoin(mX, mY, aSunType, CoinMotion::COIN_MOTION_FROM_PLANT);
            mBoard->AddCoin(mX, mY, aSunType, CoinMotion::COIN_MOTION_FROM_PLANT);
        }
        else if (mSeedType == SeedType::SEED_POWERFLOWER)
        {
            mBoard->AddCoin(mX, mY, aSunType, CoinMotion::COIN_MOTION_FROM_PLANT);
            mBoard->AddCoin(mX, mY, aSunType, CoinMotion::COIN_MOTION_FROM_PLANT);
            mBoard->AddCoin(mX, mY, aSunType, CoinMotion::COIN_MOTION_FROM_PLANT);
        }
        else if (mSeedType == SeedType::SEED_MARIGOLD)
        {
            mBoard->AddCoin(mX, mY, (Sexy::Rand(100) < 10) ? CoinType::COIN_GOLD : CoinType::COIN_SILVER, CoinMotion::COIN_MOTION_COIN);
        }

        if (mApp->mGameMode == GameMode::GAMEMODE_CHALLENGE_BIG_TIME && mSeedType == SeedType::SEED_MARIGOLD)
        {
            mBoard->AddCoin(mX, mY, CoinType::COIN_SILVER, CoinMotion::COIN_MOTION_COIN);
        }
    }
}

//0x45FB70
void Plant::UpdateSunShroom()
{
    Reanimation* aBodyReanim = mApp->ReanimationGet(mBodyReanimID);
    if (mState == PlantState::STATE_SUNSHROOM_SMALL)
    {
        if (mStateCountdown == 0)
        {
            PlayBodyReanim("anim_grow", ReanimLoopType::REANIM_PLAY_ONCE_AND_HOLD, 10, 12.0f);
            mState = PlantState::STATE_SUNSHROOM_GROWING;
            mApp->PlayFoley(FoleyType::FOLEY_PLANTGROW);
        }

        UpdateProductionPlant();
    }
    else if (mState == PlantState::STATE_SUNSHROOM_GROWING)
    {
        if (aBodyReanim->mLoopCount > 0)
        {
            PlayBodyReanim("anim_bigidle", ReanimLoopType::REANIM_LOOP, 10, RandRangeFloat(12.0f, 15.0f));
            mState = PlantState::STATE_SUNSHROOM_BIG;
        }
    }
    else
    {
        UpdateProductionPlant();
    }
}

//0x45FC70
void Plant::UpdateGraveBuster()
{
    if (mState == PlantState::STATE_GRAVEBUSTER_LANDING)
    {
        if (mApp->ReanimationGet(mBodyReanimID)->mLoopCount > 0)
        {
            PlayBodyReanim("anim_idle", ReanimLoopType::REANIM_LOOP, 10, 12.0f);
            mStateCountdown = 400;
            mState = PlantState::STATE_GRAVEBUSTER_EATING;
            AddAttachedParticle(mX + 40, mY + 40, mRenderOrder + 4, ParticleEffect::PARTICLE_GRAVE_BUSTER);
        }
    }
    else if (mState == PlantState::STATE_GRAVEBUSTER_EATING && mStateCountdown == 0)
    {
        GridItem* aGraveStone = mBoard->GetGraveStoneAt(mPlantCol, mRow);
        if (aGraveStone)
        {
            aGraveStone->GridItemDie();
            mBoard->mGravesCleared++;
        }

        mApp->AddTodParticle(mX + 40, mY + 40, mRenderOrder + 4, ParticleEffect::PARTICLE_GRAVE_BUSTER_DIE);
        Die();
        mBoard->DropLootPiece(mX + 40, mY, 12);
    }
}

//0x45FD90
void Plant::PlayBodyReanim(const char* theTrackName, ReanimLoopType theLoopType, int theBlendTime, float theAnimRate)
{
    Reanimation* aBodyReanim = mApp->ReanimationGet(mBodyReanimID);

    if (theBlendTime > 0)
        aBodyReanim->StartBlend(theBlendTime);
    if (theAnimRate > 0.0f)
        aBodyReanim->mAnimRate = theAnimRate;

    aBodyReanim->mLoopType = theLoopType;
    aBodyReanim->mLoopCount = 0;
    aBodyReanim->SetFramesForLayer(theTrackName);
}

//0x45FE20
void Plant::UpdatePotato()
{
    Reanimation* aBodyReanim = mApp->ReanimationGet(mBodyReanimID);

    if (mState == PlantState::STATE_NOTREADY)
    {
        if (mStateCountdown == 0)
        {
            mApp->AddTodParticle(mX + mWidth / 2, mY + mHeight / 2, mRenderOrder, ParticleEffect::PARTICLE_POTATO_MINE_RISE);
            PlayBodyReanim("anim_rise", ReanimLoopType::REANIM_PLAY_ONCE_AND_HOLD, 20, 18.0f);
            mState = PlantState::STATE_POTATO_RISING;
            mApp->PlayFoley(FoleyType::FOLEY_DIRT_RISE);
        }
    }
    else if (mState == PlantState::STATE_POTATO_RISING)
    {
        if (aBodyReanim->mLoopCount > 0)
        {
            float aRate = RandRangeFloat(12.0f, 15.0f);
            PlayBodyReanim("anim_armed", ReanimLoopType::REANIM_LOOP, 0, aRate);

            Reanimation* aLightReanim = mApp->AddReanimation(0.0f, 0.0f, mRenderOrder + 2, GetPlantDefinition(mSeedType).mReanimationType);
            aLightReanim->mLoopType = ReanimLoopType::REANIM_LOOP;
            aLightReanim->mAnimRate = aRate - 2.0f;
            aLightReanim->SetFramesForLayer("anim_glow");
            aLightReanim->mFrameCount = 10;
            aLightReanim->ShowOnlyTrack("anim_glow");
            aLightReanim->SetTruncateDisappearingFrames("anim_glow", false);
            mLightReanimID = mApp->ReanimationGetID(aLightReanim);
            aLightReanim->AttachToAnotherReanimation(aBodyReanim, "anim_light");

            mState = PlantState::STATE_POTATO_ARMED;
            mBlinkCountdown = 400 + Sexy::Rand(4000);
        }
    }
    else if (mState == PlantState::STATE_POTATO_ARMED)
    {
        if (FindTargetZombie(mRow, PlantWeapon::WEAPON_PRIMARY))
        {
            DoSpecial();
        }
        else
        {
            Reanimation* aLightReanim = mApp->ReanimationTryToGet(mLightReanimID);
            if (aLightReanim)
            {
                aLightReanim->mFrameCount = TodAnimateCurve(200, 50, DistanceToClosestZombie(), 10, 3, TodCurves::CURVE_LINEAR);
            }
        }
    }
}

//0x460060
void Plant::UpdateTanglekelp()
{
    if (mState != PlantState::STATE_TANGLEKELP_GRABBING)
    {
        Zombie* aZombie = FindTargetZombie(mRow, PlantWeapon::WEAPON_PRIMARY);
        if (aZombie)
        {
            mApp->PlayFoley(FoleyType::FOLEY_FLOOP);
            mState = PlantState::STATE_TANGLEKELP_GRABBING;
            mStateCountdown = 100;
            aZombie->PoolSplash(false);

            float aVinesPosX = -13.0f;
            float aVinesPosY = 15.0f;
            if (Zombie::IsSnorkel(aZombie->mZombieType))
            {
                aVinesPosX = -43.0f;
                aVinesPosY = 55.0f;
            }
            if (aZombie->mZombiePhase == ZombiePhase::PHASE_DOLPHIN_RIDING)
            {
                aVinesPosX = -20.0f;
                aVinesPosY = 37.0f;
            }
            Reanimation* aGrabReanim = aZombie->AddAttachedReanim(aVinesPosX, aVinesPosY, ReanimationType::REANIM_TANGLEKELP);
            if (aGrabReanim)
            {
                aGrabReanim->SetFramesForLayer("anim_grab");
                aGrabReanim->mAnimRate = 24.0f;
                aGrabReanim->mLoopType = ReanimLoopType::REANIM_PLAY_ONCE_AND_HOLD;
            }

            mTargetZombieID = mBoard->ZombieGetID(aZombie);
        }
    }
    else
    {
        if (mStateCountdown == 50)
        {
            Zombie* aZombie = mBoard->ZombieTryToGet(mTargetZombieID);
            if (aZombie)
            {
                if (aZombie->mBodyMaxHealth + aZombie->mHelmMaxHealth + aZombie->mShieldMaxHealth + aZombie->mFlyingMaxHealth <= 1800)
                {
                    aZombie->DragUnder();
                    aZombie->PoolSplash(false);
                }
            }
        }

        if (mStateCountdown == 20)
        {
            int aRenderPosition = Board::MakeRenderOrder(RenderLayer::RENDER_LAYER_PARTICLE, mRow, 0);
            Reanimation* aSplashReanim = mApp->AddReanimation(mX - 23, mY + 7, aRenderPosition, ReanimationType::REANIM_SPLASH);
            aSplashReanim->OverrideScale(1.3f, 1.3f);

            mApp->AddTodParticle(mX + 31, mY + 64, aRenderPosition, ParticleEffect::PARTICLE_PLANTING_POOL);
            mApp->PlayFoley(FoleyType::FOLEY_ZOMBIE_ENTERING_WATER);
        }

        if (mStateCountdown == 0)
        {
            Die();

            Zombie* aZombie = mBoard->ZombieTryToGet(mTargetZombieID);
            if (aZombie)
            {
                if (aZombie->mBodyMaxHealth + aZombie->mHelmMaxHealth + aZombie->mShieldMaxHealth + aZombie->mFlyingMaxHealth <= 1800)
                {
                    aZombie->DieWithLoot();
                }
                else
                {
                    aZombie->TakeDamage(1800, 0U);
                }
            }
        }
    }
}

//0x460320
void Plant::SpikeweedAttack()
{
    TOD_ASSERT(IsSpiky());

    if (mState != PlantState::STATE_SPIKEWEED_ATTACKING)
    {
        PlayBodyReanim("anim_attack", ReanimLoopType::REANIM_PLAY_ONCE_AND_HOLD, 20, 18.0f);
        mApp->PlaySample(SOUND_THROW);
        
        mState = PlantState::STATE_SPIKEWEED_ATTACKING;
        mLaunchCounter = mLaunchRate - Rand(10);
    }
}

//0x460370
void Plant::UpdateSpikeweed()
{
    if (mLaunchCounter > 0) 
    {
        mLaunchCounter--;
    }
    Reanimation* aBodyReanim = mApp->ReanimationGet(mBodyReanimID);
    if (mState == PlantState::STATE_SPIKEWEED_ATTACKING)
    {
        if (mLaunchCounter == 0)
        {
            mState = PlantState::STATE_NOTREADY;
        }
        else if (mSeedType == SeedType::SEED_SPIKEROCK)
        {
            if (mLaunchCounter == 69 || mLaunchCounter == 33)
            {
                DoRowAreaDamage(20, 33U, PlantWeapon::WEAPON_PRIMARY);
            }
        }
        else if (mLaunchCounter == 75)
        {
            DoRowAreaDamage(20, 33U, PlantWeapon::WEAPON_PRIMARY);
        }

        if (aBodyReanim->mLoopCount > 0)
        {
            PlayIdleAnim(RandRangeFloat(12.0f, 15.0f));
        }
    }
    else if (FindTargetZombie(mRow, PlantWeapon::WEAPON_PRIMARY))
    {
        SpikeweedAttack();
    }
}

//0x460420
void Plant::UpdateScaredyShroom()
{
    if (mShootingCounter > 0)
        return;

    bool aHasZombieNearby = false;

    Zombie* aZombie = nullptr;
    while (mBoard->IterateZombies(aZombie))
    {
        Rect aZombieRect = aZombie->GetZombieRect();
        int aDiffY = (aZombie->mZombieType == ZombieType::ZOMBIE_BOSS) ? 0 : (aZombie->mRow - mRow);
        if (!aZombie->mMindControlled && !aZombie->IsDeadOrDying() && GetCircleRectOverlap(mX, mY + 20.0f, 120, aZombieRect))
        {
            
            if (aDiffY <= 1 && aDiffY >= -1 && mSeedType != SeedType::SEED_SHAMROCKSTAR)
            {
                aHasZombieNearby = true;
                break;
            }
            else if (aDiffY == 0)
            {
                aHasZombieNearby = true;
                break;
            }
        }
    }

    Reanimation* aBodyReanim = mApp->ReanimationGet(mBodyReanimID);
    if (mState == PlantState::STATE_READY)
    {
        if (aHasZombieNearby)
        {
            mState = PlantState::STATE_SCAREDYSHROOM_LOWERING;
            PlayBodyReanim("anim_scared", ReanimLoopType::REANIM_PLAY_ONCE_AND_HOLD, 10, 10.0f);
        }
    }
    else if (mState == PlantState::STATE_SCAREDYSHROOM_LOWERING)
    {
        if (aBodyReanim->mLoopCount > 0)
        {
            mState = PlantState::STATE_SCAREDYSHROOM_SCARED;
            PlayBodyReanim("anim_scaredidle", ReanimLoopType::REANIM_LOOP, 10, 0.0f);
        }
    }
    else if (mState == PlantState::STATE_SCAREDYSHROOM_SCARED)
    {
        if (!aHasZombieNearby)
        {
            mState = PlantState::STATE_SCAREDYSHROOM_RAISING;

            float aAnimRate = RandRangeFloat(7.0f, 12.0f);
            PlayBodyReanim("anim_grow", ReanimLoopType::REANIM_PLAY_ONCE_AND_HOLD, 10, aAnimRate);
        }
    }
    else if (mState == PlantState::STATE_SCAREDYSHROOM_RAISING)
    {
        if (aBodyReanim->mLoopCount > 0)
        {
            mState = PlantState::STATE_READY;

            float aAnimRate = RandRangeFloat(10.0f, 15.0f);
            PlayIdleAnim(aAnimRate);
        }
    }

    if (mState != PlantState::STATE_READY)
    {
        mLaunchCounter = mLaunchRate;
    }
}

//0x460420
void Plant::UpdateVineNut()
{
    if (mRecentlyEatenCountdown > 0)
    {
        if (mPlantAge % 4 == 0)
        {
            mLaunchCounter--;
            if (mLaunchCounter < 0)
            {
                mLaunchCounter = mLaunchRate - Sexy::Rand(5);
            }
        }
    }
    else {
        mLaunchCounter = 1;
    }
}

void Plant::UpdateInfiNut()
{
    if (mRecentlyEatenCountdown > 0 && mState != PlantState::STATE_INVULNERABLE)
    {
        if (mState != PlantState::STATE_NOTREADY)
        {
            mState = PlantState::STATE_NOTREADY;
        }
        mStateCountdown = 1500;
    }
    if ((mState == PlantState::STATE_INVULNERABLE || mState == PlantState::STATE_NOTREADY) && mStateCountdown == 0)
    {
        mState = PlantState::STATE_READY;
    }
    else if (mState == PlantState::STATE_READY && mRecentlyEatenCountdown <= 0 && mPlantHealth < mPlantMaxHealth)
    {
        mPlantHealth++;
    }
}

void Plant::UpdateMelee()
{
    if (mLaunchCounter > 0 && mState != PlantState::STATE_DOING_PLANT_FOOD)
    {
        mLaunchCounter--;
    }
    Reanimation* aBodyReanim = mApp->ReanimationGet(mBodyReanimID);
    if (mState == PlantState::STATE_CHOMPER_BITING && mLaunchCounter == 0)
    {
        bool aSingleTarget = mSeedType == SeedType::SEED_BONKCHOY;
        Zombie* aZombie = FindTargetZombie(mRow, PlantWeapon::WEAPON_PRIMARY);
        if (aZombie)
        {
            int aDamage = mSeedType == SeedType::SEED_BONKCHOY ? 15 : 
                mSeedType == SeedType::SEED_BEETBOXER ? 80 : 40;
            
            aSingleTarget ? aZombie->TakeDamage(aDamage, 0U) : DoRowAreaDamage(aDamage, 0U, PlantWeapon::WEAPON_PRIMARY);
            mApp->PlayFoley(FoleyType::FOLEY_SPLAT);
        }
        mState = PlantState::STATE_NOTREADY;
    }
    else if (mState == PlantState::STATE_NOTREADY && aBodyReanim->mLoopCount > 0)
    {
        PlayIdleAnim(aBodyReanim->mDefinition->mFPS);
    }
    else if (mState == PlantState::STATE_NOTREADY && FindTargetZombie(mRow, PlantWeapon::WEAPON_PRIMARY))
    {
        PlayBodyReanim("anim_bite", ReanimLoopType::REANIM_PLAY_ONCE_AND_HOLD, 20, 24.0f);
        mApp->PlaySample(SOUND_THROW);

        mState = PlantState::STATE_CHOMPER_BITING;
        mLaunchCounter = mLaunchRate - Rand(5);
    }
    else if (mState == PlantState::STATE_DOING_PLANT_FOOD)
    {
        if (mStateCountdown == 0)
        {
            mState = PlantState::STATE_NOTREADY;
            PlayIdleAnim(aBodyReanim->mDefinition->mFPS);
        }
        if (mStateCountdown % 8 == 1)
        {
            DoRowAreaDamage(40, 0U, PlantWeapon::WEAPON_SECONDARY);
        }
    }
}

void Plant::UpdateChargingShooter()
{
    if (mState == PlantState::STATE_NOTREADY && mStateCountdown <= 0)
        mState = PlantState::STATE_READY;
}

void Plant::UpdateFakePlantFood() 
{
    
    if (mState == PlantState::STATE_DOING_PLANT_FOOD && mStateCountdown <= 1)
    {
        mState = PlantState::STATE_NOTREADY;
        mStateCountdown = 9000;
    }
    else if (!mPlantFoodReady && mStateCountdown <= 0) {
        mPlantFoodReady = true;
    }
}

//0x460610
void Plant::UpdateTorchwood()
{
    Rect aAttackRect = GetPlantAttackRect(PlantWeapon::WEAPON_PRIMARY);

    Projectile* aProjectile = nullptr;
    while (mBoard->IterateProjectiles(aProjectile))
    {
        if ((aProjectile->mRow == mRow) && 
            (aProjectile->mProjectileType == ProjectileType::PROJECTILE_PEA 
                || aProjectile->mProjectileType == ProjectileType::PROJECTILE_SNOWPEA 
                || aProjectile->mProjectileType == ProjectileType::PROJECTILE_ICEPEA 
                || aProjectile->mProjectileType == ProjectileType::PROJECTILE_FIREPEA
                || aProjectile->mProjectileType == ProjectileType::PROJECTILE_FIREBALL
                ))
        {
            Rect aProjectileRect = aProjectile->GetProjectileRect();
            if (GetRectOverlap(aAttackRect, aProjectileRect) >= 10)
            {
                if (aProjectile->mProjectileType == ProjectileType::PROJECTILE_PEA)
                {
                    aProjectile->ConvertToFirepea(mPlantCol);
                }
                else if (aProjectile->mProjectileType == ProjectileType::PROJECTILE_FIREBALL)
                {
                    aProjectile->ConvertToPlasmaball(mPlantCol);
                }
                else if (aProjectile->mProjectileType == ProjectileType::PROJECTILE_FIREPEA)
                {
                    aProjectile->ConvertToPlasmapea(mPlantCol);
                }
                else if (aProjectile->mProjectileType == ProjectileType::PROJECTILE_SNOWPEA)
                {
                    aProjectile->ConvertToPea(mPlantCol);
                }
                else if (aProjectile->mProjectileType == ProjectileType::PROJECTILE_ICEPEA)
                {
                    aProjectile->ConvertToSnowPea(mPlantCol);
                }
            }
        }
    }
}

//0x460610
void Plant::UpdateFlamewood()
{
    Rect aAttackRect = GetPlantAttackRect(PlantWeapon::WEAPON_PRIMARY);

    Projectile* aProjectile = nullptr;
    while (mBoard->IterateProjectiles(aProjectile))
    {
        if ((aProjectile->mRow == mRow) &&
            (aProjectile->mProjectileType == ProjectileType::PROJECTILE_PEA 
                || aProjectile->mProjectileType == ProjectileType::PROJECTILE_SNOWPEA 
                || aProjectile->mProjectileType == ProjectileType::PROJECTILE_ICEPEA 
                || aProjectile->mProjectileType == ProjectileType::PROJECTILE_FIREPEA 
                || aProjectile->mProjectileType == ProjectileType::PROJECTILE_FIREBALL
                ))
        {
            Rect aProjectileRect = aProjectile->GetProjectileRect();
            if (GetRectOverlap(aAttackRect, aProjectileRect) >= 10)
            {
                if (aProjectile->mProjectileType == ProjectileType::PROJECTILE_FIREPEA || aProjectile->mProjectileType == ProjectileType::PROJECTILE_PEA)
                {
                    aProjectile->ConvertToPlasmapea(mPlantCol);
                }
                else if (aProjectile->mProjectileType == ProjectileType::PROJECTILE_FIREBALL)
                {
                    aProjectile->ConvertToPlasmaball(mPlantCol);
                }
                else if (aProjectile->mProjectileType == ProjectileType::PROJECTILE_SNOWPEA)
                {
                    aProjectile->ConvertToFirepea(mPlantCol);
                }
                else if (aProjectile->mProjectileType == ProjectileType::PROJECTILE_ICEPEA)
                {
                    aProjectile->ConvertToPea(mPlantCol);
                }
            }
        }
    }
}

void Plant::UpdateIceberg()
{
    Zombie* aZombie = FindTargetZombie(mRow, PlantWeapon::WEAPON_PRIMARY);
    if (aZombie && aZombie->CanBeFrozen())
    {
        DoSpecial();
    }
}

//0x4606F0
void Plant::DoSquashDamage()
{
    int aDamageRangeFlags = GetDamageRangeFlags(PlantWeapon::WEAPON_PRIMARY);
    Rect aAttackRect = GetPlantAttackRect(PlantWeapon::WEAPON_PRIMARY);

    Zombie* aZombie = nullptr;
    while (mBoard->IterateZombies(aZombie))
    {
        if ((aZombie->mRow == mRow || aZombie->mZombieType == ZombieType::ZOMBIE_BOSS) && aZombie->EffectedByDamage(aDamageRangeFlags))
        {
            Rect aZombieRect = aZombie->GetZombieRect();
            if (GetRectOverlap(aAttackRect, aZombieRect) > (Zombie::IsFootball(aZombie->mZombieType) ? -20 : 0))
            {
                aZombie->TakeDamage(1800, 18U);
            }
        }
    }
}

//0x4607E0
Zombie* Plant::FindSquashTarget()
{
    int aDamageRangeFlags = GetDamageRangeFlags(PlantWeapon::WEAPON_PRIMARY);
    Rect aAttackRect = GetPlantAttackRect(PlantWeapon::WEAPON_PRIMARY);

    int aClosestRange = 0;
    Zombie* aClosestZombie = nullptr;

    Zombie* aZombie = nullptr;
    while (mBoard->IterateZombies(aZombie))
    {
        if ((aZombie->mRow == mRow || aZombie->mZombieType == ZombieType::ZOMBIE_BOSS) &&
            aZombie->mHasHead && !aZombie->IsTangleKelpTarget() && aZombie->EffectedByDamage(aDamageRangeFlags))
        {
            Rect aZombieRect = aZombie->GetZombieRect();

            if ((
                    aZombie->mZombiePhase == ZombiePhase::PHASE_POLEVAULTER_PRE_VAULT && aZombieRect.mX < mX + 20
                ) || (
                    aZombie->mZombiePhase != ZombiePhase::PHASE_POLEVAULTER_PRE_VAULT && 
                    aZombie->mZombiePhase != ZombiePhase::PHASE_POLEVAULTER_IN_VAULT &&
                    aZombie->mZombiePhase != ZombiePhase::PHASE_SNORKEL_INTO_POOL && 
                    aZombie->mZombiePhase != ZombiePhase::PHASE_DOLPHIN_INTO_POOL &&
                    aZombie->mZombiePhase != ZombiePhase::PHASE_DOLPHIN_RIDING && 
                    aZombie->mZombiePhase != ZombiePhase::PHASE_DOLPHIN_IN_JUMP &&
                    !aZombie->IsBobsledTeamWithSled()
                ))
            {
                int aRange = -GetRectOverlap(aAttackRect, aZombieRect);
                if (aRange <= (aZombie->mIsEating ? 110 : 70))
                {
                    int aPlantX = aAttackRect.mX;
                    if (aZombie->mZombiePhase == ZombiePhase::PHASE_POLEVAULTER_POST_VAULT || aZombie->mZombiePhase == ZombiePhase::PHASE_POLEVAULTER_PRE_VAULT ||
                        aZombie->mZombiePhase == ZombiePhase::PHASE_DOLPHIN_WALKING_IN_POOL || Zombie::IsImp(aZombie->mZombieType) ||
                        Zombie::IsFootball(aZombie->mZombieType) || mApp->IsScaryPotterLevel())
                    {
                        aPlantX = aAttackRect.mX - 60;
                    }

                    if (aZombie->IsWalkingBackwards() || aZombieRect.mX + aZombieRect.mWidth >= aPlantX)
                    {
                        if (mBoard->ZombieGetID(aZombie) == mTargetZombieID)
                            return aZombie;  // 是锁定的目标僵尸，则直接返回该僵尸

                        if (aClosestZombie == nullptr || aRange < aClosestRange)
                        {
                            aClosestZombie = aZombie;
                            aClosestRange = aRange;
                        }
                    }
                }
            }
        }
    }

    return aClosestZombie;
}

//0x4609D0
void Plant::UpdateSquash()
{
    Reanimation* aBodyReanim = mApp->ReanimationTryToGet(mBodyReanimID);
    TOD_ASSERT(aBodyReanim);

    if (mState == PlantState::STATE_NOTREADY)
    {
        Zombie* aZombie = FindSquashTarget();
        if (aZombie)
        {
            mTargetZombieID = mBoard->ZombieGetID(aZombie);
            mTargetX = aZombie->ZombieTargetLeadX(0.0f) - mWidth / 2;
            mState = PlantState::STATE_SQUASH_LOOK;
            mStateCountdown = 80;
            PlayBodyReanim(mTargetX < mX ? "anim_lookleft" : "anim_lookright", ReanimLoopType::REANIM_PLAY_ONCE_AND_HOLD, 10, 24.0f);
            mApp->PlayFoley(FoleyType::FOLEY_SQUASH_HMM);
        }
    }
    else if (mState == PlantState::STATE_SQUASH_LOOK)
    {
        if (mStateCountdown <= 0)
        {
            PlayBodyReanim("anim_jumpup", ReanimLoopType::REANIM_PLAY_ONCE_AND_HOLD, 20, 24.0f);
            mState = PlantState::STATE_SQUASH_PRE_LAUNCH;
            mStateCountdown = 30;
        }
    }
    else if (mState == PlantState::STATE_SQUASH_PRE_LAUNCH)
    {
        if (mStateCountdown <= 0)
        {
            Zombie* aZombie = FindSquashTarget();
            if (aZombie)
            {
                mTargetX = aZombie->ZombieTargetLeadX(30.0f) - mWidth / 2;
            }

            mState = PlantState::STATE_SQUASH_RISING;
            mStateCountdown = 50;
            mRenderOrder = Board::MakeRenderOrder(RenderLayer::RENDER_LAYER_PARTICLE, mRow, 0);
        }
    }
    else
    {
        int aTargetCol = mBoard->PixelToGridXKeepOnBoard(mTargetX, mY);
        int aDestY = mBoard->GridToPixelY(aTargetCol, mRow) + 8;

        if (mState == PlantState::STATE_SQUASH_RISING)
        {
            if (mSeedType == SEED_SPOOKYSQUASH && aTargetCol < 8 && mBoard->CanPlantAt(aTargetCol, mRow, mSeedType) == PlantingReason::PLANTING_OK) mX = TodAnimateCurve(50, 20, mStateCountdown, mBoard->GridToPixelX(mPlantCol, mRow), mBoard->GridToPixelX(aTargetCol, mRow), TodCurves::CURVE_EASE_IN_OUT);
            else mX = TodAnimateCurve(50, 20, mStateCountdown, mBoard->GridToPixelX(mPlantCol, mStartRow), mTargetX, TodCurves::CURVE_EASE_IN_OUT);
            mY = TodAnimateCurve(50, 20, mStateCountdown, mBoard->GridToPixelY(mPlantCol, mStartRow), aDestY - 120, TodCurves::CURVE_EASE_IN_OUT);

            if (mStateCountdown == 0)
            {
                PlayBodyReanim("anim_jumpdown", ReanimLoopType::REANIM_PLAY_ONCE_AND_HOLD, 0, 60.0f);
                mState = PlantState::STATE_SQUASH_FALLING;
                mStateCountdown = 10;
            }
        }
        else if (mState == PlantState::STATE_SQUASH_FALLING)
        {
            mY = TodAnimateCurve(10, 0, mStateCountdown, aDestY - 120, aDestY, TodCurves::CURVE_EASE_IN_OUT);

            if (mStateCountdown == 5)
            {
                DoSquashDamage();
            }

            if (mStateCountdown == 0)
            {
                if (mBoard->IsPoolSquare(aTargetCol, mRow))
                {
                    mApp->AddReanimation(mX - 11, mY + 20, mRenderOrder + 1, ReanimationType::REANIM_SPLASH);
                    mApp->PlayFoley(FoleyType::FOLEY_SPLAT);
                    mApp->PlaySample(SOUND_ZOMBIESPLASH);

                    Die();
                }
                else
                {
                    mState = PlantState::STATE_SQUASH_DONE_FALLING;
                    mStateCountdown = 100;

                    mBoard->ShakeBoard(1, 4);
                    mApp->PlayFoley(FoleyType::FOLEY_THUMP);
                    float aOffsetY = mBoard->StageHasRoof() ? 69.0f : 80.0f;
                    mApp->AddTodParticle(mX + 40, mY + aOffsetY, mRenderOrder + 4, ParticleEffect::PARTICLE_DUST_SQUASH);
                }
            }
        }
        else if (mState == PlantState::STATE_SQUASH_DONE_FALLING)
        {
            if (mStateCountdown == 0)
            {
                if (mSeedType == SEED_SPOOKYSQUASH && aTargetCol < 8 && mBoard->CanPlantAt(aTargetCol, mRow, mSeedType) == PlantingReason::PLANTING_OK)
                {
                    mState = PlantState::STATE_NOTREADY;
                    PlayIdleAnim(RandRangeFloat(10.0f, 15.0f));
                    mPlantCol = aTargetCol;
                }
                else Die();
            }
        }
    }
}

//0x460DD0
void Plant::UpdateDoomShroom()
{
    if (mIsAsleep || mState == PlantState::STATE_DOINGSPECIAL)
        return;

    mState = PlantState::STATE_DOINGSPECIAL;
    mDoSpecialCountdown = 100;

    Reanimation* aBodyReanim = mApp->ReanimationTryToGet(mBodyReanimID);
    TOD_ASSERT(aBodyReanim);

    aBodyReanim->SetFramesForLayer("anim_explode");
    aBodyReanim->mAnimRate = 23.0f;
    aBodyReanim->mLoopType = ReanimLoopType::REANIM_PLAY_ONCE_AND_HOLD;
    aBodyReanim->SetShakeOverride("DoomShroom_head1", 1.0f);
    aBodyReanim->SetShakeOverride("DoomShroom_head2", 2.0f);
    aBodyReanim->SetShakeOverride("DoomShroom_head3", 2.0f);
    mApp->PlayFoley(FoleyType::FOLEY_REVERSE_EXPLOSION);
}

void Plant::UpdateIceShroom()
{
    if (!mIsAsleep && mState != PlantState::STATE_DOINGSPECIAL)
    {
        mState = PlantState::STATE_DOINGSPECIAL;
        mDoSpecialCountdown = 100;
    }
}

//0x460F00
void Plant::UpdateBlover()
{
    Reanimation* aBodyReanim = mApp->ReanimationGet(mBodyReanimID);
    if (aBodyReanim->mLoopCount > 0 && aBodyReanim->mLoopType != ReanimLoopType::REANIM_LOOP)
    {
        aBodyReanim->SetFramesForLayer("anim_loop");
        aBodyReanim->mLoopType = ReanimLoopType::REANIM_LOOP;
    }

    if (mState != PlantState::STATE_DOINGSPECIAL && mStateCountdown == 0)
    {
        DoSpecial();
    }
}

void Plant::UpdateGoldbloom()
{
    Reanimation* aBodyReanim = mApp->ReanimationGet(mBodyReanimID);
    if (aBodyReanim->mLoopCount > 0 && aBodyReanim->mLoopType != ReanimLoopType::REANIM_LOOP)
    {
        aBodyReanim->SetFramesForLayer("anim_loop");
        aBodyReanim->mLoopType = ReanimLoopType::REANIM_LOOP;
    }

    if (mState == PlantState::STATE_NOTREADY && mStateCountdown == 0)
    {
        aBodyReanim->SetFramesForLayer("anim_blow");
        aBodyReanim->mLoopType = ReanimLoopType::REANIM_PLAY_ONCE_AND_HOLD;
        aBodyReanim->mAnimRate = 20.0f;
        mDoSpecialCountdown = 50;
        mState = PlantState::STATE_READY;
    }
    else if (mState == PlantState::STATE_READY && mStateCountdown == 0)
    {
        DoSpecial();
    }
}

void Plant::UpdateUnderPlant()
{
    if (mState == PlantState::STATE_INVULNERABLE && mStateCountdown == 0)
        mState = PlantState::STATE_NOTREADY;
}

//0x460F60
void Plant::UpdateCoffeeBean()
{
    if (mState == PlantState::STATE_DOINGSPECIAL)
    {
        Reanimation* aBodyReanim = mApp->ReanimationGet(mBodyReanimID);
        if (aBodyReanim->mLoopCount > 0)
        {
            Die();
        }
    }
}

//0x460FA0
void Plant::UpdateUmbrella()
{
    if (mState == PlantState::STATE_UMBRELLA_TRIGGERED)
    {
        if (mStateCountdown == 0)
        {
            mRenderOrder = Board::MakeRenderOrder(RenderLayer::RENDER_LAYER_PROJECTILE, mRow + 1, 0);
            mState = PlantState::STATE_UMBRELLA_REFLECTING;
        }
    }
    else if (mState == PlantState::STATE_UMBRELLA_REFLECTING)
    {
        Reanimation* aBodyReanim = mApp->ReanimationGet(mBodyReanimID);
        if (aBodyReanim->mLoopCount > 0)
        {
            PlayIdleAnim(0.0f);
            mState = PlantState::STATE_NOTREADY;
            mRenderOrder = CalcRenderOrder();
        }
    }
}

//0x461020
void Plant::UpdateCobCannon()
{
    if (mState == PlantState::STATE_COBCANNON_ARMING)
    {
        if (mStateCountdown == 0)
        {
            mState = PlantState::STATE_COBCANNON_LOADING;
            PlayBodyReanim("anim_charge", ReanimLoopType::REANIM_PLAY_ONCE_AND_HOLD, 20, 12.0f);
        }
    }
    else if (mState == PlantState::STATE_COBCANNON_LOADING)
    {
        Reanimation* aBodyReanim = mApp->ReanimationGet(mBodyReanimID);
        if (aBodyReanim->ShouldTriggerTimedEvent(0.5f))
        {
            mApp->PlayFoley(FoleyType::FOLEY_SHOOP);
        }
        if (aBodyReanim->mLoopCount > 0)
        {
            mState = PlantState::STATE_COBCANNON_READY;
            PlayIdleAnim(12.0f);
        }
    }
    else if (mState == PlantState::STATE_COBCANNON_READY)
    {
        Reanimation* aBodyReanim = mApp->ReanimationGet(mBodyReanimID);
        ReanimatorTrackInstance* aCobTrack = aBodyReanim->GetTrackInstanceByName("CobCannon_cob");
        aCobTrack->mTrackColor = GetFlashingColor(mBoard->mMainCounter, 75);
    }
    else if (mState == PlantState::STATE_COBCANNON_FIRING)
    {
        Reanimation* aBodyReanim = mApp->ReanimationGet(mBodyReanimID);
        if (aBodyReanim->ShouldTriggerTimedEvent(0.48f))
        {
            mApp->PlayFoley(FoleyType::FOLEY_COB_LAUNCH);
        }
    }
}

//0x4611F0
void Plant::UpdateCactus()
{
    if (mShootingCounter > 0)
        return;

    Reanimation* aBodyReanim = mApp->ReanimationGet(mBodyReanimID);
    if (mState == PlantState::STATE_CACTUS_RISING)
    {
        if (aBodyReanim->mLoopCount > 0)
        {
            mState = PlantState::STATE_CACTUS_HIGH;
            PlayBodyReanim("anim_idlehigh", ReanimLoopType::REANIM_LOOP, 20, 0.0f);
            if (mApp->IsIZombieLevel())
            {
                aBodyReanim->mAnimRate = 0;
            }

            mLaunchCounter = 1;
        }
    }
    else if (mState == PlantState::STATE_CACTUS_HIGH)
    {
        if (FindTargetZombie(mRow, PlantWeapon::WEAPON_PRIMARY) == nullptr)
        {
            mState = PlantState::STATE_CACTUS_LOWERING;
            PlayBodyReanim("anim_lower", ReanimLoopType::REANIM_PLAY_ONCE_AND_HOLD, 20, aBodyReanim->mDefinition->mFPS);
        }
    }
    else if (mState == PlantState::STATE_CACTUS_LOWERING)
    {
        if (aBodyReanim->mLoopCount > 0)
        {
            mState = PlantState::STATE_CACTUS_LOW;
            PlayIdleAnim(0.0f);
        }
    }
    else if (FindTargetZombie(mRow, PlantWeapon::WEAPON_PRIMARY))
    {
        mState = PlantState::STATE_CACTUS_RISING;
        PlayBodyReanim("anim_rise", ReanimLoopType::REANIM_PLAY_ONCE_AND_HOLD, 20, aBodyReanim->mDefinition->mFPS);
        mApp->PlayFoley(FoleyType::FOLEY_PLANTGROW);
    }
}

//0x461320
void Plant::UpdateChomper()
{
    Reanimation* aBodyReanim = mApp->ReanimationTryToGet(mBodyReanimID);
    if (mState == PlantState::STATE_READY && FindTargetZombie(mRow, PlantWeapon::WEAPON_PRIMARY))
    {
        PlayBodyReanim("anim_bite", ReanimLoopType::REANIM_PLAY_ONCE_AND_HOLD, 20, 24.0f);
        mState = PlantState::STATE_CHOMPER_BITING;
        mStateCountdown = 70;
    }
    else if (mState == PlantState::STATE_CHOMPER_BITING && mStateCountdown == 0)
    {
        mApp->PlayFoley(FoleyType::FOLEY_BIGCHOMP);

        if (mSeedType != SeedType::SEED_SUPERCHOMPER)
        {
            Zombie* aZombie = FindTargetZombie(mRow, PlantWeapon::WEAPON_PRIMARY);
            bool doBite = false;
            bool doMiss = true;
            if (aZombie)
            {
                doMiss = false;
                if (!aZombie->IsImmobilizied() && (aZombie->IsBouncingPogo() ||
                    aZombie->mZombiePhase == ZombiePhase::PHASE_POLEVAULTER_IN_VAULT || aZombie->mZombiePhase == ZombiePhase::PHASE_POLEVAULTER_PRE_VAULT))
                    doMiss = true;
                else if (aZombie->IsDeadOrDying()) doMiss = true;
                else if (aZombie->Health() >= 1800 ||
                    aZombie->mZombieType == ZombieType::ZOMBIE_BOSS)
                    doBite = true;
            }

            if (doBite)
            {
                mApp->PlayFoley(FoleyType::FOLEY_SPLAT);
                aZombie->TakeDamage(40, 0U);
                mState = PlantState::STATE_CHOMPER_BITING_MISSED;
            }
            else if (doMiss)
            {
                mState = PlantState::STATE_CHOMPER_BITING_MISSED;
            }
            else
            {
                aZombie->DieWithLoot();
                mState = PlantState::STATE_CHOMPER_BITING_GOT_ONE;
            }
        }
        // Super Chomper
        else
        {
            Rect aAttackRect = GetPlantAttackRect(PlantWeapon::WEAPON_PRIMARY);
            Zombie* aZombie = nullptr;
            bool gotOne = false;
            while (mBoard->IterateZombies(aZombie))
            {
                if ((aZombie->mRow == mRow || aZombie->mZombieType == ZombieType::ZOMBIE_BOSS))
                {
                    bool doBite = false;
                    bool doMiss = true;
                    Rect aZombieRect = aZombie->GetZombieRect();
                    if (aZombie->mZombiePhase == ZombiePhase::PHASE_DIGGER_WALKING)
                    {
                        aAttackRect.mX += 20;
                        aAttackRect.mWidth -= 20;
                    }
                    if (GetRectOverlap(aAttackRect, aZombieRect) > (Zombie::IsFootball(aZombie->mZombieType) ? -80 : -60))
                    {
                        doMiss = false;
                        if (!aZombie->IsImmobilizied() && (aZombie->IsBouncingPogo() ||
                            aZombie->mZombiePhase == ZombiePhase::PHASE_POLEVAULTER_IN_VAULT || aZombie->mZombiePhase == ZombiePhase::PHASE_POLEVAULTER_PRE_VAULT))
                            doMiss = true;
                        else if (aZombie->IsDeadOrDying()) doMiss = true;
                        else if (aZombie->Health() >= 1800 ||
                            aZombie->mZombieType == ZombieType::ZOMBIE_BOSS)
                            doBite = true;
                        else gotOne = true;
                    }
                    if (doBite)
                    {
                        mApp->PlayFoley(FoleyType::FOLEY_SPLAT);
                        aZombie->TakeDamage(40, 0U);
                    }
                    else if (!doMiss)
                    {
                        aZombie->DieWithLoot();
                    }
                }
            }
            if (gotOne) {
                mState = PlantState::STATE_CHOMPER_BITING_GOT_ONE;
            }
            else {
                mState = PlantState::STATE_CHOMPER_BITING_MISSED;
            }
        }
    }
    else if (mState == PlantState::STATE_CHOMPER_BITING_GOT_ONE)
    {
        if (aBodyReanim->mLoopCount > 0)
        {
            PlayBodyReanim("anim_chew", ReanimLoopType::REANIM_LOOP, 0, 15.0f);
            if (mApp->IsIZombieLevel())
            {
                aBodyReanim->mAnimRate = 0;
            }

            mState = PlantState::STATE_CHOMPER_DIGESTING;
            mStateCountdown = 4000;
        }
    }
    else if (mState == PlantState::STATE_CHOMPER_DIGESTING)
    {
        if (mStateCountdown == 0)
        {
            PlayBodyReanim("anim_swallow", ReanimLoopType::REANIM_PLAY_ONCE_AND_HOLD, 20, 12.0f);
            mState = PlantState::STATE_CHOMPER_SWALLOWING;
        }
    }
    else if ((mState == PlantState::STATE_CHOMPER_SWALLOWING || mState == PlantState::STATE_CHOMPER_BITING_MISSED) && aBodyReanim->mLoopCount > 0)
    {
        PlayIdleAnim(aBodyReanim->mDefinition->mFPS);
        mState = PlantState::STATE_READY;
    }
}

//0x4615D0
MagnetItem* Plant::GetFreeMagnetItem()
{
    if (mSeedType == SeedType::SEED_GOLD_MAGNET)
    {
        for (int i = 0; i < MAX_MAGNET_ITEMS; i++)
        {
            if (mMagnetItems[i].mItemType == MagnetItemType::MAGNET_ITEM_NONE)
            {
                return &mMagnetItems[i];
            }
        }

        return nullptr;
    }

    return &mMagnetItems[0];
}

//0x460610
void Plant::MagnetShroomAttactItem(Zombie* theZombie)
{
    mState = PlantState::STATE_MAGNETSHROOM_SUCKING;
    mStateCountdown = 1500;
    PlayBodyReanim("anim_shooting", ReanimLoopType::REANIM_PLAY_ONCE_AND_HOLD, 20, 12.0f);
    mApp->PlayFoley(FoleyType::FOLEY_MAGNETSHROOM);

    MagnetItem* aMagnetItem = GetFreeMagnetItem();
    if (theZombie->mHelmType == HelmType::HELMTYPE_PAIL)
    {
        int aDamageIndex = theZombie->GetHelmDamageIndex();

        theZombie->mHelmHealth = 0;
        theZombie->mHelmType = HelmType::HELMTYPE_NONE;
        theZombie->GetTrackPosition("anim_bucket", aMagnetItem->mPosX, aMagnetItem->mPosY);
        theZombie->ReanimShowPrefix("anim_bucket", RENDER_GROUP_HIDDEN);
        theZombie->ReanimShowPrefix("anim_hair", RENDER_GROUP_NORMAL);

        aMagnetItem->mPosX -= IMAGE_REANIM_ZOMBIE_BUCKET1->GetWidth() / 2;
        aMagnetItem->mPosY -= IMAGE_REANIM_ZOMBIE_BUCKET1->GetHeight() / 2;
        aMagnetItem->mDestOffsetX = RandRangeFloat(-10.0f, 10.0f) + 25.0f;
        aMagnetItem->mDestOffsetY = RandRangeFloat(-10.0f, 10.0f) + 20.0f;
        aMagnetItem->mItemType = (MagnetItemType)((int)MagnetItemType::MAGNET_ITEM_PAIL_1 + aDamageIndex);
    }
    else if (theZombie->mHelmType == HelmType::HELMTYPE_FOOTBALL)
    {
        int aDamageIndex = theZombie->GetHelmDamageIndex();

        theZombie->mHelmHealth = 0;
        theZombie->mHelmType = HelmType::HELMTYPE_NONE;
        theZombie->GetTrackPosition("zombie_football_helmet", aMagnetItem->mPosX, aMagnetItem->mPosY);
        theZombie->ReanimShowPrefix("zombie_football_helmet", RENDER_GROUP_HIDDEN);
        theZombie->ReanimShowPrefix("anim_hair", RENDER_GROUP_NORMAL);

        aMagnetItem->mPosX += 37.0f;
        aMagnetItem->mPosY -= 60.0f;
        aMagnetItem->mDestOffsetX = RandRangeFloat(-10.0f, 10.0f) + 20.0f;
        aMagnetItem->mDestOffsetY = RandRangeFloat(-10.0f, 10.0f) + 20.0f;
        aMagnetItem->mItemType = (MagnetItemType)((int)MagnetItemType::MAGNET_ITEM_FOOTBALL_HELMET_1 + aDamageIndex);
    }
    else if (theZombie->mHelmType == HelmType::HELMTYPE_HELMET)
    {
        int aDamageIndex = theZombie->GetHelmDamageIndex();

        theZombie->mHelmHealth = 0;
        theZombie->mHelmType = HelmType::HELMTYPE_NONE;
        theZombie->GetTrackPosition("anim_helmet", aMagnetItem->mPosX, aMagnetItem->mPosY);
        theZombie->ReanimShowPrefix("anim_helmet", RENDER_GROUP_HIDDEN);

        aMagnetItem->mPosX -= IMAGE_REANIM_ZOMBIE_GARGANTUAR_HELMET1->GetWidth() / 2;
        aMagnetItem->mPosY -= IMAGE_REANIM_ZOMBIE_GARGANTUAR_HELMET1->GetHeight() / 2;
        aMagnetItem->mDestOffsetX = RandRangeFloat(-10.0f, 10.0f) + 20.0f;
        aMagnetItem->mDestOffsetY = RandRangeFloat(-10.0f, 10.0f) + 20.0f;
        aMagnetItem->mItemType = (MagnetItemType)((int)MagnetItemType::MAGNET_ITEM_HELMET_1 + aDamageIndex);
    }
    else if (theZombie->mHelmType == HelmType::HELMTYPE_POT)
    {
        int aDamageIndex = theZombie->GetHelmDamageIndex();

        theZombie->mHelmHealth = 0;
        theZombie->mHelmType = HelmType::HELMTYPE_NONE;
        theZombie->GetTrackPosition("anim_cone", aMagnetItem->mPosX, aMagnetItem->mPosY);
        theZombie->ReanimShowPrefix("anim_cone", RENDER_GROUP_HIDDEN);

        aMagnetItem->mPosX -= IMAGE_REANIM_ZOMBIE_POT1->GetWidth() / 2;
        aMagnetItem->mPosY -= IMAGE_REANIM_ZOMBIE_POT1->GetHeight() / 2;
        aMagnetItem->mDestOffsetX = RandRangeFloat(-10.0f, 10.0f) + 20.0f;
        aMagnetItem->mDestOffsetY = RandRangeFloat(-10.0f, 10.0f) + 20.0f;
        aMagnetItem->mItemType = (MagnetItemType)((int)MagnetItemType::MAGNET_ITEM_POT_1 + aDamageIndex);
    }
    else if (theZombie->mShieldType == ShieldType::SHIELDTYPE_DOOR)
    {
        int aDamageIndex = theZombie->GetShieldDamageIndex();

        theZombie->DetachShield();
        theZombie->mZombiePhase = ZombiePhase::PHASE_ZOMBIE_NORMAL;
        if (!theZombie->mIsEating)
        {
            TOD_ASSERT(theZombie->mZombieHeight == ZombieHeight::HEIGHT_ZOMBIE_NORMAL);
            theZombie->StartWalkAnim(0);
        }
        theZombie->GetTrackPosition("anim_screendoor", aMagnetItem->mPosX, aMagnetItem->mPosY);

        aMagnetItem->mPosX -= IMAGE_REANIM_ZOMBIE_SCREENDOOR1->GetWidth() / 2;
        aMagnetItem->mPosY -= IMAGE_REANIM_ZOMBIE_SCREENDOOR1->GetHeight() / 2;
        aMagnetItem->mDestOffsetX = RandRangeFloat(-10.0f, 10.0f) + 30.0f;
        aMagnetItem->mDestOffsetY = RandRangeFloat(-10.0f, 10.0f);
        aMagnetItem->mItemType = (MagnetItemType)((int)MagnetItemType::MAGNET_ITEM_DOOR_1 + aDamageIndex);
    }
    else if (theZombie->mShieldType == ShieldType::SHIELDTYPE_LADDER)
    {
        int aDamageIndex = theZombie->GetShieldDamageIndex();

        theZombie->DetachShield();

        aMagnetItem->mPosX = theZombie->mPosX + 31.0f;
        aMagnetItem->mPosY = theZombie->mPosY + 20.0f;
        aMagnetItem->mPosX -= IMAGE_REANIM_ZOMBIE_LADDER_5->GetWidth() / 2;
        aMagnetItem->mPosY -= IMAGE_REANIM_ZOMBIE_LADDER_5->GetHeight() / 2;
        aMagnetItem->mDestOffsetX = RandRangeFloat(-10.0f, 10.0f) + 30.0f;
        aMagnetItem->mDestOffsetY = RandRangeFloat(-10.0f, 10.0f);
        aMagnetItem->mItemType = (MagnetItemType)((int)MagnetItemType::MAGNET_ITEM_LADDER_1 + aDamageIndex);
    }
    else if (Zombie::IsPogo(theZombie->mZombieType))
    {
        theZombie->PogoBreak(16U);
        // ZombieDrawPosition aDrawPos;
        // theZombie->GetDrawPos(aDrawPos);
        theZombie->GetTrackPosition("Zombie_pogo_stick", aMagnetItem->mPosX, aMagnetItem->mPosY);

        aMagnetItem->mPosX += 40.0f - IMAGE_REANIM_ZOMBIE_LADDER_5->GetWidth() / 2;
        aMagnetItem->mPosY += 84.0f - IMAGE_REANIM_ZOMBIE_LADDER_5->GetHeight() / 2;
        aMagnetItem->mDestOffsetX = RandRangeFloat(-10.0f, 10.0f) + 30.0f;
        aMagnetItem->mDestOffsetY = RandRangeFloat(-10.0f, 10.0f);
        aMagnetItem->mItemType = theZombie->mHasArm ? MagnetItemType::MAGNET_ITEM_POGO_1 : MagnetItemType::MAGNET_ITEM_POGO_3;
    }
    else if (theZombie->mZombiePhase == ZombiePhase::PHASE_JACK_IN_THE_BOX_RUNNING)
    {
        theZombie->StopZombieSound();
        theZombie->PickRandomSpeed();
        theZombie->mZombiePhase = ZombiePhase::PHASE_ZOMBIE_NORMAL;
        theZombie->ReanimShowPrefix("Zombie_jackbox_box", RENDER_GROUP_HIDDEN);
        theZombie->ReanimShowPrefix("Zombie_jackbox_handle", RENDER_GROUP_HIDDEN);
        theZombie->GetTrackPosition("Zombie_jackbox_box", aMagnetItem->mPosX, aMagnetItem->mPosY);

        aMagnetItem->mPosX -= IMAGE_REANIM_ZOMBIE_JACKBOX_BOX->GetWidth() / 2;
        aMagnetItem->mPosY -= IMAGE_REANIM_ZOMBIE_JACKBOX_BOX->GetHeight() / 2;
        aMagnetItem->mDestOffsetX = RandRangeFloat(-10.0f, 10.0f) + 20.0f;
        aMagnetItem->mDestOffsetY = RandRangeFloat(-10.0f, 10.0f) + 15.0f;
        aMagnetItem->mItemType = MagnetItemType::MAGNET_ITEM_JACK_IN_THE_BOX;
    }
    else if (Zombie::IsDigger(theZombie->mZombieType))
    {
        theZombie->DiggerLoseAxe();
        theZombie->GetTrackPosition("Zombie_digger_pickaxe", aMagnetItem->mPosX, aMagnetItem->mPosY);

        aMagnetItem->mPosX -= IMAGE_REANIM_ZOMBIE_DIGGER_PICKAXE->GetWidth() / 2;
        aMagnetItem->mPosY -= IMAGE_REANIM_ZOMBIE_DIGGER_PICKAXE->GetHeight() / 2;
        aMagnetItem->mDestOffsetX = RandRangeFloat(-10.0f, 10.0f) + 45.0f;
        aMagnetItem->mDestOffsetY = RandRangeFloat(-10.0f, 10.0f) + 15.0f;
        aMagnetItem->mItemType = MagnetItemType::MAGNET_ITEM_PICK_AXE;
    }
}

//0x461CD0
bool Plant::DrawMagnetItemsOnTop()
{
    if (mSeedType == SeedType::SEED_GOLD_MAGNET)
    {
        for (int i = 0; i < MAX_MAGNET_ITEMS; i++)
        {
            if (mMagnetItems[i].mItemType != MagnetItemType::MAGNET_ITEM_NONE)
            {
                return true;
            }
        }

        return false;
    }

    if (mSeedType == SeedType::SEED_MAGNETSHROOM)
    {
        for (int i = 0; i < MAX_MAGNET_ITEMS; i++)
        {
            MagnetItem* aMagnetItem = &mMagnetItems[i];
            if (aMagnetItem->mItemType != MagnetItemType::MAGNET_ITEM_NONE)
            {
                SexyVector2 aVectorToPlant(mX + aMagnetItem->mDestOffsetX - aMagnetItem->mPosX, mY + aMagnetItem->mDestOffsetY - aMagnetItem->mPosY);
                if (aVectorToPlant.Magnitude() > 20.0f)
                {
                    return true;
                }
            }
        }

        return false;
    }
}

//0x461D90
void Plant::UpdateMagnetShroom()
{
    for (int i = 0; i < MAX_MAGNET_ITEMS; i++)
    {
        MagnetItem* aMagnetItem = &mMagnetItems[i];
        if (aMagnetItem->mItemType != MagnetItemType::MAGNET_ITEM_NONE)
        {
            SexyVector2 aVectorToPlant(mX + aMagnetItem->mDestOffsetX - aMagnetItem->mPosX, mY + aMagnetItem->mDestOffsetY - aMagnetItem->mPosY);
            if (aVectorToPlant.Magnitude() > 20.0f)
            {
                aMagnetItem->mPosX += aVectorToPlant.x * 0.05f;
                aMagnetItem->mPosY += aVectorToPlant.y * 0.05f;
            }
        }
    }

    if (mState == PlantState::STATE_MAGNETSHROOM_CHARGING)
    {
        if (mStateCountdown == 0)
        {
            mState = PlantState::STATE_READY;

            float aAnimRate = RandRangeFloat(10.0f, 15.0f);
            PlayBodyReanim("anim_idle", ReanimLoopType::REANIM_LOOP, 30, aAnimRate);
            if (mApp->IsIZombieLevel())
            {
                Reanimation* aBodyReanim = mApp->ReanimationTryToGet(mBodyReanimID);
                aBodyReanim->mAnimRate = 0.0f;
            }

            mMagnetItems[0].mItemType = MagnetItemType::MAGNET_ITEM_NONE;
        }
    }
    else if (mState == PlantState::STATE_MAGNETSHROOM_SUCKING)
    {
        Reanimation* aBodyReanim = mApp->ReanimationTryToGet(mBodyReanimID);
        if (aBodyReanim->mLoopCount > 0)
        {
            PlayBodyReanim("anim_nonactive_idle2", ReanimLoopType::REANIM_LOOP, 20, 2.0f);
            if (mApp->IsIZombieLevel())
            {
                aBodyReanim = mApp->ReanimationTryToGet(mBodyReanimID);
                aBodyReanim->mAnimRate = 0.0f;
            }

            mState = PlantState::STATE_MAGNETSHROOM_CHARGING;
        }
    }
    else
    {
        float aClosestDistance = 0.0f;
        Zombie* aClosestZombie = nullptr;

        Zombie* aZombie = nullptr;
        while (mBoard->IterateZombies(aZombie))
        {
            int aDiffY = aZombie->mRow - mRow;
            Rect aZombieRect = aZombie->GetZombieRect();

            if (aZombie->mMindControlled)
                continue;

            if (!aZombie->mHasHead)
                continue;

            if (aZombie->mZombieHeight != ZombieHeight::HEIGHT_ZOMBIE_NORMAL || aZombie->mZombiePhase == ZombiePhase::PHASE_RISING_FROM_GRAVE)
                continue;

            if (aZombie->IsDeadOrDying())
                continue;

            if (aZombieRect.mX > BOARD_WIDTH || aDiffY > 2 || aDiffY < -2)
                continue;

            if (aZombie->mZombiePhase == ZombiePhase::PHASE_DIGGER_TUNNELING ||
                aZombie->mZombiePhase == ZombiePhase::PHASE_DIGGER_STUNNED ||
                aZombie->mZombiePhase == ZombiePhase::PHASE_DIGGER_WALKING ||
                Zombie::IsPogo(aZombie->mZombieType))
            {
                if (!aZombie->mHasObject)
                    continue;
            }
            else if (!(aZombie->mHelmType == HelmType::HELMTYPE_PAIL ||
                aZombie->mHelmType == HelmType::HELMTYPE_FOOTBALL ||
                aZombie->mHelmType == HelmType::HELMTYPE_POT ||
                aZombie->mHelmType == HelmType::HELMTYPE_HELMET ||
                aZombie->mShieldType == ShieldType::SHIELDTYPE_DOOR ||
                aZombie->mShieldType == ShieldType::SHIELDTYPE_LADDER ||
                aZombie->mZombiePhase == ZombiePhase::PHASE_JACK_IN_THE_BOX_RUNNING))
                continue;

            int aRadius = aZombie->mIsEating ? 320 : 270;
            if (GetCircleRectOverlap(mX, mY + 20, aRadius, aZombieRect))
            {
                float aDistance = Distance2D(mX, mY, aZombieRect.mX, aZombieRect.mY);
                aDistance += abs(aDiffY) * 80.0f;

                if (aClosestZombie == nullptr || aDistance < aClosestDistance)
                {
                    aClosestZombie = aZombie;
                    aClosestDistance = aDistance;
                }
            }
        }

        if (aClosestZombie)
        {
            MagnetShroomAttactItem(aClosestZombie);
            return;
        }

        ////////////////////

        float aClosestLadderDist = 0.0f;
        GridItem* aClosestLadder = nullptr;
        
        GridItem* aGridItem = nullptr;
        while (mBoard->IterateGridItems(aGridItem))
        {
            if (aGridItem->mGridItemType == GridItemType::GRIDITEM_LADDER)
            {
                int aDiffX = abs(aGridItem->mGridX - mPlantCol);
                int aDiffY = abs(aGridItem->mGridY - mRow);
                int aSquareDistance = max(aDiffX, aDiffY);
                if (aSquareDistance <= 2)
                {
                    float aDistance = aSquareDistance + aDiffY * 0.05f;
                    if (aClosestLadder == nullptr || aDistance < aClosestLadderDist)
                    {
                        aClosestLadder = aGridItem;
                        aClosestLadderDist = aDistance;
                    }
                }
            }
        }

        if (aClosestLadder)
        {
            mState = PlantState::STATE_MAGNETSHROOM_SUCKING;
            mStateCountdown = 1500;
            PlayBodyReanim("anim_shooting", ReanimLoopType::REANIM_PLAY_ONCE_AND_HOLD, 20, 12.0f);
            mApp->PlayFoley(FoleyType::FOLEY_MAGNETSHROOM);

            aClosestLadder->GridItemDie();

            MagnetItem* aMagnetItem = GetFreeMagnetItem();
            aMagnetItem->mPosX = mBoard->GridToPixelX(aClosestLadder->mGridX, aClosestLadder->mGridY) + 40;
            aMagnetItem->mPosY = mBoard->GridToPixelY(aClosestLadder->mGridX, aClosestLadder->mGridY);
            aMagnetItem->mDestOffsetX = RandRangeFloat(-10.0f, 10.0f) + 10.0f;
            aMagnetItem->mDestOffsetY = RandRangeFloat(-10.0f, 10.0f);
            aMagnetItem->mItemType = MagnetItemType::MAGNET_ITEM_LADDER_PLACED;
        }
    }
}

//0x462390
Coin* Plant::FindGoldMagnetTarget()
{
    Coin* aClosestCoin = nullptr;
    float aClosestDistance = 0.0f;

    Coin* aCoin = nullptr;
    while (mBoard->IterateCoins(aCoin))
    {
        if (aCoin->IsMoney() || aCoin->IsSun())
        {
            if (aCoin->mCoinMotion != CoinMotion::COIN_MOTION_FROM_PRESENT && !aCoin->mIsBeingCollected && aCoin->mCoinAge >= 50) {
                float aDistance = Distance2D(mX + mWidth / 2, mY + mHeight / 2, aCoin->mPosX + aCoin->mWidth / 2, aCoin->mPosY + aCoin->mHeight / 2);
                if (aClosestCoin == nullptr || aDistance < aClosestDistance)
                {
                    aClosestCoin = aCoin;
                    aClosestDistance = aDistance;
                }
            }
        }
    }

    return aClosestCoin;
}

//0x4624B0
void Plant::GoldMagnetFindTargets()
{
    if (GetFreeMagnetItem() == nullptr)
    {
        TOD_ASSERT();
        return;
    }

    for (;;)
    {
        MagnetItem* aMagnetItem = GetFreeMagnetItem();
        if (aMagnetItem == nullptr)
            break;

        Coin* aCoin = FindGoldMagnetTarget();
        if (aCoin == nullptr)
            break;

        aMagnetItem->mPosX = aCoin->mPosX + 15.0f;
        aMagnetItem->mPosY = aCoin->mPosY + 15.0f;
        aMagnetItem->mDestOffsetX = RandRangeFloat(20.0f, 40.0f);
        aMagnetItem->mDestOffsetY = RandRangeFloat(-20.0f, 0.0f) + 20.0f;

        switch (aCoin->mType)
        {
        case CoinType::COIN_SILVER:     aMagnetItem->mItemType = MagnetItemType::MAGNET_ITEM_SILVER_COIN;   break;
        case CoinType::COIN_GOLD:       aMagnetItem->mItemType = MagnetItemType::MAGNET_ITEM_GOLD_COIN;     break;
        case CoinType::COIN_DIAMOND:    aMagnetItem->mItemType = MagnetItemType::MAGNET_ITEM_DIAMOND;       break;
        case CoinType::COIN_SUN:        aMagnetItem->mItemType = MagnetItemType::MAGNET_ITEM_SUN;           break;
        case CoinType::COIN_LARGESUN:   aMagnetItem->mItemType = MagnetItemType::MAGNET_ITEM_LARGESUN;      break;
        case CoinType::COIN_SMALLSUN:   aMagnetItem->mItemType = MagnetItemType::MAGNET_ITEM_SMALLSUN;      break;
        default:                        TOD_ASSERT();                                                       return;
        }
        
        aCoin->Die();
    }
}

//0x4625D0
bool Plant::IsAGoldMagnetAboutToSuck()
{
    Plant* aPlant = nullptr;
    while (mBoard->IteratePlants(aPlant))
    {
        if (!aPlant->NotOnGround() && aPlant->mSeedType == SeedType::SEED_GOLD_MAGNET && aPlant->mState == PlantState::STATE_MAGNETSHROOM_SUCKING)
        {
            Reanimation* aBodyReanim = mApp->ReanimationGet(aPlant->mBodyReanimID);
            if (aBodyReanim->mAnimTime < 0.5f)
            {
                return true;
            }
        }
    }

    return false;
}

//0x462680
void Plant::UpdateGoldMagnetShroom()
{
    Reanimation* aBodyReanim = mApp->ReanimationGet(mBodyReanimID);

    bool aIsSuckingCoin = false;
    for (int i = 0; i < MAX_MAGNET_ITEMS; i++)
    {
        MagnetItem* aMagnetItem = &mMagnetItems[i];
        if (aMagnetItem->mItemType != MagnetItemType::MAGNET_ITEM_NONE)
        {
            SexyVector2 aVectorToPlant(mX + aMagnetItem->mDestOffsetX - aMagnetItem->mPosX, mY + aMagnetItem->mDestOffsetY - aMagnetItem->mPosY);
            float aDistance = aVectorToPlant.Magnitude();
            if (aDistance < 20.0f)
            {
                CoinType aCoinType;
                switch (aMagnetItem->mItemType)
                {
                case MagnetItemType::MAGNET_ITEM_SILVER_COIN:   aCoinType = CoinType::COIN_SILVER;      break;
                case MagnetItemType::MAGNET_ITEM_GOLD_COIN:     aCoinType = CoinType::COIN_GOLD;        break;
                case MagnetItemType::MAGNET_ITEM_DIAMOND:       aCoinType = CoinType::COIN_DIAMOND;     break;
                case MagnetItemType::MAGNET_ITEM_SUN:           aCoinType = CoinType::COIN_SUN;         break;
                case MagnetItemType::MAGNET_ITEM_SMALLSUN:      aCoinType = CoinType::COIN_SMALLSUN;    break;
                case MagnetItemType::MAGNET_ITEM_LARGESUN:      aCoinType = CoinType::COIN_LARGESUN;    break;
                default:                                        TOD_ASSERT();                           return;
                }
                int aValue = Coin::GetCoinValue(aCoinType);
                if (Coin::IsMoney(aCoinType)) {
                    mApp->mPlayerInfo->AddCoins(aValue);
                    mBoard->mCoinsCollected += aValue;
                    mApp->PlayFoley(FoleyType::FOLEY_COIN);
                }
                else {
                    mBoard->AddSunMoney(aValue);
                    mApp->PlayFoley(FoleyType::FOLEY_SUN);
                }
                

                aMagnetItem->mItemType = MagnetItemType::MAGNET_ITEM_NONE;
            }
            else
            {
                float aSpeed = TodAnimateCurveFloatTime(30.0f, 0.0f, aDistance, 0.02f, 0.05f, TodCurves::CURVE_LINEAR);
                aMagnetItem->mPosX += aVectorToPlant.x * aSpeed;
                aMagnetItem->mPosY += aVectorToPlant.y * aSpeed;

                aIsSuckingCoin = true;
            }
        }
    }

    if (mState == PlantState::STATE_MAGNETSHROOM_CHARGING)
    {
        if (mStateCountdown == 0)
        {
            mState = PlantState::STATE_READY;
        }
    }
    else if (mState == PlantState::STATE_MAGNETSHROOM_SUCKING)
    {
        if (aBodyReanim->ShouldTriggerTimedEvent(0.4f))
        {
            mApp->PlayFoley(FoleyType::FOLEY_MAGNETSHROOM);
            GoldMagnetFindTargets();
        }

        if (aBodyReanim->mLoopCount > 0 && !aIsSuckingCoin)
        {
            PlayIdleAnim(14.0f);
            mState = PlantState::STATE_MAGNETSHROOM_CHARGING;
            mStateCountdown = RandRangeInt(200, 300);
        }
    }
    else if (!IsAGoldMagnetAboutToSuck() && Sexy::Rand(50) == 0 && FindGoldMagnetTarget())
    {
        mBoard->ShowCoinBank();
        mState = PlantState::STATE_MAGNETSHROOM_SUCKING;
        PlayBodyReanim("anim_attract", ReanimLoopType::REANIM_PLAY_ONCE_AND_HOLD, 20, 12.0f);
    }
}

//0x4629F0
void Plant::RemoveEffects()
{
    mApp->RemoveParticle(mParticleID);
    mApp->RemoveReanimation(mBodyReanimID);
    mApp->RemoveReanimation(mHeadReanimID);
    mApp->RemoveReanimation(mHeadReanimID2);
    mApp->RemoveReanimation(mHeadReanimID3);
    mApp->RemoveReanimation(mLightReanimID);
    mApp->RemoveReanimation(mBlinkReanimID);
    mApp->RemoveReanimation(mSleepingReanimID);
}

//0x462B80
void Plant::Squish()
{
    if (NotOnGround())
        return;

    if (!mIsAsleep && mIceTrapCounter <= 0)
    {
        if (mSeedType == SeedType::SEED_CHERRYBOMB || mSeedType == SeedType::SEED_JALAPENO ||
            mSeedType == SeedType::SEED_DOOMSHROOM || mSeedType == SeedType::SEED_ICESHROOM || 
            mSeedType == SeedType::SEED_ICEBERGLETTUCE || mSeedType == SeedType::SEED_PICKLEDPEPPER || 
            mSeedType == SeedType::SEED_CHILLYPEPPER || mSeedType == SeedType::SEED_SEASTARFRUIT
            )
        {
            DoSpecial();
            return;
        }
        else if ((mSeedType == SeedType::SEED_POTATOMINE || mSeedType == SeedType::SEED_POPCORN || mSeedType == SeedType::SEED_GOLDBLOOM) && mState != PlantState::STATE_NOTREADY)
        {
            DoSpecial();
            return;
        }
    }

    if ((mSeedType == SeedType::SEED_SQUASH || mSeedType == SeedType::SEED_SPOOKYSQUASH) && mState != PlantState::STATE_NOTREADY)
        return;

    // Walls Survive

    Plant* aNormalPlant = mBoard->GetTopPlantAt(mPlantCol, mRow, PlantPriority::TOPPLANT_ONLY_NORMAL_POSITION);
    Plant* aPumpkinPlant = mBoard->GetTopPlantAt(mPlantCol, mRow, PlantPriority::TOPPLANT_ONLY_PUMPKIN);

    if (aNormalPlant != nullptr && aNormalPlant->mPlantHealth > 2000 && aNormalPlant != this && aPumpkinPlant != this) return;
    if (aPumpkinPlant != nullptr && aPumpkinPlant->mPlantHealth > 2000 && aPumpkinPlant != this) return;

    if (mPlantHealth > 2000) 
    {
        mPlantHealth -= 2000;
        return;
    }

    if (mApp->GetConfigCheckbox(false, CONFIG_CHECKBOX_PLANT_SOUNDS))
    {
        if (mDeathSound == DeathSoundType::DEATH_SOUND_SUNFLOWER) mApp->PlayFoley(FoleyType::FOLEY_SUNFLOWER_DEATH);
        if (mDeathSound == DeathSoundType::DEATH_SOUND_PEASHOOTER) mApp->PlayFoley(FoleyType::FOLEY_PEASHOOTER_DEATH);
    }
    

    mBoard->mPlantsEaten++;

    mRenderOrder = Board::MakeRenderOrder(RenderLayer::RENDER_LAYER_GRAVE_STONE, mRow, 8);
    mSquished = true;
    mDisappearCountdown = 500;
    mApp->PlayFoley(FoleyType::FOLEY_SQUISH);
    RemoveEffects();

    GridItem* aLadder = mBoard->GetLadderAt(mPlantCol, mRow);
    if (aLadder)
    {
        aLadder->GridItemDie();
    }

    if (mApp->IsIZombieLevel())
    {
        mBoard->mChallenge->IZombiePlantDropRemainingSun(this);
    }
}

void Plant::Eaten() {
    if (mSeedType == SeedType::SEED_INFINUT && !mBoard->IsPoolSquare(mPlantCol, mRow) && !mBoard->GetFlowerPotAt(mPlantCol, mRow) && !mBoard->GetWaterPotAt(mPlantCol, mRow))
    {
        mState = PlantState::STATE_INVULNERABLE;
        mStateCountdown = 1500;
        mPlantHealth = 1;
        return;
    }
    mBoard->mPlantsEaten++;
    if (mSeedType == SeedType::SEED_EXPLODE_O_NUT || mSeedType == SeedType::SEED_HAZYNUT || mSeedType == SeedType::SEED_FROSTACHIO) {
        DoSpecial();
        return;
    }
    if (mApp->GetConfigCheckbox(false, CONFIG_CHECKBOX_PLANT_SOUNDS))
    {
        if (mDeathSound == DeathSoundType::DEATH_SOUND_SUNFLOWER) mApp->PlayFoley(FoleyType::FOLEY_SUNFLOWER_DEATH);
        if (mDeathSound == DeathSoundType::DEATH_SOUND_PEASHOOTER) mApp->PlayFoley(FoleyType::FOLEY_PEASHOOTER_DEATH);
    }
    
    Die();

}

//0x462CE0
void Plant::UpdateBowling()
{
    Reanimation* aBodyReanim = mApp->ReanimationTryToGet(mBodyReanimID);
    if (aBodyReanim && aBodyReanim->TrackExists("_ground"))
    {
        float aSpeed = aBodyReanim->GetTrackVelocity("_ground");
        if (mSeedType == SeedType::SEED_GIANT_WALLNUT)
            aSpeed *= 2;
        if (mSeedType == SeedType::SEED_INFINUT && mLaunchCounter % 2 == 1)
            aSpeed *= -1;
        if (mSeedType == SeedType::SEED_INFINUT)
            aSpeed = (int)(1.5 * (float)aSpeed);

        mX -= aSpeed;
        if (mX > WIDE_BOARD_WIDTH)
            Die();
    }

    if (mState == PlantState::STATE_BOWLING_UP)
    {
        mY -= 2;
    }
    else if (mState == PlantState::STATE_BOWLING_DOWN)
    {
        mY += 2;
    }
    int aDistToGrid = mBoard->GridToPixelY(0, mRow) - mY;
    if (aDistToGrid < -2 || aDistToGrid > 2)
        return;

    PlantState aNewState = mState;
    if (mState == PlantState::STATE_BOWLING_UP && mRow <= 0)
    {
        aNewState = PlantState::STATE_BOWLING_DOWN;
    }
    else if (mState == PlantState::STATE_BOWLING_DOWN && mRow >= (mBoard->StageHas6Rows() ? 5 : 4))
    {
        aNewState = PlantState::STATE_BOWLING_UP;
    }
    if (mSeedType == SeedType::SEED_INFINUT && mLaunchCounter % 2 == 1 && mX <= LAWN_XMIN)
    {
        mLaunchCounter--;
    }

    Zombie* aZombie = FindTargetZombie(mRow, PlantWeapon::WEAPON_PRIMARY);
    if (aZombie)
    {
        int aPosX = mX + mWidth / 2;
        int aPosY = mY + mHeight / 2;

        if (mSeedType == SeedType::SEED_EXPLODE_O_NUT || mSeedType == SeedType::SEED_HAZYNUT || mSeedType == SeedType::SEED_FROSTACHIO)
        {
            DoSpecial();

            return;
        }

        mApp->PlayFoley(FoleyType::FOLEY_BOWLINGIMPACT);
        mBoard->ShakeBoard(1, -2);

        if (mSeedType == SeedType::SEED_GIANT_WALLNUT)
            aZombie->TakeBodyDamage(1800, 0U);
        else if (Zombie::IsJack(aZombie->mZombieType))
        {
            aZombie->mMindControlled = true;
            if (aZombie->mZombiePhase == ZombiePhase::PHASE_JACK_IN_THE_BOX_RUNNING) aZombie->mPhaseCounter = 0;
        }
        else if (aZombie->mShieldType != ShieldType::SHIELDTYPE_NONE && mState != PlantState::STATE_BOWLING_DOWN && mState != PlantState::STATE_BOWLING_UP)
            aZombie->TakeShieldDamage(450, 0U);
        else if (aZombie->mArmorType != ArmorType::ARMORTYPE_NONE)
            aZombie->TakeArmorDamage(450, 0U);
        else if (aZombie->mHelmType != HelmType::HELMTYPE_NONE)
        {
            if (aZombie->mHelmType == HelmType::HELMTYPE_PAIL || aZombie->mHelmType == HelmType::HELMTYPE_POT || aZombie->mHelmType == HelmType::HELMTYPE_HELMET)
                mApp->PlayFoley(FoleyType::FOLEY_SHIELD_HIT);
            else if (aZombie->mHelmType == HelmType::HELMTYPE_TRAFFIC_CONE)
                mApp->PlayFoley(FoleyType::FOLEY_PLASTIC_HIT);
            aZombie->TakeHelmDamage(900, 0U);
        }
        else
            aZombie->TakeBodyDamage(1800, 0U);

        if ((!mApp->IsFirstTimeAdventureMode() || mApp->mPlayerInfo->GetLevel() > 10) && mSeedType != SeedType::SEED_GIANT_WALLNUT)
        {
            mLaunchCounter++;
            if (mLaunchCounter == 2)
            {
                mApp->PlayFoley(FoleyType::FOLEY_SPAWN_SUN);
                mBoard->AddCoin(aPosX, aPosY, CoinType::COIN_SILVER, CoinMotion::COIN_MOTION_COIN);
            }
            else if (mLaunchCounter == 3)
            {
                mApp->PlayFoley(FoleyType::FOLEY_SPAWN_SUN);
                mBoard->AddCoin(aPosX - 5.0f, aPosY, CoinType::COIN_SILVER, CoinMotion::COIN_MOTION_COIN);
                mBoard->AddCoin(aPosX + 5.0f, aPosY, CoinType::COIN_SILVER, CoinMotion::COIN_MOTION_COIN);
            }
            else if (mLaunchCounter == 4)
            {
                mApp->PlayFoley(FoleyType::FOLEY_SPAWN_SUN);
                mBoard->AddCoin(aPosX - 10.0f, aPosY, CoinType::COIN_SILVER, CoinMotion::COIN_MOTION_COIN);
                mBoard->AddCoin(aPosX, aPosY, CoinType::COIN_SILVER, CoinMotion::COIN_MOTION_COIN);
                mBoard->AddCoin(aPosX + 10.0f, aPosY, CoinType::COIN_SILVER, CoinMotion::COIN_MOTION_COIN);
            }
            else if (mLaunchCounter >= 5)
            {
                mApp->PlayFoley(FoleyType::FOLEY_SPAWN_SUN);
                mBoard->AddCoin(aPosX, aPosY, CoinType::COIN_GOLD, CoinMotion::COIN_MOTION_COIN);
            }
        }

        if (mSeedType == SEED_INFINUT)
        {
            if (mRow == 4)
                aNewState = PlantState::STATE_BOWLING_UP;
            else if (mRow == 0)
                aNewState = PlantState::STATE_BOWLING_DOWN;
            else if (mState == PlantState::STATE_BOWLING_UP || mState == PlantState::STATE_BOWLING_DOWN)
                aNewState = mState;
            else 
                aNewState = Sexy::Rand(2) ? PlantState::STATE_BOWLING_UP : PlantState::STATE_BOWLING_DOWN;
        }
        else if (mSeedType != SeedType::SEED_GIANT_WALLNUT)
        {
            if (mRow == 4 || mState == PlantState::STATE_BOWLING_DOWN)
                aNewState = PlantState::STATE_BOWLING_UP;
            else if (mRow == 0 || mState == PlantState::STATE_BOWLING_UP)
                aNewState = PlantState::STATE_BOWLING_DOWN;
            else
                aNewState = Sexy::Rand(2) ? PlantState::STATE_BOWLING_UP : PlantState::STATE_BOWLING_DOWN;
        }
    }

    if (aNewState == PlantState::STATE_BOWLING_UP)
    {
        mRow--;
        mState = PlantState::STATE_BOWLING_UP;
        mRenderOrder = CalcRenderOrder();
    }
    else if (aNewState == PlantState::STATE_BOWLING_DOWN)
    {
        mState = PlantState::STATE_BOWLING_DOWN;
        mRenderOrder = CalcRenderOrder();
        mRow++;
    }
}

//0x463150
void Plant::UpdateAbilities()
{
    if (!IsInPlay())
        return;

    if (mState == PlantState::STATE_DOINGSPECIAL || mSquished)
    {
        mDisappearCountdown--;
        if (mDisappearCountdown < 0)
        {
            Die();
            return;
        }
    }

    if (mWakeUpCounter > 0)
    {
        mWakeUpCounter--;
        if (mWakeUpCounter == 60)
        {
            mApp->PlayFoley(FoleyType::FOLEY_WAKEUP);
        }
        if (mWakeUpCounter == 0)
        {
            SetSleeping(false);
        }
    }

    if (mIsAsleep || mSquished || mOnBungeeState != PlantOnBungeeState::NOT_ON_BUNGEE)
        return;
    
    UpdateShooting();

    if (mStateCountdown > 0)
        mStateCountdown--;

    if ((mApp->IsWallnutBowlingLevel() && IsWallNut(mSeedType)) || mSeedType == SeedType::SEED_BOWLNUT)
    {
        UpdateBowling();
        return;
    }

    if (mSeedType == SeedType::SEED_SQUASH || mSeedType == SeedType::SEED_SPOOKYSQUASH)         UpdateSquash();
    else if (mSeedType == SeedType::SEED_DOOMSHROOM)                                            UpdateDoomShroom();
    else if (mSeedType == SeedType::SEED_ICESHROOM)                                             UpdateIceShroom();
    else if (mSeedType == SeedType::SEED_CHOMPER || mSeedType == SeedType::SEED_SUPERCHOMPER)   UpdateChomper();
    else if (mSeedType == SeedType::SEED_BLOVER)                                                UpdateBlover();
    else if (mSeedType == SeedType::SEED_FLOWERPOT || mSeedType == SeedType::SEED_LILYPAD
        || mSeedType == SeedType::SEED_WATERPOT || mSeedType == SeedType::SEED_COFFEEPOT)       UpdateUnderPlant();
    else if (mSeedType == SeedType::SEED_IMITATER)                                              UpdateImitater();
    else if (mSeedType == SeedType::SEED_INSTANT_COFFEE 
        || mSeedType == SeedType::SEED_HOTPOTATO)                                               UpdateCoffeeBean();
    else if (mSeedType == SeedType::SEED_UMBRELLA)                                              UpdateUmbrella();
    else if (mSeedType == SeedType::SEED_COBCANNON)                                             UpdateCobCannon();
    else if (mSeedType == SeedType::SEED_CACTUS)                                                UpdateCactus();
    else if (mSeedType == SeedType::SEED_MAGNETSHROOM)                                          UpdateMagnetShroom();
    else if (mSeedType == SeedType::SEED_GOLD_MAGNET)                                           UpdateGoldMagnetShroom();
    else if (mSeedType == SeedType::SEED_SUNSHROOM)                                             UpdateSunShroom();
    else if (MakesSun(mSeedType) 
        || mSeedType == SeedType::SEED_MARIGOLD 
        || mSeedType == SeedType::SEED_SHADOWFLOWER)                                            UpdateProductionPlant();
    else if (mSeedType == SeedType::SEED_GRAVEBUSTER)                                           UpdateGraveBuster();
    else if (mSeedType == SeedType::SEED_TORCHWOOD && !mIsBoosted)                              UpdateTorchwood();
    else if (mSeedType == SeedType::SEED_FLAMEWOOD || mSeedType == SeedType::SEED_TORCHWOOD)    UpdateFlamewood();
    else if (mSeedType == SeedType::SEED_POTATOMINE || mSeedType == SeedType::SEED_POPCORN)     UpdatePotato();
    else if (mSeedType == SeedType::SEED_SPIKEWEED || mSeedType == SeedType::SEED_SPIKEROCK)    UpdateSpikeweed();
    else if (mSeedType == SeedType::SEED_BEET 
        || mSeedType == SeedType::SEED_BEETBOXER
        || mSeedType == SeedType::SEED_BONKCHOY)                                                UpdateMelee();
    else if (mSeedType == SeedType::SEED_TANGLEKELP)                                            UpdateTanglekelp();
    else if (mSeedType == SeedType::SEED_SCAREDYSHROOM
        || mSeedType == SeedType::SEED_SHAMROCK
        || mSeedType == SeedType::SEED_SHAMROCKSTAR)                                            UpdateScaredyShroom();
    else if (mSeedType == SeedType::SEED_VINENUT || mSeedType == SeedType::SEED_FIRESHROOM)     UpdateVineNut();
    else if (mSeedType == SeedType::SEED_PLASMAPEA)                                             UpdateChargingShooter();
    else if (mSeedType == SeedType::SEED_ICEBERGLETTUCE)                                        UpdateIceberg();
    else if (mSeedType == SeedType::SEED_GOLDBLOOM)                                             UpdateGoldbloom();
    else if (mSeedType == SeedType::SEED_INFINUT)                                               UpdateInfiNut();
    if (IsFakePlantFood(mSeedType))                                                             UpdateFakePlantFood();
    if (mSubclass == PlantSubClass::SUBCLASS_SHOOTER)                                           UpdateShooter();
    if (mDoSpecialCountdown > 0)
    {
        mDoSpecialCountdown--;
        if (mDoSpecialCountdown == 0)
        {
            DoSpecial();
        }
    }
}

void Plant::UpdateCounters()
{
    if (mBoostCounter > 0)
    {
        mBoostCounter--;
        if (mBoostCounter <= 0) {
            mIsBoosted = false;
        }
        else
        {
            mIsBoosted = true;
        }
    }
    
    if (mBoard && !IsWarm(mSeedType) && mBoard->FindWarmPlant(mPlantCol, mRow))
    {
        mWarmCounter = max(mWarmCounter, RandRangeInt(400, 600));
    }
        

    if (mWarmCounter > 0 || mIsBoosted)
    {
        if (mWarmCounter > 0) mWarmCounter--;

        if (mChilledCounter > 0)
        {
            mChilledCounter -= 2;
            if (mChilledCounter < 0) mChilledCounter = 0;
        }

        if (mIceTrapCounter != 0)
        {
            mIceTrapCounter = 0;
            AddAttachedParticle(75, 106, 0, ParticleEffect::PARTICLE_ICE_TRAP_RELEASE);
        }
    }
    else if (mBoard && mBoard->StageIsWinter() && CanBeChilled(mSeedType))
    {
        if (mChilledCounter < 2000)
            mChilledCounter++;
        else if (mIceTrapCounter < 300)
            mIceTrapCounter++;
    }
    else
    {
        if (mChilledCounter > 0)
            mChilledCounter--;
        if (mIceTrapCounter > 0)
        {
            mIceTrapCounter--;
            if (mIceTrapCounter == 0)
            {
                AddAttachedParticle(75, 106, 0, ParticleEffect::PARTICLE_ICE_TRAP_RELEASE);
            }
        }
    }

    if (mPoisonCounter > 0)
    {
        mPoisonCounter--;
        if (mPoisonRate > 0) {
            mPoisonRate--;
        }
        else {
            mPoisonRate = BASE_POISON_TICKS;
            TakeDamage(mPoisonDamage, 33U);
        }
        if (mPoisonCounter == 0)
        {
            mPoisonDamage = BASE_POISON_DAMAGE;
            mPoisonRate = BASE_POISON_TICKS;
        }
    }
}

//0x463420
bool Plant::IsPartOfUpgradableTo(SeedType theUpgradedType)
{
    if (theUpgradedType == SeedType::SEED_COBCANNON && mSeedType == SeedType::SEED_KERNELPULT)
    {
        return mBoard->IsValidCobCannonSpot(mPlantCol, mRow) || mBoard->IsValidCobCannonSpot(mPlantCol - 1, mRow);
    }

    return IsUpgradableTo(theUpgradedType);
}

//0x463470
bool Plant::IsUpgradableTo(SeedType theUpgradedType)
{
    if (theUpgradedType == SeedType::SEED_COBCANNON && mSeedType == SeedType::SEED_KERNELPULT)
    {
        return mBoard->IsValidCobCannonSpot(mPlantCol, mRow);
    }
    return false;
}

//0x4635C0
void Plant::UpdateReanimColor()
{
    if (!IsOnBoard())
        return;

    Reanimation* aBodyReanim = mApp->ReanimationTryToGet(mBodyReanimID);
    if (aBodyReanim == nullptr)
        return;

    SeedType aSeedType = mBoard->GetSeedTypeInCursor();
    Color aColorOverride;

    bool isOnGlove = false;
    if (mBoard->mCursorObject->mCursorType == CursorType::CURSOR_TYPE_PLANT_FROM_GLOVE)
    {
        Plant* aPlant = mBoard->mPlants.DataArrayTryToGet((unsigned int)mBoard->mCursorObject->mGlovePlantID);
        if (aPlant && aPlant->mPlantCol == mPlantCol && aPlant->mRow == mRow)
        {
            isOnGlove = true;
        }
    }

    if (isOnGlove)
    {
        aColorOverride = Color(128, 128, 128);
    }
    else if (IsPartOfUpgradableTo(aSeedType) && mBoard->CanPlantAt(mPlantCol, mRow, aSeedType) == PLANTING_OK)
    {
        aColorOverride = GetFlashingColor(mBoard->mMainCounter, 90);
    }
    else if (mPlantFoodReady)
    {
        aColorOverride = GetFlashingColor(mBoard->mMainCounter, 90);
    }
    else if (aSeedType == SeedType::SEED_COBCANNON && mSeedType == SeedType::SEED_KERNELPULT && mBoard->CanPlantAt(mPlantCol - 1, mRow, aSeedType) == PLANTING_OK)
    {
        aColorOverride = GetFlashingColor(mBoard->mMainCounter, 90);
    }
    else if (mChilledCounter > 0)
    {
        aColorOverride = Color(75, 75, 255);
    }
    else if (mSeedType == SeedType::SEED_HAZYNUT)
    {
        aColorOverride = Color(255, 64, 255);
    }
    else if (mSeedType == SeedType::SEED_FROSTACHIO)
    {
        aColorOverride = Color(64, 255, 255);
    }
    else if (mSeedType == SeedType::SEED_SHADOWFLOWER)
    {
        aColorOverride = Color(64, 64, 255);
    }
    else if (mSeedType == SeedType::SEED_SHAMROCK || mSeedType == SeedType::SEED_SHAMROCKSTAR)
    {
        aColorOverride = Color(64, 255, 64);
    }
    else if (mSeedType == SeedType::SEED_MOLDSHROOM)
    {
        aColorOverride = Color(64, 255, 54);
    }
    else
    {
        aColorOverride = Color(255, 255, 255);
    }

    aBodyReanim->mColorOverride = aColorOverride;

    if (mHighlighted)
    {
        aBodyReanim->mExtraAdditiveColor = Color(255, 255, 255, 196);
        aBodyReanim->mEnableExtraAdditiveDraw = true;
        if (mImitaterType == SeedType::SEED_IMITATER)
        {
            aBodyReanim->mExtraAdditiveColor = Color(255, 255, 255, 92);
        }
    }
    else if (mBeghouledFlashCountdown > 0)
    {
        int anAlpha = TodAnimateCurve(50, 0, mBeghouledFlashCountdown % 50, 0, 128, TodCurves::CURVE_BOUNCE);
        aBodyReanim->mExtraAdditiveColor = Color(255, 255, 255, anAlpha);
        aBodyReanim->mEnableExtraAdditiveDraw = true;
    }
    else if (mEatenFlashCountdown > 0)
    {
        int aGrayness = ClampInt(mEatenFlashCountdown * 3, 0, mImitaterType == SeedType::SEED_IMITATER ? 128 : 255);
        aBodyReanim->mExtraAdditiveColor = Color(aGrayness, aGrayness, aGrayness);
        aBodyReanim->mEnableExtraAdditiveDraw = true;
    }
    else
    {
        aBodyReanim->mEnableExtraAdditiveDraw = false;
    }

    if (mBeghouledFlashCountdown > 0)
    {
        int anAlpha = TodAnimateCurve(50, 0, mBeghouledFlashCountdown % 50, 0, 128, TodCurves::CURVE_BOUNCE);
        aBodyReanim->mExtraOverlayColor = Color(255, 255, 255, anAlpha);
        aBodyReanim->mEnableExtraOverlayDraw = true;
    }
    else
    {
        aBodyReanim->mEnableExtraOverlayDraw = false;
    }

    aBodyReanim->PropogateColorToAttachments();
}

bool Plant::IsOnBoard()
{
    if (!mIsOnBoard)
        return false;

    TOD_ASSERT(mBoard);
    return true;
}

//0x4638F0
bool Plant::IsInPlay()
{
    return IsOnBoard() && mApp->mGameMode != GameMode::GAMEMODE_CHALLENGE_ZEN_GARDEN && mApp->mGameMode != GameMode::GAMEMODE_TREE_OF_WISDOM;
}

//0x463920
void Plant::UpdateReanim()
{
    Reanimation* aBodyReanim = mApp->ReanimationTryToGet(mBodyReanimID);
    if (aBodyReanim == nullptr)
        return;

    UpdateReanimColor();

    float aOffsetX = mShakeOffsetX;
    float aOffsetY = PlantDrawHeightOffset(mBoard, this, mSeedType, mPlantCol, mRow);
    float aScaleX = 1.0f, aScaleY = 1.0f;
    if ((mApp->mGameMode == GameMode::GAMEMODE_CHALLENGE_BIG_TIME) &&
        (mSeedType == SeedType::SEED_WALLNUT || mSeedType == SeedType::SEED_SUNFLOWER || mSeedType == SeedType::SEED_MARIGOLD))
    {
        aScaleX = 1.5f;
        aScaleY = 1.5f;
        aOffsetX -= 20.0f;
        aOffsetY -= 40.0f;
    }
    else if (mSeedType == SeedType::SEED_GIANT_WALLNUT)
    {
        aScaleX = 2.0f;
        aScaleY = 2.0f;
        aOffsetX -= 76.0f;
        aOffsetY -= 64.0f;
    }
    else if (mSeedType == SeedType::SEED_INSTANT_COFFEE || mSeedType == SeedType::SEED_HOTPOTATO)
    {
        aScaleX = 0.8f;
        aScaleY = 0.8f;
        aOffsetX += 12.0f;
        aOffsetY += 10.0f;
    }
    else if (mSeedType == SeedType::SEED_POTATOMINE)
    {
        aScaleX = 0.8f;
        aScaleY = 0.8f;
        aOffsetX += 12.0f;
        aOffsetY += 12.0f;
    }
    else if (mSeedType == SeedType::SEED_POPCORN)
    {
        aScaleX = 0.8f;
        aScaleY = 0.8f;
        aOffsetX += 12.0f;
        aOffsetY += 12.0f;
    }
    else if (mSeedType == SeedType::SEED_ICEBERGLETTUCE)
    {
        aScaleX = 0.8f;
        aScaleY = 0.8f;
        aOffsetX += 12.0f;
        aOffsetY += 12.0f;
    }
    if (mState == PlantState::STATE_GRAVEBUSTER_EATING)
    {
        aOffsetY += TodAnimateCurveFloat(400, 0, mStateCountdown, 0.0f, 30.0f, TodCurves::CURVE_LINEAR);
    }
    if (mWakeUpCounter > 0)
    {
        float aScaleFactor = TodAnimateCurveFloat(70, 0, mWakeUpCounter, 1.0f, 0.8f, TodCurves::CURVE_EASE_SIN_WAVE);
        aScaleY *= aScaleFactor;
        aOffsetY += 80.0f - 80.0f * aScaleFactor;
    }

    aBodyReanim->Update();

    if (mSeedType == SeedType::SEED_LEFTPEATER)
    {
        aOffsetX += 80.0f * aScaleX;
        aScaleX *= -1.0f;
    }

    if (mPottedPlantIndex != -1)
    {
        PottedPlant* aPottedPlant = &mApp->mPlayerInfo->mPottedPlant[mPottedPlantIndex];

        if (aPottedPlant->mFacing == PottedPlant::FacingDirection::FACING_LEFT)
        {
            aOffsetX += 80.0f * aScaleX;
            aScaleX *= -1.0f;
        }

        float aOffsetXStart, aOffsetXEnd;
        float aOffsetYStart, aOffsetYEnd;
        float aScaleStart, aScaleEnd;
        if (aPottedPlant->mPlantAge == PottedPlantAge::PLANTAGE_SMALL)
        {
            aOffsetXStart = 20.0f;
            aOffsetXEnd = 20.0f;
            aOffsetYStart = 40.0f;
            aOffsetYEnd = 40.0f;
            aScaleStart = 0.5f;
            aScaleEnd = 0.5f;
        }
        else if (aPottedPlant->mPlantAge == PottedPlantAge::PLANTAGE_MEDIUM)
        {
            aOffsetXStart = 20.0f;
            aOffsetXEnd = 10.0f;
            aOffsetYStart = 40.0f;
            aOffsetYEnd = 20.0f;
            aScaleStart = 0.5f;
            aScaleEnd = 0.75f;
        }
        else
        {
            aOffsetXStart = 10.0f;
            aOffsetXEnd = 0.0f;
            aOffsetYStart = 20.0f;
            aOffsetYEnd = 0.0f;
            aScaleStart = 0.75f;
            aScaleEnd = 1.0f;
        }

        float aAnimatedOffsetX = TodAnimateCurveFloat(100, 0, mStateCountdown, aOffsetXStart, aOffsetXEnd, TodCurves::CURVE_LINEAR);
        float aAnimatedOffsetY = TodAnimateCurveFloat(100, 0, mStateCountdown, aOffsetYStart, aOffsetYEnd, TodCurves::CURVE_LINEAR);
        float aAnimatedScale = TodAnimateCurveFloat(100, 0, mStateCountdown, aScaleStart, aScaleEnd, TodCurves::CURVE_LINEAR);

        aOffsetX += aAnimatedOffsetX * aScaleX;
        aOffsetY += aAnimatedOffsetY * aScaleY;
        aScaleX *= aAnimatedScale;
        aScaleY *= aAnimatedScale;
        aOffsetX += mApp->mZenGarden->ZenPlantOffsetX(aPottedPlant);
        aOffsetY += mApp->mZenGarden->PlantPottedDrawHeightOffset(mSeedType, aScaleY);
    }

    aBodyReanim->SetPosition(aOffsetX, aOffsetY);
    aBodyReanim->OverrideScale(aScaleX, aScaleY);
}

//0x463E40
void Plant::Update()
{
    bool doUpdate = false;
    if (IsOnBoard() && mApp->mGameScene == GameScenes::SCENE_LEVEL_INTRO && (mApp->IsWallnutBowlingLevel()))
        doUpdate = true;
    else if (IsOnBoard() && mApp->mGameMode == GameMode::GAMEMODE_CHALLENGE_ZEN_GARDEN)
        doUpdate = true;
    else if (IsOnBoard() && mBoard->mCutScene->ShouldRunUpsellBoard())
        doUpdate = true;
    else if (!IsOnBoard() || mApp->mGameScene == GameScenes::SCENE_PLAYING)
        doUpdate = true;

    mPlantAge++;


    if (doUpdate)
    {
        UpdateCounters();
        if ((mChilledCounter > 0 && mPlantAge % 2 == 1) || mIceTrapCounter > 0) {
            return;
        }
        UpdateAbilities();
        Animate();

        if (mPlantHealth < 0)
            Die();

        UpdateReanim();

        if (mIsBoosted) {
            UpdateAbilities();
            Animate();

            if (mPlantHealth < 0)
                Die();

            UpdateReanim();
        }
    }
}

//0x463F30
Reanimation* Plant::AttachBlinkAnim(Reanimation* theReanimBody)
{
    const PlantDefinition& aPlantDef = GetPlantDefinition(mSeedType);
    LawnApp* aApp = (LawnApp*)gSexyAppBase;
    Reanimation* aAnimToAttach = theReanimBody;
    const char* aTrackToPlay = "anim_blink";
    const char* aTrackToAttach = nullptr;

    if (IsNut(mSeedType))
    {
        int aHit = Rand(10);
        if (aHit < 1 && theReanimBody->TrackExists("anim_blink_twitch"))
        {
            aTrackToPlay = "anim_blink_twitch";
        }
        else
        {
            aTrackToPlay = aHit < 7 ? "anim_blink_twice" : "anim_blink_thrice";
        }
    }
    else if (mSeedType == SeedType::SEED_THREEPEATER)
    {
        int aHit = Rand(3);
        if (aHit == 0)
        {
            aTrackToPlay = "anim_blink1";
            aTrackToAttach = "anim_face1";
            ReanimatorTrackInstance* aTrackInstance = theReanimBody->GetTrackInstanceByName("anim_head1");
            aAnimToAttach = FindReanimAttachment(aTrackInstance->mAttachmentID);
        }
        else if (aHit == 1)
        {
            aTrackToPlay = "anim_blink2";
            aTrackToAttach = "anim_face2";
            ReanimatorTrackInstance* aTrackInstance = theReanimBody->GetTrackInstanceByName("anim_head2");
            aAnimToAttach = FindReanimAttachment(aTrackInstance->mAttachmentID);
        }
        else
        {
            aTrackToPlay = "anim_blink3";
            aTrackToAttach = "anim_face3";
            ReanimatorTrackInstance* aTrackInstance = theReanimBody->GetTrackInstanceByName("anim_head3");
            aAnimToAttach = FindReanimAttachment(aTrackInstance->mAttachmentID);
        }
    }
    else if (mSeedType == SeedType::SEED_SPLITPEA)
    {
        if (Rand(2) == 0)
        {
            aTrackToPlay = "anim_blink";
            aTrackToAttach = "anim_face";
            aAnimToAttach = mApp->ReanimationTryToGet(mHeadReanimID);
        }
        else
        {
            aTrackToPlay = "anim_blink2";
            aTrackToAttach = "anim_face2";
            aAnimToAttach = mApp->ReanimationTryToGet(mHeadReanimID2);
        }
    }
    else if (mSeedType == SeedType::SEED_TWINSUNFLOWER)
    {
        if (Rand(2) == 0)
        {
            aTrackToPlay = "anim_blink";
            aTrackToAttach = "anim_face";
        }
        else
        {
            aTrackToPlay = "anim_blink2";
            aTrackToAttach = "anim_face2";
        }
    }
    else if (IsPeashooter(mSeedType))
    {
        if (theReanimBody->TrackExists("anim_stem"))
        {
            ReanimatorTrackInstance* aTrackInstance = theReanimBody->GetTrackInstanceByName("anim_stem");
            aAnimToAttach = FindReanimAttachment(aTrackInstance->mAttachmentID);
        }
        else if (theReanimBody->TrackExists("anim_idle"))
        {
            ReanimatorTrackInstance* aTrackInstance = theReanimBody->GetTrackInstanceByName("anim_idle");
            aAnimToAttach = FindReanimAttachment(aTrackInstance->mAttachmentID);
        }
    }

    if (aAnimToAttach == nullptr)
    {
        TodTrace("Missing head anim");
        return nullptr;
    }

    if (!theReanimBody->TrackExists(aTrackToPlay))
        return nullptr;

    Reanimation* aBlinkReanim = aApp->mEffectSystem->mReanimationHolder->AllocReanimation(0.0f, 0.0f, 0, aPlantDef.mReanimationType);
    aBlinkReanim->SetFramesForLayer(aTrackToPlay);
    aBlinkReanim->mLoopType = ReanimLoopType::REANIM_PLAY_ONCE_FULL_LAST_FRAME_AND_HOLD;
    aBlinkReanim->mAnimRate = 15.0f;
    aBlinkReanim->mColorOverride = theReanimBody->mColorOverride;

    if (aTrackToAttach && aAnimToAttach->TrackExists(aTrackToAttach))
    {
        aBlinkReanim->AttachToAnotherReanimation(aAnimToAttach, aTrackToAttach);
    }
    else if (aAnimToAttach->TrackExists("anim_face"))
    {
        aBlinkReanim->AttachToAnotherReanimation(aAnimToAttach, "anim_face");
    }
    else if (aAnimToAttach->TrackExists("anim_idle"))
    {
        aBlinkReanim->AttachToAnotherReanimation(aAnimToAttach, "anim_idle");
    }
    else
    {
        TodTrace("Missing anim_idle for blink");
    }

    aBlinkReanim->mFilterEffect = theReanimBody->mFilterEffect;
    return aBlinkReanim;
}

//0x464230
void Plant::DoBlink()
{
    mBlinkCountdown = 400 + Rand(400);

    if (NotOnGround() || mShootingCounter != 0)
        return;

    if ((mSeedType == SeedType::SEED_POTATOMINE || mSeedType == SeedType::SEED_POPCORN) && mState != PlantState::STATE_POTATO_ARMED)
        return;

    if (mState == PlantState::STATE_CACTUS_RISING || mState == PlantState::STATE_CACTUS_HIGH || mState == PlantState::STATE_CACTUS_LOWERING ||
        mState == PlantState::STATE_MAGNETSHROOM_SUCKING || mState == PlantState::STATE_MAGNETSHROOM_CHARGING)
        return;

    EndBlink();
    Reanimation* aBodyReanim = mApp->ReanimationTryToGet(mBodyReanimID);
    if (aBodyReanim == nullptr)
        return;

    if ((mSeedType == SeedType::SEED_TALLNUT && aBodyReanim->GetImageOverride("anim_idle") == IMAGE_REANIM_TALLNUT_CRACKED2) || 
        (mSeedType == SeedType::SEED_GARLIC && aBodyReanim->GetImageOverride("anim_face") == IMAGE_REANIM_GARLIC_BODY3))
        return;

    if (IsNut(mSeedType))
    {
        mBlinkCountdown = 1000 + Rand(1000);
    }

    Reanimation* aBlinkReanim = AttachBlinkAnim(aBodyReanim);
    if (aBlinkReanim)
    {
        mBlinkReanimID = mApp->ReanimationGetID(aBlinkReanim);
    }
    aBodyReanim->AssignRenderGroupToPrefix("anim_eye", RENDER_GROUP_HIDDEN);
}

//0x464390
void Plant::EndBlink()
{
    if (mBlinkReanimID != ReanimationID::REANIMATIONID_NULL)
    {
        mApp->RemoveReanimation(mBlinkReanimID);
        mBlinkReanimID = ReanimationID::REANIMATIONID_NULL;

        Reanimation* aBodyReanim = mApp->ReanimationTryToGet(mBodyReanimID);
        if (aBodyReanim)
        {
            aBodyReanim->AssignRenderGroupToPrefix("anim_eye", RENDER_GROUP_NORMAL);
        }
    }
}

//0x464410
void Plant::UpdateBlink()
{
    if (mBlinkReanimID != ReanimationID::REANIMATIONID_NULL)
    {
        Reanimation* aBlinkReanim = mApp->ReanimationTryToGet(mBlinkReanimID);
        if (aBlinkReanim == nullptr || aBlinkReanim->mLoopCount > 0)
        {
            EndBlink();
        }
    }

    if (mIsAsleep)
        return;

    if (mBlinkCountdown > 0)
    {
        mBlinkCountdown--;
        if (mBlinkCountdown == 0)
        {
            DoBlink();
        }
    }
}

//0x464480
void Plant::AnimateNuts()
{
    Reanimation* aBodyReanim = mApp->ReanimationTryToGet(mBodyReanimID);
    if (aBodyReanim == nullptr)
        return;

    Image* aCracked1;
    Image* aCracked2;
    const char* aTrackToOverride;
    if (IsWallNut(mSeedType))
    {
        aCracked1 = IMAGE_REANIM_WALLNUT_CRACKED1;
        aCracked2 = IMAGE_REANIM_WALLNUT_CRACKED2;
        aTrackToOverride = "anim_face";
    }
    else if (mSeedType == SeedType::SEED_TALLNUT || mSeedType == SeedType::SEED_VINENUT)
    {
        aCracked1 = IMAGE_REANIM_TALLNUT_CRACKED1;
        aCracked2 = IMAGE_REANIM_TALLNUT_CRACKED2;
        aTrackToOverride = "anim_idle";
    }
    else return;

    int aPosX = mX + 40;
    int aPosY = mY + 10;
    if (mSeedType == SeedType::SEED_TALLNUT || mSeedType == SeedType::SEED_VINENUT)
    {
        aPosY -= 32;
    }

    Image* aImageOverride = aBodyReanim->GetImageOverride(aTrackToOverride);
    if (mPlantHealth < mPlantMaxHealth / 3)
    {
        if (aImageOverride != aCracked2)
        {
            aBodyReanim->SetImageOverride(aTrackToOverride, aCracked2);
            mApp->AddTodParticle(aPosX, aPosY, mRenderOrder + 4, ParticleEffect::PARTICLE_WALLNUT_EAT_LARGE);
        }
    }
    else if (mPlantHealth < mPlantMaxHealth * 2 / 3)
    {
        if (aImageOverride != aCracked1)
        {
            aBodyReanim->SetImageOverride(aTrackToOverride, aCracked1);
            mApp->AddTodParticle(aPosX, aPosY, mRenderOrder + 4, ParticleEffect::PARTICLE_WALLNUT_EAT_LARGE);
        }
    }
    else
    {
        aBodyReanim->SetImageOverride(aTrackToOverride, nullptr);
    }

    if (IsInPlay() && !mApp->IsIZombieLevel())
    {
        if (mRecentlyEatenCountdown > 0)
        {
            aBodyReanim->mAnimRate = 0.1f;
            return;
        }

        if (aBodyReanim->mAnimRate < 1.0f && mOnBungeeState != PlantOnBungeeState::RISING_WITH_BUNGEE)
        {
            aBodyReanim->mAnimRate = RandRangeFloat(10.0f, 15.0f);
        }
    }
}

//0x464680
void Plant::AnimateGarlic()
{
    Reanimation* aBodyReanim = mApp->ReanimationGet(mBodyReanimID);
    Image* aImageOverride = aBodyReanim->GetImageOverride("anim_face");

    if (mPlantHealth < mPlantMaxHealth / 3)
    {
        if (aImageOverride != IMAGE_REANIM_GARLIC_BODY3)
        {
            aBodyReanim->SetImageOverride("anim_face", IMAGE_REANIM_GARLIC_BODY3);
            aBodyReanim->AssignRenderGroupToPrefix("Garlic_stem", RENDER_GROUP_HIDDEN);
        }
    }
    else if (mPlantHealth < mPlantMaxHealth * 2 / 3)
    {
        if (aImageOverride != IMAGE_REANIM_GARLIC_BODY2)
        {
            aBodyReanim->SetImageOverride("anim_face", IMAGE_REANIM_GARLIC_BODY2);
        }
    }
    else
    {
        aBodyReanim->SetImageOverride("anim_face", nullptr);
    }
}

//0x464760
void Plant::AnimatePumpkin()
{
    Reanimation* aBodyReanim = mApp->ReanimationGet(mBodyReanimID);
    Image* aImageOverride = aBodyReanim->GetImageOverride("Pumpkin_front");

    if (mPlantHealth < mPlantMaxHealth / 3)
    {
        if (aImageOverride != IMAGE_REANIM_PUMPKIN_DAMAGE3)
            aBodyReanim->SetImageOverride("Pumpkin_front", IMAGE_REANIM_PUMPKIN_DAMAGE3);
    }
    else if (mPlantHealth < mPlantMaxHealth * 2 / 3)
    {
        if (aImageOverride != IMAGE_REANIM_PUMPKIN_DAMAGE1)
            aBodyReanim->SetImageOverride("Pumpkin_front", IMAGE_REANIM_PUMPKIN_DAMAGE1);
    }
    else
    {
        aBodyReanim->SetImageOverride("Pumpkin_front", nullptr);
    }
}

//0x464820
void Plant::UpdateShooting()
{
    if (NotOnGround() || mShootingCounter == 0)
        return;

    mShootingCounter--;

    if (mSeedType == SeedType::SEED_FUMESHROOM || mSeedType == SeedType::SEED_ICYFUMESHROOM)
    {
        if (mShootingCounter == 15) {
            int aRenderPosition = Board::MakeRenderOrder(RenderLayer::RENDER_LAYER_PARTICLE, mRow, 0);
            AddAttachedParticle(mX + 85, mY + 31, aRenderPosition, ParticleEffect::PARTICLE_FUMECLOUD);
        }
    }

    if (mSeedType == SeedType::SEED_GLOOMSHROOM)
    {
        if (mShootingCounter == 136 || mShootingCounter == 108 || mShootingCounter == 80 || mShootingCounter == 52)
        {
            int aRenderPosition = Board::MakeRenderOrder(RenderLayer::RENDER_LAYER_PARTICLE, mRow, 0);
            AddAttachedParticle(mX + 40, mY + 40, aRenderPosition, ParticleEffect::PARTICLE_GLOOMCLOUD);
        }
        if (mShootingCounter == 126 || mShootingCounter == 98 || mShootingCounter == 70 || mShootingCounter == 42)
        {
            Fire(nullptr, mRow, PlantWeapon::WEAPON_PRIMARY);
        }
    }
    else if (mSeedType == SeedType::SEED_GATLINGPEA)
    {
        if (mShootingCounter == 18 || mShootingCounter == 35 || mShootingCounter == 51 || mShootingCounter == 68)
        {
            Fire(nullptr, mRow, PlantWeapon::WEAPON_PRIMARY);
        }
    }
    else if (mSeedType == SeedType::SEED_CATTAIL)
    {
        if (mShootingCounter == 19)
        {
            Zombie* aZombie = FindTargetZombie(mRow, PlantWeapon::WEAPON_PRIMARY);
            if (aZombie)
            {
                Fire(aZombie, mRow, PlantWeapon::WEAPON_PRIMARY);
            }
        }
    }

    else if (mShootingCounter == 1)
    {
        if (mSeedType == SeedType::SEED_THREEPEATER)
        {
            int rowAbove = mRow - 1;
            int rowBelow = mRow + 1;
            Reanimation* aHeadReanim2 = mApp->ReanimationGet(mHeadReanimID2);
            Reanimation* aHeadReanim3 = mApp->ReanimationGet(mHeadReanimID3);
            Reanimation* aHeadReanim1 = mApp->ReanimationGet(mHeadReanimID);

            if (aHeadReanim1->mLoopType == ReanimLoopType::REANIM_PLAY_ONCE_AND_HOLD && FindTargetZombie(rowBelow, PlantWeapon::WEAPON_PRIMARY))
                Fire(nullptr, rowBelow, PlantWeapon::WEAPON_PRIMARY);
            if (aHeadReanim2->mLoopType == ReanimLoopType::REANIM_PLAY_ONCE_AND_HOLD && FindTargetZombie(mRow, PlantWeapon::WEAPON_PRIMARY))
                Fire(nullptr, mRow, PlantWeapon::WEAPON_PRIMARY);
            if (aHeadReanim3->mLoopType == ReanimLoopType::REANIM_PLAY_ONCE_AND_HOLD && FindTargetZombie(rowAbove, PlantWeapon::WEAPON_PRIMARY))
                Fire(nullptr, rowAbove, PlantWeapon::WEAPON_PRIMARY);
        }
        else if (mSeedType == SeedType::SEED_SPLITPEA)
        {
            Reanimation* aHeadBackReanim = mApp->ReanimationTryToGet(mHeadReanimID2);
            Reanimation* aHeadFrontReanim = mApp->ReanimationTryToGet(mHeadReanimID);
            if (aHeadFrontReanim->mLoopType == ReanimLoopType::REANIM_PLAY_ONCE_AND_HOLD)
                Fire(nullptr, mRow, PlantWeapon::WEAPON_PRIMARY);
            if (aHeadBackReanim->mLoopType == ReanimLoopType::REANIM_PLAY_ONCE_AND_HOLD)
                Fire(nullptr, mRow, PlantWeapon::WEAPON_SECONDARY);
        }
        else if (mState == PlantState::STATE_CACTUS_LOW)
            Fire(nullptr, mRow, PlantWeapon::WEAPON_SECONDARY);
        else if (mSeedType == SeedType::SEED_MOLDSHROOM || mSeedType == SeedType::SEED_ICEBERGLETTUCEBETA || mSeedType == SeedType::SEED_CABBAGEPULT 
            || mSeedType == SeedType::SEED_PEPPERPULT || mSeedType == SeedType::SEED_KERNELPULT || mSeedType == SeedType::SEED_MELONPULT 
            || mSeedType == SeedType::SEED_WINTERMELON || mSeedType == SeedType::SEED_SPORESHROOM)
        {
            PlantWeapon aPlantWeapon = PlantWeapon::WEAPON_PRIMARY;
            if (mState == PlantState::STATE_KERNELPULT_BUTTER)
            {
                Reanimation* aBodyReanim = mApp->ReanimationGet(mBodyReanimID);
                aBodyReanim->AssignRenderGroupToPrefix("Cornpult_butter", RENDER_GROUP_HIDDEN);
                aBodyReanim->AssignRenderGroupToPrefix("Cornpult_kernal", RENDER_GROUP_NORMAL);
                mState = PlantState::STATE_NOTREADY;
                aPlantWeapon = PlantWeapon::WEAPON_SECONDARY;
            }

            Zombie* aZombie = FindTargetZombie(mRow, aPlantWeapon);
            Fire(aZombie, mRow, aPlantWeapon);
        }
        else if (mSeedType == SeedType::SEED_BEESHOOTER || mSeedType == SeedType::SEED_FUTURESTAR)
        {
            Zombie* aZombie = FindTargetZombie(mRow, PlantWeapon::WEAPON_PRIMARY);
            if (aZombie)
            {
                Fire(aZombie, mRow, PlantWeapon::WEAPON_PRIMARY);
            }
        }
        else if (mSeedType == SeedType::SEED_PLASMAPEA && mState == PlantState::STATE_READY)
            Fire(nullptr, mRow, PlantWeapon::WEAPON_SECONDARY);
        else if (mSeedType == SeedType::SEED_BLOOMERANG)
        {
            Zombie* aZombie = FindBloomerangTarget();
            if (aZombie) Fire(aZombie, mRow, PlantWeapon::WEAPON_PRIMARY);
        }
        else if (mSeedType == SeedType::SEED_BAMBOOSHOOT || mSeedType == SeedType::SEED_BAMBOOM)
        {
            Zombie* aZombie = FindTargetZombie(mRow, PlantWeapon::WEAPON_PRIMARY);
            if (mLaunchCounter <= mStoredLaunchRate - 100)
            {
                aZombie = FindTargetZombie(mRow + 1, PlantWeapon::WEAPON_PRIMARY);
                Fire(aZombie, mRow + 1, PlantWeapon::WEAPON_PRIMARY);
            }
            else if (mLaunchCounter <= mStoredLaunchRate - 50)
            {
                aZombie = FindTargetZombie(mRow - 1, PlantWeapon::WEAPON_PRIMARY);
                Fire(aZombie, mRow - 1, PlantWeapon::WEAPON_PRIMARY);
            }
            else
                Fire(aZombie, mRow, PlantWeapon::WEAPON_PRIMARY);
        }
        else
            Fire(nullptr, mRow, PlantWeapon::WEAPON_PRIMARY);

        return;
    }

    if (mShootingCounter != 0)
        return;

    Reanimation* aBodyReanim = mApp->ReanimationTryToGet(mBodyReanimID);
    Reanimation* aHeadReanim = mApp->ReanimationTryToGet(mHeadReanimID);
    if (mSeedType == SeedType::SEED_THREEPEATER)
    {
        Reanimation* aHeadReanim2 = mApp->ReanimationGet(mHeadReanimID2);
        Reanimation* aHeadReanim3 = mApp->ReanimationGet(mHeadReanimID3);

        if (aHeadReanim2->mLoopCount > 0)
        {
            if (aHeadReanim->mLoopType == ReanimLoopType::REANIM_PLAY_ONCE_AND_HOLD)
            {
                aHeadReanim->StartBlend(20);
                aHeadReanim->mLoopType = ReanimLoopType::REANIM_LOOP;
                aHeadReanim->SetFramesForLayer("anim_head_idle1");
                aHeadReanim->mAnimRate = aBodyReanim->mAnimRate;
                aHeadReanim->mAnimTime = aBodyReanim->mAnimTime;
            }

            aHeadReanim2->StartBlend(20);
            aHeadReanim2->mLoopType = ReanimLoopType::REANIM_LOOP;
            aHeadReanim2->SetFramesForLayer("anim_head_idle2");
            aHeadReanim2->mAnimRate = aBodyReanim->mAnimRate;
            aHeadReanim2->mAnimTime = aBodyReanim->mAnimTime;

            if (aHeadReanim3->mLoopType == ReanimLoopType::REANIM_PLAY_ONCE_AND_HOLD)
            {
                aHeadReanim3->StartBlend(20);
                aHeadReanim3->mLoopType = ReanimLoopType::REANIM_LOOP;
                aHeadReanim3->SetFramesForLayer("anim_head_idle3");
                aHeadReanim3->mAnimRate = aBodyReanim->mAnimRate;
                aHeadReanim3->mAnimTime = aBodyReanim->mAnimTime;
            }
            
            return;
        }
    }
    else if (mSeedType == SeedType::SEED_SPLITPEA)
    {
        Reanimation* aHeadReanim2 = mApp->ReanimationGet(mHeadReanimID2);

        if (aHeadReanim->mLoopCount > 0)
        {
            aHeadReanim->StartBlend(20);
            aHeadReanim->mLoopType = ReanimLoopType::REANIM_LOOP;
            aHeadReanim->SetFramesForLayer("anim_head_idle");
            aHeadReanim->mAnimRate = aBodyReanim->mAnimRate;
            aHeadReanim->mAnimTime = aBodyReanim->mAnimTime;
        }

        if (aHeadReanim2->mLoopCount > 0)
        {
            aHeadReanim2->StartBlend(20);
            aHeadReanim2->mLoopType = ReanimLoopType::REANIM_LOOP;
            aHeadReanim2->SetFramesForLayer("anim_splitpea_idle");
            aHeadReanim2->mAnimRate = aBodyReanim->mAnimRate;
            aHeadReanim2->mAnimTime = aBodyReanim->mAnimTime;
        }

        return;
    }
    else if (mState == PlantState::STATE_CACTUS_HIGH)
    {
        if (aBodyReanim->mLoopCount > 0)
        {
            PlayBodyReanim("anim_idlehigh", ReanimLoopType::REANIM_LOOP, 20, 0.0f);

            aBodyReanim->mAnimRate = aBodyReanim->mDefinition->mFPS;
            if (mApp->IsIZombieLevel())
            {
                aBodyReanim->mAnimRate = 0.0f;
            }

            return;
        }
    }
    else if (aHeadReanim)
    {
        if (aHeadReanim->mLoopCount > 0)
        {
            aHeadReanim->StartBlend(20);
            aHeadReanim->mLoopType = ReanimLoopType::REANIM_LOOP;
            aHeadReanim->SetFramesForLayer("anim_head_idle");
            aHeadReanim->mAnimRate = aBodyReanim->mAnimRate;
            aHeadReanim->mAnimTime = aBodyReanim->mAnimTime;
            return;
        }
    }
    else if (mSeedType == SeedType::SEED_COBCANNON)
    {
        if (aBodyReanim->mLoopCount > 0)
        {
            mState = PlantState::STATE_COBCANNON_ARMING;
            mStateCountdown = 3000;
            PlayBodyReanim("anim_unarmed_idle", ReanimLoopType::REANIM_LOOP, 20, aBodyReanim->mDefinition->mFPS);
            return;
        }
    }
    else if (aBodyReanim && aBodyReanim->mLoopCount > 0)
    {
        PlayIdleAnim(aBodyReanim->mDefinition->mFPS);
        return;
    }

    mShootingCounter = 1;
}

//0x464DB0
void Plant::Animate()
{
    if ((mSeedType == SeedType::SEED_CHERRYBOMB 
        || mSeedType == SeedType::SEED_JALAPENO 
        || mSeedType == SeedType::SEED_PICKLEDPEPPER 
        || mSeedType == SeedType::SEED_CHILLYPEPPER) && mApp->mGameMode != GameMode::GAMEMODE_CHALLENGE_ZEN_GARDEN)
    {
        mShakeOffsetX = RandRangeFloat(-1.0f, 1.0f);
        mShakeOffsetY = RandRangeFloat(-1.0f, 1.0f);
    }

    if (mRecentlyEatenCountdown > 0)
    {
        mRecentlyEatenCountdown--;
    }
    if (mEatenFlashCountdown > 0)
    {
        mEatenFlashCountdown--;
    }
    if (mBeghouledFlashCountdown > 0)
    {
        mBeghouledFlashCountdown--;
    }

    if (mSquished)
    {
        mFrame = 0;
        return;
    }

    if (IsNut(mSeedType))
    {
        AnimateNuts();
    }
    else if (mSeedType == SeedType::SEED_GARLIC)
    {
        AnimateGarlic();
    }
    else if (mSeedType == SeedType::SEED_PUMPKINSHELL)
    {
        AnimatePumpkin();
    }

    UpdateBlink();

    if (mAnimPing)
    {
        if (mAnimCounter < mFrameLength * mNumFrames - 1)
        {
            mAnimCounter++;
        }
        else
        {
            mAnimPing = false;
            mAnimCounter -= mFrameLength;
        }
    }
    else if (mAnimCounter > 0)
    {
        mAnimCounter--;
    }
    else
    {
        mAnimPing = true;
        mAnimCounter += mFrameLength;
    }
    mFrame = mAnimCounter / mFrameLength;
}

//0x464EF0
float PlantFlowerPotHeightOffset(SeedType theSeedType, float theFlowerPotScale)
{
    float aHeightOffset = -5.0f * theFlowerPotScale;
    float aScaleOffsetFix = 0.0f;

    switch (theSeedType)
    {
    case SeedType::SEED_SUPERCHOMPER:
    case SeedType::SEED_CHOMPER:
    case SeedType::SEED_PLANTERN:
        aHeightOffset -= 5.0f;
        break;
    case SeedType::SEED_SHAMROCK:
    case SeedType::SEED_SHAMROCKSTAR:
    case SeedType::SEED_SCAREDYSHROOM:
        aHeightOffset += 5.0f;
        aScaleOffsetFix -= 8.0f;
        break;
    case SeedType::SEED_SUNSHROOM:
    case SeedType::SEED_PUFFSHROOM:
    case SeedType::SEED_MOLDSHROOM:
        aScaleOffsetFix -= 4.0f;
        break;
    case SeedType::SEED_HYPNOSHROOM:
    case SeedType::SEED_MAGNETSHROOM:
    case SeedType::SEED_PEASHOOTER:
    case SeedType::SEED_PEANIS:
    case SeedType::SEED_REPEATER:
    case SeedType::SEED_LEFTPEATER:
    case SeedType::SEED_SNOWPEA:
    case SeedType::SEED_PLASMAPEA:
    case SeedType::SEED_FIREPEA:
    case SeedType::SEED_SWEETPEA:
    case SeedType::SEED_ICEQUEENPEA:
    case SeedType::SEED_THREEPEATER:
    case SeedType::SEED_SUNFLOWER:
    case SeedType::SEED_POWERFLOWER:
    case SeedType::SEED_MARIGOLD:
    case SeedType::SEED_CABBAGEPULT:
    case SeedType::SEED_PEPPERPULT:
    case SeedType::SEED_ICEBERGLETTUCEBETA:
    case SeedType::SEED_MELONPULT:
    case SeedType::SEED_TANGLEKELP:
    case SeedType::SEED_BLOVER:
    case SeedType::SEED_SPIKEWEED:
    case SeedType::SEED_ASPEARAGUS:
    case SeedType::SEED_ACEPEARAGUS:
        aScaleOffsetFix -= 8.0f;
        break;
    case SeedType::SEED_SEASHROOM:
    case SeedType::SEED_POTATOMINE:
    case SeedType::SEED_POPCORN:
    case SeedType::SEED_ICEBERGLETTUCE:
        aScaleOffsetFix -= 4.0f;
        break;
    case SeedType::SEED_LILYPAD:
        aScaleOffsetFix -= 16.0f;
        break;
    case SeedType::SEED_INSTANT_COFFEE:
    case SeedType::SEED_HOTPOTATO:
        aScaleOffsetFix -= 20.0f;
        break;
    }

    return aHeightOffset + (theFlowerPotScale * aScaleOffsetFix - aScaleOffsetFix);
}

//0x465040
float PlantDrawHeightOffset(Board* theBoard, Plant* thePlant, SeedType theSeedType, int theCol, int theRow)
{
    float aHeightOffset = 0.0f;

    bool doFloating = false;
    if (Plant::IsFlying(theSeedType))
    {
        doFloating = false;
    }
    else if (theBoard == nullptr)
    {
        if (Plant::IsAquatic(theSeedType))
            doFloating = true;
    }
    else if (theBoard->IsPoolSquare(theCol, theRow))
        doFloating = true;

    if (doFloating)
    {
        int aCounter;
        if (theBoard)
            aCounter = theBoard->mMainCounter;
        else
            aCounter = gLawnApp->mAppCounter;

        float aPos = theRow * PI + theCol * 0.25f * PI;
        float aTime = aCounter * 2.0f * PI / 200.0f;
        float aFloatingHeight = sin(aPos + aTime) * 2.0f;
        aHeightOffset += aFloatingHeight;
    }

    if (theBoard && (thePlant == nullptr || !thePlant->mSquished))
    {
        Plant* aPot = theBoard->GetFlowerPotAt(theCol, theRow);
        Plant* aWaterPot = theBoard->GetWaterPotAt(theCol, theRow);
        Plant* aCoffeePot = theBoard->GetCoffeePotAt(theCol, theRow);
        if ((aPot && !aPot->mSquished && theSeedType != SeedType::SEED_FLOWERPOT) 
            || (aWaterPot && !aWaterPot->mSquished && theSeedType != SeedType::SEED_WATERPOT) 
            || (aCoffeePot && !aCoffeePot->mSquished && theSeedType != SeedType::SEED_COFFEEPOT))
            aHeightOffset += PlantFlowerPotHeightOffset(theSeedType, 1.0f);
    }

    if (theSeedType == SeedType::SEED_FLOWERPOT || theSeedType == SeedType::SEED_WATERPOT || theSeedType == SeedType::SEED_COFFEEPOT)
        aHeightOffset += 26.0f;
    else if (theSeedType == SeedType::SEED_LILYPAD)
        aHeightOffset += 25.0f;
    else if (theSeedType == SeedType::SEED_STARFRUIT || theSeedType == SeedType::SEED_FUTURESTAR)
        aHeightOffset += 10.0f;
    else if (theSeedType == SeedType::SEED_TANGLEKELP)
        aHeightOffset += 24.0f;
    else if (theSeedType == SeedType::SEED_SEASHROOM)
        aHeightOffset += 28.0f;
    else if (theSeedType == SeedType::SEED_INSTANT_COFFEE || theSeedType == SeedType::SEED_HOTPOTATO)
        aHeightOffset -= 20.0f;
    else if (Plant::IsFlying(theSeedType))
        aHeightOffset -= 30.0f;
    else if (theSeedType == SeedType::SEED_CACTUS)
        return aHeightOffset;
    else if (theSeedType == SeedType::SEED_PUMPKINSHELL)
        aHeightOffset += 15.0f;
    else if (theSeedType == SeedType::SEED_PUFFSHROOM)
        aHeightOffset += 5.0f;
    else if (theSeedType == SeedType::SEED_SCAREDYSHROOM || theSeedType == SeedType::SEED_SHAMROCK || theSeedType == SeedType::SEED_SHAMROCKSTAR)
        aHeightOffset -= 14.0f;
    else if (theSeedType == SeedType::SEED_GRAVEBUSTER)
        aHeightOffset -= 40.0f;
    else if (theSeedType == SeedType::SEED_SPIKEWEED || theSeedType == SeedType::SEED_SPIKEROCK)
    {
        int aBottomRow = 4;
        if (theBoard && theBoard->StageHas6Rows())
            aBottomRow = 5;

        if (theSeedType == SeedType::SEED_SPIKEROCK)
            aHeightOffset += 6.0f;

        if (theBoard && (theBoard->GetFlowerPotAt(theCol, theRow) || theBoard->GetWaterPotAt(theCol, theRow)) && gLawnApp->mGameMode != GameMode::GAMEMODE_CHALLENGE_ZEN_GARDEN)
            aHeightOffset += 5.0f;
        else if (theBoard && (theBoard->StageHasRoof() || theBoard->IsTileSquare(theCol, theRow)))
            aHeightOffset += 15.0f;
        else if (theBoard && theBoard->IsPoolSquare(theCol, theRow))
            aHeightOffset += 0.0f;
        else if (theRow == aBottomRow && theCol >= 7 && theBoard->StageHas6Rows())
            aHeightOffset += 1.0f;
        else if (theRow == aBottomRow && theCol < 7)
            aHeightOffset += 12.0f;
        else
            aHeightOffset += 15.0f;
    }

    return aHeightOffset;
}

//0x465380
void Plant::GetPeaHeadOffset(int& theOffsetX, int& theOffsetY)
{
    Reanimation* aBodyReanim = mApp->ReanimationTryToGet(mBodyReanimID);

    int aTrackIndex = 0;
    if (aBodyReanim->TrackExists("anim_stem"))
    {
        aTrackIndex = aBodyReanim->FindTrackIndex("anim_stem");
    }
    else if(aBodyReanim->TrackExists("anim_idle"))
    {
        aTrackIndex = aBodyReanim->FindTrackIndex("anim_idle");
    }

    ReanimatorTransform aTransform;
    aBodyReanim->GetCurrentTransform(aTrackIndex, &aTransform);
    theOffsetX = aTransform.mTransX;
    theOffsetY = aTransform.mTransY;
}

//0x465460
void Plant::DrawMagnetItems(Graphics* g)
{
    float aOffsetX = 0.0f;
    float aOffsetY = PlantDrawHeightOffset(mBoard, this, mSeedType, mPlantCol, mRow);

    for (int i = 0; i < MAX_MAGNET_ITEMS; i++)
    {
        MagnetItem* aMagnetItem = &mMagnetItems[i];
        if (aMagnetItem->mItemType != MagnetItemType::MAGNET_ITEM_NONE)
        {
            int aCelRow = 0, aCelCol = 0;
            Image* aImage = nullptr;
            float aScale = 0.8f;

            if (aMagnetItem->mItemType == MagnetItemType::MAGNET_ITEM_PAIL_1)
            {
                aImage = IMAGE_REANIM_ZOMBIE_BUCKET1;
            }
            else if (aMagnetItem->mItemType == MagnetItemType::MAGNET_ITEM_PAIL_2)
            {
                aImage = IMAGE_REANIM_ZOMBIE_BUCKET2;
            }
            else if (aMagnetItem->mItemType == MagnetItemType::MAGNET_ITEM_PAIL_3)
            {
                aImage = IMAGE_REANIM_ZOMBIE_BUCKET3;
            }
            else if (aMagnetItem->mItemType == MagnetItemType::MAGNET_ITEM_FOOTBALL_HELMET_1)
            {
                aImage = IMAGE_REANIM_ZOMBIE_FOOTBALL_HELMET;
            }
            else if (aMagnetItem->mItemType == MagnetItemType::MAGNET_ITEM_FOOTBALL_HELMET_2)
            {
                aImage = IMAGE_REANIM_ZOMBIE_FOOTBALL_HELMET2;
            }
            else if (aMagnetItem->mItemType == MagnetItemType::MAGNET_ITEM_FOOTBALL_HELMET_3)
            {
                aImage = IMAGE_REANIM_ZOMBIE_FOOTBALL_HELMET3;
            }
            else if (aMagnetItem->mItemType == MagnetItemType::MAGNET_ITEM_DOOR_1)
            {
                aImage = IMAGE_REANIM_ZOMBIE_SCREENDOOR1;
            }
            else if (aMagnetItem->mItemType == MagnetItemType::MAGNET_ITEM_DOOR_2)
            {
                aImage = IMAGE_REANIM_ZOMBIE_SCREENDOOR2;
            }
            else if (aMagnetItem->mItemType == MagnetItemType::MAGNET_ITEM_DOOR_3)
            {
                aImage = IMAGE_REANIM_ZOMBIE_SCREENDOOR3;
            }
            else if (aMagnetItem->mItemType >= MagnetItemType::MAGNET_ITEM_POGO_1 && aMagnetItem->mItemType <= MagnetItemType::MAGNET_ITEM_POGO_3)
            {
                aCelCol = (int)aMagnetItem->mItemType - (int)MagnetItemType::MAGNET_ITEM_POGO_1;
                aImage = IMAGE_ZOMBIEPOGO;
            }
            else if (aMagnetItem->mItemType == MagnetItemType::MAGNET_ITEM_LADDER_1)
            {
                aImage = IMAGE_REANIM_ZOMBIE_LADDER_1;
            }
            else if (aMagnetItem->mItemType == MagnetItemType::MAGNET_ITEM_LADDER_2)
            {
                aImage = IMAGE_REANIM_ZOMBIE_LADDER_1_DAMAGE1;
            }
            else if (aMagnetItem->mItemType == MagnetItemType::MAGNET_ITEM_LADDER_3)
            {
                aImage = IMAGE_REANIM_ZOMBIE_LADDER_1_DAMAGE2;
            }
            else if (aMagnetItem->mItemType == MagnetItemType::MAGNET_ITEM_LADDER_PLACED)
            {
                aImage = IMAGE_REANIM_ZOMBIE_LADDER_5;
            }
            else if (aMagnetItem->mItemType == MagnetItemType::MAGNET_ITEM_JACK_IN_THE_BOX)
            {
                aImage = IMAGE_REANIM_ZOMBIE_JACKBOX_BOX;
            }
            else if (aMagnetItem->mItemType == MagnetItemType::MAGNET_ITEM_PICK_AXE)
            {
                aImage = IMAGE_REANIM_ZOMBIE_DIGGER_PICKAXE;
            }
            else if (aMagnetItem->mItemType == MagnetItemType::MAGNET_ITEM_SILVER_COIN)
            {
                aScale = 1.0f;
                aImage = IMAGE_REANIM_COIN_SILVER_DOLLAR;
            }
            else if (aMagnetItem->mItemType == MagnetItemType::MAGNET_ITEM_GOLD_COIN)
            {
                aScale = 1.0f;
                aImage = IMAGE_REANIM_COIN_GOLD_DOLLAR;
            }
            else if (aMagnetItem->mItemType == MagnetItemType::MAGNET_ITEM_DIAMOND)
            {
                aScale = 1.0f;
                aImage = IMAGE_REANIM_DIAMOND;
            }
            else if (aMagnetItem->mItemType == MagnetItemType::MAGNET_ITEM_SUN)
            {
                aScale = 1.0f;
                aImage = IMAGE_REANIM_DIAMOND;
            }
            else if (aMagnetItem->mItemType == MagnetItemType::MAGNET_ITEM_SMALLSUN)
            {
                aScale = 1.0f;
                aImage = IMAGE_REANIM_DIAMOND;
            }
            else if (aMagnetItem->mItemType == MagnetItemType::MAGNET_ITEM_LARGESUN)
            {
                aScale = 1.0f;
                aImage = IMAGE_REANIM_DIAMOND;
            }
            else if (aMagnetItem->mItemType == MagnetItemType::MAGNET_ITEM_HELMET_1)
            {
                aImage = IMAGE_REANIM_ZOMBIE_GARGANTUAR_HELMET1;
            }
            else if (aMagnetItem->mItemType == MagnetItemType::MAGNET_ITEM_HELMET_2)
            {
                aImage = IMAGE_REANIM_ZOMBIE_GARGANTUAR_HELMET2;
            }
            else if (aMagnetItem->mItemType == MagnetItemType::MAGNET_ITEM_HELMET_3)
            {
                aImage = IMAGE_REANIM_ZOMBIE_GARGANTUAR_HELMET3;
            }
            else if (aMagnetItem->mItemType == MagnetItemType::MAGNET_ITEM_POT_1)
            {
                aImage = IMAGE_REANIM_ZOMBIE_POT1;
                }
            else if (aMagnetItem->mItemType == MagnetItemType::MAGNET_ITEM_POT_2)
            {
                aImage = IMAGE_REANIM_ZOMBIE_POT2;
                }
            else if (aMagnetItem->mItemType == MagnetItemType::MAGNET_ITEM_POT_3)
            {
                aImage = IMAGE_REANIM_ZOMBIE_POT3;
            }
            else
            {
                TOD_ASSERT();
            }

            if (aScale == 1.0f)
            {
                g->DrawImageCel(aImage, aMagnetItem->mPosX - mX + aOffsetX, aMagnetItem->mPosY - mY + aOffsetY, aCelCol, aCelRow);
            }
            else
            {
                TodDrawImageCelScaledF(g, aImage, aMagnetItem->mPosX - mX + aOffsetX, aMagnetItem->mPosY - mY + aOffsetY, aCelCol, aCelRow, aScale, aScale);
            }
        }
    }
}

Image* Plant::GetImage(SeedType theSeedType)
{
    Image** aImages = GetPlantDefinition(theSeedType).mPlantImage;
    return aImages ? aImages[0] : nullptr;
}

//0x465680
void Plant::DrawShadow(Sexy::Graphics* g, float theOffsetX, float theOffsetY)
{
    if (mSeedType == SeedType::SEED_LILYPAD || mSeedType == SeedType::SEED_STARFRUIT || mSeedType == SeedType::SEED_FUTURESTAR || mSeedType == SeedType::SEED_TANGLEKELP ||
        mSeedType == SeedType::SEED_SEASHROOM || mSeedType == SeedType::SEED_COBCANNON || mSeedType == SeedType::SEED_SPIKEWEED || 
        mSeedType == SeedType::SEED_SPIKEROCK || mSeedType == SeedType::SEED_GRAVEBUSTER || mSeedType == SeedType::SEED_CATTAIL || 
        mOnBungeeState == PlantOnBungeeState::RISING_WITH_BUNGEE)
        return;

    if (IsOnBoard() && mApp->mGameMode == GameMode::GAMEMODE_CHALLENGE_ZEN_GARDEN && mApp->mZenGarden->mGardenType == GardenType::GARDEN_MAIN)
        return;

    int aShadowType = 0;
    float aShadowOffsetX = -3.0f;
    float aShadowOffsetY = 51.0f;
    float aScale = 1.0f;
    if (mBoard && mBoard->StageIsNight())
    {
        aShadowType = 1;
    }

    if (mSeedType == SeedType::SEED_SQUASH || mSeedType == SeedType::SEED_SPOOKYSQUASH)
    {
        if (mBoard)
        {
            aShadowOffsetY += mBoard->GridToPixelY(mPlantCol, mRow) - mY;
        }
        aShadowOffsetY += 5.0f;
    }
    else if (mSeedType == SeedType::SEED_PUFFSHROOM || mSeedType == SeedType::SEED_MOLDSHROOM)
    {
        aScale = 0.5f;
        aShadowOffsetY = 42.0f;
    }
    else if (mSeedType == SeedType::SEED_SUNSHROOM)
    {
        aShadowOffsetY = 42.0f;
        if (mState == PlantState::STATE_SUNSHROOM_SMALL)
        {
            aScale = 0.5f;
        }
        else if (mState == PlantState::STATE_SUNSHROOM_GROWING)
        {
            Reanimation* aBodyReanim = mApp->ReanimationGet(mBodyReanimID);
            aScale = 0.5f + 0.5f * aBodyReanim->mAnimTime;
        }
    }
    else if (mSeedType == SeedType::SEED_UMBRELLA)
    {
        aScale = 0.5f;
        aShadowOffsetX = -7.0f;
        aShadowOffsetY = 52.0f;
    }
    else if (mSeedType == SeedType::SEED_FUMESHROOM || mSeedType == SeedType::SEED_ICYFUMESHROOM || mSeedType == SeedType::SEED_GLOOMSHROOM)
    {
        aScale = 1.3f;
        aShadowOffsetY = 47.0f;
    }
    else if (mSeedType == SeedType::SEED_PEPPERPULT || mSeedType == SeedType::SEED_CABBAGEPULT || mSeedType == SeedType::SEED_ICEBERGLETTUCEBETA || mSeedType == SeedType::SEED_MELONPULT || mSeedType == SeedType::SEED_WINTERMELON)
    {
        aShadowOffsetY = 47.0f;
    }
    else if (mSeedType == SeedType::SEED_KERNELPULT)
    {
        aShadowOffsetX = 0.0f;
        aShadowOffsetY = 47.0f;
    }
    else if (mSeedType == SeedType::SEED_SCAREDYSHROOM || mSeedType == SeedType::SEED_SHAMROCK || mSeedType == SeedType::SEED_SHAMROCKSTAR)
    {
        aShadowOffsetX = -9.0f;
        aShadowOffsetY = 55.0f;
    }
    else if (mSeedType == SeedType::SEED_CHOMPER || mSeedType == SeedType::SEED_SUPERCHOMPER)
    {
        aShadowOffsetX = -21.0f;
        aShadowOffsetY = 57.0f;
    }
    else if (mSeedType == SeedType::SEED_FLOWERPOT || mSeedType == SeedType::SEED_WATERPOT || mSeedType == SeedType::SEED_COFFEEPOT)
    {
        aShadowOffsetX = -4.0f;
        aShadowOffsetY = 46.0f;
    }
    else if (mSeedType == SeedType::SEED_TALLNUT || mSeedType == SeedType::SEED_VINENUT)
    {
        aShadowOffsetY = 54.0f;
        aScale = 1.3f;
    }
    else if (mSeedType == SeedType::SEED_PUMPKINSHELL)
    {
        aShadowOffsetY = 46.0f;
        aScale = 1.4f;
    }
    else if (mSeedType == SeedType::SEED_CACTUS)
    {
        aShadowOffsetX = -8.0f;
        aShadowOffsetY = 50.0f;
    }
    else if (mSeedType == SeedType::SEED_PLANTERN)
    {
        aShadowOffsetY = 57.0f;
    }
    else if (mSeedType == SeedType::SEED_INSTANT_COFFEE || mSeedType == SeedType::SEED_HOTPOTATO)
    {
        aShadowOffsetY = 71.0f;
    }
    else if (mSeedType == SeedType::SEED_GIANT_WALLNUT)
    {
        aShadowOffsetX = -33.0f;
        aShadowOffsetY = 56.0f;
        aScale = 1.7f;
    }

    if (Plant::IsFlying(mSeedType))
    {
        aShadowOffsetY += 10.0f;
        if (mBoard && (mBoard->GetTopPlantAt(mPlantCol, mRow, TOPPLANT_ONLY_NORMAL_POSITION) || mBoard->GetTopPlantAt(mPlantCol, mRow, TOPPLANT_ONLY_PUMPKIN)))
            return;
    }

    if (aShadowType == 0)
    {
        TodDrawImageCelCenterScaledF(g, IMAGE_PLANTSHADOW, theOffsetX + aShadowOffsetX, theOffsetY + aShadowOffsetY, 0, aScale, aScale);
    }
    else
    {
        TodDrawImageCelCenterScaledF(g, IMAGE_PLANTSHADOW2, theOffsetX + aShadowOffsetX, theOffsetY + aShadowOffsetY, 0, aScale, aScale);
    }
}

//0x465A90
void Plant::Draw(Graphics* g)
{
    float aOffsetX = 0.0f;
    float aOffsetY = PlantDrawHeightOffset(mBoard, this, mSeedType, mPlantCol, mRow);
    if (Plant::IsFlying(mSeedType) && mSquished)
    {
        aOffsetY += 30.0f;
    }
    if (mIceTrapCounter > 0)
    {
        TodDrawImageScaledF(g, IMAGE_ICETRAP, aOffsetX, aOffsetY, 1, 1);
    }

    int aImageIndex = mFrame;
    Image* aPlantImage = Plant::GetImage(mSeedType);

    if (mSquished)
    {
        if (mSeedType == SeedType::SEED_FLOWERPOT || mSeedType == SeedType::SEED_WATERPOT || mSeedType == SeedType::SEED_COFFEEPOT)
        {
            aOffsetY -= 15.0f;
        }
        else if (mSeedType == SeedType::SEED_INSTANT_COFFEE || mSeedType == SeedType::SEED_HOTPOTATO)
        {
            aOffsetY -= 20.0f;
        }

        g->SetScale(1.0f, 0.25f, 0.0f, 0.0f);
        DrawSeedType(g, mSeedType, mImitaterType, DrawVariation::VARIATION_NORMAL, aOffsetX, 60.0f + aOffsetY);
        g->SetScale(1.0f, 1.0f, 0.0f, 0.0f);
    }
    else
    {
        bool aDrawPumpkinBack = false;
        Plant* aPumpkin = nullptr;

        if (IsOnBoard())
        {
            aPumpkin = mBoard->GetPumpkinAt(mPlantCol, mRow);
            if (aPumpkin)
            {
                Plant* aPlantInPumpkin = mBoard->GetTopPlantAt(mPlantCol, mRow, PlantPriority::TOPPLANT_ONLY_NORMAL_POSITION);
                if (aPlantInPumpkin)
                {
                    if (aPlantInPumpkin->mRenderOrder > aPumpkin->mRenderOrder || aPlantInPumpkin->mOnBungeeState == GETTING_GRABBED_BY_BUNGEE)
                    {
                        aPlantInPumpkin = nullptr;
                    }
                }

                if (aPlantInPumpkin == this)
                {
                    aDrawPumpkinBack = true;
                }
                if (aPlantInPumpkin == nullptr && mSeedType == SeedType::SEED_PUMPKINSHELL)
                {
                    aDrawPumpkinBack = true;
                }
            }
            else if (mSeedType == SeedType::SEED_PUMPKINSHELL)
            {
                aDrawPumpkinBack = true;
                aPumpkin = this;
            }
        }
        else if (mSeedType == SeedType::SEED_PUMPKINSHELL)
        {
            aDrawPumpkinBack = true;
            aPumpkin = this;
        }

        DrawShadow(g, aOffsetX, aOffsetY);

        if (Plant::IsFlying(mSeedType))
        {
            int aCounter;
            if (IsOnBoard())
            {
                aCounter = mBoard->mMainCounter;
            }
            else
            {
                aCounter = mApp->mAppCounter;
            }

            float aTime = (mRow * 97 + mPlantCol * 61 + aCounter) * 0.03f;
            float aWave = sin(aTime) * 2.0f;
            aOffsetY += aWave;
        }

        if (aDrawPumpkinBack)
        {
            Reanimation* aPumpkinReanim = mApp->ReanimationGet(aPumpkin->mBodyReanimID);
            Graphics aPumpkinGraphics(*g);
            aPumpkinGraphics.mTransX += aPumpkin->mX - mX;
            aPumpkinGraphics.mTransY += aPumpkin->mY - mY;
            aPumpkinReanim->DrawRenderGroup(&aPumpkinGraphics, 1);
        }

        aOffsetX += mShakeOffsetX;
        aOffsetY += mShakeOffsetY;
        if (IsInPlay() && mApp->IsIZombieLevel())
        {
            mBoard->mChallenge->IZombieDrawPlant(g, this);
        }
        else if (mBodyReanimID != ReanimationID::REANIMATIONID_NULL)
        {
            Reanimation* aBodyReanim = mApp->ReanimationTryToGet(mBodyReanimID);
            if (aBodyReanim)
            {
                if (!mApp->Is3DAccelerated() && (mSeedType == SeedType::SEED_FLOWERPOT || mSeedType == SeedType::SEED_WATERPOT || mSeedType == SeedType::SEED_COFFEEPOT) && IsOnBoard() &&
                    aBodyReanim->mAnimRate == 0.0f && aBodyReanim->IsAnimPlaying("anim_idle"))
                {
                    mApp->mReanimatorCache->DrawCachedPlant(g, aOffsetX, aOffsetY, mSeedType, DrawVariation::VARIATION_NORMAL);
                }
                else
                {
                    aBodyReanim->Draw(g);
                }
            }
        }
        else
        {
            SeedType aSeedType = SeedType::SEED_NONE;
            if (mBoard)
            {
                aSeedType = mBoard->GetSeedTypeInCursor();
            }

            if (IsPartOfUpgradableTo(aSeedType) && mBoard->CanPlantAt(mPlantCol, mRow, aSeedType) == PlantingReason::PLANTING_OK)
            {
                g->SetColorizeImages(true);
                g->SetColor(GetFlashingColor(mBoard->mMainCounter, 90));
            }
            else if (aSeedType == SeedType::SEED_COBCANNON && mBoard->CanPlantAt(mPlantCol - 1, mRow, aSeedType) == PlantingReason::PLANTING_OK)
            {
                g->SetColorizeImages(true);
                g->SetColor(GetFlashingColor(mBoard->mMainCounter, 90));
            }
            else if (mBoard && mBoard->mTutorialState == TutorialState::TUTORIAL_SHOVEL_DIG)
            {
                g->SetColorizeImages(true);
                g->SetColor(GetFlashingColor(mBoard->mMainCounter, 90));
            }

            TodDrawImageCelF(g, aPlantImage, aOffsetX, aOffsetY, aImageIndex, 0);
            g->SetColorizeImages(false);
            if (mHighlighted)
            {
                g->SetDrawMode(Graphics::DRAWMODE_ADDITIVE);
                g->SetColorizeImages(true);
                g->SetColor(Color(255, 255, 255, 196));
                TodDrawImageCelF(g, aPlantImage, aOffsetX, aOffsetY, aImageIndex, 0);
                g->SetDrawMode(Graphics::DRAWMODE_NORMAL);
                g->SetColorizeImages(false);
            }
            else if (mEatenFlashCountdown > 0)
            {
                g->SetDrawMode(Graphics::DRAWMODE_ADDITIVE);
                g->SetColorizeImages(true);
                g->SetColor(Color(255, 255, 255, ClampInt(mEatenFlashCountdown * 3, 0, 255)));
                TodDrawImageCelF(g, aPlantImage, aOffsetX, aOffsetY, aImageIndex, 0);
                g->SetDrawMode(Graphics::DRAWMODE_NORMAL);
                g->SetColorizeImages(false);
            }
        }

        if (mSeedType == SeedType::SEED_MAGNETSHROOM && !DrawMagnetItemsOnTop())
        {
            DrawMagnetItems(g);
        }
    }
}

//0x4660B0
void Plant::DrawSeedType(Graphics* g, SeedType theSeedType, SeedType theImitaterType, DrawVariation theDrawVariation, float thePosX, float thePosY)
{
    Graphics aSeedG(*g);
    int aCelRow = 0;
    int aCelCol = 2;
    float aOffsetX = 0.0f;
    float aOffsetY = 0.0f;
    SeedType aSeedType = theSeedType;
    DrawVariation aDrawVariation = theDrawVariation;

    if (theSeedType == SeedType::SEED_IMITATER && theImitaterType != SeedType::SEED_NONE)
    {
        aSeedType = theImitaterType;
        aDrawVariation = DrawVariation::VARIATION_IMITATER;
        if (theImitaterType == SeedType::SEED_HYPNOSHROOM || theImitaterType == SeedType::SEED_SQUASH || theImitaterType == SeedType::SEED_SPOOKYSQUASH
            || theImitaterType == SeedType::SEED_POTATOMINE || theImitaterType == SeedType::SEED_GARLIC || theImitaterType == SeedType::SEED_LILYPAD)
            aDrawVariation = DrawVariation::VARIATION_IMITATER_LESS;
    }
    else if (theDrawVariation == DrawVariation::VARIATION_NORMAL && theSeedType == SeedType::SEED_TANGLEKELP)
    {
        aDrawVariation = DrawVariation::VARIATION_AQUARIUM;
    }

    if (((LawnApp*)gSexyAppBase)->mGameMode == GameMode::GAMEMODE_CHALLENGE_BIG_TIME &&
        (aSeedType == SeedType::SEED_WALLNUT || aSeedType == SeedType::SEED_SUNFLOWER || aSeedType == SeedType::SEED_MARIGOLD))
    {
        aSeedG.mScaleX *= 1.5f;
        aSeedG.mScaleY *= 1.5f;
        aOffsetX = -20.0f;
        aOffsetY = -40.0f;
    }
    if (aSeedType == SeedType::SEED_LEFTPEATER)
    {
        aOffsetX += aSeedG.mScaleX * 80.0f;
        aSeedG.mScaleX *= -1.0f;
    }

    if (Challenge::IsZombieSeedType(aSeedType))
    {
        ZombieType aZombieType = Challenge::IZombieSeedTypeToZombieType(aSeedType);
        gLawnApp->mReanimatorCache->DrawCachedZombie(&aSeedG, thePosX + aOffsetX, thePosY + aOffsetY, aZombieType);
    }
    else
    {
        const PlantDefinition& aPlantDef = GetPlantDefinition(aSeedType);

        if (aSeedType == SeedType::SEED_GIANT_WALLNUT)
        {
            aSeedG.mScaleX *= 1.4f;
            aSeedG.mScaleY *= 1.4f;
            TodDrawImageScaledF(&aSeedG, IMAGE_REANIM_WALLNUT_BODY, thePosX - 53.0f, thePosY - 56.0f, aSeedG.mScaleX, aSeedG.mScaleY);
        }
        else if (aPlantDef.mReanimationType != ReanimationType::REANIM_NONE)
        {
            gLawnApp->mReanimatorCache->DrawCachedPlant(&aSeedG, thePosX + aOffsetX, thePosY + aOffsetY, aSeedType, aDrawVariation);
        }
        else
        {
            if (aSeedType == SeedType::SEED_KERNELPULT)
            {
                aCelRow = 2;
            }
            else if (aSeedType == SeedType::SEED_TWINSUNFLOWER)
            {
                aCelRow = 1;
            }

            Image* aPlantImage = Plant::GetImage(aSeedType);
            if (aPlantImage->mNumCols <= 2)
            {
                aCelCol = aPlantImage->mNumCols - 1;
            }

            TodDrawImageCelScaledF(&aSeedG, aPlantImage, thePosX + aOffsetX, thePosY + aOffsetY, aCelCol, aCelRow, aSeedG.mScaleX, aSeedG.mScaleY);
        }
    }
}

//0x466390
void Plant::MouseDown(int x, int y, int theClickCount)
{
    if (theClickCount < 0)
        return;

    if (mState == PlantState::STATE_COBCANNON_READY)
    {
        mBoard->ClearCursor();
        mBoard->mCursorObject->mType = SeedType::SEED_NONE;
        mBoard->mCursorObject->mCursorType = CursorType::CURSOR_TYPE_COBCANNON_TARGET;
        mBoard->mCursorObject->mSeedBankIndex = -1;
        mBoard->mCursorObject->mCoinID = CoinID::COINID_NULL;
        mBoard->mCursorObject->mCobCannonPlantID = (PlantID)mBoard->mPlants.DataArrayGetID(this);
        mBoard->mCobCannonCursorDelayCounter = 30;
        mBoard->mCobCannonMouseX = x;
        mBoard->mCobCannonMouseY = y;
    }
    else if (mPlantFoodReady)
    {
        DoSpecial();
    }
}

//0x466420
void Plant::IceZombies()
{
    Zombie* aZombie = nullptr;
    while (mBoard->IterateZombies(aZombie))
    {
        aZombie->HitIceTrap();
        aZombie->TakeDamage(20, 1U);
    }

    mBoard->mIceTrapCounter = 300;
    TodParticleSystem* aPoolSparklyParticle = mApp->ParticleTryToGet(mBoard->mPoolSparklyParticleID);
    if (aPoolSparklyParticle)
    {
        aPoolSparklyParticle->mDontUpdate = false;
    }

    Zombie* aBossZombie = mBoard->GetBossZombie();
    if (aBossZombie)
    {
        aBossZombie->BossDestroyFireball();
    }
}

//0x4665B0
void Plant::BlowAwayFliers(int theX, int theRow)
{
    Zombie* aZombie = nullptr;
    while (mBoard->IterateZombies(aZombie))
    {
        if (!aZombie->IsDeadOrDying())
        {
            Rect aZombieRect = aZombie->GetZombieRect();
            if (aZombie->IsFloating())
            {
                aZombie->mBlowingAway = true;
            }
        }
    }

    mApp->PlaySample(SOUND_BLOVER);
    mBoard->mFogBlownCountDown = 4000;
}

//0x466650
void Plant::KillAllPlantsNearDoom()
{
    Plant* aPlant = nullptr;
    while (mBoard->IteratePlants(aPlant))
    {
        if (aPlant->mRow == mRow && aPlant->mPlantCol == mPlantCol)
        {
            aPlant->Die();
        }
    }
}

//0x4666A0
void Plant::DoSpecial()
{
    int aPosX = mX + mWidth / 2;
    int aPosY = mY + mHeight / 2;
    int aDamageRangeFlags = GetDamageRangeFlags(PlantWeapon::WEAPON_PRIMARY);

    switch (mSeedType)
    {
        case SeedType::SEED_BLOVER:
        {
            if (mState != PlantState::STATE_DOINGSPECIAL)
            {
                mState = PlantState::STATE_DOINGSPECIAL;
                BlowAwayFliers(mX, mRow);
            }
            break;
        }
        case SeedType::SEED_GOLDBLOOM:
        {
            if (mState != PlantState::STATE_DOINGSPECIAL)
            {
                mState = PlantState::STATE_DOINGSPECIAL;
                for (int i = 0; i < 3; i++)
                {
                    mBoard->AddCoin(mX, mY, COIN_SUN, CoinMotion::COIN_MOTION_FROM_PLANT);
                    Coin* aCustomSun = mBoard->AddCoin(mX, mY, COIN_SUN, CoinMotion::COIN_MOTION_FROM_PLANT);
                    aCustomSun->mValue = 3;
                    aCustomSun->mScale = aCustomSun->GetSunScale();
                }
            }
            break;
        }
        case SeedType::SEED_EXPLODE_O_NUT:
        {
            mApp->PlayFoley(FoleyType::FOLEY_CHERRYBOMB);
            mApp->PlaySample(SOUND_BOWLINGIMPACT2);

            int aDamageRangeFlags = GetDamageRangeFlags(PlantWeapon::WEAPON_PRIMARY) | 32U;
            int aDamage = mApp->IsWallnutBowlingLevel() ? 1800 : 900;
            mBoard->KillAllZombiesInRadius(mRow, aPosX, aPosY, 90, 1, true, aDamage, aDamageRangeFlags, 274U);
            mApp->AddTodParticle(aPosX, aPosY, (int)RenderLayer::RENDER_LAYER_TOP, ParticleEffect::PARTICLE_POWIE);
            mBoard->ShakeBoard(3, -4);

            Die();

            break;
        }
        case SeedType::SEED_HAZYNUT:
        {
            mApp->PlayFoley(FoleyType::FOLEY_CHERRYBOMB);
            mApp->PlaySample(SOUND_BOWLINGIMPACT2);

            int aDamageRangeFlags = GetDamageRangeFlags(PlantWeapon::WEAPON_PRIMARY) | 32U;
            int aDamage = mApp->IsWallnutBowlingLevel() ? 900 : 450;
            mBoard->KillAllZombiesInRadius(mRow, aPosX, aPosY, 90, 1, false, aDamage, aDamageRangeFlags, 82U);
            mApp->AddTodParticle(aPosX, aPosY, (int)RenderLayer::RENDER_LAYER_TOP, ParticleEffect::PARTICLE_POWIE);
            mBoard->ShakeBoard(3, -4);

            Die();

            break;
        }
        case SeedType::SEED_FROSTACHIO:
        {
            mApp->PlayFoley(FoleyType::FOLEY_CHERRYBOMB);
            mApp->PlaySample(SOUND_BOWLINGIMPACT2);

            int aDamageRangeFlags = GetDamageRangeFlags(PlantWeapon::WEAPON_PRIMARY) | 32U;
            int aDamage = mApp->IsWallnutBowlingLevel() ? 900 : 20;
            mBoard->KillAllZombiesInRadius(mRow, aPosX, aPosY, 90, 1, false, aDamage, aDamageRangeFlags, 146U);
            mApp->AddTodParticle(aPosX, aPosY, (int)RenderLayer::RENDER_LAYER_TOP, ParticleEffect::PARTICLE_POWIE);
            mBoard->ShakeBoard(3, -4);

            Die();

            break;
        }
        case SeedType::SEED_CHERRYBOMB:
        {
            mApp->PlayFoley(FoleyType::FOLEY_CHERRYBOMB);
            mApp->PlayFoley(FoleyType::FOLEY_JUICY);

            mBoard->KillAllZombiesInRadius(mRow, aPosX, aPosY, 115, 1, true, 1800, aDamageRangeFlags, 274U);

            mApp->AddTodParticle(aPosX, aPosY, (int)RenderLayer::RENDER_LAYER_TOP, ParticleEffect::PARTICLE_POWIE);
            mBoard->ShakeBoard(3, -4);

            Die();
            break;
        }
        case SeedType::SEED_SEASTARFRUIT:
        {
            mApp->PlayFoley(FoleyType::FOLEY_CHERRYBOMB);
            mApp->PlayFoley(FoleyType::FOLEY_JUICY);
            Fire(nullptr, mRow, PlantWeapon::WEAPON_PRIMARY);
            Die();
            break;
        }
        case SeedType::SEED_DOOMSHROOM:
        {
            mApp->PlaySample(SOUND_DOOMSHROOM);

            mBoard->KillAllZombiesInRadius(mRow, aPosX, aPosY, 250, 3, true, mApp->GetConfigCheckbox(true, CONFIG_CHECKBOX_NERF_DOOM) ? 900 : 1800, aDamageRangeFlags, 274U);
            if (mApp->GetConfigCheckbox(true, CONFIG_CHECKBOX_NERF_DOOM))
                mBoard->KillAllZombiesInRadius(mRow, aPosX, aPosY, 115, 1, true, 900, aDamageRangeFlags, 274U);
            KillAllPlantsNearDoom();

            mApp->AddTodParticle(aPosX, aPosY, (int)RenderLayer::RENDER_LAYER_TOP, ParticleEffect::PARTICLE_DOOM);
            mBoard->AddACrater(mPlantCol, mRow)->mGridItemCounter = 18000;
            mBoard->ShakeBoard(3, -4);

            Die();
            break;
        }
        case SeedType::SEED_JALAPENO:
        {
            mApp->PlayFoley(FoleyType::FOLEY_JALAPENO_IGNITE);
            mApp->PlayFoley(FoleyType::FOLEY_JUICY);

            mBoard->DoFwoosh(mRow);
            mBoard->ShakeBoard(3, -4);

            mBoard->KillZombieRow(mRow, true, 1800, GetDamageRangeFlags(WEAPON_PRIMARY), 274U);
            mBoard->mIceTimer[mRow] = 20;

            Die();
            break;
        }
        case SeedType::SEED_CHILLYPEPPER:
        {
            mApp->PlayFoley(FoleyType::FOLEY_FROZEN);
            mApp->PlayFoley(FoleyType::FOLEY_JUICY);

            mBoard->DoFwoosh(mRow);
            mBoard->ShakeBoard(3, -4);

            mBoard->KillZombieRow(mRow, false, 900, GetDamageRangeFlags(WEAPON_PRIMARY), 146U);

            Die();
            break;
        }
        case SeedType::SEED_PICKLEDPEPPER:
        {
            mApp->PlayFoley(FoleyType::FOLEY_JALAPENO_IGNITE);
            mApp->PlayFoley(FoleyType::FOLEY_JUICY);

            mBoard->DoFwooshCol(mPlantCol);
            mBoard->ShakeBoard(3, -4);

            mBoard->KillZombieCol(mPlantCol, true, 1800, GetDamageRangeFlags(WEAPON_PRIMARY), 274U);

            Die();
            break;
        }
        case SeedType::SEED_UMBRELLA:
        {
            if (mState != PlantState::STATE_UMBRELLA_TRIGGERED && mState != PlantState::STATE_UMBRELLA_REFLECTING)
            {
                mState = PlantState::STATE_UMBRELLA_TRIGGERED;
                mStateCountdown = 5;

                PlayBodyReanim("anim_block", ReanimLoopType::REANIM_PLAY_ONCE_AND_HOLD, 0, 22.0f);
            }

            break;
        }
        case SeedType::SEED_ICESHROOM:
        {
            mApp->PlayFoley(FoleyType::FOLEY_FROZEN);
            IceZombies();
            mApp->AddTodParticle(aPosX, aPosY, (int)RenderLayer::RENDER_LAYER_TOP, ParticleEffect::PARTICLE_ICE_TRAP);

            Die();
            break;
        }
        case SeedType::SEED_POPCORN:
        {
            aPosX = mX + mWidth / 2 - 20;
            aPosY = mY + mHeight / 2;

            mBoard->KillAllZombiesInRadius(mRow, aPosX, aPosY, 115, 1, true, 1800, aDamageRangeFlags, 274U);

            int aRenderOrder = Board::MakeRenderOrder(RenderLayer::RENDER_LAYER_GROUND, mRow, 2);
            mApp->AddTodParticle(aPosX, aPosY, aRenderOrder, ParticleEffect::PARTICLE_BLASTMARK);
            mApp->AddTodParticle(aPosX, aPosY, mRenderOrder + 1, ParticleEffect::PARTICLE_POPCORNSPLASH);
            mApp->PlaySample(SOUND_DOOMSHROOM);
            mBoard->ShakeBoard(3, -4);

            Die();
            break;
        }
        case SeedType::SEED_POTATOMINE:
        {
            aPosX = mX + mWidth / 2 - 20;
            aPosY = mY + mHeight / 2;

            mApp->PlaySample(SOUND_POTATO_MINE);
            mBoard->KillAllZombiesInRadius(mRow, aPosX, aPosY, 60, 0, false, 1800, aDamageRangeFlags, 18U);

            int aRenderPosition = Board::MakeRenderOrder(RenderLayer::RENDER_LAYER_PARTICLE, mRow, 0);
            mApp->AddTodParticle(aPosX + 20.0f, aPosY, aRenderPosition, ParticleEffect::PARTICLE_POTATO_MINE);
            mBoard->ShakeBoard(3, -4);

            Die();
            break;
        }
        case SeedType::SEED_ICEBERGLETTUCE:
        {
            Zombie* aZombie = FindTargetZombie(mRow, PlantWeapon::WEAPON_PRIMARY);
            if (aZombie && aZombie->CanBeFrozen())
            {
                mApp->PlayFoley(FoleyType::FOLEY_FROZEN);
                aZombie->HitIceTrapCustom(1000);
                Die();
            }
            break;
        }
        case SeedType::SEED_INSTANT_COFFEE:
        {
            Plant* aPlant = mBoard->GetTopPlantAt(mPlantCol, mRow, PlantPriority::TOPPLANT_ONLY_NORMAL_POSITION);
            if (aPlant && aPlant->mIsAsleep)
                aPlant->mWakeUpCounter = 100;
            else if (aPlant && mApp->GetConfigCheckbox(true, CONFIG_CHECKBOX_COFFEE_BOOST))
            {
                aPlant->mBoostCounter = 3000;
                if (mBoard->GetCoffeePotAt(mPlantCol, mRow))
                {
                    Plant* aIteratePlant = nullptr;
                    while (mBoard->IteratePlants(aIteratePlant))
                    {
                        if (mBoard->GetCoffeePotAt(aIteratePlant->mPlantCol, aIteratePlant->mRow)) aIteratePlant->mBoostCounter = 3000;
                    }
                }
            }

            mState = PlantState::STATE_DOINGSPECIAL;
            PlayBodyReanim("anim_crumble", ReanimLoopType::REANIM_PLAY_ONCE_AND_HOLD, 20, 22.0f);
            mApp->PlayFoley(FoleyType::FOLEY_COFFEE);

            break;
        }
        case SeedType::SEED_HOTPOTATO:
        {
            Plant* aPlant = mBoard->GetTopPlantAt(mPlantCol, mRow, PlantPriority::TOPPLANT_ONLY_NORMAL_POSITION);
            if (aPlant)
            {
                aPlant->mIceTrapCounter = 0;
                aPlant->mChilledCounter = 0;
                aPlant->mWarmCounter = 2000;
            }
            mState = PlantState::STATE_DOINGSPECIAL;
            PlayBodyReanim("anim_crumble", ReanimLoopType::REANIM_PLAY_ONCE_AND_HOLD, 20, 22.0f);
            mApp->PlayFoley(FoleyType::FOLEY_COFFEE);

            break;
        }
        case SeedType::SEED_OAKARCHER:
        {
            Zombie* aZombie = nullptr;
            while (mBoard->IterateZombies(aZombie))
            {
                if (!aZombie->IsDeadOrDying())
                    Fire(aZombie, aZombie->mRow, PlantWeapon::WEAPON_SECONDARY);
            }
            mState = PlantState::STATE_DOING_PLANT_FOOD;
            mStateCountdown = 70;
            mLaunchCounter = mLaunchCounter;
            mPlantFoodReady = false;
            
            break;
        }
        case SeedType::SEED_BONKCHOY:
        {
            mState = PlantState::STATE_DOING_PLANT_FOOD;
            mStateCountdown = 300;
            mLaunchCounter = mLaunchCounter;
            mPlantFoodReady = false;

            break;
        }
        case SeedType::SEED_LASERBEAN:
        {
            DoRowAreaDamage(1800, 2U, PlantWeapon::WEAPON_PRIMARY);
            mState = PlantState::STATE_DOING_PLANT_FOOD;
            mStateCountdown = 70;
            mLaunchCounter = mLaunchCounter;
            mPlantFoodReady = false;

            break;
        }
    }
}

//0x466B80
void Plant::ImitaterMorph()
{
    Die();
    Plant* aPlant = mBoard->AddPlant(mPlantCol, mRow, mImitaterType, SeedType::SEED_IMITATER);

    FilterEffect aFilter = FilterEffect::FILTER_EFFECT_WASHED_OUT;
    if (mImitaterType == SeedType::SEED_HYPNOSHROOM || mImitaterType == SeedType::SEED_SQUASH 
        || mImitaterType == SeedType::SEED_SPOOKYSQUASH || mImitaterType == SeedType::SEED_POTATOMINE
        || mImitaterType == SeedType::SEED_GARLIC || mImitaterType == SeedType::SEED_LILYPAD)
        aFilter = FilterEffect::FILTER_EFFECT_LESS_WASHED_OUT;

    Reanimation* aBodyReanim = mApp->ReanimationTryToGet(aPlant->mBodyReanimID);
    if (aBodyReanim)
    {
        aBodyReanim->mFilterEffect = aFilter;
    }
    Reanimation* aHeadReanim = mApp->ReanimationTryToGet(aPlant->mHeadReanimID);
    if (aHeadReanim)
    {
        aHeadReanim->mFilterEffect = aFilter;
    }Reanimation* aHeadReanim2 = mApp->ReanimationTryToGet(aPlant->mHeadReanimID2);
    if (aHeadReanim2)
    {
        aHeadReanim2->mFilterEffect = aFilter;
    }Reanimation* aHeadReanim3 = mApp->ReanimationTryToGet(aPlant->mHeadReanimID3);
    if (aHeadReanim3)
    {
        aHeadReanim3->mFilterEffect = aFilter;
    }
}

//0x466CA0
void Plant::UpdateImitater()
{
    if (mState != PlantState::STATE_IMITATER_MORPHING)
    {
        if (mStateCountdown == 0)
        {
            mState = PlantState::STATE_IMITATER_MORPHING;
            PlayBodyReanim("anim_explode", ReanimLoopType::REANIM_PLAY_ONCE_AND_HOLD, 0, 26.0f);
        }
    }
    else
    {
        Reanimation* aBodyReanim = mApp->ReanimationGet(mBodyReanimID);
        if (aBodyReanim->ShouldTriggerTimedEvent(0.8f))
        {
            mApp->AddTodParticle(mX + 40, mY + 40, (int)RenderLayer::RENDER_LAYER_TOP, ParticleEffect::PARTICLE_IMITATER_MORPH);
        }
        if (aBodyReanim->mLoopCount > 0)
        {
            ImitaterMorph();
        }
    }
}

//0x466D50
void Plant::CobCannonFire(int theTargetX, int theTargetY)
{
    TOD_ASSERT(mState == PlantState::STATE_COBCANNON_READY);

    mState = PlantState::STATE_COBCANNON_FIRING;
    mShootingCounter = 206;
    PlayBodyReanim("anim_shooting", ReanimLoopType::REANIM_PLAY_ONCE_AND_HOLD, 20, 12.0f);

    mTargetX = theTargetX - 47.0f;
    mTargetY = theTargetY;

    Reanimation* aBodyReanim = mApp->ReanimationGet(mBodyReanimID);
    ReanimatorTrackInstance* aTrackInstance = aBodyReanim->GetTrackInstanceByName("CobCannon_Cob");
    aTrackInstance->mTrackColor = Color::White;
}

//0x466E00
void Plant::Fire(Zombie* theTargetZombie, int theRow, PlantWeapon thePlantWeapon)
{
    if (mSeedType == SeedType::SEED_FUMESHROOM)
    {
        DoRowAreaDamage(20, 2U, PlantWeapon::WEAPON_PRIMARY);
        mApp->PlayFoley(FoleyType::FOLEY_FUME);
        return;
    }
    if (mSeedType == SeedType::SEED_LASERBEAN)
    {
        DoRowAreaDamage(40, 2U, PlantWeapon::WEAPON_PRIMARY);
        mApp->PlayFoley(FoleyType::FOLEY_FUME);
        return;
    }
    if (mSeedType == SeedType::SEED_ICYFUMESHROOM)
    {
        unsigned int iceDamageFlags = 2U;
        SetBit(iceDamageFlags, (int)DamageFlags::DAMAGE_FREEZE, true);
        DoRowAreaDamage(20, iceDamageFlags, PlantWeapon::WEAPON_PRIMARY);
        mApp->PlayFoley(FoleyType::FOLEY_FUME);
        return;
    }
    if (mSeedType == SeedType::SEED_GLOOMSHROOM)
    {
        DoRowAreaDamage(20, 2U, PlantWeapon::WEAPON_PRIMARY);
        return;
    }
    if (mSeedType == SeedType::SEED_STARFRUIT)
    {
        StarFruitFire();
        return;
    }
    if (mSeedType == SeedType::SEED_FUTURESTAR) {
        
        FutureStarFire(5, theTargetZombie);
        return;
    }

    ProjectileType aProjectileType;
    switch (mSeedType)
    {
    case SeedType::SEED_PEASHOOTER:

    case SeedType::SEED_REPEATER:
    case SeedType::SEED_THREEPEATER:
    case SeedType::SEED_SPLITPEA:
    case SeedType::SEED_GATLINGPEA:
    case SeedType::SEED_LEFTPEATER:
        aProjectileType = ProjectileType::PROJECTILE_PEA;
        break;
    case SeedType::SEED_SNOWPEA:
        aProjectileType = ProjectileType::PROJECTILE_SNOWPEA;
        break;
    case SeedType::SEED_PUFFSHROOM:
    case SeedType::SEED_SCAREDYSHROOM:
    case SeedType::SEED_SEASHROOM:
        aProjectileType = ProjectileType::PROJECTILE_PUFF;
        break;
    case SeedType::SEED_CACTUS:
    case SeedType::SEED_CATTAIL:
        aProjectileType = ProjectileType::PROJECTILE_SPIKE;
        break;
    case SeedType::SEED_PEPPERPULT:
        aProjectileType = ProjectileType::PROJECTILE_PEPPER;
        break;
    case SeedType::SEED_CABBAGEPULT:
        aProjectileType = ProjectileType::PROJECTILE_CABBAGE;
        break;
    case SeedType::SEED_ICEBERGLETTUCEBETA:
        aProjectileType = ProjectileType::PROJECTILE_LETTUCE;
        break;
    case SeedType::SEED_KERNELPULT:
        aProjectileType = ProjectileType::PROJECTILE_KERNEL;
        break;
    case SeedType::SEED_MELONPULT:
        aProjectileType = ProjectileType::PROJECTILE_MELON;
        break;
    case SeedType::SEED_WINTERMELON:
        aProjectileType = ProjectileType::PROJECTILE_WINTERMELON;
        break;
    case SeedType::SEED_COBCANNON:
        aProjectileType = ProjectileType::PROJECTILE_COBBIG;
        break;
        // New
    case SeedType::SEED_PEANIS:
        aProjectileType = ProjectileType::PROJECTILE_BOMB;
        break;
    case SeedType::SEED_FIREPEA:
        aProjectileType = ProjectileType::PROJECTILE_FIREPEA;
        break;
    case SeedType::SEED_PLASMAPEA:
        aProjectileType = ProjectileType::PROJECTILE_PLASMAPEA;
        mState = PlantState::STATE_NOTREADY;
        mStateCountdown = mStoredLaunchRate * 4;
        break;
    case SeedType::SEED_ICEQUEENPEA:
        aProjectileType = ProjectileType::PROJECTILE_ICEPEA;
        break;
    case SeedType::SEED_SWEETPEA:
        aProjectileType = ProjectileType::PROJECTILE_MINT;
        break;
    case SeedType::SEED_STUFFYSUNFLOWER:
        aProjectileType = ProjectileType::PROJECTILE_SUN;
        break;
    case SeedType::SEED_ASPEARAGUS:
    case SeedType::SEED_ACEPEARAGUS:
        aProjectileType = ProjectileType::PROJECTILE_ASPARAGUS;
        break;
    case SeedType::SEED_SHAMROCK:
    case SeedType::SEED_SHAMROCKSTAR:
        aProjectileType = ProjectileType::PROJECTILE_CABBAGE;
        break;
    case SeedType::SEED_OAKARCHER:
        aProjectileType = ProjectileType::PROJECTILE_ARROW;
        break;
    case SeedType::SEED_BAMBOOSHOOT:
        aProjectileType = ProjectileType::PROJECTILE_CABBAGE;
        break;
    case SeedType::SEED_BAMBOOM:
        aProjectileType = ProjectileType::PROJECTILE_MELON;
        break;
    case SeedType::SEED_FIREFLOWER:
        aProjectileType = ProjectileType::PROJECTILE_FIREBALL;
        break;
    case SeedType::SEED_BLOOMERANG:
        aProjectileType = ProjectileType::PROJECTILE_BOOMERANG;
        break;
    case SeedType::SEED_PRIMALPEASHOOTER:
        aProjectileType = ProjectileType::PROJECTILE_ROCKPEA;
        break;
    case SeedType::SEED_GOOPEASHOOTER:
        aProjectileType = ProjectileType::PROJECTILE_GOOPEA;
        break;
    case SeedType::SEED_SEASTARFRUIT:
        aProjectileType = ProjectileType::PROJECTILE_SEASTARFRUIT;
        break;
    case SeedType::SEED_MOLDSHROOM:
        aProjectileType = ProjectileType::PROJECTILE_MOLD;
        break;
    case SeedType::SEED_SPORESHROOM:
        aProjectileType = ProjectileType::PROJECTILE_SPORE;
        break;
    default:
        TOD_ASSERT();
        break;
    }
    if (mSeedType == SeedType::SEED_KERNELPULT && thePlantWeapon == PlantWeapon::WEAPON_SECONDARY)
    {
        aProjectileType = ProjectileType::PROJECTILE_BUTTER;
    }
    if (mSeedType == SeedType::SEED_OAKARCHER && thePlantWeapon == PlantWeapon::WEAPON_SECONDARY)
    {
        aProjectileType = ProjectileType::PROJECTILE_SPECIALARROW;
    }
    if (mSeedType == SeedType::SEED_PLASMAPEA && thePlantWeapon == PlantWeapon::WEAPON_SECONDARY)
    {
        aProjectileType = ProjectileType::PROJECTILE_BIGPLASMAPEA;
    }

    mApp->PlayFoley(FoleyType::FOLEY_THROW);
    if (mSeedType == SeedType::SEED_SNOWPEA || mSeedType == SeedType::SEED_WINTERMELON || mSeedType == SeedType::SEED_ICEQUEENPEA)
    {
        mApp->PlayFoley(FoleyType::FOLEY_SNOW_PEA_SPARKLES);
    }
    else if (mSeedType == SeedType::SEED_PUFFSHROOM || mSeedType == SeedType::SEED_SCAREDYSHROOM || mSeedType == SeedType::SEED_SEASHROOM)
    {
        mApp->PlayFoley(FoleyType::FOLEY_PUFF);
    }

    int aOriginX, aOriginY;
    if (mSeedType == SeedType::SEED_PUFFSHROOM || mSeedType == SeedType::SEED_MOLDSHROOM)
    {
        aOriginX = mX + 40;
        aOriginY = mY + 40;
    }
    else if (mSeedType == SeedType::SEED_SEASHROOM)
    {
        aOriginX = mX + 45;
        aOriginY = mY + 63;
    }
    else if (mSeedType == SeedType::SEED_PEPPERPULT || mSeedType == SeedType::SEED_CABBAGEPULT || mSeedType == SeedType::SEED_ICEBERGLETTUCEBETA)
    {
        aOriginX = mX + 5;
        aOriginY = mY - 12;
    }
    else if (mSeedType == SeedType::SEED_MELONPULT || mSeedType == SeedType::SEED_WINTERMELON)
    {
        aOriginX = mX + 25;
        aOriginY = mY - 46;
    }
    else if (mSeedType == SeedType::SEED_CATTAIL)
    {
        aOriginX = mX + 20;
        aOriginY = mY - 3;
    }
    else if (mSeedType == SeedType::SEED_KERNELPULT && thePlantWeapon == PlantWeapon::WEAPON_PRIMARY)
    {
        aOriginX = mX + 19;
        aOriginY = mY - 37;
    }
    else if (mSeedType == SeedType::SEED_KERNELPULT && thePlantWeapon == PlantWeapon::WEAPON_SECONDARY)
    {
        aOriginX = mX + 12;
        aOriginY = mY - 56;
    }
    else if (IsPeashooter(mSeedType))
    {
        int aOffsetX, aOffsetY;
        GetPeaHeadOffset(aOffsetX, aOffsetY);
        aOriginX = mX + aOffsetX + 24;
        aOriginY = mY + aOffsetY - 33;
    }
    else if (mSeedType == SeedType::SEED_LEFTPEATER)
    {
        int aOffsetX, aOffsetY;
        GetPeaHeadOffset(aOffsetX, aOffsetY);
        aOriginX = mX + aOffsetX + 27;
        aOriginY = mY + aOffsetY - 33;
    }
    else if (mSeedType == SeedType::SEED_GATLINGPEA)
    {
        int aOffsetX, aOffsetY;
        GetPeaHeadOffset(aOffsetX, aOffsetY);
        aOriginX = mX + aOffsetX + 34;
        aOriginY = mY + aOffsetY - 33;
    }
    else if (mSeedType == SeedType::SEED_SPLITPEA)
    {
        int aOffsetX, aOffsetY;
        GetPeaHeadOffset(aOffsetX, aOffsetY);
        aOriginY = mY + aOffsetY - 33;

        if (thePlantWeapon == PlantWeapon::WEAPON_SECONDARY)
        {
            aOriginX = mX + aOffsetX - 64;
        }
        else
        {
            aOriginX = mX + aOffsetX + 24;
        }
    }
    else if (mSeedType == SeedType::SEED_THREEPEATER)
    {
        aOriginX = mX + 45;
        aOriginY = mY + 10;
    }
    else if (mSeedType == SeedType::SEED_SCAREDYSHROOM || mSeedType == SeedType::SEED_SHAMROCK || mSeedType == SeedType::SEED_SHAMROCKSTAR)
    {
        aOriginX = mX + 29;
        aOriginY = mY + 21;
    }
    else if (mSeedType == SeedType::SEED_CACTUS)
    {
        if (thePlantWeapon == PlantWeapon::WEAPON_PRIMARY)
        {
            aOriginX = mX + 93;
            aOriginY = mY - 50;
        }
        else
        {
            aOriginX = mX + 70;
            aOriginY = mY + 23;
        }
    }
    else if (mSeedType == SeedType::SEED_COBCANNON)
    {
        aOriginX = mX - 44;
        aOriginY = mY - 184;
    }
    else
    {
        aOriginX = mX + 10;
        aOriginY = mY + 5;
    }
    if (mBoard->GetFlowerPotAt(mPlantCol, mRow) || mBoard->GetWaterPotAt(mPlantCol, mRow))
    {
        aOriginY -= 5;
    }
    
    if (mSeedType == SeedType::SEED_SNOWPEA ||  mSeedType == SeedType::SEED_ICEQUEENPEA)
    {
        int aRenderPosition = Board::MakeRenderOrder(RenderLayer::RENDER_LAYER_LAWN_MOWER, mRow, 1);
        mApp->AddTodParticle(aOriginX + 8, aOriginY + 13, aRenderPosition, ParticleEffect::PARTICLE_SNOWPEA_PUFF);
    }
    else if (mSeedType == SeedType::SEED_PUFFSHROOM)
    {
        int aRenderPosition = Board::MakeRenderOrder(RenderLayer::RENDER_LAYER_LAWN_MOWER, mRow, 1);
        mApp->AddTodParticle(aOriginX + 18, aOriginY + 13, aRenderPosition, ParticleEffect::PARTICLE_PUFFSHROOM_MUZZLE);
    }
    else if (mSeedType == SeedType::SEED_SCAREDYSHROOM || mSeedType == SeedType::SEED_SHAMROCK || mSeedType == SeedType::SEED_SHAMROCKSTAR)
    {
        int aRenderPosition = Board::MakeRenderOrder(RenderLayer::RENDER_LAYER_LAWN_MOWER, mRow, 1);
        mApp->AddTodParticle(aOriginX + 27, aOriginY + 13, aRenderPosition, ParticleEffect::PARTICLE_PUFFSHROOM_MUZZLE);
    }

    Projectile* aProjectile = mBoard->AddProjectile(aOriginX, aOriginY, mRenderOrder - 1, theRow, aProjectileType);
    aProjectile->mDamageRangeFlags = GetDamageRangeFlags(thePlantWeapon);

    if (aProjectile->mProjectileType == ProjectileType::PROJECTILE_FIREPEA) {
        float aOffsetX = -25.0f;
        float aOffsetY = -25.0f;
        Reanimation* aFirePeaReanim = mApp->AddReanimation(0.0f, 0.0f, 0, ReanimationType::REANIM_FIRE_PEA);
        if (aProjectile->mMotionType == ProjectileMotion::MOTION_BACKWARDS)
        {
            aFirePeaReanim->OverrideScale(-1.0f, 1.0f);
            aOffsetX += 80.0f;
        }

        aFirePeaReanim->SetPosition(aProjectile->mPosX + aOffsetX, aProjectile->mPosY + aOffsetY);
        aFirePeaReanim->mLoopType = ReanimLoopType::REANIM_LOOP;
        aFirePeaReanim->mAnimRate = RandRangeFloat(50.0f, 80.0f);
        AttachReanim(aProjectile->mAttachmentID, aFirePeaReanim, aOffsetX, aOffsetY);
    }
    else if (aProjectile->mProjectileType == ProjectileType::PROJECTILE_PLASMAPEA) {
        float aOffsetX = -25.0f;
        float aOffsetY = -25.0f;
        Reanimation* aFirePeaReanim = mApp->AddReanimation(0.0f, 0.0f, 0, ReanimationType::REANIM_PLASMA_PEA);
        if (aProjectile->mMotionType == ProjectileMotion::MOTION_BACKWARDS)
        {
            aFirePeaReanim->OverrideScale(-1.0f, 1.0f);
            aOffsetX += 80.0f;
        }

        aFirePeaReanim->SetPosition(aProjectile->mPosX + aOffsetX, aProjectile->mPosY + aOffsetY);
        aFirePeaReanim->mLoopType = ReanimLoopType::REANIM_LOOP;
        aFirePeaReanim->mAnimRate = RandRangeFloat(50.0f, 80.0f);
        AttachReanim(aProjectile->mAttachmentID, aFirePeaReanim, aOffsetX, aOffsetY);
    }
    else if (aProjectile->mProjectileType == ProjectileType::PROJECTILE_BIGPLASMAPEA) {
        float aOffsetX = -64.0f;
        float aOffsetY = -64.0f;
        Reanimation* aFirePeaReanim = mApp->AddReanimation(0.0f, 0.0f, 0, ReanimationType::REANIM_PLASMA_PEA);
        if (aProjectile->mMotionType == ProjectileMotion::MOTION_BACKWARDS)
        {
            aFirePeaReanim->OverrideScale(-2.0f, 2.0f);
            aOffsetX += 80.0f;
        }
        aFirePeaReanim->OverrideScale(2.0f, 2.0f);
        aFirePeaReanim->SetPosition(aProjectile->mPosX + aOffsetX, aProjectile->mPosY + aOffsetY);
        aFirePeaReanim->mLoopType = ReanimLoopType::REANIM_LOOP;
        aFirePeaReanim->mAnimRate = RandRangeFloat(50.0f, 80.0f);
        AttachReanim(aProjectile->mAttachmentID, aFirePeaReanim, aOffsetX, aOffsetY);
    }
    else if (aProjectile->mProjectileType == ProjectileType::PROJECTILE_SUN) {
        float aOffsetX = aProjectile->mWidth * 0.5f;
        float aOffsetY = aProjectile->mHeight * 0.5f;
        ReanimationType aReanimationType = ReanimationType::REANIM_SUN;
        Reanimation* aSunReanim = mApp->AddReanimation(0.0f, 0.0f, 0, aReanimationType);
        aSunReanim->SetPosition(aProjectile->mPosX + aOffsetX, aProjectile->mPosY + aOffsetY);
        aSunReanim->mLoopType = ReanimLoopType::REANIM_LOOP;
        aSunReanim->mAnimRate = 6.0f;
        AttachReanim(aProjectile->mAttachmentID, aSunReanim, aOffsetX, aOffsetY);
    }

    if (mSeedType == SeedType::SEED_PEPPERPULT || mSeedType == SeedType::SEED_CABBAGEPULT || mSeedType == SeedType::SEED_ICEBERGLETTUCEBETA 
        || mSeedType == SeedType::SEED_KERNELPULT || mSeedType == SeedType::SEED_MELONPULT || mSeedType == SeedType::SEED_WINTERMELON
        || mSeedType == SeedType::SEED_MOLDSHROOM || mSeedType == SeedType::SEED_BAMBOOSHOOT || mSeedType == SeedType::SEED_BAMBOOM
        || mSeedType == SeedType::SEED_SPORESHROOM
        )
    {
        float aRangeX, aRangeY, aRangeZ;
        if (theTargetZombie)
        {
            Rect aZombieRect = theTargetZombie->GetZombieRect();
            aRangeX = theTargetZombie->ZombieTargetLeadX(50.0f) - aOriginX - 30.0f;
            int aOffsetY = mBoard->GridToPixelY(mBoard->PixelToGridX(aZombieRect.mX, aZombieRect.mY), theRow) - mBoard->GridToPixelY(mBoard->PixelToGridX(aZombieRect.mX, aZombieRect.mY), mRow);
            aRangeY = aOffsetY;
            aRangeZ = aZombieRect.mY - aOriginY - aOffsetY;

            if (theTargetZombie->mZombiePhase == ZombiePhase::PHASE_DOLPHIN_RIDING)
            {
                aRangeX -= 60.0f;
            }
            if (Zombie::IsMachine(theTargetZombie->mZombieType) && theTargetZombie->mHasObject)
            {
                aRangeX -= 60.0f;
            }
            if (theTargetZombie->mZombiePhase == ZombiePhase::PHASE_SNORKEL_WALKING_IN_POOL)
            {
                aRangeX -= 40.0f;
            }
            if (theTargetZombie->mZombieType == ZombieType::ZOMBIE_BOSS)
            {
                aRangeY = mBoard->GridToPixelY(8, mRow) - aOriginY;
            }
        }
        else
        {
            aRangeX = 700.0f - aOriginX;
            aRangeY = 0.0f;
            aRangeZ = 0.0f;
        }
        if (aRangeX < 40.0f)
        {
            aRangeX = 40.0f;
        }

        aProjectile->mMotionType = ProjectileMotion::MOTION_LOBBED;
        aProjectile->mVelX = aRangeX / 120.0f;
        aProjectile->mVelY = aRangeY / 120.0f;
        aProjectile->mVelZ = aRangeZ / 120.0f - 7.0f;
        aProjectile->mAccZ = 0.115f;
        aProjectile->mRow = theRow;
    }
    else if (mSeedType == SeedType::SEED_FIREFLOWER)
    {
        aProjectile->mMotionType = ProjectileMotion::MOTION_LOBBED;
        aProjectile->mVelX = 3;
        aProjectile->mVelZ = -3.5f;
        aProjectile->mAccZ = 0.115f;
        aProjectile->mRow = mRow;
    }
    else if (mSeedType == SeedType::SEED_BLOOMERANG)
    {
        
        float aRangeX;
        int aTime = 200;
        if (theTargetZombie)
        {
            aTime *= (theTargetZombie->mPosX - aOriginX) / 640.0f;
            aRangeX = theTargetZombie->ZombieTargetLeadX(aTime / 2) + 60.0f;

            if (theTargetZombie->mZombiePhase == ZombiePhase::PHASE_DOLPHIN_RIDING)
            {
                aRangeX -= 60.0f;
            }
            if (Zombie::IsMachine(theTargetZombie->mZombieType) && theTargetZombie->mHasObject)
            {
                aRangeX -= 60.0f;
            }
            if (theTargetZombie->mZombiePhase == ZombiePhase::PHASE_SNORKEL_WALKING_IN_POOL)
            {
                aRangeX -= 40.0f;
            }
        }
        else
        {
            aRangeX = 700.0f;
        }
        if (aRangeX < 40.0f)
        {
            aRangeX = 40.0f;
        }

        aProjectile->mMotionType = ProjectileMotion::MOTION_BOOMERANG;
        aProjectile->mTargetX = aRangeX;
        aProjectile->mOriginX = aOriginX;
        aProjectile->mTime = aTime;
    }
    else if (mSeedType == SeedType::SEED_THREEPEATER)
    {
        if (theRow < mRow)
        {
            aProjectile->mMotionType = ProjectileMotion::MOTION_THREEPEATER;
            aProjectile->mVelY = -3.0f;
            aProjectile->mShadowY += 80.0f;
        }
        else if (theRow > mRow)
        {
            aProjectile->mMotionType = ProjectileMotion::MOTION_THREEPEATER;
            aProjectile->mVelY = 3.0f;
            aProjectile->mShadowY -= 80.0f;
        }
    }
    else if (mSeedType == SeedType::SEED_PUFFSHROOM || mSeedType == SeedType::SEED_SEASHROOM)
    {
        aProjectile->mMotionType = ProjectileMotion::MOTION_PUFF;
    }
    else if (mSeedType == SeedType::SEED_SPLITPEA && thePlantWeapon == PlantWeapon::WEAPON_SECONDARY)
    {
        aProjectile->mMotionType = ProjectileMotion::MOTION_BACKWARDS;
    }
    else if (mSeedType == SeedType::SEED_LEFTPEATER)
    {
        aProjectile->mMotionType = ProjectileMotion::MOTION_BACKWARDS;
    }
    else if (mSeedType == SeedType::SEED_CATTAIL)
    {
        aProjectile->mVelX = 2.0f;
        aProjectile->mMotionType = ProjectileMotion::MOTION_HOMING;
        aProjectile->mTargetZombieID = mBoard->ZombieGetID(theTargetZombie);
    }
    else if (mSeedType == SeedType::SEED_COBCANNON)
    {
        aProjectile->mVelX = 0.001f;
        aProjectile->mDamageRangeFlags = GetDamageRangeFlags(PlantWeapon::WEAPON_PRIMARY);
        aProjectile->mMotionType = ProjectileMotion::MOTION_LOBBED;
        aProjectile->mVelY = 0.0f;
        aProjectile->mAccZ = 0.0f;
        aProjectile->mVelZ = -8.0f;
        aProjectile->mCobTargetX = mTargetX - 40;
        aProjectile->mCobTargetRow = mBoard->PixelToGridYKeepOnBoard(mTargetX, mTargetY);
    }
    else if (mSeedType == SeedType::SEED_OAKARCHER && thePlantWeapon == PlantWeapon::WEAPON_SECONDARY)
    {
        aProjectile->mVelX = 0.001f;
        aProjectile->mDamageRangeFlags = GetDamageRangeFlags(PlantWeapon::WEAPON_SECONDARY);
        aProjectile->mMotionType = ProjectileMotion::MOTION_LOBBED;
        aProjectile->mVelY = 0.0f;
        aProjectile->mAccZ = 0.0f;
        aProjectile->mVelZ = -8.0f;
        aProjectile->mCobTargetX = theTargetZombie->ZombieTargetLeadX(50.0f) - 30.0f;
        aProjectile->mCobTargetRow = theRow;
    }
}

//0x4675C0
Zombie* Plant::FindTargetZombie(int theRow, PlantWeapon thePlantWeapon)
{
    int aDamageRangeFlags = GetDamageRangeFlags(thePlantWeapon);
    Rect aAttackRect = GetPlantAttackRect(thePlantWeapon);
    int aHighestWeight = 0;
    Zombie* aBestZombie = nullptr;

    Zombie* aZombie = nullptr;
    while (mBoard->IterateZombies(aZombie))
    {
        int aRowDeviation = aZombie->mRow - theRow;
        if (aZombie->mZombieType == ZombieType::ZOMBIE_BOSS)
        {
            aRowDeviation = 0;
        }

        if (!aZombie->mHasHead || aZombie->IsTangleKelpTarget())
        {
            if (mSeedType == SeedType::SEED_POTATOMINE || mSeedType == SeedType::SEED_CHOMPER || mSeedType == SeedType::SEED_SUPERCHOMPER || mSeedType == SeedType::SEED_TANGLEKELP || mSeedType == SeedType::SEED_ICEBERGLETTUCE)
            {
                continue;
            }
        }

        bool needPortalCheck = false;
        if (mApp->IsPortalLevel())
        {
            if (mSeedType == SeedType::SEED_PEASHOOTER || mSeedType == SeedType::SEED_CACTUS || mSeedType == SeedType::SEED_REPEATER)
            {
                needPortalCheck = true;
            }
        }

        if (mSeedType != SeedType::SEED_CATTAIL && mSeedType != SeedType::SEED_FUTURESTAR)
        {
            if (mSeedType == SeedType::SEED_GLOOMSHROOM || mSeedType == SeedType::SEED_BEESHOOTER)
            {
                if (aRowDeviation < -1 || aRowDeviation > 1)
                {
                    continue;
                }
            }
            else if (needPortalCheck)
            {
                if (!mBoard->mChallenge->CanTargetZombieWithPortals(this, aZombie))
                {
                    continue;
                }
            }
            else if (aRowDeviation)
            {
                continue;
            }
        }

        if (aZombie->EffectedByDamage(aDamageRangeFlags))
        {
            int aExtraRange = 0;

            if (mSeedType == SeedType::SEED_CHOMPER || mSeedType == SeedType::SEED_SUPERCHOMPER || mSeedType == SeedType::SEED_BEET || mSeedType == SeedType::SEED_BEETBOXER)
            {
                if (aZombie->mZombiePhase == ZombiePhase::PHASE_DIGGER_WALKING)
                {
                    aAttackRect.mX += 20;
                    aAttackRect.mWidth -= 20;
                }

                if (aZombie->mZombiePhase == ZombiePhase::PHASE_POGO_BOUNCING || (Zombie::IsBungee(aZombie->mZombieType) && aZombie->mTargetCol == mPlantCol))
                {
                    continue;
                }

                if (aZombie->mIsEating || mState == PlantState::STATE_CHOMPER_BITING)
                {
                    aExtraRange = 60;
                }
            }

            if (mSeedType == SeedType::SEED_POTATOMINE || mSeedType == SeedType::SEED_POPCORN || mSeedType == SeedType::SEED_ICEBERGLETTUCE)
            {
                if ((Zombie::IsPogo(aZombie->mZombieType) && aZombie->mHasObject) ||
                    aZombie->mZombiePhase == ZombiePhase::PHASE_POLEVAULTER_IN_VAULT || aZombie->mZombiePhase == ZombiePhase::PHASE_POLEVAULTER_PRE_VAULT)
                {
                    continue;
                }

                if (Zombie::IsPole(aZombie->mZombieType))
                {
                    aAttackRect.mX += 40;
                    aAttackRect.mWidth -= 40;  // 原版经典土豆地雷 Bug 及“四撑杆引雷”的原理
                }

                if (Zombie::IsBungee(aZombie->mZombieType) && aZombie->mTargetCol != mPlantCol)
                {
                    continue;
                }

                if (aZombie->mIsEating)
                {
                    aExtraRange = 30;
                }
            }

            if (((mSeedType == SeedType::SEED_EXPLODE_O_NUT || mSeedType == SeedType::SEED_HAZYNUT || mSeedType == SeedType::SEED_FROSTACHIO) && aZombie->mZombiePhase == ZombiePhase::PHASE_POLEVAULTER_IN_VAULT) ||
                (mSeedType == SeedType::SEED_TANGLEKELP && mBoard->IsPoolSquare(mPlantCol, mRow) != aZombie->mInPool))
            {
                continue;
            }

            Rect aZombieRect = aZombie->GetZombieRect();
            if (!needPortalCheck && GetRectOverlap(aAttackRect, aZombieRect) < -aExtraRange)
            {
                continue;
            }

            ////////////////////

            int aWeight = -aZombieRect.mX;
            if (mSeedType == SeedType::SEED_CATTAIL || mSeedType == SeedType::SEED_FUTURESTAR || mSeedType == SeedType::SEED_BEESHOOTER)
            {
                Rect aZombieRect = aZombie->GetZombieRect();
                int aDistance = Distance2D(mX + 40.0f, mY + 40.0f, aZombieRect.mX + aZombieRect.mWidth / 2, aZombieRect.mY + aZombieRect.mHeight / 2);
                int aWeight = -pow(aDistance, 2) + pow(aZombie->Health(), 2);
                if (aZombie->IsFloating())
                {
                    aWeight += 10000;  // 优先攻击飞行单位
                }
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

//0x4678B0
int Plant::DistanceToClosestZombie()
{
    int aDamageRangeFlags = GetDamageRangeFlags(PlantWeapon::WEAPON_PRIMARY);
    Rect aAttackRect = GetPlantAttackRect(PlantWeapon::WEAPON_PRIMARY);
    int aClosestDistance = 1000;

    Zombie* aZombie = nullptr;
    while (mBoard->IterateZombies(aZombie))
    {
        if (aZombie->mRow == mRow && aZombie->EffectedByDamage(aDamageRangeFlags))
        {
            Rect aZombieRect = aZombie->GetZombieRect();
            int aDistance = -GetRectOverlap(aAttackRect, aZombieRect);
            if (aDistance < aClosestDistance)
            {
                aClosestDistance = max(aDistance, 0);
            }
        }
    }

    return aClosestDistance;
}

//0x4679B0
void Plant::Die()
{
    if (IsOnBoard() && mSeedType == SeedType::SEED_TANGLEKELP)
    {
        Zombie* aZombie = mBoard->ZombieTryToGet(mTargetZombieID);
        if (aZombie)
        {
            aZombie->DieWithLoot();
        }
    }

    mDead = true;
    RemoveEffects();

    if (!Plant::IsFlying(mSeedType) && IsOnBoard())
    {
        GridItem* aLadder = mBoard->GetLadderAt(mPlantCol, mRow);
        if (aLadder)
        {
            aLadder->GridItemDie();
        }
    }

    if (IsOnBoard())
    {
        Plant* aTopPlant = mBoard->GetTopPlantAt(mPlantCol, mRow, PlantPriority::TOPPLANT_BUNGEE_ORDER);
        Plant* aFlowerPot = mBoard->GetFlowerPotAt(mPlantCol, mRow);
        if (aFlowerPot && aTopPlant == aFlowerPot)
        {
            Reanimation* aPotReanim = mApp->ReanimationGet(aFlowerPot->mBodyReanimID);
            aPotReanim->mAnimRate = RandRangeFloat(10.0f, 15.0f);
        }
    }
}

PlantDefinition& GetPlantDefinition(SeedType theSeedType)
{
    TOD_ASSERT(gPlantDefs[theSeedType].mSeedType == theSeedType);
    TOD_ASSERT(theSeedType >= 0 && theSeedType < (int)SeedType::NUM_SEED_TYPES);
    
    return gPlantDefs[theSeedType];
}

//0x467B00
int Plant::GetCost(SeedType theSeedType, SeedType theImitaterType)
{
    if (gLawnApp->mGameMode == GameMode::GAMEMODE_CHALLENGE_BEGHOULED || gLawnApp->mGameMode == GameMode::GAMEMODE_CHALLENGE_BEGHOULED_TWIST)
    {
        if (theSeedType == SeedType::SEED_REPEATER)
        {
            return 80;
        }
        else if (theSeedType == SeedType::SEED_FUMESHROOM)
        {
            return 40;
        }
        else if (theSeedType == SeedType::SEED_TALLNUT)
        {
            return 20;
        }
        else if (theSeedType == SeedType::SEED_FUTURESTAR)
        {
            return 80;
        }
        else if (theSeedType == SeedType::SEED_BEGHOULED_BUTTON_SHUFFLE)
        {
            return 8;
        }
        else if (theSeedType == SeedType::SEED_BEGHOULED_BUTTON_CRATER)
        {
            return 16;
        }
    }

    switch (theSeedType)
    {
    case SeedType::SEED_SLOT_MACHINE_SUN:           return 0;
    case SeedType::SEED_SLOT_MACHINE_DIAMOND:       return 0;
    case SeedType::SEED_ZOMBIQUARIUM_SNORKLE:       return 8;
    case SeedType::SEED_ZOMBIQUARIUM_TROPHY:        return 80;
    case SeedType::SEED_ZOMBIE_NORMAL:              return 4;
    case SeedType::SEED_ZOMBIE_TRAFFIC_CONE:        return 6;
    case SeedType::SEED_ZOMBIE_POLEVAULTER:         return 6;
    case SeedType::SEED_ZOMBIE_PAIL:                return 10;
    case SeedType::SEED_ZOMBIE_LADDER:              return 12;
    case SeedType::SEED_ZOMBIE_DIGGER:              return 10;
    case SeedType::SEED_ZOMBIE_BUNGEE:              return 10;
    case SeedType::SEED_ZOMBIE_FOOTBALL:            return 14;
    case SeedType::SEED_ZOMBIE_BALLOON:             return 12;
    case SeedType::SEED_ZOMBIE_SCREEN_DOOR:         return 8;
    case SeedType::SEED_ZOMBONI:                    return 14;
    case SeedType::SEED_ZOMBIE_POGO:                return 16;
    case SeedType::SEED_ZOMBIE_DANCER:              return 28;
    case SeedType::SEED_ZOMBIE_GARGANTUAR:          return 24;
    case SeedType::SEED_ZOMBIE_IMP:                 return 4;
    default:
    {
        if (gLawnApp->GetConfigCheckbox(true, CONFIG_CHECKBOX_NERF_DOOM) && (theSeedType == SeedType::SEED_DOOMSHROOM || theImitaterType == SeedType::SEED_DOOMSHROOM))
        {
            return 14;
        }
        if (theSeedType == SeedType::SEED_IMITATER && theImitaterType != SeedType::SEED_NONE)
        {
            const PlantDefinition& aPlantDef = GetPlantDefinition(theImitaterType);
            return aPlantDef.mSeedCost;
        }
        else
        {
            const PlantDefinition& aPlantDef = GetPlantDefinition(theSeedType);
            return aPlantDef.mSeedCost;
        }
    }
    }
}

PlantSoundType Plant::GetPlantSoundType(SeedType theSeedType) {
    const PlantDefinition& aPlantDef = GetPlantDefinition(theSeedType);
    return aPlantDef.mPlantSound;
}

//0x467C00
SexyString Plant::GetNameString(SeedType theSeedType, SeedType theImitaterType)
{
    const PlantDefinition& aPlantDef = GetPlantDefinition(theSeedType);
    SexyString aName = StrFormat(_S("[%s]"), aPlantDef.mPlantName);
    SexyString aTranslatedName = TodStringTranslate(StringToSexyStringFast(aName));

    if (theSeedType == SeedType::SEED_IMITATER && theImitaterType != SeedType::SEED_NONE)
    {
        const PlantDefinition& aImitaterDef = GetPlantDefinition(theImitaterType);
        std::string aImitaterName = StrFormat(_S("[%s]"), aImitaterDef.mPlantName);
        std::string aTranslatedImitaterName = TodStringTranslate(StringToSexyStringFast(aImitaterName));
        return StrFormat(_S("%s %s"), aTranslatedName.c_str(), aTranslatedImitaterName.c_str());
    }

    return aTranslatedName;
}

//0x467DB0
SexyString Plant::GetToolTip(SeedType theSeedType)
{
    const PlantDefinition& aPlantDef = GetPlantDefinition(theSeedType);
    SexyString aToolTip = StrFormat(_S("[%s_TOOLTIP]"), aPlantDef.mPlantName);
    return TodStringTranslate(aToolTip);
}

//0x467E30
int Plant::GetRefreshTime(SeedType theSeedType, SeedType theImitaterType)
{
    if (Challenge::IsZombieSeedType(theSeedType))
    {
        return 0;
    }

    if (theSeedType == SeedType::SEED_IMITATER && theImitaterType != SeedType::SEED_NONE)
    {
        const PlantDefinition& aPlantDef = GetPlantDefinition(theImitaterType);
        return aPlantDef.mRefreshTime;
    }
    else
    {
        const PlantDefinition& aPlantDef = GetPlantDefinition(theSeedType);
        return aPlantDef.mRefreshTime;
    }
}

//0x467EF0
Rect Plant::GetPlantRect()
{
    Rect aRect;
    if (mSeedType == SeedType::SEED_TALLNUT || mSeedType == SeedType::SEED_VINENUT)
    {
        aRect = Rect(mX + 10, mY, mWidth, mHeight);
    }
    else if (mSeedType == SeedType::SEED_PUMPKINSHELL)
    {
        aRect = Rect(mX, mY, mWidth - 20, mHeight);
    }
    else if (mSeedType == SeedType::SEED_COBCANNON)
    {
        aRect = Rect(mX, mY, 140, 80);
    }
    else
    {
        aRect = Rect(mX + 10, mY, mWidth - 20, mHeight);
    }

    return aRect;
}
//0x467F90
Rect Plant::GetPlantAttackRect(PlantWeapon thePlantWeapon)
{
    Rect aRect;
    if (mApp->IsWallnutBowlingLevel() && IsWallNut(mSeedType))
    {
        aRect = Rect(mX, mY, mWidth - 20, mHeight);
    }
    else if (thePlantWeapon == PlantWeapon::WEAPON_SECONDARY && mSeedType == SeedType::SEED_SPLITPEA)
    {
        aRect = Rect(0, mY, mX + 16, mHeight);
    }
    else if (thePlantWeapon == PlantWeapon::WEAPON_SECONDARY && mSeedType == SeedType::SEED_BONKCHOY)
    {
        aRect = Rect(mX - 80, mY - 80, mWidth + 160, mWidth + 160);
    }
    else switch (mSeedType)
    {
    case SeedType::SEED_LEFTPEATER:     aRect = Rect(0,             mY,             mX,                 mHeight);               break;
    case SeedType::SEED_SPOOKYSQUASH:
    case SeedType::SEED_SQUASH:         aRect = Rect(mX + 20,       mY,             mWidth - 35,        mHeight);               break;
    case SeedType::SEED_SUPERCHOMPER:
    case SeedType::SEED_CHOMPER:        aRect = Rect(mX + 80,       mY,             40,                 mHeight);               break;
    case SeedType::SEED_BEET:
    case SeedType::SEED_BEETBOXER:      aRect = Rect(mX + 80,       mY,             40,                 mHeight);               break;
    case SeedType::SEED_BONKCHOY:       aRect = Rect(mX - 120,      mY,             mWidth + 240,       mHeight);               break;
    case SeedType::SEED_SPIKEWEED:
    case SeedType::SEED_SPIKEROCK:      aRect = Rect(mX + 20,       mY,             mWidth - 50,        mHeight);               break;
    case SeedType::SEED_POPCORN:
    case SeedType::SEED_POTATOMINE:
    case SeedType::SEED_ICEBERGLETTUCE: aRect = Rect(mX,            mY,             mWidth - 25,        mHeight);               break;
    case SeedType::SEED_FLAMEWOOD:
    case SeedType::SEED_TORCHWOOD:      aRect = Rect(mX + 50,       mY,             30,                 mHeight);               break;
    case SeedType::SEED_PUFFSHROOM:
    case SeedType::SEED_SEASHROOM:      aRect = Rect(mX + 60,       mY,             230,                mHeight);               break;
    case SeedType::SEED_FUMESHROOM:
    case SeedType::SEED_ICYFUMESHROOM:  aRect = Rect(mX + 60,       mY,             340,                mHeight);               break;
    case SeedType::SEED_GLOOMSHROOM:    aRect = Rect(mX - 80,       mY - 80,        240,                240);                   break;
    case SeedType::SEED_TANGLEKELP:     aRect = Rect(mX,            mY,             mWidth,             mHeight);               break;
    case SeedType::SEED_CATTAIL:        aRect = Rect(-BOARD_WIDTH,  -BOARD_HEIGHT,  BOARD_WIDTH * 2,    BOARD_HEIGHT * 2);      break;
    case SeedType::SEED_BOWLNUT:        aRect = Rect(mX,            mY,             mWidth - 20,        mHeight);               break;
    case SeedType::SEED_BAMBOOM:
    case SeedType::SEED_BAMBOOSHOOT:    aRect = Rect(mX + 60,       mY - 80,        BOARD_WIDTH * 2,    240);                   break;
    default:                            aRect = Rect(mX + 60,       mY,             BOARD_WIDTH * 2,    mHeight);               break;
    }

    return aRect;
}

//0x4681E0
void Plant::PreloadPlantResources(SeedType theSeedType)
{
    const PlantDefinition& aPlantDef = GetPlantDefinition(theSeedType);
    if (aPlantDef.mReanimationType != ReanimationType::REANIM_NONE)
    {
        ReanimatorEnsureDefinitionLoaded(aPlantDef.mReanimationType, true);
    }

    if (theSeedType == SeedType::SEED_CHERRYBOMB)
    {
        ReanimatorEnsureDefinitionLoaded(ReanimationType::REANIM_ZOMBIE_CHARRED, true);
    }
    else if (theSeedType == SeedType::SEED_JALAPENO || theSeedType == SeedType::SEED_PICKLEDPEPPER || theSeedType == SeedType::SEED_CHILLYPEPPER)
    {
        ReanimatorEnsureDefinitionLoaded(ReanimationType::REANIM_JALAPENO_FIRE, true);
    }
    else if (theSeedType == SeedType::SEED_TORCHWOOD || theSeedType == SeedType::SEED_FLAMEWOOD)
    {
        ReanimatorEnsureDefinitionLoaded(ReanimationType::REANIM_FIRE_PEA, true);
        ReanimatorEnsureDefinitionLoaded(ReanimationType::REANIM_PLASMA_PEA, true);
        ReanimatorEnsureDefinitionLoaded(ReanimationType::REANIM_JALAPENO_FIRE, true);
    }
    else if (Plant::IsNocturnal(theSeedType))
    {
        ReanimatorEnsureDefinitionLoaded(ReanimationType::REANIM_SLEEPING, true);
    }
}

//0x468280
void Plant::PlayIdleAnim(float theRate)
{
    Reanimation* aBodyReanim = mApp->ReanimationTryToGet(mBodyReanimID);
    if (aBodyReanim)
    {
        PlayBodyReanim("anim_idle", ReanimLoopType::REANIM_LOOP, 20, theRate);
        if (mApp->IsIZombieLevel())
        {
            aBodyReanim->mAnimRate = 0.0f;
        }
    }
}

// Bloomerang

Zombie* Plant::FindBloomerangTarget()
{

    int aDamageRangeFlags = GetDamageRangeFlags(PlantWeapon::WEAPON_PRIMARY);
    Rect aAttackRect = GetPlantAttackRect();

    Zombie* aBestZombie[3];
    int aHighestWeight[3];
    for (int i = 0; i < 3; i++)
    {
        aBestZombie[i] = nullptr;
        aHighestWeight[i] = -BOARD_WIDTH;
    }

    Zombie* aZombie = nullptr;
    while (mBoard->IterateZombies(aZombie))
    {
        Rect aZombieRect = aZombie->GetZombieRect();
        if (aZombie->EffectedByDamage(aDamageRangeFlags) && aZombie->mRow == mRow && GetRectOverlap(aAttackRect, aZombieRect) > 0)
        {
            int aWeight = -aZombieRect.mX;

            if (aWeight > aHighestWeight[0] && aWeight < aHighestWeight[1])
            {
                aHighestWeight[0] = aWeight;
                aBestZombie[0] = aZombie;
            }
            else if (aWeight > aHighestWeight[1] && aWeight < aHighestWeight[2])
            {
                aHighestWeight[0] = aHighestWeight[1];
                aHighestWeight[1] = aWeight;
                aBestZombie[0] = aBestZombie[1];
                aBestZombie[1] = aZombie;
            }
            else if (aWeight > aHighestWeight[2])
            {
                aHighestWeight[0] = aHighestWeight[1];
                aHighestWeight[1] = aHighestWeight[2];
                aHighestWeight[2] = aWeight;
                aBestZombie[0] = aBestZombie[1];
                aBestZombie[1] = aBestZombie[2];
                aBestZombie[2] = aZombie;
            }
        }
    }

    for (int i = 0; i < 3; i++)
    {
        if (aBestZombie[i])
            return aBestZombie[i];
    }
    return nullptr;
}

void Plant::LaunchBloomerang()
{
    if (FindBloomerangTarget())
    {
        PlayBodyReanim("anim_shoot", ReanimLoopType::REANIM_PLAY_ONCE_AND_HOLD, 20, 28.0f);
        mShootingCounter = 40;
    }
}

// Zombotany

void Plant::TakeDamage(int theDamage, unsigned int theDamageFlags)
{
    if (!TestBit(theDamageFlags, (int)DamageFlags::DAMAGE_DOESNT_CAUSE_FLASH))
    {
        mEatenFlashCountdown = max(mEatenFlashCountdown, 25);
    }

    if (TestBit(theDamageFlags, (int)DamageFlags::DAMAGE_FREEZE))
    {
        ApplyChill(false);
    }
    if (TestBit(theDamageFlags, (int)DamageFlags::DAMAGE_POISON))
    {
        ApplyPoison(300, 5, true);
    }
    if (TestBit(theDamageFlags, (int)DamageFlags::DAMAGE_ICE))
    {
        HitIceTrap();
    }

    mPlantHealth -= theDamage;
    if (mSeedType == SeedType::SEED_INFINUT)
    {
        if (mState != STATE_INVULNERABLE)
        {
            mState = STATE_NOTREADY;
            mStateCountdown = 1500;
        }
        if (mPlantHealth <= 0)
            mPlantHealth = 0;
    }
}

void Plant::ApplyPoison(int aPoisonTime, int aPoisonDamage, bool theDoesStack)
{
    if (mPoisonCounter == 0)
    {
        mApp->PlayFoley(FoleyType::FOLEY_FROZEN);
    }

    mPoisonCounter = max(aPoisonTime, mPoisonCounter);

    if (theDoesStack) {
        mPoisonDamage = min(MAX_POISON_STACK, mPoisonDamage + aPoisonDamage);
    }
    else
    {
        mPoisonDamage = aPoisonDamage;
    }
}

void Plant::HitIceTrap()
{
    bool cold = false;
    if (mChilledCounter > 0 || mIceTrapCounter != 0)
        cold = true;
    ApplyChill(true);
    if (!CanBeChilled(mSeedType))
        return;
    int aIceTrapTime = RandRangeInt(400, 600);
    if (cold && !mApp->GetConfigCheckbox(true, CONFIG_CHECKBOX_CHILL_REBALANCE))
        aIceTrapTime = RandRangeInt(300, 400);
    mIceTrapCounter = max(aIceTrapTime, mIceTrapCounter);
}

void Plant::HitIceTrapCustom(int theTime)
{
    bool cold = false;
    if (mChilledCounter > 0 || mIceTrapCounter != 0)
        cold = true;
    ApplyChill(true);
    if (!CanBeChilled(mSeedType))
        return;
    int aIceTrapTime = theTime - Rand(200);
    if (cold && !mApp->GetConfigCheckbox(true, CONFIG_CHECKBOX_CHILL_REBALANCE))
        aIceTrapTime = RandRangeInt(theTime - 300, theTime - 200);
    mIceTrapCounter = max(aIceTrapTime, mIceTrapCounter);
}

void Plant::ApplyChill(bool theIsIceTrap)
{
    if (!CanBeChilled(mSeedType))
        return;

    if (mChilledCounter == 0)
    {
        mApp->PlayFoley(FoleyType::FOLEY_FROZEN);
    }

    int aChillTime = 1000;
    if (theIsIceTrap && !mApp->GetConfigCheckbox(true, CONFIG_CHECKBOX_CHILL_REBALANCE))
    {
        aChillTime = 2000;
    }
    mChilledCounter = max(aChillTime, mChilledCounter);

    //UpdateAnimSpeed();
}

void Plant::RemoveColdEffects()
{
    if (mIceTrapCounter > 0)
        mIceTrapCounter = 0;

    if (mChilledCounter > 0)
        mChilledCounter = 0;
}

// Get Type

bool Plant::NotOnGround()
{
    if (mSeedType == SeedType::SEED_SQUASH || mSeedType == SeedType::SEED_SPOOKYSQUASH)
    {
        if (mState == PlantState::STATE_SQUASH_RISING || mState == PlantState::STATE_SQUASH_FALLING || mState == PlantState::STATE_SQUASH_DONE_FALLING)
            return true;
    }

    return mSquished || mOnBungeeState == PlantOnBungeeState::RISING_WITH_BUNGEE || mDead;
}

const bool Plant::MakesSun(SeedType theSeedtype)
{
    return theSeedtype == SeedType::SEED_SUNFLOWER
        || theSeedtype == SeedType::SEED_TWINSUNFLOWER
        || theSeedtype == SeedType::SEED_SUNSHROOM
        || theSeedtype == SeedType::SEED_PRIMALSUNFLOWER
        || theSeedtype == SeedType::SEED_POWERFLOWER
        ;
}

const bool Plant::IsPeashooter(SeedType theSeedtype)
{
    return
        theSeedtype == SeedType::SEED_PEASHOOTER 
        || theSeedtype == SeedType::SEED_SNOWPEA 
        || theSeedtype == SeedType::SEED_REPEATER 
        || theSeedtype == SeedType::SEED_ICEQUEENPEA 
        || theSeedtype == SeedType::SEED_FIREPEA 
        || theSeedtype == SeedType::SEED_PLASMAPEA 
        || theSeedtype == SeedType::SEED_SWEETPEA 
        || theSeedtype == SeedType::SEED_BEESHOOTER 
        || theSeedtype == SeedType::SEED_ASPEARAGUS 
        || theSeedtype == SeedType::SEED_ACEPEARAGUS 
        || theSeedtype == SeedType::SEED_PRIMALPEASHOOTER
        || theSeedtype == SeedType::SEED_GOOPEASHOOTER
        || theSeedtype == SeedType::SEED_PEANIS
        ;
}

const bool Plant::IsNocturnal(SeedType theSeedtype)
{
    return
        theSeedtype == SeedType::SEED_PUFFSHROOM 
        || theSeedtype == SeedType::SEED_SEASHROOM 
        || theSeedtype == SeedType::SEED_SUNSHROOM 
        || theSeedtype == SeedType::SEED_FUMESHROOM 
        || theSeedtype == SeedType::SEED_HYPNOSHROOM 
        || theSeedtype == SeedType::SEED_DOOMSHROOM 
        || theSeedtype == SeedType::SEED_ICESHROOM 
        || theSeedtype == SeedType::SEED_MAGNETSHROOM 
        || theSeedtype == SeedType::SEED_SCAREDYSHROOM 
        || theSeedtype == SeedType::SEED_GLOOMSHROOM 
        || theSeedtype == SeedType::SEED_MOLDSHROOM
        || theSeedtype == SeedType::SEED_ICYFUMESHROOM
        || theSeedtype == SeedType::SEED_FIRESHROOM
        || theSeedtype == SeedType::SEED_MOLDSHROOM
        || theSeedtype == SeedType::SEED_SPORESHROOM
        ;
}

const bool Plant::IsAquatic(SeedType theSeedType)
{
    return
        theSeedType == SeedType::SEED_LILYPAD 
        || theSeedType == SeedType::SEED_TANGLEKELP 
        || theSeedType == SeedType::SEED_SEASHROOM 
        || theSeedType == SeedType::SEED_CATTAIL
        ;
}

const bool Plant::IsRoof(SeedType theSeedType)
{
    return
        theSeedType == SeedType::SEED_FLOWERPOT 
        || theSeedType == SeedType::SEED_WATERPOT
        || theSeedType == SeedType::SEED_COFFEEPOT
        || theSeedType == SeedType::SEED_SPIKEWEED 
        || theSeedType == SeedType::SEED_SPIKEROCK 
        || theSeedType == SeedType::SEED_MOLDSHROOM
        || theSeedType == SeedType::SEED_SPORESHROOM
        ;
}

const bool Plant::IsFlying(SeedType theSeedtype)
{
    return theSeedtype == SeedType::SEED_INSTANT_COFFEE
        || theSeedtype == SeedType::SEED_HOTPOTATO
        ;
}

//0x467EC0
const bool Plant::IsUpgrade(SeedType theSeedtype)
{
    return theSeedtype == SeedType::SEED_COBCANNON;
}

const bool Plant::IsPremium(SeedType theSeedtype)
{
    return theSeedtype == SeedType::SEED_GATLINGPEA 
        || theSeedtype == SeedType::SEED_TWINSUNFLOWER 
        || theSeedtype == SeedType::SEED_GLOOMSHROOM 
        || theSeedtype == SeedType::SEED_CATTAIL 
        || theSeedtype == SeedType::SEED_WINTERMELON 
        || theSeedtype == SeedType::SEED_GOLD_MAGNET 
        || theSeedtype == SeedType::SEED_SPIKEROCK 
        || theSeedtype == SeedType::SEED_ICEQUEENPEA 
        || theSeedtype == SeedType::SEED_SWEETPEA 
        || theSeedtype == SeedType::SEED_BEESHOOTER 
        || theSeedtype == SeedType::SEED_ACEPEARAGUS 
        || theSeedtype == SeedType::SEED_POWERFLOWER 
        || theSeedtype == SeedType::SEED_BEETBOXER 
        || theSeedtype == SeedType::SEED_PICKLEDPEPPER 
        || theSeedtype == SeedType::SEED_ICYFUMESHROOM 
        || theSeedtype == SeedType::SEED_FLAMEWOOD 
        || theSeedtype == SeedType::SEED_FIRESHROOM
        || theSeedtype == SeedType::SEED_ICYFUMESHROOM 
        || theSeedtype == SeedType::SEED_VINENUT
        || theSeedtype == SeedType::SEED_SUPERCHOMPER
        || theSeedtype == SeedType::SEED_FUTURESTAR
        || theSeedtype == SeedType::SEED_STUFFYSUNFLOWER
        || theSeedtype == SeedType::SEED_SHADOWFLOWER
        || theSeedtype == SeedType::SEED_PLASMAPEA
        || theSeedtype == SeedType::SEED_OAKARCHER
        || theSeedtype == SeedType::SEED_LASERBEAN
        || theSeedtype == SeedType::SEED_BONKCHOY
        || theSeedtype == SeedType::SEED_HARDNUT
        || theSeedtype == SeedType::SEED_SHAMROCKSTAR
        ;
}

const bool Plant::IsFakePlantFood(SeedType theSeedtype)
{
    return theSeedtype == SeedType::SEED_OAKARCHER 
        || theSeedtype == SeedType::SEED_BONKCHOY
        || theSeedtype == SeedType::SEED_LASERBEAN
        ;
}

const bool Plant::CanBeChilled(SeedType theSeedType)
{
    return
        theSeedType != SeedType::SEED_PLANTERN
        && theSeedType != SeedType::SEED_CHERRYBOMB
        && theSeedType != SeedType::SEED_JALAPENO
        && theSeedType != SeedType::SEED_PICKLEDPEPPER
        && theSeedType != SeedType::SEED_POTATOMINE
        && theSeedType != SeedType::SEED_POPCORN
        && theSeedType != SeedType::SEED_EXPLODE_O_NUT
        && theSeedType != SeedType::SEED_INSTANT_COFFEE
        && theSeedType != SeedType::SEED_HOTPOTATO
        && !IsCold(theSeedType)
        && !IsWarm(theSeedType)
        ;
}

const bool Plant::IsCold(SeedType theSeedType)
{
    return
        theSeedType == SeedType::SEED_SNOWPEA
        || theSeedType == SeedType::SEED_WINTERMELON
        || theSeedType == SeedType::SEED_ICESHROOM
        || theSeedType == SeedType::SEED_ICEBERGLETTUCEBETA
        || theSeedType == SeedType::SEED_ICEBERGLETTUCE
        || theSeedType == SeedType::SEED_ICYFUMESHROOM
        || theSeedType == SeedType::SEED_ICEQUEENPEA
        || theSeedType == SeedType::SEED_FROSTACHIO
        ;
}

const bool Plant::IsWarm(SeedType theSeedType)
{
    return
        theSeedType == SeedType::SEED_TORCHWOOD
        || theSeedType == SeedType::SEED_FIREPEA
        || theSeedType == SeedType::SEED_PEPPERPULT
        || theSeedType == SeedType::SEED_PLASMAPEA
        || theSeedType == SeedType::SEED_FLAMEWOOD
        || theSeedType == SeedType::SEED_FIRESHROOM
        || theSeedType == SeedType::SEED_FIREFLOWER
        || theSeedType == SeedType::SEED_SPOOKYSQUASH
        ;
}

const bool Plant::IsToxic(SeedType theSeedType)
{
    return
        false
        ;
}

const bool Plant::IsNut(SeedType theSeedtype)
{
    return IsWallNut(theSeedtype) 
        || theSeedtype == SeedType::SEED_TALLNUT 
        || theSeedtype == SeedType::SEED_VINENUT
        ;
}

const bool Plant::IsWallNut(SeedType theSeedtype)
{
    return theSeedtype == SeedType::SEED_WALLNUT 
        || theSeedtype == SeedType::SEED_EXPLODE_O_NUT 
        || theSeedtype == SeedType::SEED_GIANT_WALLNUT 
        || theSeedtype == SeedType::SEED_INFINUT
        || theSeedtype == SeedType::SEED_BOWLNUT
        || theSeedtype == SeedType::SEED_HAZYNUT
        || theSeedtype == SeedType::SEED_FROSTACHIO
        ;
}

const bool Plant::IsWall(SeedType theSeedtype)
{
    return IsNut(theSeedtype)
        || theSeedtype == SeedType::SEED_PUMPKINSHELL
        || theSeedtype == SeedType::SEED_SHAMROCK
        || theSeedtype == SeedType::SEED_SHAMROCKSTAR
        ;
}

const bool Plant::IsInsomniac()
{
    return
        IsWall(mSeedType)
        || mSeedType == SeedType::SEED_LILYPAD
        || mSeedType == SeedType::SEED_FLOWERPOT
        || mSeedType == SeedType::SEED_WATERPOT
        || mSeedType == SeedType::SEED_COFFEEPOT
        || mBoard->GetCoffeePotAt(mPlantCol, mRow)
        ;
}
