#include "main.h"

INT_PTR CALLBACK OptionsProc(HWND hDlg, UINT uiMsg, WPARAM wParam, LPARAM)
{
    switch (uiMsg)
    {
        case WM_INITDIALOG:
            SetDlgItemText(hDlg, IDC_TEXT, szText);
            CheckDlgButton(hDlg, IDC_WHITE, iBrushColor == WHITE_BRUSH ? BST_CHECKED : BST_UNCHECKED);
            CheckDlgButton(hDlg, IDC_LGRAY, iBrushColor == LTGRAY_BRUSH ? BST_CHECKED : BST_UNCHECKED);
            CheckDlgButton(hDlg, IDC_GRAY, iBrushColor == GRAY_BRUSH ? BST_CHECKED : BST_UNCHECKED);
            CheckDlgButton(hDlg, IDC_DGRAY, iBrushColor == DKGRAY_BRUSH ? BST_CHECKED : BST_UNCHECKED);
            return TRUE;
            
        case WM_COMMAND:
            switch (LOWORD(wParam))
            {
                case IDOK:
                    GetDlgItemText(hDlg, IDC_TEXT, szText, 256);
                    if (IsDlgButtonChecked(hDlg, IDC_WHITE) == BST_CHECKED)
                        iBrushColor = WHITE_BRUSH;
                    if (IsDlgButtonChecked(hDlg, IDC_LGRAY) == BST_CHECKED)
                        iBrushColor = LTGRAY_BRUSH;
                    if (IsDlgButtonChecked(hDlg, IDC_GRAY) == BST_CHECKED)
                        iBrushColor = GRAY_BRUSH;
                    if (IsDlgButtonChecked(hDlg, IDC_DGRAY) == BST_CHECKED)
                        iBrushColor = DKGRAY_BRUSH;
                case IDCANCEL:
                    EndDialog(hDlg, LOWORD(wParam));
                    return TRUE;
            }
            break;
    }
    return FALSE;
}
