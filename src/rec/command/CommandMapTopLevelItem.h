#ifndef __REC_COMMAND_COMMANDMAPTOPLEVELITEM__
#define __REC_COMMAND_COMMANDMAPTOPLEVELITEM__

#include "rec/command/CommandMapCategoryItem.h"

namespace rec {
namespace command {

//==============================================================================
class CommandMapTopLevelItem   : public TreeViewItem,
                                 public ChangeListener,
                                 public ButtonListener
{
public:
    explicit CommandMapTopLevelItem (CommandMapEditor& owner_)
        : owner (owner_)
    {
        setLinesDrawnForSubItems (false);
        owner.getChangeBroadcaster().addChangeListener (this);
    }

    ~CommandMapTopLevelItem()
    {
        owner.getChangeBroadcaster().removeChangeListener (this);
    }

    bool mightContainSubItems()             { return true; }
    const String getUniqueName() const      { return "keys"; }

    void changeListenerCallback (ChangeBroadcaster*)
    {
        const OpennessRestorer openness (*this);
        clearSubItems();

        const StringArray categories (owner.getCommandManager().getCommandCategories());

        for (int i = 0; i < categories.size(); ++i)
        {
            const Array <CommandID> commands (owner.getCommandManager().getCommandsInCategory (categories[i]));
            int count = 0;

            for (int j = 0; j < commands.size(); ++j)
                if (owner.shouldCommandBeIncluded (commands[j]))
                    ++count;

            if (count > 0)
                addSubItem (new CommandMapCategoryItem(owner, categories[i]));
        }
    }

    static void resetToDefaultsCallback (int result, CommandMapEditor* owner)
    {
        // if (result != 0 && owner != nullptr)
        //     owner->getMappings().resetToDefaultMappings();
    }

    void buttonClicked (Button*)
    {
        AlertWindow::showOkCancelBox (AlertWindow::QuestionIcon,
                                      TRANS("Reset to defaults"),
                                      TRANS("Are you sure you want to reset all the key-mappings to their default state?"),
                                      TRANS("Reset"),
                                      String::empty,
                                      &owner,
                                      ModalCallbackFunction::forComponent (resetToDefaultsCallback, &owner));
    }

protected:
    CommandMapEditor& owner;
};

}  // namespace command
}  // namespace rec

#endif  // __REC_COMMAND_COMMANDMAPTOPLEVELITEM__
