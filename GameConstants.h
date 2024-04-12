#pragma once

#include <wtypes.h>
#include "ConstEnums.h"
constexpr const double PI = 3.141592653589793;

// ============================================================
// ■ Board
// ============================================================

const int			BOARD_WIDTH = 1280;
const int			BOARD_HEIGHT = 720;
const int			BOARD_OFFSET_X = 240; // + 200 For Unwidened
const int			BOARD_OFFSET_Y = 0; // + 60 for Unwidened
const int			BOARD_EDGE = -100;
const int			BOARD_IMAGE_WIDTH_OFFSET = 1880 - BOARD_OFFSET_X;
const int           BOARD_ICE_START = 1280;
const int           LAWN_XMIN = 496 - BOARD_OFFSET_X; // 40 Initially
const int           LAWN_YMIN = 140 - BOARD_OFFSET_Y; // 80 Initially
const int           HIGH_GROUND_HEIGHT = 30;
const int			WIDE_BOARD_WIDTH = 760 + LAWN_XMIN;
const int			BOARD_XCENTER = BOARD_WIDTH / 2;
const int			BOARD_YCENTER = BOARD_HEIGHT / 2;
const int			MAIN_MENU_BUTTON_X = BOARD_WIDTH - 172;
const int			FASTFORWARD_BUTTON_X = BOARD_WIDTH - 180;
const int			FASTFORWARD_BUTTON_Y = 8;

// ============================================================
// ■ Seed Chooser
// ============================================================

const int			MENU_BUTTON_X = BOARD_WIDTH - 119;
const int			MENU_BUTTON_Y = 8;
const int           SEEDBANK_MAX = 10;
const int           SEED_BANK_OFFSET_X = 0;
const int           SEED_BANK_OFFSET_X_END = 10;
const int           SEED_CHOOSER_OFFSET_Y = 516;
const int           SEED_CHOOSER_TEXT_X = 450;
const int           SEED_CHOOSER_TEXT_Y = 110;
const int           SEED_CHOOSER_PLAYBUTTON_X = 450;
const int           SEED_CHOOSER_PLAYBUTTON_Y = 545;
const int           SEED_BANK_OFFSET_Y_END = -40;
const int           SEED_PACKET_WIDTH = 50;
const int           SEED_PACKET_HEIGHT = 70;
const int           IMITATER_DIALOG_WIDTH = 500;
const int           IMITATER_DIALOG_HEIGHT = 600;
const int           SEED_CHOOSER_ZOMBIE_XOFFSET = WIDE_BOARD_WIDTH + 150;
const int           SEED_CHOOSER_ZOMBIE_YOFFSET = 70;

// ============================================================
// ■ Challenge Screen
// ============================================================

const int			CHALLENGE_BACK_OFFSET_X = 18;
const int			CHALLENGE_BACK_OFFSET_Y = BOARD_HEIGHT - 48;
const int			CHALLENGE_PAGE_OFFSET_Y = BOARD_HEIGHT - 64;
const int			CHALLENGE_TITLE_OFFSET_X = BOARD_XCENTER;
const int			CHALLENGE_TITLE_OFFSET_Y = 110;
const int			CHALLENGE_TROPHY_OFFSET_X = BOARD_XCENTER + 380;
const int			CHALLENGE_TROPHY_OFFSET_Y = 80;
const int			SURVIVAL_TROPHIES = 12;
const int			PUZZLE_TROPHIES = 18;
const int			MINIGAME_TROPHIES = 20;

