#ifndef __REC_COMMAND_COMMANDMAPCATEGORYITEM__
#define __REC_COMMAND_COMMANDMAPCATEGORYITEM__

#include "rec/command/CommandMapItemComponent.h"
#include "rec/command/CommandMapEditor.h"

namespace rec {
namespace command {

//==============================================================================
class CommandMapCategoryItem  : public TreeViewItem
{
public:
    CommandMapCategoryItem (CommandMapEditor& owner_, const String& name)
        : owner (owner_), categoryName (name)
    {
    }

    virtual CommandMapEditorMappingItem* createItemComponent(CommandID cmd) const
    {
        return new CommandMapEditorMappingItem(owner, cmd);
    }

    const String getUniqueName() const          { return categoryName + "_cat"; }
    bool mightContainSubItems()                 { return true; }
    int getItemHeight() const                   { return 28; }

    void paintItem (Graphics& g, int width, int height)
    {
        g.setFont (height * 0.6f, Font::bold);
        g.setColour (owner.findColour (CommandMapEditor::textColourId));

        g.drawText (categoryName,
                    2, 0, width - 2, height,
                    Justification::centredLeft, true);
    }

    void itemOpennessChanged (bool isNowOpen)
    {
        if (isNowOpen)
        {
            if (getNumSubItems() == 0)
            {
                Array <CommandID> commands (owner.getCommandManager().getCommandsInCategory (categoryName));

                for (int i = 0; i < commands.size(); ++i)
                {
                    if (owner.shouldCommandBeIncluded (commands[i]))
                      addSubItem (createItemComponent (commands[i]));  // TODO: why?
                }
            }
        }
        else
        {
            clearSubItems();
        }
    }

private:
    CommandMapEditor& owner;
    String categoryName;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (CommandMapCategoryItem);
};


}  // namespace command
}  // namespace rec

#endif  // __REC_COMMAND_COMMANDMAPCATEGORYITEM__
