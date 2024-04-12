#include "../../GameConstants.h"
#include "../../LawnApp.h"
#include "../../Resources.h"
#include "../../Sexy.TodLib/TodCommon.h"
#include "../../Sexy.TodLib/TodDebug.h"
#include "../../Sexy.TodLib/TodFoley.h"
#include "../../Sexy.TodLib/TodStringFile.h"
#include "../../SexyAppFramework/Checkbox.h"
#include "../../SexyAppFramework/Debug.h"
#include "../../SexyAppFramework/Slider.h"
#include "../../SexyAppFramework/WidgetManager.h"
#include "../System/Music.h"
#include "../System/PlayerInfo.h"
#include "../ToolTipWidget.h"
#include "ConfigScreen.h"
#include "GameButton.h"

ConfigCheckboxDefinition gConfigCheckboxDefs[NUM_CONFIG_CHECKBOXES] = {
	{ ConfigCheckbox::CONFIG_CHECKBOX_GIGA_MODE,			ConfigPage::CONFIG_PAGE_CHALLENGE,	0,  0,  _S("CONFIG_GIGA_MODE")},
	{ ConfigCheckbox::CONFIG_CHECKBOX_DEADPLANTS_MODE,		ConfigPage::CONFIG_PAGE_CHALLENGE,	1,  0,  _S("CONFIG_DEADPLANTS_MODE")},
	{ ConfigCheckbox::CONFIG_CHECKBOX_PLANT_SOUNDS,			ConfigPage::CONFIG_PAGE_PLANTS,		0,  0,  _S("CONFIG_PLANT_SOUNDS")},
	{ ConfigCheckbox::CONFIG_CHECKBOX_KILL_SOUND,			ConfigPage::CONFIG_PAGE_ZOMBIES,	0,  0,  _S("CONFIG_KILL_SOUND")},
	{ ConfigCheckbox::CONFIG_CHECKBOX_COFFEE_BOOST,			ConfigPage::CONFIG_PAGE_PLANTS,		1,  0,  _S("CONFIG_COFFEE_BOOST")},
	{ ConfigCheckbox::CONFIG_CHECKBOX_INDEPENDANT_UPGRADES,	ConfigPage::CONFIG_PAGE_PLANTS,		2,  0,  _S("CONFIG_INDEPENDANT_UPGRADES")},
	{ ConfigCheckbox::CONFIG_CHECKBOX_NEW_PLANTS,			ConfigPage::CONFIG_PAGE_PLANTS,		3,  0,  _S("CONFIG_NEW_PLANTS")},
	{ ConfigCheckbox::CONFIG_CHECKBOX_NERF_DOOM,			ConfigPage::CONFIG_PAGE_PLANTS,		0,  1,  _S("CONFIG_DOOM")},
	{ ConfigCheckbox::CONFIG_CHECKBOX_SUN_AMOUNT,			ConfigPage::CONFIG_PAGE_TEMP,		0,  0,  _S("CONFIG_SUN_AMOUNT")},
	{ ConfigCheckbox::CONFIG_CHECKBOX_COIN_AMOUNT,			ConfigPage::CONFIG_PAGE_TEMP,		1,  0,  _S("CONFIG_COIN_AMOUNT")},
	{ ConfigCheckbox::CONFIG_CHECKBOX_SLOT_HOTKEYS,			ConfigPage::CONFIG_PAGE_TEMP,		2,  0,  _S("CONFIG_SLOT_HOTKEYS")},
	{ ConfigCheckbox::CONFIG_CHECKBOX_HOLD_TO_COLLECT,		ConfigPage::CONFIG_PAGE_TEMP,		0,  1,  _S("CONFIG_HOLD_TO_COLLECT")},
	{ ConfigCheckbox::CONFIG_CHECKBOX_GIGA_ABILITIES,		ConfigPage::CONFIG_PAGE_ZOMBIES,	1,  0,  _S("CONFIG_GIGA_ABILITIES")},
	{ ConfigCheckbox::CONFIG_CHECKBOX_CHILL_REBALANCE,		ConfigPage::CONFIG_PAGE_TEMP,		3,  0,  _S("CONFIG_GIGA_ABILITIES")},
};

ConfigCheckboxDefinition& GetConfigCheckboxDefinition(int theConfigCheckbox)
{
	TOD_ASSERT(theConfigCheckbox >= 0 && theConfigCheckbox < ConfigCheckbox::NUM_CONFIG_CHECKBOXES);

	ConfigCheckboxDefinition& aDef = gConfigCheckboxDefs[theConfigCheckbox];
	TOD_ASSERT(aDef.mConfigCheckbox == theConfigCheckbox + ConfigCheckbox::CONFIG_CHECKBOX_GIGA_MODE);

	return gConfigCheckboxDefs[theConfigCheckbox];
}

