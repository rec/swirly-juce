#ifndef __REC_COMMAND_COMMANDMAPEDITORMAPPINGITEM__
#define __REC_COMMAND_COMMANDMAPEDITORMAPPINGITEM__

#include "rec/command/CommandMapItemComponent.h"

namespace rec {
namespace command {

//==============================================================================
class CommandMapEditorMappingItem  : public TreeViewItem
{
public:
    CommandMapEditorMappingItem (CommandMapEditor& owner_, const CommandID commandID_)
        : owner (owner_), commandID (commandID_)
    {
    }

    const String getUniqueName() const          { return String ((int) commandID) + "_id"; }
    bool mightContainSubItems()                 { return false; }
    int getItemHeight() const                   { return 20; }

    Component* createItemComponent()
    {
        return new CommandMapItemComponent(owner, commandID);
    }

private:
    CommandMapEditor& owner;
    const CommandID commandID;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (CommandMapEditorMappingItem);
};

}  // namespace command
}  // namespace rec

#endif  // __REC_COMMAND_COMMANDMAPEDITORMAPPINGITEM__
