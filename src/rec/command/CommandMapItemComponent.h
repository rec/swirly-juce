#ifndef __REC_COMMAND_COMMANDMAPITEMCOMPONENT__
#define __REC_COMMAND_COMMANDMAPITEMCOMPONENT__

#include "rec/command/CommandMapEditor.h"

namespace rec {
namespace command {

class CommandMapItemComponent  : public Component
{
public:
    CommandMapItemComponent(CommandMapEditor& owner_, const CommandID commandID_)
        : commandID (commandID_), owner(owner_)
    {
        setInterceptsMouseClicks (false, true);

        owner.addChildren(this);
    }

    void addKeyPressButton (const String& desc, const int index, const bool isReadOnly)
    {
        CommandMapEditButton* const b = new CommandMapEditButton(owner, commandID, desc, index);
        buttons.add (b);

        b->setEnabled (! isReadOnly);
        b->setVisible (buttons.size() <= (int) maxNumAssignments);
        addChildComponent (b);
    }

    void paint (Graphics& g)
    {
        g.setFont (getHeight() * 0.7f);
        g.setColour (findColour (CommandMapEditor::textColourId));

        g.drawFittedText (owner.getCommandManager().getNameOfCommand (commandID),
                          4, 0, jmax (40, getChildComponent (0)->getX() - 5), getHeight(),
                          Justification::centredLeft, true);
    }

    void resized()
    {
        int x = getWidth() - 4;

        for (int i = buttons.size(); --i >= 0;)
        {
            CommandMapEditButton* const b = buttons.getUnchecked(i);

            b->fitToContent (getHeight() - 2);
            b->setTopRightPosition (x, 1);
            x = b->getX() - 5;
        }
    }

    const CommandID commandID;

private:
    CommandMapEditor& owner;
    OwnedArray<CommandMapEditButton> buttons;

    enum { maxNumAssignments = 3 };

    // Juce_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (CommandMapItemComponent);
};

}  // namespace command
}  // namespace rec

#endif  // __REC_COMMAND_COMMANDMAPITEMCOMPONENT__
