//*****************************************************************************
//-----------------------------------------------------------------------------
// AntGUI.h 
//
// A GUI for the AntRenderer, the style of GUI creation and layout was inspired
// but the DXUT GUI class but is not a direct copy.
//
// Currently unfinished
//-----------------------------------------------------------------------------
//*****************************************************************************


#pragma once
#ifndef GUI_H
#define GUI_H
#include <vector>
#include <string>
//#include <strsafe.h>

#include "AntWrap.h"

#include "Ant.h"

//-----------------------------------------------------------------------------
// Forward declations
//-----------------------------------------------------------------------------
class AntGUIDialog;
class AntGUIComponent;
class AntGUIImageText;
class AntGUIButton;
class AntGUIComboBox;
class AntGUICheckBox;
class AntGUITextBox;
class AntGUITextArea;
class AntGUIListBox;
class AntGUISlider;
class AntGUIProgressBar;
class AntGUITab;
class AntGUITabPane;

//-----------------------------------------------------------------------------
// enums
//-----------------------------------------------------------------------------
enum ANTGUI_COMPONENT_TYPE
{
	ANTGUI_COMPONENT_IMAGETEXT,
	ANTGUI_COMPONENT_BUTTON,
	ANTGUI_COMPONENT_COMBOBOX,
	ANTGUI_COMPONENT_CHECKBOX,
	ANTGUI_COMPONENT_TEXTBOX,
	ANTGUI_COMPONENT_TEXTAREA,
	ANTGUI_COMPONENT_LISTBOX,
	ANTGUI_COMPONENT_SLIDER,
	ANTGUI_COMPONENT_PROGRESSBAR,
};

//-----------------------------------------------------------------------------
// Structs and Classes for AntGUI types
//-----------------------------------------------------------------------------
class AntGUIElement
{
public:
	void SetTexture(UINT id, RECT textureRect);
	void SetTexture(UINT id, RECT* textureRect, AntFontColorARGB fontColor);
	void SetFont(UINT id, AntFontColorARGB fontColor);
	void SetFont(AntFontColorARGB fontColor);
	int GetFont() { return _fontId; }
	AntFontColorARGB* GetFontColor() { return &_fontColor; }
	int GetTexture() { return _textureId; }
	RECT* GetTextureRect() { return &_textureRect; }
protected:
	UINT _fontId;
	UINT _textureId;
	AntFontColorARGB _fontColor; 
	RECT _textureRect;
};

struct AntGUITabPaneElem
{
	AntGUITab* tab;
	AntGUIDialog* pane;
};

class AntResourceManager
{
	struct AntResourceManagerDialogTextureInfo
	{
		UINT _textureId;
		AntTexture* _texture;
		UINT _renderTextureId;
		std::wstring _fileName;
	};
	struct AntResourceManagerDialogInfo
	{
		int _dialogId;
		std::vector<AntResourceManagerDialogTextureInfo> _textureInfos;
	};
public:
	AntRenderer* _renderer;
	UINT numTextures;
	void AddTexture(int dialogId, UINT textureId, std::wstring string);
	UINT GetTexture(int dialogId, UINT textureId);
	std::vector<AntResourceManagerDialogInfo> _dialogInfos;
};

//-----------------------------------------------------------------------------
// Inlines
//-----------------------------------------------------------------------------
inline int Width(RECT* r) { return r->right - r->left; }
inline int Height(RECT* r) { return r->bottom - r->top; }

//-----------------------------------------------------------------------------
// A gui dialog class
//-----------------------------------------------------------------------------
class AntGUIDialog
{
public:
	AntGUIDialog();
	~AntGUIDialog();
	void Init(AntRenderer* renderer);
	void Init(AntResourceManager* manager, bool somebool, std::wstring string) {  Init(manager); SetTexture(0, string); }
	void Init(AntResourceManager* manager) {  _manager = manager; _renderer = manager->_renderer; }

	void AddImageText(int id, const std::wstring& string, int x, int y, int h, int w);
	void AddStatic(int id, const std::wstring& string, int x, int y, int h, int w) { AddImageText(id,string,x,y,h,w); }
	void AddButton(int id, const std::wstring& string, int x, int y, int h, int w, UINT nHotkey=0);
	void AddComboBox(int id, int x, int y, int h, int w);
	void AddCheckBox(int id, const std::wstring& string, int x, int y, int h, int w);
	void AddTextBox(int id, const std::wstring& string, int x, int y, int h, int w);
	void AddEditBox(int id, const std::wstring& string, int x, int y, int h, int w) { AddTextBox(id,string,x,y,h,w); }
	void AddTextArea(int id, const std::wstring& string, int x, int y, int h, int w);
	void AddListBox(int id, int x, int y, int h, int w);
	void AddSlider(int id, int x, int y, int h, int w, int min=0, int max=100, int value=0);
	void AddProgressBar(int id, int x, int y, int h, int w, int min=0, int max=100, int value=0);

