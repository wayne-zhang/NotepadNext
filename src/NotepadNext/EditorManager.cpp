/*
 * This file is part of Notepad Next.
 * Copyright 2021 Justin Dailey
 *
 * Notepad Next is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Notepad Next is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Notepad Next.  If not, see <https://www.gnu.org/licenses/>.
 */

#include <QApplication>

#include "EditorManager.h"
#include "ScintillaNext.h"
#include "Scintilla.h"
#include "Settings.h"
#include "NotepadNextApplication.h"

// Editor decorators
#include "BraceMatch.h"
#include "HighlightedScrollBar.h"
#include "SmartHighlighter.h"
#include "SurroundSelection.h"
#include "LineNumbers.h"
#include "BetterMultiSelection.h"
#include "AutoIndentation.h"
#include "AutoCompletion.h"
#include "URLFinder.h"
#include "BookMarkDecorator.h"
#include "ThemeColors.h"

const int MARK_HIDELINESBEGIN = 23;
const int MARK_HIDELINESEND = 22;
const int MARK_HIDELINESUNDERLINE = 21;

static int invertColor(int color)
{
    // implementation by XOR
    return color ^ 0xFFFFFF;
}

static int DefaultFontSize()
{
    QFont font = QApplication::font();

    // Make it slightly larger than the UI font
    return font.pointSize() + 2;
}


EditorManager::EditorManager(QObject *parent) : QObject(parent)
{
    connect(this, &EditorManager::editorCreated, this, [=](ScintillaNext *editor) {
        connect(editor, &ScintillaNext::closed, this, [=]() {
            emit editorClosed(editor);
        });
    });
}

ScintillaNext *EditorManager::createEditor(const QString &name)
{
    ScintillaNext *editor = new ScintillaNext(name);

    manageEditor(editor);

    return editor;
}

ScintillaNext *EditorManager::createEditorFromFile(const QString &filePath, bool tryToCreate)
{
    ScintillaNext *editor = ScintillaNext::fromFile(filePath, tryToCreate);

    if (editor) {
        manageEditor(editor);
    }

    return editor;
}

ScintillaNext *EditorManager::getEditorByFilePath(const QString &filePath)
{
    QFileInfo newInfo(filePath);
    newInfo.makeAbsolute();

    purgeOldEditorPointers();

    for (ScintillaNext *editor : qAsConst(editors)) {
        if (editor->isFile() && editor->getFileInfo() == newInfo) {
            return editor;
        }
    }

    return Q_NULLPTR;
}

void EditorManager::manageEditor(ScintillaNext *editor)
{
    editors.append(QPointer<ScintillaNext>(editor));

    setupEditor(editor);

    emit editorCreated(editor);
}

