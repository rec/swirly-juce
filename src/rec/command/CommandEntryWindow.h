#ifndef __REC_COMMAND_COMMANDENTRYWINDOW__
#define __REC_COMMAND_COMMANDENTRYWINDOW__

#include "rec/base/base.h"

namespace rec {
namespace command {

//==============================================================================
class CommandEntryWindow  : public AlertWindow
{
public:
    CommandEntryWindow (const String& caption)
        : AlertWindow (TRANS("New command mapping"),
                       caption,  // TRANS("Please press a key combination now..."),
                       AlertWindow::NoIcon)
    {
        addButton (TRANS("Ok"), 1);
        addButton (TRANS("Cancel"), 0);

        // Probably not needed in the general case but no harm...
        // (avoid return + escape keys getting processed by the buttons..)
        for (int i = getNumChildComponents(); --i >= 0;)
            getChildComponent (i)->setWantsKeyboardFocus (false);

        setWantsKeyboardFocus (true);
        grabKeyboardFocus();
    }

private:

    JUCE_DECLARE_NON_COPYABLE (CommandEntryWindow);
};

}  // namespace command
}  // namespace rec

#endif  // __REC_COMMAND_COMMANDENTRYWINDOW__
