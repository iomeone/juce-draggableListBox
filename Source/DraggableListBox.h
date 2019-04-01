#pragma once
#include "JuceHeader.h"

// Your item-data container must inherit from this, and override at least the first
// four member functions.
struct DraggableListBoxItemData
{
    virtual int getNumItems() = 0;
    virtual void paintContents(int, Graphics&, Rectangle<int>) = 0;

    virtual void moveBefore(int indexOfItemToMove, int indexOfItemToPlaceBefore) = 0;
    virtual void moveAfter(int indexOfItemToMove, int indexOfItemToPlaceAfter) = 0;

    // If you need a dynamic list, override these functions as well.
    virtual void deleteItem(int /*indexOfItemToDelete*/) {};
    virtual void addItemAtEnd() {};
};

// DraggableListBox is basically just a ListBox, that inherits from DragAndDropContainer.
// Declare your list box using this type.
class DraggableListBox : public ListBox, public DragAndDropContainer
{
};

// Everything below this point should be generic.
class DraggableListBoxItem : public Component, public DragAndDropTarget
{
public:
    DraggableListBoxItem(DraggableListBox& lb, DraggableListBoxItemData& data, int rn)
        : listBox(lb), modelData(data), rowNum(rn) {}

    // Component
    void paint(Graphics& g) override;
    void mouseEnter(const MouseEvent&) override;
    void mouseExit(const MouseEvent&) override;
    void mouseDrag(const MouseEvent&) override;

    // DragAndDropTarget
    bool isInterestedInDragSource(const SourceDetails&) override { return true; }
    void itemDragEnter(const SourceDetails&) override;
    void itemDragMove(const SourceDetails&) override;
    void itemDragExit(const SourceDetails&) override;
    void itemDropped(const SourceDetails&) override;
    bool shouldDrawDragImageWhenOver() override { return true; }

    // DraggableListBoxItem
protected:
    void updateInsertLines(const SourceDetails &dragSourceDetails);
    void hideInsertLines();

    int rowNum;
    DraggableListBoxItemData& modelData;
    DraggableListBox& listBox;

    MouseCursor savedCursor;
    bool insertAfter = false;
    bool insertBefore = false;
};

class DraggableListBoxModel : public ListBoxModel
{
public:
    DraggableListBoxModel(DraggableListBox& lb, DraggableListBoxItemData& md)
        : listBox(lb), modelData(md) {}

    int getNumRows() override { return modelData.getNumItems(); }
    void paintListBoxItem(int, Graphics &, int, int, bool) override {}

    Component* refreshComponentForRow(int, bool, Component*) override;

protected:
    // Draggable model has a reference to its owner ListBox, so it can tell it to update after DnD.
    DraggableListBox &listBox;

    // It also has a reference to the model data, which it uses to get the current items count,
    // and which it passes to the DraggableListBoxItem objects it creates/updates.
    DraggableListBoxItemData& modelData;
};
