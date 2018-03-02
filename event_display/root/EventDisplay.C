#include <TGClient.h>
#include <TCanvas.h>
#include <TF1.h>
#include <TRandom.h>
#include <TGButton.h>
#include <TGFrame.h>
#include <TRootEmbeddedCanvas.h>
#include <RQ_OBJECT.h>

#include "ev_display_WheelReco.cxx"

class MyMainFrame {
   RQ_OBJECT("MyMainFrame")
private:
   TGMainFrame           *fMain;
   
   TRootEmbeddedCanvas *canvas1;
   TRootEmbeddedCanvas *canvas2;
   
   TGTextButton         *fStart;
   TGTextButton       *setParam;
   
   TGTextEntry        *diskREnt;
   TGTextEntry       *attenLEnt;
   TGTextEntry         *nPosEnt;
   TGTextEntry            *rEnit;
   TGTextEntry        *thetaEnt;
   TGTextEntry            *NEnt;

   TThread    *t0;
  
   WheelReco Reco;
 
   bool           start = false;

public:
   MyMainFrame(const TGWindow *p,UInt_t w,UInt_t h);
   virtual ~MyMainFrame();
   void DoDraw();
   char GetEvents();
   void ChangeStartLabel();
   void StartReco();
   void StopReco();
   void SetParameters();
  
};
MyMainFrame::MyMainFrame(const TGWindow *p,UInt_t w,UInt_t h) {
   // Create a main frame
   fMain = new TGMainFrame(p,w,h);

   // Create a horizontal frame
   TGHorizontalFrame *hframe1 = new TGHorizontalFrame(fMain,1500,1000);

   // Create vertical frames
   TGVerticalFrame *vframe1 = new TGVerticalFrame(hframe1,400,1000);
   TGVerticalFrame *vframe2 = new TGVerticalFrame(hframe1,800,1000);
   TGVerticalFrame *vframe3 = new TGVerticalFrame(hframe1,400,1000);

   // 1st vertical frame
   TGHorizontalFrame *vhframe1 = new TGHorizontalFrame(vframe1,400,20);
   
   TGLabel *goTo = new TGLabel(vhframe1, "Go to: ");
   vhframe1->AddFrame(goTo, new TGLayoutHints(kLHintsLeft,0,5,5,5) );

   TGComboBox *combobox = new TGComboBox(vhframe1, 100, 20);
   char events = GetEvents();
   char tmp[10];
   for (int i = 0; i < 10; ++i) {
      sprintf(tmp, "Event %i", i+1);
      combobox->AddEntry(tmp, i+1);
   }
   combobox->Resize(100,20);
   vhframe1->AddFrame(combobox, new TGLayoutHints(kLHintsExpandX | kLHintsTop,5,0,5,5) );

   vframe1->AddFrame(vhframe1, new TGLayoutHints(kLHintsExpandX,5,5,2,10) );

   TGLabel *event = new TGLabel(vframe1, "Event");
   vframe1->AddFrame(event, new TGLayoutHints(kLHintsLeft,5,5,2,2) );
 
   TGLabel *run = new TGLabel(vframe1, "Run");
   vframe1->AddFrame(run, new TGLayoutHints(kLHintsLeft,5,5,2,2) );

   TGTextButton *next = new TGTextButton(vframe1,"&Next");
   next->Connect("Clicked()","MyMainFrame",this,"DoDraw()");
  // next->Resize(100,40);
   vframe1->AddFrame(next, new TGLayoutHints(kLHintsExpandX,5,5,50,5) );

   TGTextButton *previous = new TGTextButton(vframe1,"&Previous");
   previous->Connect("Clicked()","MyMainFrame",this,"DoDraw()");
   vframe1->AddFrame(previous, new TGLayoutHints(kLHintsExpandX,5,5,5,5) );
   
   TGTextButton *quit = new TGTextButton(vframe1,"&Quit", "gApplication->Terminate(0)");
   vframe1->AddFrame(quit, new TGLayoutHints(kLHintsExpandX | kLHintsBottom,5,5,5,5) );

   vframe1->Resize(200,1000);
   
   hframe1->AddFrame(vframe1, new TGLayoutHints(kLHintsLeft | kLHintsExpandY,5,5,30,100));

  // 2nd vertical frame
   TRootEmbeddedCanvas *canvas1 = new TRootEmbeddedCanvas("Canvas1",vframe2,200,200);   
   vframe2->AddFrame(canvas1, new TGLayoutHints(kLHintsExpandX | kLHintsCenterX | kLHintsCenterY | kLHintsExpandY,5,5,5,5));   

   TRootEmbeddedCanvas *canvas2 = new TRootEmbeddedCanvas("Canvas2",vframe2,200,200);
   vframe2->AddFrame(canvas2, new TGLayoutHints(kLHintsExpandX | kLHintsCenterX | kLHintsCenterY | kLHintsExpandY,5,5,5,5));

   //TRootEmbeddedCanvas *canvas3 = new TRootEmbeddedCanvas("Canvas3",vframe2,200,200);
   //vframe2->AddFrame(canvas3, new TGLayoutHints(kLHintsCenterX | kLHintsExpandX | kLHintsCenterY | kLHintsExpandY,5,5,5,5));

   vframe2->Resize(1100,1000);
  
   hframe1->AddFrame(vframe2, new TGLayoutHints(kLHintsLeft | kLHintsExpandX | kLHintsCenterY | kLHintsExpandY, 5,5,20,20) );


   // 3rd vertical frame

   /* Title with font */
   TGGC *fTextGC;
   const TGFont *bfont = gClient->GetFont("-*-adobe-bold-r-*-*-18-*-*-*-*-*-*-*");
   if (!bfont) {
	bfont = gClient->GetResourcePool()->GetDefaultFont();
   }
   FontStruct_t labelfont = bfont->GetFontStruct();
   GCValues_t   gval;
   gval.fMask = kGCBackground | kGCFont | kGCForeground;
   gval.fFont = bfont->GetFontHandle();
   gClient->GetColorByName("black", gval.fBackground);
   fTextGC = gClient->GetGC(&gval, kTRUE);
   ULong_t bcolor, blackcolor;
   gClient->GetColorByName("black", blackcolor);
   TGLabel *title = new TGLabel(vframe3, "SiPM Wheel\nEvent Display", fTextGC->GetGC(), labelfont, kChildFrame);
   
   vframe3->AddFrame(title, new TGLayoutHints(kLHintsTop | kLHintsLeft, 5, 5, 50, 5));
   title->SetTextColor(blackcolor);

   /* Reco Parameters */
   TGGC *fTextGC1;
   const TGFont *font = gClient->GetFont("-*-adobe-r-*-*-12-*-*-*-*-*-*-*");
   if (!font) {
        font = gClient->GetResourcePool()->GetDefaultFont();
   }
   FontStruct_t label1font = font->GetFontStruct();
   GCValues_t   gval1;
   gval1.fMask = kGCBackground | kGCFont | kGCForeground;
   gval1.fFont = font->GetFontHandle();
   gClient->GetColorByName("black", gval1.fBackground);
   fTextGC1 = gClient->GetGC(&gval1, kTRUE);
   gClient->GetColorByName("black", blackcolor);
   TGLabel *tRecoParam = new TGLabel(vframe3, "Reco Parameters", fTextGC->GetGC(), labelfont, kChildFrame);
   vframe3->AddFrame(tRecoParam, new TGLayoutHints(kLHintsCenterX, 5,5,50,5));

   /* Disk Radius */
   TGHorizontalFrame *vhframe6 = new TGHorizontalFrame(vframe3,400,20);

   vhframe6->AddFrame(new TGLabel(vhframe6, "Disk Radius: ", fTextGC1->GetGC(), label1font, kChildFrame), new TGLayoutHints(kLHintsLeft, 0,5,5,5));
   TGTextBuffer *diskRBuf = new TGTextBuffer(10);
   diskRBuf->AddText(0, "15.0");
   diskREnt = new TGTextEntry(vhframe6, diskRBuf);
   diskREnt->Resize(50, diskREnt->GetDefaultHeight());
   diskREnt->SetFont("-adobe-courier-r-*-*-12-*-*-*-*-*-iso8859-1");
   vhframe6->AddFrame(diskREnt, new TGLayoutHints(kLHintsTop,5,5,2,5));

   vhframe6->AddFrame(new TGLabel(vhframe6, "cm", fTextGC1->GetGC(), label1font, kChildFrame), new TGLayoutHints(kLHintsLeft, 0,5,5,5));

   vframe3->AddFrame(vhframe6, new TGLayoutHints(kLHintsLeft | kLHintsExpandX, 5,5,5,5));

   /* Attenuation Length */
   TGHorizontalFrame *vhframe5 = new TGHorizontalFrame(vframe3,400,20);

   vhframe5->AddFrame(new TGLabel(vhframe5, "Attenuation Length: ", fTextGC1->GetGC(), label1font, kChildFrame), new TGLayoutHints(kLHintsLeft, 0,5,5,5));
   TGTextBuffer *attenLBuf = new TGTextBuffer(10);
   attenLBuf->AddText(0, "2.0");
   attenLEnt = new TGTextEntry(vhframe5, attenLBuf);
   attenLEnt->Resize(50, attenLEnt->GetDefaultHeight());
   attenLEnt->SetFont("-adobe-courier-r-*-*-12-*-*-*-*-*-iso8859-1");
   vhframe5->AddFrame(attenLEnt, new TGLayoutHints(kLHintsTop,5,5,2,5));

   vhframe5->AddFrame(new TGLabel(vhframe5, "cm", fTextGC1->GetGC(), label1font, kChildFrame), new TGLayoutHints(kLHintsLeft, 0,5,5,5));

   vframe3->AddFrame(vhframe5, new TGLayoutHints(kLHintsLeft | kLHintsExpandX, 5,5,5,5));

   /* Number of Positions */
   TGHorizontalFrame *vhframe7 = new TGHorizontalFrame(vframe3,400,20);

   vhframe7->AddFrame(new TGLabel(vhframe7, "Number of Positions: ", fTextGC1->GetGC(), label1font, kChildFrame), new TGLayoutHints(kLHintsLeft, 0,5,5,5));
   TGTextBuffer *nPosBuf = new TGTextBuffer(10);
   nPosBuf->AddText(0, "8");
   nPosEnt = new TGTextEntry(vhframe7, nPosBuf);
   nPosEnt->Resize(50, nPosEnt->GetDefaultHeight());
   nPosEnt->SetFont("-adobe-courier-r-*-*-12-*-*-*-*-*-iso8859-1");
   vhframe7->AddFrame(nPosEnt, new TGLayoutHints(kLHintsTop,5,5,2,5));

   //vhframe6->AddFrame(new TGLabel(vhframe6, "", fTextGC1->GetGC(), label1font, kChildFrame), new TGLayoutHints(kLHintsLeft, 0,5,5,5));

   vframe3->AddFrame(vhframe7, new TGLayoutHints(kLHintsLeft | kLHintsExpandX, 5,5,5,5));

   /* R Inc */
   TGHorizontalFrame *vhframe2 = new TGHorizontalFrame(vframe3,400,20); 

   vhframe2->AddFrame(new TGLabel(vhframe2, "Radius Increment: ", fTextGC1->GetGC(), label1font, kChildFrame), new TGLayoutHints(kLHintsLeft, 0,5,5,5));
   TGTextBuffer *rBuf = new TGTextBuffer(10);
   rBuf->AddText(0, "0.1");
   rEnt = new TGTextEntry(vhframe2, rBuf);
   rEnt->Resize(50, rEnt->GetDefaultHeight());
   rEnt->SetFont("-adobe-courier-r-*-*-12-*-*-*-*-*-iso8859-1");
   vhframe2->AddFrame(rEnt, new TGLayoutHints(kLHintsTop,5,5,2,5));

   vhframe2->AddFrame(new TGLabel(vhframe2, "cm", fTextGC1->GetGC(), label1font, kChildFrame), new TGLayoutHints(kLHintsLeft, 0,5,5,5));

   vframe3->AddFrame(vhframe2, new TGLayoutHints(kLHintsLeft | kLHintsExpandX, 5,5,5,5));

   /* Theta Inc */
   TGHorizontalFrame *vhframe3 = new TGHorizontalFrame(vframe3,400,20);

   vhframe3->AddFrame(new TGLabel(vhframe3, "Theta Increment: ", fTextGC1->GetGC(), label1font, kChildFrame), new TGLayoutHints(kLHintsLeft, 0,5,5,5));
   TGTextBuffer *thetaBuf = new TGTextBuffer(10);
   thetaBuf->AddText(0, "1");
   thetaEnt = new TGTextEntry(vhframe3, thetaBuf);
   thetaEnt->Resize(50, thetaEnt->GetDefaultHeight());
   thetaEnt->SetFont("-adobe-courier-r-*-*-12-*-*-*-*-*-iso8859-1");
   vhframe3->AddFrame(thetaEnt, new TGLayoutHints(kLHintsTop,5,5,2,5));

   vhframe3->AddFrame(new TGLabel(vhframe3, "deg", fTextGC1->GetGC(), label1font, kChildFrame), new TGLayoutHints(kLHintsLeft, 0,5,5,5));

   vframe3->AddFrame(vhframe3, new TGLayoutHints(kLHintsLeft | kLHintsExpandX, 5,5,5,5));

   /* N Inc*/
   TGHorizontalFrame *vhframe4 = new TGHorizontalFrame(vframe3,400,20);

   vhframe4->AddFrame(new TGLabel(vhframe4, "N Increment: ", fTextGC1->GetGC(), label1font, kChildFrame), new TGLayoutHints(kLHintsLeft, 0,5,5,5));
   TGTextBuffer *NBuf = new TGTextBuffer(10);
   NBuf->AddText(0, "1");
   NEnt = new TGTextEntry(vhframe4, NBuf);
   NEnt->Resize(50, NEnt->GetDefaultHeight());
   NEnt->SetFont("-adobe-courier-r-*-*-12-*-*-*-*-*-iso8859-1");
   vhframe4->AddFrame(NEnt, new TGLayoutHints(kLHintsTop,5,5,2,5));

   vhframe4->AddFrame(new TGLabel(vhframe4, "p.e.", fTextGC1->GetGC(), label1font, kChildFrame), new TGLayoutHints(kLHintsLeft, 0,5,5,5));

   vframe3->AddFrame(vhframe4, new TGLayoutHints(kLHintsLeft | kLHintsExpandX, 5,5,5,5));


   /* Set Params */
   setParam = new TGTextButton(vframe3,"Set Parameters");
   setParam->Connect("Clicked()","MyMainFrame",this,"SetParameters()");
   vframe3->AddFrame(setParam, new TGLayoutHints(kLHintsExpandX,5,5,5,5) );
   
   /* Run Reco */
   TGLabel *tReco = new TGLabel(vframe3, "Run Reco", fTextGC->GetGC(), labelfont, kChildFrame);
   vframe3->AddFrame(tReco, new TGLayoutHints(kLHintsCenterX, 5,5,300,5));

   /* Start button */
   fStart = new TGTextButton(vframe3,"&Start");
   fStart->Connect("Clicked()","MyMainFrame",this,"ChangeStartLabel()");
   vframe3->AddFrame(fStart, new TGLayoutHints(kLHintsExpandX ,5,5,5,5));

   vframe3->Resize(400,1000);
   hframe1->AddFrame(vframe3, new TGLayoutHints(kLHintsLeft | kLHintsExpandY, 5,5,5,5) );






   // Add horizontal to main frame //
 
   fMain->AddFrame(hframe1, new TGLayoutHints(kLHintsExpandX | kLHintsExpandY,5,5,5,5));

   // Set a name to the main frame //
   fMain->SetWindowName("Event Display");

   // Map all subwindows of main frame //
   fMain->MapSubwindows();

   // Initialize the layout algorithm // 
   fMain->Resize(fMain->GetDefaultSize());

   // Map main frame //
   fMain->MapWindow();

   fMain->Resize(1500, 1000);
}
MyMainFrame::~MyMainFrame() {
   // Clean up used widgets: frames, buttons, layout hints
   fMain->Cleanup();
   delete fMain;
}

