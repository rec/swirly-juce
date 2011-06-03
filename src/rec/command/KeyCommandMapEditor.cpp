#include "rec/command/KeyCommandMapEditor.h"
#include "rec/command/CommandMapItemComponent.h"

namespace rec {
namespace command {

class KeyCommandEntryWindow : public CommandEntryWindow {
 public:
    KeyCommandEntryWindow(KeyCommandMapEditor& owner_)
        : CommandEntryWindow("Please press a key combination now..."), owner(owner_)
    {
    }

    bool keyPressed (const KeyPress& key)
    {
        lastPress = key;
        String message (TRANS("Key: ") + owner.getDescriptionForKeyPress (key));

        const CommandID previousCommand = owner.getMappings().findCommandForKeyPress (key);

        if (previousCommand != 0)
            message << "\n\n" << TRANS("(Currently assigned to \"")
                    << owner.getCommandManager().getNameOfCommand (previousCommand) << "\")";

        setMessage (message);
        return true;
    }

    bool keyStateChanged (bool)
    {
        return true;
    }

    KeyPress lastPress;
    KeyCommandMapEditor& owner;
};

KeyCommandMapEditor::KeyCommandMapEditor(KeyPressMappingSet& mappingSet)
    : CommandMapEditor(*mappingSet.getCommandManager(), mappingSet),
      mappings(mappingSet)
{
}

static void assignNewKeyCallback (int result, CommandMapEditButton* button, KeyPress newKey)
{
     if (result != 0 && button != nullptr) {
         KeyCommandMapEditor* editor = dynamic_cast<KeyCommandMapEditor*>(&button->getOwner());
         editor->setNewKey (button, newKey, true);
     }
}

static void setNewKey (KeyPressMappingSet& mappings, CommandMapEditButton* button,
                       const KeyPress& newKey, bool dontAskUser)
{
    if (newKey.isValid())
    {
        const CommandID previousCommand = mappings.findCommandForKeyPress (newKey);

        if (previousCommand == 0 || dontAskUser)
        {
            mappings.removeKeyPress (newKey);

            if (button->keyNum >= 0)
                mappings.removeKeyPress (button->commandID, button->keyNum);

            mappings.addKeyPress (button->commandID, newKey, button->keyNum);
        }
        else
        {
            AlertWindow::showOkCancelBox (AlertWindow::WarningIcon,
                                          TRANS("Change key-mapping"),
                                          TRANS("This key is already assigned to the command \"")
                                            + mappings.getCommandManager()->getNameOfCommand (previousCommand)
                                            + TRANS("\"\n\nDo you want to re-assign it to this new command instead?"),
                                          TRANS("Re-assign"),
                                          TRANS("Cancel"),
                                          this,
                                          ModalCallbackFunction::forComponent (assignNewKeyCallback,
                                                                               button, KeyPress (newKey)));
        }
    }
}

void keyChosen (int result, CommandMapEditButton* button)
{
    KeyCommandEntryWindow* window = dynamic_cast<KeyCommandEntryWindow*>(button->getCommandEntryWindow());
    if (result != 0 && button != nullptr && window != nullptr)
    {
        window->setVisible (false);
        setNewKey (window->owner.getMappings(), button, window->lastPress, false);
    }

    button->setCommandEntryWindow();
}

void KeyCommandMapEditor::addButton(CommandMapEditButton* button)
{
    button->setCommandEntryWindow(new KeyCommandEntryWindow(*this));
    button->getCommandEntryWindow()->enterModalState (true, ModalCallbackFunction::forComponent(keyChosen, button));
}

void KeyCommandMapEditor::removeButton(CommandMapEditButton* button)
{
    getMappings().removeKeyPress(button->commandID, button->keyNum);
}


void KeyCommandMapEditor::addChildren(CommandMapItemComponent* comp) {
  const bool isReadOnly = isCommandReadOnly(comp->commandID);
  const Array <KeyPress> keyPresses (getMappings().getKeyPressesAssignedToCommand (comp->commandID));
  for (int i = 0; i < jmin ((int) MAX_NUM_ASSIGNMENTS, keyPresses.size()); ++i)
    comp->addKeyPressButton (getDescriptionForKeyPress (keyPresses.getReference (i)), i, isReadOnly);
  comp->addKeyPressButton (String::empty, -1, isReadOnly);
}

}  // namespace command
}  // namespace rec
