/*
  CSimplePdf - class for created simple .pdf files for Borland C Builder 6.0
  https://github.com/podoroges/simplepdf

  ///////// Usage Example: ////////////////
  CSimplePdf * pdf = new CSimplePdf();
  pdf->EmbedFont("F1","c:\\Home\\Temp\\PdfTest2\\aricyr.ttf");
  pdf->EmbedFont("F2","c:\\Home\\Temp\\PdfTest2\\calligra.ttf");
  pdf->AddPage(800,600);
  pdf->CurrentFont = "F2";
  pdf->FontSize = 25;
  pdf->Page[0]->Text(100,100,"Hello!");
  pdf->Page[0]->Rect(90,90,200,140);
  pdf->FontSize = 22;
  pdf->CurrentFont = "F1";
  pdf->Page[0]->Text(100,200,"QWERTY");
  pdf->Page[0]->Text(100,150,"ZXCVBN");
  pdf->LineWidth = 5;
  pdf->Page[0]->Line(100,300,100,500);
  pdf->SaveToFile("C:\\test1.pdf");
  delete pdf;
  ////////////////////////////

  Class CTTFParser based on ttfparser.php and makefont.php
  by Olivier PLATHEY http://www.fpdf.org

  Converted to Borland C++ Builder class
  by Podoroges https://github.com/podoroges

  Created solely for cp1251 cyrillic encoding
  Please feel free to modify it for your needs

  ////////////////////////////

  Tool for onlyne .PDF analysis:
  https://www.pdf-online.com/osa/repair.aspx

*/

#include <vcl.h>
#include <vector>

class CSimplePdf{
  private:
  class CCat;
  class CPages;
  class CFontDiff;
  class CFontFile;
  class CFontDescriptor;
  class CFontTTF;
  class CObj{
    public:
    CSimplePdf * parent;
    int xref;
    int ObjectID(){
      for(unsigned int a = 0;a<parent->Objects.size();a++)
        if(parent->Objects[a]==this)
          return a+1;
      return 0;
    };
    virtual AnsiString AsString() = 0;
    virtual char Type() = 0;
  };
  friend class CObj;
  std::vector <CObj *> Objects;
  class CPageContents;
  class CPage: public CObj{
    private:
    AnsiString FontsString();
    public:
    CPageContents * Contents;
    double Width, Height;
    CPage(double W,double H,CSimplePdf * p):Width(W),Height(H){parent = p;Rotate = 0;}
    void Rect(double x1,double y1,double x2,double y2);
    void SetGrayStroking(double);
    void SetGrayNonStroking(double);
    void FillRect(double x1,double y1,double x2,double y2);
    void Line(double x1,double y1,double x2,double y2);
    void LineDotted(double x1,double y1,double x2,double y2);


    void LineTo(double x1,double y1);
    void MoveTo(double x1,double y1);
    void Cubic(double x1,double y1,double x2,double y2,double x3,double y3);

    void Dash(AnsiString = "");

    void Stroke();
    void Fill();
    void Text(double x1,double y1,AnsiString st,double maxwidth = -1);
    void RightText(double x1,double y1,AnsiString st);

    void Custom(AnsiString st);
    int MultilineText(double x1,double y1,AnsiString st,double w = -1, double h = -1);
    void ImgInline(AnsiString,double,double);
    void ImgInlineBW(AnsiString,double,double);
    AnsiString AsString();
    char Type(){
      return 'P';
    }
    int Rotate;//90
  };
  friend class CPage;
  int _GetPageCount();
  CPage * _GetPage(int index);
  void _out(TMemoryStream *,AnsiString);
  public:
  void Decompress(AnsiString,AnsiString);
  int Compress;
  int Justify;
  double LineWidth;
  unsigned long PenColor;
  unsigned long FillColor;
  double FontGray;
  int FontSize;
  AnsiString CurrentFont;
  int TextWidth(AnsiString);
  __property CPage * Page[int index] = {read=_GetPage};
  __property int PageCount = {read=_GetPageCount};
  void AddPage(double Width=612,double Height=792);
  CSimplePdf();
  ~CSimplePdf(){
    for(unsigned int a=0;a<Objects.size();a++)
      delete Objects[a];
    Objects.clear();
  }
  void EmbedFont(AnsiString name,AnsiString fname);
  void SaveToFile(AnsiString fname);
};