void MyMainFrame::ChangeStartLabel()
{
  // Slot connected to the Clicked() signal.
  // It will toggle labels "Start" and "Stop".

  fStart->SetState(kButtonDown);
  if (!start) {
     fStart->SetText("&Stop");
     StartReco();
     start = true;
  } else {
     fStart->SetText("&Start");
     StopReco();
     start = false;
  }
  fStart->SetState(kButtonUp);

}
void MyMainFrame::DoDraw() {
   // Draws function graphics in randomly chosen interval
    
  /* if (!img) {
	printf("Could not create an image, exiting\n");
	return;
   }
   
   TCanvas *fCanvas = canvas1->GetCanvas();
   fCanvas->cd();
   fCanvas->Update();*/
}
char MyMainFrame::GetEvents() {
   /*// Open the file containing the histograms
   TFile *f = new TFile("../output/Histograms.root", "READ");
   if (!f->IsOpen()) {
   	std::cout << "Error. Cannot open histogram file.\n";
    	exit(1) ;
   }

   TList *list = f->GetListOfKeys() ;
   if (!list) { 
	printf("Error. No keys found in file\n"); 
	exit(1); 
   }
   TIter next(list);
   TKey *key;
   TObject *obj;
    
   std::vector<std::string> eventNames;
  
   while ( (key = (TKey*)next()) ) {
    	obj = key->ReadObj();
    	if ( (strcmp(obj->IsA()->GetName(),"TProfile")!=0) && (!obj->InheritsFrom("TH2")) ) {
      		printf("<W> Object %s is not 1D or 2D histogram : " " will not be converted\n",obj->GetName()) ;
    	}
	eventNames.push_back( obj->GetName()  );
	std::cout << obj->GetName() << std::endl;
    	//printf("Histo name:%s title:%s\n",obj->GetName(),obj->GetTitle());
   }*/
   
   char h = 'a';
   return h;

}
void MyMainFrame::StartReco() {
   // Start Reco
   std::cout << "Reconstruction starting...\n";
   t0 = new TThread("t0", handle);
   t0->Run();
}
void *handle(void *ptr) 
{
   Reco.Start();

}
void MyMainFrame::StopReco() {
   // Stop Reco
   std::cout << "Reconstruction stopping...\n";
   t0->Kill();
}
void MyMainFrame::SetParameters() {
   // Get the current settings
   auto R         = std::stod(diskREnt->GetText());
   auto attenL    = std::stod(attenLEnt->GetText());
   auto nPos      = std::stoi(nPosEnt->GetText());
   auto rInc      = std::stod(rEnt->GetText());
   auto thetaInc  = std::stod(thetaEnt->GetText());
   auto NInc      = std::stod(NEnt->GetText());

   std::cout << "Disk radius set to: " << R << "\n";
   std::cout << "Attenuation length set to: " << attenL << "\n";
   std::cout << "Number of positions set to: " << nPos << "\n";
   std::cout << "R increment set to: " << rInc << "\n";
   std::cout << "Theta increment set to: " << thetaInc << "\n";
   std::cout << "N increment set to: " << NInc << "\n";
   
   reco.diskRadius        = R;
   reco.incrementRadius   = rInc;
   reco.incrementTheta    = thetaInc;
   reco.attenuationLength = attenL;
   reco.numberOfPositions = nPos;
   reco.incrementN        = NInc;
}
void EventDisplay() {
   // Popup the GUI...
   new MyMainFrame(gClient->GetRoot(),1500,1000);
}
