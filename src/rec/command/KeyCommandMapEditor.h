#ifndef __REC_COMMAND_KEYCOMMANDMAPEDITOR__
#define __REC_COMMAND_KEYCOMMANDMAPEDITOR__

#include "rec/command/CommandMapEditor.h"
#include "rec/command/CommandMapEditButton.h"
#include "rec/command/CommandMapEditorMappingItem.h"

namespace rec {
namespace command {

class KeyCommandMapEditor : public CommandMapEditor {
public:
    KeyCommandMapEditor(KeyPressMappingSet& mappingSet);
    KeyPressMappingSet& getMappings() { return mappings; }

    virtual const String getDescriptionForKeyPress (const KeyPress& key) {
       return key.getTextDescription();
    }

    virtual void addButton(CommandMapEditButton* button);
    virtual void removeButton(CommandMapEditButton* button);
    virtual void addChildren(CommandMapItemComponent* comp);

    static const int MAX_NUM_ASSIGNMENTS = 3;

private:
    KeyPressMappingSet& mappings;
};

}  // namespace command
}  // namespace rec

#endif  // __REC_COMMAND_KEYCOMMANDMAPEDITOR__