	AntGUIImageText* GetImageText(int id) { return (AntGUIImageText*) GetComponent(id, ANTGUI_COMPONENT_IMAGETEXT); }
	AntGUIImageText* GetStatic(int id) { return (AntGUIImageText*) GetComponent(id, ANTGUI_COMPONENT_IMAGETEXT); }
	AntGUIButton* GetButton(int id) { return (AntGUIButton*) GetComponent(id, ANTGUI_COMPONENT_BUTTON); }
	AntGUIComboBox* GetComboBox(int id) { return (AntGUIComboBox*) GetComponent(id, ANTGUI_COMPONENT_COMBOBOX); }
	AntGUICheckBox* GetCheckBox(int id) { return (AntGUICheckBox*) GetComponent(id, ANTGUI_COMPONENT_CHECKBOX); }
	AntGUITextBox* GetTextBox(int id) { return (AntGUITextBox*) GetComponent(id, ANTGUI_COMPONENT_TEXTBOX); }
	AntGUITextBox* GetEditBox(int id) { return (AntGUITextBox*) GetComponent(id, ANTGUI_COMPONENT_TEXTBOX); }
	AntGUITextArea* GetTextArea(int id) { return (AntGUITextArea*) GetComponent(id, ANTGUI_COMPONENT_TEXTAREA); }
	AntGUIListBox* GetListBox(int id) { return (AntGUIListBox*) GetComponent(id, ANTGUI_COMPONENT_LISTBOX); }
	AntGUISlider* GetSlider(int id) { return (AntGUISlider*) GetComponent(id, ANTGUI_COMPONENT_SLIDER); }
	AntGUIProgressBar* GetProgressBar(int id) { return (AntGUIProgressBar*) GetComponent(id, ANTGUI_COMPONENT_PROGRESSBAR); }

	AntGUIComponent* GetComponent(int id);
	AntGUIComponent* GetComponent(int id, ANTGUI_COMPONENT_TYPE);
	AntGUIComponent* GetIntersect(int x, int y);
	void AddComponent(AntGUIComponent* component);

	void SetSize(int h, int w) { _h=h; _w=w; }
	void SetPos(int x, int y) { _x=x; _y=y; }
	void SetLocation(int x, int y) { _x=x; _y=y; }
	void SetTexture(UINT index, std::wstring string);
	void SetFont(UINT index, std::wstring name, long height, long weight);
	typedef void(*EventFunction)(UINT event, int componentId, AntGUIComponent* component, void* vParam);
	void SetEventCaller(EventFunction f) { _eventFunction=f; }
	void SetCallback(EventFunction f) { _eventFunction=f; }

    bool Event(UINT Event, UINT p1, UINT p2);
	void Render(float time) { Render(time,_renderer); }
	void Render(float time, AntRenderer* renderer);

	void DrawText(UINT fontId, const std::wstring& string, AntFontColorARGB* fontColor, RECT* dst);
	void DrawSprite(UINT textureID, RECT* src, RECT* dst);

	void Show(bool show) { _visible=show; }
	bool IsVisible() { return _visible; }
	bool GetVisible() { return _visible; }
	void SetEnabled(bool enabled) {_enabled=enabled;}
	void SetVisible(bool visible) {_visible=visible;}

	void RequestFocus(AntGUIComponent* focus) { _focus = focus; }
	void EventCall(UINT event, int componentId, AntGUIComponent* component, void* vParam);

protected:
	RECT _boundingBox;
	int _x, _y;
	int _h, _w;
	bool _enabled;
	bool _visible;
	AntGUIComponent* _focus;
	AntGUIComponent* _mouseOver;
	int _id;

	std::vector<AntGUIComponent*> _components;
	AntRenderer* _renderer;
	AntResourceManager* _manager;
	EventFunction _eventFunction;
};

//-----------------------------------------------------------------------------
// Base class for a gui component
//-----------------------------------------------------------------------------
class AntGUIComponent
{
public:
	AntGUIComponent();
    virtual bool Event(UINT Event, UINT p1, UINT p2) { return false; }

	virtual void Render(float time) { };

	void SetSize(int h, int w) { _h=h; _w=w; UpdateBoundingBox();}
	void SetPos(int x, int y) { _x=x; _y=y; UpdateBoundingBox();}

	UINT GetId() { return _id; }
	void SetId(int id) { _id=id; }

	UINT GetType() { return _type; }

	void AddElement(AntGUIElement element) { _elements.push_back(element); }
	AntGUIElement* GetElement(UINT index) { return &_elements.at(index); }

	virtual void MouseEnter() { _mouseOver=true; }
	virtual void MouseExit() { _mouseOver=false; }

	virtual void UpdateBoundingBox();
	RECT GetBoundingBox() { return _boundingBox; }
	
	typedef void(*EventFunction)(void* vParam);
	void SetEventCaller(EventFunction f) { _eventFunction=f; }
	void EventCall(void *vParam);

	void SetEnabled(bool enabled) { _enabled=enabled; }
	void SetVisible(bool visible) {}
	bool IsVisible() { }
	bool GetVisible() { }

	RECT _boundingBox;  
	int _x, _y;
	int _h, _w;
	bool _enabled;
	bool _mouseOver;
	int _id;
	ANTGUI_COMPONENT_TYPE _type;
	AntGUIDialog* _parent;
	std::vector<AntGUIElement> _elements;
	EventFunction _eventFunction;
};