ConfigScreen::ConfigScreen(LawnApp* theApp, ConfigPage thePage)
{
	mApp = theApp;
	mPageIndex = thePage;
	TodLoadResources("DelayLoad_ChallengeScreen");

	mBackButton = MakeNewButton(ConfigScreen::ConfigScreen_Back, this, _S("[BACK_TO_MENU]"), nullptr, Sexy::IMAGE_SEEDCHOOSER_BUTTON2,
		Sexy::IMAGE_SEEDCHOOSER_BUTTON2_GLOW, Sexy::IMAGE_SEEDCHOOSER_BUTTON2_GLOW);
	mBackButton->mTextDownOffsetX = 1;
	mBackButton->mTextDownOffsetY = 1;
	mBackButton->mColors[ButtonWidget::COLOR_LABEL] = Color(42, 42, 90);
	mBackButton->mColors[ButtonWidget::COLOR_LABEL_HILITE] = Color(42, 42, 90);
	mBackButton->Resize(CHALLENGE_BACK_OFFSET_X, CHALLENGE_BACK_OFFSET_Y, 111, 26);

	for (int aPageIdx = 0; aPageIdx < MAX_CONFIG_PAGES; aPageIdx++)
	{
		ButtonWidget* aPageButton = new ButtonWidget(ConfigScreen::ConfigScreen_Page + aPageIdx, this);
		aPageButton->mDoFinger = true;
		mPageButton[aPageIdx] = aPageButton;
		aPageButton->mLabel = aPageIdx == CONFIG_PAGE_CHALLENGE ? TodStringTranslate(_S("[CONFIG_PAGE_CHALLENGE]")) :
			aPageIdx == CONFIG_PAGE_ZOMBIES ? TodStringTranslate(_S("[CONFIG_PAGE_ZOMBIES]")) :
			aPageIdx == CONFIG_PAGE_PLANTS ? TodStringTranslate(_S("[CONFIG_PAGE_PLANTS]")) :
			aPageIdx == CONFIG_PAGE_TEMP ? TodStringTranslate(_S("[CONFIG_PAGE_TEMP]")) :
			TodReplaceNumberString(_S("[PAGE_X]"), _S("{PAGE}"), aPageIdx);
		aPageButton->mButtonImage = Sexy::IMAGE_BLANK;
		aPageButton->mOverImage = Sexy::IMAGE_BLANK;
		aPageButton->mDownImage = Sexy::IMAGE_BLANK;
		aPageButton->SetFont(Sexy::FONT_BRIANNETOD12);
		aPageButton->mColors[ButtonWidget::COLOR_LABEL] = Color(255, 240, 0);
		aPageButton->mColors[ButtonWidget::COLOR_LABEL_HILITE] = Color(220, 220, 0);
		aPageButton->Resize(BOARD_XCENTER + 100 * (aPageIdx - float(MAX_CONFIG_PAGES) / 2), CHALLENGE_PAGE_OFFSET_Y, 100, 75);
	}

	for (int aConfigCheckbox = 0; aConfigCheckbox < NUM_CONFIG_CHECKBOXES; aConfigCheckbox++)
	{
		ConfigCheckboxDefinition& aConfigCheckboxDef = GetConfigCheckboxDefinition(aConfigCheckbox);
		Sexy::Checkbox* aNewConfigCheckbox = MakeNewCheckbox(ConfigScreen::ConfigScreen_Checkbox + aConfigCheckbox, this, theApp->mPlayerInfo->mConfigCheckbox[aConfigCheckbox]);
		mConfigCheckbox[aConfigCheckbox] = aNewConfigCheckbox;
		aNewConfigCheckbox->Resize(CHECKBOX_OFFSET_X + BOARD_XCENTER + (aConfigCheckboxDef.mCol - (NUM_COLUMNS / 2)) * COLUMN_SIZE, CHECKBOX_OFFSET_Y + BOARD_YCENTER + (aConfigCheckboxDef.mRow - (NUM_ROWS / 2)) * ROW_SIZE, 46, 45);
	}

	UpdateButtons();
}

ConfigScreen::~ConfigScreen()
{
	delete mBackButton;
	for (ButtonWidget* aPageButton : mPageButton) delete aPageButton;
	for (Sexy::Checkbox* aCheckbox : mConfigCheckbox) delete aCheckbox;
}

//0x45CFF0
void ConfigScreen::CheckboxChecked(int theId, bool checked)
{
	mApp->mPlayerInfo->mConfigCheckbox[theId - ConfigScreen::ConfigScreen_Checkbox] = checked;
}

void ConfigScreen::AddedToManager(WidgetManager* theWidgetManager)
{
	Widget::AddedToManager(theWidgetManager);
	AddWidget(mBackButton);
	for (ButtonWidget* aPageButton : mPageButton) AddWidget(aPageButton);
	for (Sexy::Checkbox* aCheckbox : mConfigCheckbox) AddWidget(aCheckbox);
}

