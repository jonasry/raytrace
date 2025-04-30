#include <edit.h>
#include <scrollba.h>
#include <checkbox.h>

#define OPTIONS	155

_CLASSDEF(OptionDialog)
class OptionDialog : public TDialog {
	PTEdit X_Edit;
	PTEdit Y_Edit;
	PTScrollBar X_Scroll;
	PTScrollBar Y_Scroll;
  PTCheckBox  Aspect;

public:
	OptionDialog(PTWindowsObject AParent) : TDialog(AParent, OPTIONS) {
		TransferBuffer = (void far*) &(((TMainWindow *)Parent)->TransferStruct);

		X_Edit   = new TEdit(this, 201, 6);
		Y_Edit   = new TEdit(this, 202, 6);
		Aspect   = new TCheckBox(this, 204, NULL);
		X_Scroll = new TScrollBar(this, 205); X_Scroll->SetRange(8,1200);
		Y_Scroll = new TScrollBar(this, 206); Y_Scroll->SetRange(8,1200);


	}
	~OptionDialog() {
		delete X_Edit; delete Y_Edit;
		delete X_Scroll; delete Y_Scroll;
		delete Aspect;
	}

};