// ============================================================
// ■ Levels
// ============================================================
const int			ADVENTURE_AREAS = 15;
const int			LEVELS_PER_AREA = 10;
const int           NUM_LEVELS = ADVENTURE_AREAS * LEVELS_PER_AREA;
const int			NUM_LEVEL_SEEDS = NUM_LEVELS * 8;
const int			FINAL_LEVEL = NUM_LEVELS;
const int           FLAG_RAISE_TIME = 100;
const int           LAST_STAND_FLAGS = 5;
const int           ZOMBIE_COUNTDOWN_FIRST_WAVE = 1800;
const int           ZOMBIE_COUNTDOWN = 2500;
const int           ZOMBIE_COUNTDOWN_RANGE = 600;
const int           ZOMBIE_COUNTDOWN_BEFORE_FLAG = 4500;
const int           ZOMBIE_COUNTDOWN_BEFORE_REPICK = 5499;
const int           ZOMBIE_COUNTDOWN_MIN = 400;
const int           FOG_BLOW_RETURN_TIME = 2000;
const int           SUN_COUNTDOWN = 425;
const int           SUN_COUNTDOWN_RANGE = 275;
const int           SUN_COUNTDOWN_MAX = 950;
const int           SURVIVAL_NORMAL_FLAGS = 5;
const int           SURVIVAL_HARD_FLAGS = 10;
// New Level Stuff (For Convenience)
const int			LEVEL_MID_AREA = LEVELS_PER_AREA / 2;
const int			DAY_LEVEL_INDEX = 0 * LEVELS_PER_AREA;
const int			NIGHT_LEVEL_INDEX = 1 * LEVELS_PER_AREA;
const int			POOL_LEVEL_INDEX = 2 * LEVELS_PER_AREA;
const int			FOG_LEVEL_INDEX = 3 * LEVELS_PER_AREA;
const int			ROOF_LEVEL_INDEX = 4 * LEVELS_PER_AREA;
const int			MOON_LEVEL_INDEX = 5 * LEVELS_PER_AREA;
const int			DAY_FALL_LEVEL_INDEX = 6 * LEVELS_PER_AREA;
const int			NIGHT_FALL_LEVEL_INDEX = DAY_FALL_LEVEL_INDEX + LEVEL_MID_AREA;
const int			POOL_FALL_LEVEL_INDEX = 7 * LEVELS_PER_AREA;
const int			FOG_FALL_LEVEL_INDEX = POOL_FALL_LEVEL_INDEX + LEVEL_MID_AREA;
const int			ROOF_FALL_LEVEL_INDEX = 8 * LEVELS_PER_AREA;
const int			MOON_FALL_LEVEL_INDEX = ROOF_FALL_LEVEL_INDEX + LEVEL_MID_AREA;
const int			DAY_WINTER_LEVEL_INDEX = 9 * LEVELS_PER_AREA;
const int			NIGHT_WINTER_LEVEL_INDEX = DAY_WINTER_LEVEL_INDEX + LEVEL_MID_AREA;
const int			POOL_WINTER_LEVEL_INDEX = 10 * LEVELS_PER_AREA;
const int			FOG_WINTER_LEVEL_INDEX = POOL_WINTER_LEVEL_INDEX + LEVEL_MID_AREA;
const int			ROOF_WINTER_LEVEL_INDEX = 11 * LEVELS_PER_AREA;
const int			MOON_WINTER_LEVEL_INDEX = ROOF_WINTER_LEVEL_INDEX + LEVEL_MID_AREA;
const int			DAY_SPRING_LEVEL_INDEX = 12 * LEVELS_PER_AREA;
const int			NIGHT_SPRING_LEVEL_INDEX = DAY_SPRING_LEVEL_INDEX + LEVEL_MID_AREA;
const int			POOL_SPRING_LEVEL_INDEX = 13 * LEVELS_PER_AREA;
const int			FOG_SPRING_LEVEL_INDEX = DAY_SPRING_LEVEL_INDEX + LEVEL_MID_AREA;
const int			ROOF_SPRING_LEVEL_INDEX = 14 * LEVELS_PER_AREA;
const int			MOON_SPRING_LEVEL_INDEX = DAY_SPRING_LEVEL_INDEX + LEVEL_MID_AREA;
const int			BOWLING_LEVEL = DAY_LEVEL_INDEX + LEVEL_MID_AREA;
const int			MALLET_LEVEL = NIGHT_LEVEL_INDEX + LEVEL_MID_AREA;
const int			LITTLE_LEVEL = POOL_LEVEL_INDEX + LEVEL_MID_AREA;
const int			VASE_LEVEL = FOG_LEVEL_INDEX + LEVEL_MID_AREA;
const int			STORMY_LEVEL = FOG_LEVEL_INDEX + LEVELS_PER_AREA;
const int			BUNGEE_LEVEL = ROOF_LEVEL_INDEX + LEVEL_MID_AREA;
const int			COLUMN_LEVEL = MOON_LEVEL_INDEX + LEVEL_MID_AREA;
const int			PORTAL_LEVEL = DAY_FALL_LEVEL_INDEX + LEVEL_MID_AREA;
const int			ZOMBOSS_LEVEL = MOON_LEVEL_INDEX + LEVELS_PER_AREA;

