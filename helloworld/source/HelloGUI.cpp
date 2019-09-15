#include "../include/HelloGUI.h"


void HelloGUI::OnDraw(CDC* pdc) {

	CRect  rect1;
	this->GetClientRect(&rect1);
	CRect  rect(0, 0, 255, 255); // rect1.Width(), rect1.Height());

	for (int i = 0; i < 20; ++i) {
		COLORREF  rgb = RGB(255 - (i * 10), 0, 0);
		pdc->Draw3dRect(&rect, rgb, rgb);
		rect.bottom--;
		rect.top++;
		rect.left++;
		rect.right--;
	}

}