void EditorManager::setupEditor(ScintillaNext *editor)
{
    qInfo(Q_FUNC_INFO);

    editor->clearCmdKey(SCK_INSERT);

    editor->setFoldMarkers(QStringLiteral("box"));

    editor->setIdleStyling(SC_IDLESTYLING_TOVISIBLE);
    editor->setEndAtLastLine(false);

    editor->setMultipleSelection(true);
    editor->setAdditionalSelectionTyping(true);
    editor->setMultiPaste(SC_MULTIPASTE_EACH);
    editor->setVirtualSpaceOptions(SCVS_RECTANGULARSELECTION);

    editor->setMarginLeft(2);

    editor->setMarginWidthN(0, 30);
    editor->setMarginMaskN(1, (1<<MARK_HIDELINESBEGIN) | (1<<MARK_HIDELINESEND) | (1<<MARK_HIDELINESUNDERLINE));
    editor->setMarginMaskN(2, SC_MASK_FOLDERS);
    editor->setMarginWidthN(2, 14);

    editor->markerDefine(MARK_HIDELINESUNDERLINE, SC_MARK_UNDERLINE);
    

    editor->markerDefine(MARK_HIDELINESBEGIN, SC_MARK_ARROW);
    editor->markerDefine(MARK_HIDELINESEND, SC_MARK_ARROWDOWN);

    editor->setMarginSensitiveN(2, true);

    editor->setFoldFlags(SC_FOLDFLAG_LINEAFTER_CONTRACTED);
    editor->setScrollWidthTracking(true);
    editor->setScrollWidth(1);

    editor->setTabDrawMode(SCTD_STRIKEOUT);
    editor->setTabWidth(4);
    editor->setBackSpaceUnIndents(true);

    editor->setCaretLineVisible(true);
    editor->setCaretLineVisibleAlways(true);
    editor->setCaretWidth(2);
    
    editor->setIndentationGuides(SC_IV_LOOKBOTH);

    editor->setAutomaticFold(SC_AUTOMATICFOLD_SHOW | SC_AUTOMATICFOLD_CLICK | SC_AUTOMATICFOLD_CHANGE);
    editor->markerEnableHighlight(true);    
    
    editor->setWhitespaceSize(2);
    
    editor->styleSetSize(STYLE_DEFAULT, DefaultFontSize());
    editor->styleSetFont(STYLE_DEFAULT, "Courier New"); 
    
    editor->styleSetBold(STYLE_LINENUMBER, false);

    // set up editor theme, only colour now, dark mode aware
    setupEditorTheme(editor);

    // STYLE_CONTROLCHAR
    // STYLE_CALLTIP
    // STYLE_FOLDDISPLAYTEXT

    // Decorators
    SmartHighlighter *s = new SmartHighlighter(editor);
    s->setEnabled(true);

    HighlightedScrollBarDecorator *h = new HighlightedScrollBarDecorator(editor);
    h->setEnabled(true);

    BraceMatch *b = new BraceMatch(editor);
    b->setEnabled(true);

    LineNumbers *l = new LineNumbers(editor);
    l->setEnabled(true);

    SurroundSelection *ss = new SurroundSelection(editor);
    ss->setEnabled(true);

    BetterMultiSelection *bms = new BetterMultiSelection(editor);
    bms->setEnabled(true);

    AutoIndentation *ai = new AutoIndentation(editor);
    ai->setEnabled(true);

    AutoCompletion *ac = new AutoCompletion(editor);
    ac->setEnabled(true);

    URLFinder *uf = new URLFinder(editor);
    uf->setEnabled(true);

    BookMarkDecorator *bm = new BookMarkDecorator(editor);
    bm->setEnabled(true);
}