// ============================================================
// ■ Items
// ============================================================

const int			SHOVEL_LEVEL = DAY_LEVEL_INDEX + 4;

// ============================================================
// ■ Zen Garden
// ============================================================

const int           ZEN_GRID_XOFFSET = 240;
const int           ZEN_GRID_YOFFSET = 64;

// ============================================================
// ■ Store
// ============================================================
const int           STORESCREEN_ITEMOFFSET_1_X = 422;
const int           STORESCREEN_ITEMOFFSET_1_Y = 206;
const int           STORESCREEN_ITEMOFFSET_2_X = 372;
const int           STORESCREEN_ITEMOFFSET_2_Y = 310;
const int           STORESCREEN_ITEMSIZE = 74;
const int           STORESCREEN_COINBANK_X = 650;
const int           STORESCREEN_COINBANK_Y = 559;
const int           STORESCREEN_PAGESTRING_X = 470;
const int           STORESCREEN_PAGESTRING_Y = 500;


// ============================================================
// ■ Almanac
// ============================================================
const int			ALMANAC_INDEXPLANT_POSITION_X = -233 + BOARD_XCENTER;
const int			ALMANAC_INDEXPLANT_POSITION_Y = -45 + BOARD_YCENTER;
const float			ALMANAC_INDEXZOMBIE_POSITION_X = 135 + BOARD_XCENTER;
const float			ALMANAC_INDEXZOMBIE_POSITION_Y = -85 + BOARD_YCENTER;
// Widescreen
const int			ALMANAC_ZOMBIECARD_POSITION_X = BOARD_WIDTH - 345;
const int			ALMANAC_ZOMBIECARD_POSITION_Y = 160;

const int			ALMANAC_PLANTCARD_POSITION_X = BOARD_WIDTH - 341;
const int			ALMANAC_PLANTCARD_POSITION_Y = 172;

const int			ALMANAC_SEEDS_OFFSET_X = 56;
const int			ALMANAC_SEEDS_OFFSET_Y = 172;
const int			ALMANAC_ZOMBIES_OFFSET_X = 52;
const int			ALMANAC_ZOMBIES_OFFSET_Y = 160;

const int			ALMANAC_SEED_COLUMNS = 16;
const int			ALMANAC_ZOMBIE_COLUMNS = 10;

const int			ALMANAC_PLANTBUTTON_POSITION_X = -270 + BOARD_XCENTER;
const int			ALMANAC_PLANTBUTTON_POSITION_Y = 45 + BOARD_YCENTER;
const int			ALMANAC_ZOMBIEBUTTON_POSITION_X = 87 + BOARD_XCENTER;
const int			ALMANAC_ZOMBIEBUTTON_POSITION_Y = 45 + BOARD_YCENTER;
const int			ALMANAC_CLOSEBUTTON_POSITION_X = -124 + BOARD_WIDTH;
const int			ALMANAC_CLOSEBUTTON_POSITION_Y = -33 + BOARD_HEIGHT;
const int			ALMANAC_INDEXBUTTON_POSITION_X = 32;
const int			ALMANAC_INDEXBUTTON_POSITION_Y = -33 + BOARD_HEIGHT;

const int			ALMANAC_INDEXTITLE_OFFSET_X = BOARD_XCENTER;
const int			ALMANAC_INDEXTITLE_OFFSET_Y = 110;
const int			ALMANAC_PLANTTITLE_OFFSET_X = BOARD_XCENTER;
const int			ALMANAC_PLANTTITLE_OFFSET_Y = 110;
const int			ALMANAC_ZOMBIETITLE_OFFSET_X = BOARD_XCENTER;
const int			ALMANAC_ZOMBIETITLE_OFFSET_Y = 110;
