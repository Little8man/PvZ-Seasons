#ifndef __CONFIGSCREEN_H__
#define __CONFIGSCREEN_H__

#include "../../ConstEnums.h"
#include "../../SexyAppFramework/Dialog.h"
#include "../../SexyAppFramework/SliderListener.h"
#include "../../SexyAppFramework/CheckboxListener.h"
using namespace Sexy;

#define NUM_CHECKBOXES ((int)ConfigCheckbox::NUM_CONFIG_CHECKBOXES - 1)
const float NUM_COLUMNS = 3.0f;
const float NUM_ROWS = 4.0f;
const int CHECKBOX_OFFSET_X = 26;
const int CHECKBOX_OFFSET_Y = 56;
const int NAME_OFFSET_X = 70;
const int NAME_OFFSET_Y = 80;
const int DESCRIPTION_OFFSET_X = 26;
const int DESCRIPTION_OFFSET_Y = 100;
const int COLUMN_SIZE = 256;
const int ROW_SIZE = 120;

class LawnApp;
class NewLawnButton;
namespace Sexy
{
    class Slider;
    class Checkbox;
};
class ConfigScreen : public Widget, public Sexy::SliderListener, public Sexy::CheckboxListener, public ButtonListener


{
private:
    enum
    {
        ConfigScreen_Back = 100,
        ConfigScreen_Page = 200,
        ConfigScreen_Checkbox = 300,
    };

public:
    NewLawnButton* mBackButton;                                //+0x8C
    ButtonWidget* mPageButton[MAX_CONFIG_PAGES];           //+0x90
    Sexy::Checkbox* mConfigCheckbox[NUM_CONFIG_CHECKBOXES];
    LawnApp* mApp;
    ConfigPage               mPageIndex;


public:
    ConfigScreen(LawnApp* theApp, ConfigPage thePage);
    ~ConfigScreen();
    
    void					CheckboxChecked(int theId, bool checked);
    void					SliderVal(int theId, double theVal);
    void                        UpdateButtons();
    virtual void            Draw(Graphics* g);
    // Widget Stuff
    virtual void                AddedToManager(WidgetManager* theWidgetManager);
    virtual void                RemovedFromManager(WidgetManager* theWidgetManager);
    virtual void                ButtonPress(int theId);
    virtual void                ButtonDepress(int theId);
};

class ConfigCheckboxDefinition
{
public:
    ConfigCheckbox              mConfigCheckbox;
    ConfigPage                  mPage;
    int                         mRow;
    int                         mCol;
    const SexyChar*             mConfigCheckboxName;
    const SexyChar*             mConfigCheckboxDesc;
};
extern ConfigCheckboxDefinition gConfigCheckboxDefs[NUM_CONFIG_CHECKBOXES];

ConfigCheckboxDefinition& GetConfigCheckboxDefinition(int theChallengeMode);

#endif