void EditorManager::setupEditorTheme(ScintillaNext *editor)
{
    if (getSettings()->darkMode()) {  
        editor->setCaretFore(0xFFFFFF);
        
        //editor->setSelFore(true, 0xEFEFEF);
        editor->setSelBack(true, 0xA0A0A0);

        // folding/arker
        for (int i = SC_MARKNUM_FOLDEREND; i <= SC_MARKNUM_FOLDEROPEN; ++i) {
            editor->markerSetFore(i, 0x808080);
            editor->markerSetBack(i, 0x1F1F1F);
            editor->markerSetBackSelected(i, 0x0000FF);
        }
        
        editor->markerSetBack(MARK_HIDELINESUNDERLINE, invertColor(0x77CC77));     
        // end folding/marker
 
        editor->setEdgeColour(invertColor(0x80FFFF));

        // https://www.scintilla.org/ScintillaDoc.html#ElementColours 
        // SC_ELEMENT_SELECTION_TEXT
        // SC_ELEMENT_SELECTION_BACK
        // SC_ELEMENT_SELECTION_ADDITIONAL_TEXT
        // SC_ELEMENT_SELECTION_ADDITIONAL_BACK
        // SC_ELEMENT_SELECTION_SECONDARY_TEXT
        // SC_ELEMENT_SELECTION_SECONDARY_BACK
        // SC_ELEMENT_SELECTION_INACTIVE_TEXT
        editor->setElementColour(SC_ELEMENT_SELECTION_INACTIVE_BACK, invertColor(0xFFE0E0E0));
        // SC_ELEMENT_CARET
        // SC_ELEMENT_CARET_ADDITIONAL
        editor->setElementColour(SC_ELEMENT_CARET_LINE_BACK, 0x606060);
        // SC_ELEMENT_WHITE_SPACE
        // SC_ELEMENT_WHITE_SPACE_BACK
        // setElementColour supports transparency!
        editor->setElementColour(SC_ELEMENT_WHITE_SPACE, invertColor(0xFFD0D0D0));
        editor->setElementColour(SC_ELEMENT_WHITE_SPACE_BACK, DARK_DEFAULT_BG);
        
        // SC_ELEMENT_HOT_SPOT_ACTIVE
        // SC_ELEMENT_HOT_SPOT_ACTIVE_BACK
        editor->setElementColour(SC_ELEMENT_FOLD_LINE, invertColor(0xFFA0A0A0));
        // SC_ELEMENT_HIDDEN_LINE

        editor->setFoldMarginColour(true, 0x3F3F3F);
        editor->setFoldMarginHiColour(true, 0xE9E9E9);

        editor->styleSetFore(STYLE_DEFAULT, DARK_DEFAULT_FG);
        editor->styleSetBack(STYLE_DEFAULT, DARK_DEFAULT_BG);

        editor->styleSetFore(STYLE_LINENUMBER, 0xE4E4E4);
        editor->styleSetBack(STYLE_LINENUMBER, 0x808080);
       
        editor->styleSetFore(STYLE_BRACELIGHT, 0xFFFFFF);
        editor->styleSetBack(STYLE_BRACELIGHT, 0x0000FF);

        editor->styleSetFore(STYLE_BRACEBAD, 0xFFFFFF);
        editor->styleSetBack(STYLE_BRACEBAD, 0x000080);

        editor->styleSetFore(STYLE_INDENTGUIDE, 0xC0C0C0);
        editor->styleSetBack(STYLE_INDENTGUIDE, DARK_DEFAULT_BG);
    }
    else {
        for (int i = SC_MARKNUM_FOLDEREND; i <= SC_MARKNUM_FOLDEROPEN; ++i) {
            editor->markerSetFore(i, 0xF3F3F3);
            editor->markerSetBack(i, 0x808080);
            editor->markerSetBackSelected(i, 0x0000FF);
        }

        editor->markerSetBack(MARK_HIDELINESUNDERLINE, 0x77CC77);        
        editor->setEdgeColour(0x80FFFF);

        // https://www.scintilla.org/ScintillaDoc.html#ElementColours
        // SC_ELEMENT_SELECTION_TEXT
        // SC_ELEMENT_SELECTION_BACK
        // SC_ELEMENT_SELECTION_ADDITIONAL_TEXT
        // SC_ELEMENT_SELECTION_ADDITIONAL_BACK
        // SC_ELEMENT_SELECTION_SECONDARY_TEXT
        // SC_ELEMENT_SELECTION_SECONDARY_BACK
        // SC_ELEMENT_SELECTION_INACTIVE_TEXT
        editor->setElementColour(SC_ELEMENT_SELECTION_INACTIVE_BACK, 0xFFE0E0E0);
        // SC_ELEMENT_CARET
        // SC_ELEMENT_CARET_ADDITIONAL
        editor->setElementColour(SC_ELEMENT_CARET_LINE_BACK, 0xFFFFE8E8);
        editor->setElementColour(SC_ELEMENT_WHITE_SPACE, 0xFFD0D0D0);
        // SC_ELEMENT_WHITE_SPACE_BACK
        // SC_ELEMENT_HOT_SPOT_ACTIVE
        // SC_ELEMENT_HOT_SPOT_ACTIVE_BACK
        editor->setElementColour(SC_ELEMENT_FOLD_LINE, 0xFFA0A0A0);
        // SC_ELEMENT_HIDDEN_LINE

        editor->setFoldMarginColour(true, 0xFFFFFF);
        editor->setFoldMarginHiColour(true, 0xE9E9E9);

        editor->styleSetFore(STYLE_DEFAULT, 0x000000);
        editor->styleSetBack(STYLE_DEFAULT, 0xFFFFFF);

        editor->styleSetFore(STYLE_LINENUMBER, 0x808080);
        editor->styleSetBack(STYLE_LINENUMBER, 0xE4E4E4);
       
        editor->styleSetFore(STYLE_BRACELIGHT, 0x0000FF);
        editor->styleSetBack(STYLE_BRACELIGHT, 0xFFFFFF);

        editor->styleSetFore(STYLE_BRACEBAD, 0x000080);
        editor->styleSetBack(STYLE_BRACEBAD, 0xFFFFFF);

        editor->styleSetFore(STYLE_INDENTGUIDE, 0xC0C0C0);
        editor->styleSetBack(STYLE_INDENTGUIDE, 0xFFFFFF);    
    }
}

void EditorManager::purgeOldEditorPointers()
{
    QMutableListIterator<QPointer<ScintillaNext>> it(editors);

    while (it.hasNext()) {
        QPointer<ScintillaNext> pointer = it.next();
        if (pointer.isNull())
            it.remove();
    }
}

Settings* EditorManager::getSettings()
{
    return ((NotepadNextApplication*)parent())->getSettings();
}