//0x42F6B0
void ConfigScreen::RemovedFromManager(WidgetManager* theWidgetManager)
{
	Widget::RemovedFromManager(theWidgetManager);
	RemoveWidget(mBackButton);
	for (ButtonWidget* aPageButton : mPageButton) RemoveWidget(aPageButton);
	for (Sexy::Checkbox* aCheckbox : mConfigCheckbox) RemoveWidget(aCheckbox);
}

void ConfigScreen::SliderVal(int theId, double theVal)
{
    //switch (theId)
    //{
    //case ConfigScreen::ConfigScreen_ZombieHealthMultiplier:
    //    break;
    //}
}

void ConfigScreen::Draw(Graphics* g)
{
	g->SetLinearBlend(true);
	g->DrawImage(Sexy::IMAGE_CHALLENGE_BACKGROUND, 0, 0);

	SexyString aTitleString =
		mPageIndex == CONFIG_PAGE_CHALLENGE ? _S("[CONFIG_PAGE_CHALLENGE]") :
		mPageIndex == CONFIG_PAGE_ZOMBIES ? _S("[CONFIG_PAGE_ZOMBIES]") : 
		mPageIndex == CONFIG_PAGE_PLANTS ? _S("[CONFIG_PAGE_PLANTS]") : _S("[CONFIG_PAGE_TEMP]");
	TodDrawString(g, aTitleString, 640, 110, Sexy::FONT_HOUSEOFTERROR28, Color(220, 220, 220), DS_ALIGN_CENTER);

	for (int aConfigCheckbox = 0; aConfigCheckbox < NUM_CONFIG_CHECKBOXES; aConfigCheckbox++)
	{
		ConfigCheckboxDefinition aConfigCheckboxDef = GetConfigCheckboxDefinition(aConfigCheckbox);
		if (aConfigCheckboxDef.mPage == mPageIndex)
		{
			SexyString aName = StrFormat(_S("[%s]"), aConfigCheckboxDef.mConfigCheckboxName);
			TodDrawString(g, aName, 
				NAME_OFFSET_X + BOARD_XCENTER + (aConfigCheckboxDef.mCol - (NUM_COLUMNS / 2)) * COLUMN_SIZE, 
				NAME_OFFSET_Y + BOARD_YCENTER + (aConfigCheckboxDef.mRow - (NUM_ROWS / 2)) * ROW_SIZE, 
				FONT_DWARVENTODCRAFT18, Color(107, 109, 145), DS_ALIGN_LEFT);

			SexyString aDescriptionName = StrFormat(_S("[%s_DESCRIPTION]"), aConfigCheckboxDef.mConfigCheckboxName);
			TodDrawStringWrapped(g, aDescriptionName, 
				Rect(DESCRIPTION_OFFSET_X + BOARD_XCENTER + (aConfigCheckboxDef.mCol - (NUM_COLUMNS / 2)) * COLUMN_SIZE, 
					DESCRIPTION_OFFSET_Y + BOARD_YCENTER + (aConfigCheckboxDef.mRow - (NUM_ROWS / 2)) * ROW_SIZE, COLUMN_SIZE - 30, ROW_SIZE - 30),
				Sexy::FONT_BRIANNETOD12, Color(40, 50, 90), DS_ALIGN_LEFT);
		}
	}
}

void ConfigScreen::UpdateButtons()
{
	for (int aConfigCheckbox = 0; aConfigCheckbox < NUM_CONFIG_CHECKBOXES; aConfigCheckbox++)
	{
		ConfigCheckboxDefinition aConfigCheckboxDef = GetConfigCheckboxDefinition(aConfigCheckbox);
		mConfigCheckbox[aConfigCheckbox]->mVisible = aConfigCheckboxDef.mPage == mPageIndex;
	}

	for (int aPage = 0; aPage < MAX_CONFIG_PAGES; aPage++)
	{
		ButtonWidget* aPageButton = mPageButton[aPage];
		if (aPage == mPageIndex)
		{
			aPageButton->mColors[ButtonWidget::COLOR_LABEL] = Color(64, 64, 64);
			aPageButton->mDisabled = true;
		}
		else
		{
			aPageButton->mColors[ButtonWidget::COLOR_LABEL] = Color(255, 240, 0);
			aPageButton->mDisabled = false;
		}
	}
}

void ConfigScreen::ButtonPress(int theId)
{
	mApp->PlaySample(Sexy::SOUND_BUTTONCLICK);
}

void ConfigScreen::ButtonDepress(int theId)
{
	if (theId == ConfigScreen::ConfigScreen_Back)
	{
		mApp->KillConfigScreen();
		mApp->DoBackToMain();
	}

	int aPageIndex = theId - ConfigScreen::ConfigScreen_Page;
	if (aPageIndex >= 0 && aPageIndex < MAX_CONFIG_PAGES)
	{
		mPageIndex = (ConfigPage)aPageIndex;
		UpdateButtons();
	}
}