//-----------------------------------------------------------------------------
// An ImageText (Image and text with no other functionality) component class
//-----------------------------------------------------------------------------
class AntGUIImageText : public AntGUIComponent
{
public:
	AntGUIImageText(AntGUIDialog* parent=NULL);
	
	void SetText(const std::wstring& text);
	std::wstring GetText() { return _text; }
	void SetTextColor(AntFontColorARGB fontColor) { }//GetElement(0)->SetFont(fontColor); }

	virtual void Render(float time);

protected:
	std::wstring _text;
};

//-----------------------------------------------------------------------------
// A Button class
//-----------------------------------------------------------------------------
class AntGUIButton : public AntGUIImageText
{
public:
	AntGUIButton(AntGUIDialog* parent=NULL);

	virtual void Render(float time);
	virtual bool Event(UINT Event, UINT p1, UINT p2);

	bool _pressed;
};

//-----------------------------------------------------------------------------
// A CheckBox
//-----------------------------------------------------------------------------
class AntGUICheckBox : public AntGUIImageText
{
public:
	AntGUICheckBox(AntGUIDialog* parent=NULL);

	virtual void Render(float time);
	virtual bool Event(UINT uEvent, UINT p1, UINT p2);

	void SetChecked(bool checked) { _checked=checked; }
	bool GetChecked() { return _checked; }
	bool _checked;
};

//-----------------------------------------------------------------------------
// A ComboBox
//-----------------------------------------------------------------------------
class AntGUIComboBox : public AntGUIImageText
{
public:
	struct AntGUIComboBoxOption
	{
		UINT _id;
		std::wstring _text;
		void** _userData;
		void** pData;
	};

	AntGUIComboBox(AntGUIDialog* parent=NULL);

	virtual void Render(float time);
	virtual bool Event(UINT Event, UINT p1, UINT p2);
	void AddOption(int id, const std::wstring& text, void** userData);
	void SetSelected(int id);
	void Open() { _opened=true; }
	
	void UpdateBoundingBox();

	virtual void MouseExit();

	std::vector<AntGUIComboBoxOption> _options;
	AntGUIComboBoxOption* _selected;
	AntGUIComboBoxOption* _hover;
	bool _opened;
};

//-----------------------------------------------------------------------------
// A TextBox
//-----------------------------------------------------------------------------
class AntGUITextBox : public AntGUIImageText
{
public:
	AntGUITextBox(AntGUIDialog* parent=NULL);

	virtual void Render(float time);
	virtual bool Event(UINT Event, UINT p1, UINT p2);

	virtual void MouseEnter();
	virtual void MouseExit();

	void SetCaret(UINT pos);
	void ClearText() { SetText(L""); }
	int _pos;
};

//-----------------------------------------------------------------------------
// A TextArea
//-----------------------------------------------------------------------------
class AntGUITextArea : public AntGUIImageText
{
public:
	AntGUITextArea(AntGUIDialog* parent=NULL);

	virtual void Render(float time);
};

//-----------------------------------------------------------------------------
// A ListBox
//-----------------------------------------------------------------------------
class AntGUIListBox : public AntGUIComboBox
{
public:
	AntGUIListBox(AntGUIDialog* parent=NULL);

	virtual void Render(float time);

	AntGUIComboBoxOption* GetSelectedItem() { return _selected; }
};

//-----------------------------------------------------------------------------
// A ProgressBar class
//-----------------------------------------------------------------------------
class AntGUIProgressBar : public AntGUIImageText
{
public:
	AntGUIProgressBar(AntGUIDialog* parent=NULL);

	virtual void Render(float time);

	void SetMax(int max) { _max=max; }
	int GetMax() { return _max; }
	void SetMin(int min) { _min=min; }
	int GetMin() { return _min; }
	void SetValue(int value) { _value=value; }
	int GetValue() { return _value; }

protected:
	int _min;
	int _max;
	int _value;
};

//-----------------------------------------------------------------------------
// A Slider class
//-----------------------------------------------------------------------------
class AntGUISlider : public AntGUIProgressBar
{
public:
	AntGUISlider(AntGUIDialog* parent=NULL);

	virtual bool Event(UINT Event, UINT p1, UINT p2);

	virtual void Render(float time);
};

//-----------------------------------------------------------------------------
// A tab class
//-----------------------------------------------------------------------------
class AntGUITab : public AntGUIImageText
{
	AntGUITab(AntGUITabPane* parent=NULL);

	virtual bool Event(UINT Event, UINT p1, UINT p2);

	virtual void Render(float time);

	AntGUITabPane* _parent;
};

//-----------------------------------------------------------------------------
// Selectable tabs
//-----------------------------------------------------------------------------
class AntGUITabPane : public AntGUIComponent
{
	void AddPane(int id, std::wstring& text, AntGUIDialog *pane);
	void GetPane(int id);

	bool Event(UINT Event, UINT p1, UINT p2);

	void Render(float time);

	std::vector<AntGUITabPaneElem*> _panes;
};
#endif
