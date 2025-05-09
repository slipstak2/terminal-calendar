#pragma once

#include <vector>
#include <iostream>
#include <string>
#include <memory>
#include <cassert>

extern short ONE;

#define DECLARE_CREATE(T)                                         \
    template<class... Args>                                       \
    static std::shared_ptr<T> Create(Args&&... args) {            \
        return std::make_shared<T>(std::forward<Args>(args)...);  \
    }

class TerminalWindow;
using TerminalWindowPtr = std::shared_ptr<TerminalWindow>;

class TerminalCanvas;
using TerminalCanvasPtr = std::unique_ptr<TerminalCanvas>;

class TerminalControl;
using TerminalControlPtr = std::shared_ptr<TerminalControl>;

class TerminalRootControl;
using TerminalRootControlPtr = std::shared_ptr<TerminalRootControl>;

class TerminalLabel;
using TerminalLabelPtr = std::shared_ptr<TerminalLabel>;

class TerminalLabelFixedWidth;
using TerminalLabelFixedWidthPtr = std::shared_ptr<TerminalLabelFixedWidth>;

class TerminalLabelDataProvider;
using TerminalLabelDataProviderPtr = std::shared_ptr<TerminalLabelDataProvider>;

class TerminalLabelSwitcher;
using TerminalLabelSwitcherPtr = std::shared_ptr<TerminalLabelSwitcher>;

class TerminalButton;
using TerminalButtonPtr = std::shared_ptr<TerminalButton>;

class TerminalCheckBox;
using TerminalCheckBoxPtr = std::shared_ptr<TerminalCheckBox>;

class ListDataSetBase;
using ListDataSetBasePtr = std::shared_ptr<ListDataSetBase>;

class ListDataSet;
using ListDataSetPtr = std::shared_ptr<ListDataSet>;

class ListDataSetNumSequence;
using ListDataSetNumSequencePtr = std::shared_ptr<ListDataSetNumSequence>;

class ListDataProvider;
using ListDataProviderPtr = std::shared_ptr<ListDataProvider>;

class ListDynamicDataSet;
using ListDynamicDataSetPtr = std::shared_ptr<ListDynamicDataSet>;

class TerminalCompositeControl;
using TerminalCompositeControlPtr = std::shared_ptr<TerminalCompositeControl>;

class TerminalListView;
using TerminalListViewPtr = std::shared_ptr<TerminalListView>;

class VerticalScrollableControl;
using VerticalScrollableControlPtr = std::shared_ptr<VerticalScrollableControl>;

class TerminalGroupBox;
using TerminalGroupBoxPtr = std::shared_ptr<TerminalGroupBox>;

class TerminalBorderListView;
using TerminalBorderListViewPtr = std::shared_ptr<TerminalBorderListView>;

class TerminalVerticalScroll;
using TerminalVerticalScrollPtr = std::shared_ptr<TerminalVerticalScroll>;

class TerminalVerticalScrollBar;
using TerminalVerticalScrollBarPtr = std::shared_ptr<TerminalVerticalScrollBar>;

class TerminalMonthBox;
using TerminalMonthBoxPtr = std::shared_ptr<TerminalMonthBox>;

class TerminalMonthGrid;
using TerminalMonthGridPtr = std::shared_ptr<TerminalMonthGrid>;

class TerminalGrid;
using TerminalGridPtr = std::shared_ptr<TerminalGrid>;

class TerminalGridEx;
using TerminalGridExPtr = std::shared_ptr<TerminalGridEx>;

class TerminalGridCell;
using TerminalGridCellPtr = std::shared_ptr<TerminalGridCell>;

class DataFieldAccessor;
using DataFieldAccessorPtr = std::shared_ptr<DataFieldAccessor>;

class DataContainer;
using DataContainerPtr = std::shared_ptr<DataContainer>;

class DataStorage;
using DataStoragePtr = std::shared_ptr<DataStorage>;

class DataView;
using DataViewPtr = std::shared_ptr<DataView>;

class DataSet;
using DataSetPtr = std::shared_ptr<DataSet>;
